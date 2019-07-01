import random, os, sys
import numpy as np
from keras.models import *
from keras.layers import *
from keras.callbacks import *
from keras.initializers import *
import tensorflow as tf

try:
    # for transformer
    from tqdm import tqdm
    from test_dataloader import TokenList, pad_to_longest
except: pass

class Transformer:
    def __init__(self, i_tokens, o_tokens, len_limit, d_model=256,\
                 d_inner_hid=512, n_head=4, layers=2, dropout=0.1, \
                 share_word_emb=False):
        self.i_tokens = i_tokens
        self.o_tokens = o_tokens
        self.len_limit = len_limit
        self.d_model = d_model
        self.decode_model = None
        self.readout_model = None
        self.layers = layers
        d_emb = d_model
        self.src_loc_info = True

        d_k = d_v = d_model // n_head
        assert d_k * n_head == d_model and d_v == d_k

        self.pos_emb = PosEncodingLayer(len_limit, d_emb) if self.src_loc_info else None
        self.emb_dropout = Dropout(dropout)
        self.i_word_emb = Embedding(i_tokens.num(), d_emb)

        if share_word_emb:
            assert i_tokens.num() == o_tokens.num()
            self.o_word_emb = self.i_word_emb
        else:
            self.o_word_emb = Embedding(o_tokens.num(), d_emb)

        self.encoder = SelfAttention(d_model, d_inner_hid, n_head, layers, dropout)
        self.decoder = Decoder(d_model, d_inner_hid, n_head, layers, dropout)
        self.target_layer = TimeDistributed(Dense(o_tokens.num(), use_bias=False))

    def compile(self, optimizer='adam', active_layers=999):
        src_seq_input = Input(shape=(None,), dtype='int32')
        tgt_seq_input = Input(shape=(None,), dtype='int32')

        src_seq = src_seq_input
        tgt_seq = Lambda(lambda x:x[:,:-1])(tgt_seq_input)
        tgt_true = Lambda(lambda x:x[:,1:])(tgt_seq_input)

        src_emb = self.i_word_emb(src_seq)
        tgt_emb = self.o_word_emb(tgt_seq)

        if self.pos_emb:
            src_emb = add_layer([src_emb, self.pos_emb(src_seq)])
            rgt_emb = add_layer([tgt_emb, self.pos_emb(tgt_seq)])
        src_emb = self.emb_dropout(src_emb)

        enc_output = self.encoder(src_emb, src_seq, active_layers=active_layers)
        dec_output = self.decoder(tgt_emb, tgt_seq, src_seq, enc_output, active_layers=active_layers)
        final_output = self.target_layer(dec_output)

        def get_loss(y_pred, y_true):
            y_true = tf.cast(y_true, 'int32')
            loss = tf.nn.sparse_softmax_cross_entropy_with_logits(labels=y_true, logits=y_pred)
            mask = tf.cast(tf.not_equal(y_true, 0), 'float32')
            loss = tf.reduce_sum(loss * mask, -1) / tf.reduce_sum(mask, -1)
            losss = K.mean(loss)

            return loss

        def get_acc(y_pred, y_true):
            mask = tf.cast(tf.not_equal(y_true, 0), 'float32')
            corr = K.cast(K.equal(K.cast(y_true, 'int32'), K.cast(K.argmax(y_pred, axis=1), 'int32')), 'float32')
            corr = K.sum(corr * mask, -1) / K.sum(mask, -1)

            return K.mean(corr)

        loss = get_loss(final_output, tgt_true)
        self.ppl = K.exp(loss)
        self.acc = get_acc(final_output, tgt_true)
        self.model = Model([src_seq_input, tgt_seq_input], final_output)
        self.model.add_loss([loss])

        self.model.compile(optimizer, None)
        self.model.metrics_names.append('ppl')
        self.model.metrics_tensors.append(self.ppl)
        self.model.metrics_names.append('acc')
        self.model.metrics_tensors.append(self.acc)

    def make_src_seq_matrix(self, input_seqs):
        if type(input_seqs[0]) == type(''): input_seqs = [input_seqs]
        maxlen = max(map(len, input_seqs))
        src_seq = np.zeros((len(input_seqs), maxlen+3), dtype='int32')
        src_seq[:,0] = self.i_tokens.start_id()

        for i, seq in enumerate(input_seqs):
            for ii, z in enumerate(seq):
                src_seq[i, 1+ii] = self.i_tokens.id(z)

            src_seq[i, 1+len(seq)] = self.i_tokens.end_id()

        return src_seq

    def make_readout_decode_model(self, max_output_len=32):
        src_seq_input = Input(shape=(None,), dtype='int32')
        tgt_start_input = Input(shape=(1,), dtype='int32')
        src_seq = src_seq_input
        enc_mask = Lambda(lambda x:K.cast(K.greater(x, 0), 'float32'))(src_seq)
        src_emb = self.i_word_emb(src_seq)

        if self.pos_emb:
            src_emb = add_layer([src_emb, self.pos_emb(src_seq)])

        src_emb = self.emb_dropout(src_emb)
        enc_output = self.encoder(src_emb, src_seq)

        tgt_emb = self.o_word_emb(tgt_start_input)
        tgt_seq = Lambda(lambda x:K.repeat_elements(x, max_output_len, 1))(tgt_start_input)
        rep_input = Lambda(lambda x:K.repeat_elements(x, max_output_len, 1))(tgt_emb)

        cell = ReadoutDecoderCell(self.o_word_emb, self.pos_emb, self.decoder, self.target_layer)
        final_output = InferRNN(cell, return_sequences=True)(rep_input,
                        initial_state=[tgt_start_input, K.ones_like(tgt_start_input), K.zeros_like(tgt_seq)] + \
                        [rep_input for _ in self.decoder.layer], constant=[enc_output, enc_mask])

        final_output = Lambda(lambda x:K.squeeze(x, -1))(final_output)
        self.readout_model = Model([src_seq_input, tgt_start_input], final_output)

    def decode_sequence_readout_x(self, X, batch_size=32, max_output_len=64):
        if self.readout_model is None: self.make_readout_decode_model(max_output_len)
        target_seq = np.zeros((X.shape[0], 1), dtype='int32')
        target_seq[:,0] = self.o_tokens.start_id()
        ret = self.readout_model.predict([X, target_seq], batch_size=batch_size, verbose=1)

        return ret

    def generate_sentence(self, rets, delimiter=''):
        sents = []

        for x in rets:
            end_pos = min([i for i, z in enumerate(x) if z == self.o_tokens.end_id()] + [len(x)])
            rsent = [*map(self.o_tokens.token, x)][:end_pos]
            sents.append(delimiter.join(rsent))

        return sents

    def decode_sequence_readout(self, input_seqs, delimiter=''):
        if self.readout_model is None: self.make_readout_decode_model()
        src_seq = self.make_src_seq_matrix(input_seqs)
        target_seq = np.zeros((src_seq.shape[0], 1), dtype='int32')
        target_seq[:, 0] = self.o_tokens.start_id()
        rets = self.readout_model.predict([src_seq, target_seq])
        rets = self.generate_sentence(rets, delimiter)

        if type(input_seqs[0]) is type('') and len(rets) == 1: rets = rets[0]

        return rets

