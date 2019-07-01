# -*- coding: utf-8 -*-

from pickle import dump
from pickle import load
import numpy as np
import pandas as pd
from utils import make_dict

from keras import backend as K
from keras.preprocessing import sequence
from keras.utils import plot_model
from keras.models import Model
from keras.layers import Input, LSTM, GRU, Concatenate, Dense, TimeDistributed,\
    Reshape, Permute, Multiply, Lambda, Softmax, Add, Flatten, Activation, Conv2D, Conv1D
from keras.optimizers import RMSprop

import matplotlib
from matplotlib import ticker
import matplotlib.pyplot as plt

data = load(open('./data/dataset_vectorized.pkl', 'rb'))
# dict = load(open('./data/dict.pkl', 'rb'))

# Hyper-parameters
batch_size = 50
epochs = 30
latent_dim = 256
learning_rate = 0.05
clip_norm = 2.0
# num_samples = len(x_train)

# Vectorize
input_texts = []
target_texts = []
input_characters = set()
target_characters = set()

cnt = 0

for line in data:
    input_text = line['article']
    target_text = line['summary']

    # 임시 # "\t"문자를 시작 문자로, "\n"문자를 종료 문자로 사용.
    # target_text = '\t' + target_text + '\n'
    input_char = []
    target_char = []
    # for c in input_text:

    input_texts.append(input_text)
    target_texts.append(target_text)

    # # 문자 집합 생성
    # for char in input_text:
    #     if char not in input_characters:
    #         input_characters.add(char)
    #
    # for char in target_text:
    #     if char not in target_characters:
    #         target_characters.add(char)

    cnt += 1
    if cnt == 100: break

# 학습 파라미터의 개수
num_samples = len(input_texts)
# input_characters = sorted(list(input_characters))
# target_characters = sorted(list(target_characters))
# num_encoder_tokens = len(input_characters)
# num_decoder_tokens = len(target_characters)
# max_encoder_seq_length = max([len(txt) for txt in input_texts])
# max_decoder_seq_length = max([len(txt) for txt in target_texts])

num_encoder_tokens = 1
num_decoder_tokens = 1
max_encoder_seq_length = 500
max_decoder_seq_length = 500

# print('Number of samples:', num_samples)
# print('Number of unique input tokens:', num_encoder_tokens)
# print('Number of unique output tokens:', num_decoder_tokens)
# print('Max sequence length for inputs:', max_encoder_seq_length)
# print('Max sequence length for outputs:', max_decoder_seq_length)

input_texts = np.array(input_texts)
# input_texts = input_texts[:, np.newaxis]
# input_texts = np.array(input_texts)
input_texts.reshape((num_samples, max_encoder_seq_length, 1))
target_texts = np.array(target_texts)
# target_texts = target_texts[:, np.newaxis]
# target_texts = np.array(target_texts)
target_texts.reshape((num_samples, max_decoder_seq_length, 1))

encoder_shape = input_texts.shape
decoder_shape = target_texts.shape
print(input_texts.shape)
print(target_texts.shape)
# print(input_texts[0])
# print(target_texts[0])

# 문자 -> 숫자 변환용 사전
input_token_index = dict( [(char, i) for i, char in enumerate(input_characters)])
target_token_index = dict( [(char, i) for i, char in enumerate(target_characters)])

# 학습에 사용할 데이터를 담을 3차원 배열
# encoder_input_data = np.zeros((num_samples, max_encoder_seq_length, num_encoder_tokens), dtype='float32')
# decoder_input_data = np.zeros((num_samples, max_decoder_seq_length, num_decoder_tokens), dtype='float32')
# decoder_target_data = np.zeros((num_samples, max_decoder_seq_length, num_decoder_tokens), dtype='float32')

# 문장을 문자 단위로 원 핫 인코딩하면서 학습용 데이터를 만듬
# for i, (input_text, target_text) in enumerate(zip(input_texts, target_texts)):
#     for t, char in enumerate(input_text):
#         encoder_input_data[i, t, input_token_index[char]] = 1.
#     for t, char in enumerate(target_text):
#         decoder_input_data[i, t, target_token_index[char]] = 1.
#         if t > 0:
#             decoder_target_data[i, t - 1, target_token_index[char]] = 1.
#
# # 숫자 -> 문자 변환용 사전
# reverse_input_char_index = dict( (i, char) for char, i in input_token_index.items())
# reverse_target_char_index = dict( (i, char) for char, i in target_token_index.items())


#     (num_samples, max_encoder_seq_length, num_encoder_tokens), dtype='float32')
# decoder_input_data = np.zeros(
#     (num_samples, max_decoder_seq_length, num_decoder_tokens), dtype='float32')
# decoder_target_data = np.zeros(
#     (num_samples, max_decoder_seq_length, num_decoder_tokens), dtype='float32')
#
# print(encoder_input_data.shape)
# print(decoder_input_data.shape)
# print(decoder_target_data.shape)

def repeat_vector_layer(repeat, axis):
    return Lambda(lambda x: K.repeat_elements(K.expand_dims(x, axis), repeat, axis),
                  lambda x: tuple((x[0], ) + x[1:axis] + (repeat, ) + x[axis:]))


# # Encoder
# encoder_inputs = Input(shape=(1, 500))
# encoder = LSTM(500, dropout_U=0.2, dropout_W=0.2, return_sequences=True, return_state=True)
# encoder_outputs, state_h, state_c = encoder(encoder_inputs)
#
# encoder_LSTM_rev = LSTM(500, dropout_U=0.05, dropout_W=0.05, return_sequences=True, return_state=True, go_backwards=True)
# encoder_outputs_rev, state_h_rev, state_c_rev = encoder_LSTM_rev(encoder_inputs)
#
# state_h_final = Add()([state_h, state_h_rev])
# state_c_final = Add()([state_c, state_c_rev])
# encoder_outputs_final = Add()([encoder_outputs, encoder_outputs_rev])
#
# encoder_states = [state_h_final, state_c_final]
# # encoder_states = [state_h, state_c]
#
# # Decoder
# decoder_inputs = Input(shape=(1, 500))
# decoder = LSTM(500, dropout_U=0.05, dropout_W=0.05, return_sequences=True, return_state=True)
# decoder_outputs, _, _, = decoder(decoder_inputs, initial_state=encoder_states)
#
# # Attention
# repeat_decoder_layer = repeat_vector_layer(max_encoder_seq_length, 1)
# repeat_decoder = repeat_decoder_layer(decoder_outputs)
#
# repeat_encoder_layer = repeat_vector_layer(max_decoder_seq_length, 1)
# repeat_encoder = repeat_encoder_layer(encoder_outputs)
#
# concat_for_score_layer = Concatenate(axis=-1)
# concat_for_score = concat_for_score_layer([repeat_decoder, repeat_encoder])
#
# attention = TimeDistributed(Dense(1, activation='tanh'))(encoder_outputs_final)
# attention = Flatten()(attention)
# attention = Multiply()([decoder_outputs, attention])
# attention = Activation('softmax')(attention)
# attention = Permute((2, 1))(attention)
#
# # Output
# dense = Dense(500, activation='softmax')(decoder_outputs)
# output = dense
# decoder_dense = Dense(500, activation='softmax')
# decoder_outputs = decoder_dense(attention)


# # Encoder
# encoder_inputs = Input(shape=(num_encoder_tokens, max_encoder_seq_length))
# encoder = GRU(latent_dim, dropout_U=0.2, dropout_W=0.2, return_sequences=True, return_state=True)
# encoder_outputs, state_h = encoder(encoder_inputs)
#
# # Decoder
# decoder_inputs = Input(shape=(num_decoder_tokens, max_decoder_seq_length))
# decoder = GRU(latent_dim, dropout_U=0.05, dropout_W=0.05, return_sequences=True, return_state=True)
# decoder_outputs, _ = decoder(decoder_inputs, initial_state=state_h)
#
# # # Attention
# repeat_decoder_layer = repeat_vector_layer(max_encoder_seq_length, 1)
# repeat_decoder = repeat_decoder_layer(decoder_outputs)
#
# repeat_encoder_layer = repeat_vector_layer(max_decoder_seq_length, 1)
# repeat_encoder = repeat_encoder_layer(encoder_outputs)
# #
# concat_for_score_layer = Concatenate(axis=-1)
# concat_for_score = concat_for_score_layer([repeat_decoder, repeat_encoder])
# # concat_for_score = concat_for_score_layer([decoder_outputs, encoder_outputs])
#
# dense1_t_score_layer = Dense(latent_dim // 2, activation='tanh')
# dense1_score_layer = TimeDistributed(dense1_t_score_layer)
# dense1_score = dense1_score_layer(concat_for_score)
#
# dense2_t_score_layer = Dense(1)
# dense2_score_layer = TimeDistributed(dense2_t_score_layer)
# dense2_score = dense2_score_layer(dense1_score)
# # dense2_score = Reshape((max_decoder_seq_length, max_encoder_seq_length))(dense2_score)
# dense2_score = Reshape((1, max_decoder_seq_length))(dense2_score)
#
# softmax_score_layer = Softmax(axis=-1)
# softmax_score = softmax_score_layer(dense2_score)
#
# repeat_score_layer = repeat_vector_layer(latent_dim, 2)
# repeat_score = repeat_score_layer(softmax_score)
#
# permute_e = Permute((2, 1))(encoder_outputs)
#
# repeat_e_layer = repeat_vector_layer(max_decoder_seq_length, 1)
# repeat_e = repeat_e_layer(permute_e)
# #
# attended_mat_layer = Multiply()([repeat_score, repeat_e])
# # attended_mat = attended_mat_layer([repeat_score, repeat_e])
#
# context_layer = Lambda(lambda x: K.sum(x, axis=-1), lambda x: tuple(x[:-1]))(attended_mat_layer)
# # context = context_layer(attended_mat_layer)
#
# concat_context_layer = Concatenate(axis=-1)([context_layer, decoder_outputs])
# # concat_context = concat_context_layer([context_layer, decoder_outputs])
#
# attention_dense_output_layer = Dense(max_decoder_seq_length, activation='tanh')
# attention_output_layer = TimeDistributed(attention_dense_output_layer)(concat_context_layer)
# # attention_output = attention_output_layer(concat_context_layer)
# #
# decoder_dense = Dense(max_decoder_seq_length, activation='softmax')(attention_output_layer)
# # # decoder_dense = Dense(num_decoder_tokens, activation='softmax')(attention_output_layer)
# # # decoder_outputs = decoder_dense(attention_output)

encoder_inputs = Input(shape=encoder_shape)
encoder = Conv2d()


model = Model(input=[encoder_inputs, decoder_inputs], output=output)
print(model.summary())

rmsprop = RMSprop(lr=learning_rate, clipnorm=clip_norm)
model.compile(optimizer=rmsprop, loss='categorical_crossentropy', metrics=['accuracy'])

train = True
if train:
    history = model.fit([input_texts, target_texts], target_texts,
                        batch_size=batch_size, epochs=epochs, validation_split=0.2, verbose=2)
    print(model.summary())
    model.save_weights('att_seq2seq_weights.h5')

    # 손실 그래프
    plt.plot(history.history['loss'], 'y', label='train loss')
    plt.plot(history.history['val_loss'], 'r', label='val loss')
    plt.legend(loc='upper left')
    plt.show()

    # 정확도 그래프
    plt.plot(history.history['acc'], 'y', label='train acc')
    plt.plot(history.history['val_acc'], 'r', label='val acc')
    plt.legend(loc='upper left')
    plt.show()
