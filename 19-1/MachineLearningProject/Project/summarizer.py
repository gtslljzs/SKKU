# -*- coding: utf-8 -*-

import numpy as np
from pickle import load
from pickle import dump
import matplotlib.pyplot as plt

import keras
from keras.preprocessing import sequence
from keras.models import Model, Sequential
from keras.models import load_model
from keras.layers import Input, Dense, LSTM, Activation, Add, TimeDistributed, \
    Permute, Flatten, RepeatVector, merge, Lambda, Multiply, Reshape
from sklearn.model_selection import train_test_split as tts
from keras import initializers
from keras.optimizers import RMSprop

########## hyper-parameters ##########
batch_size = 50
num_classes = 1
epochs = 10
hidden_units = 200
learning_rate = 0.005
clip_norm = 2.0

########## load data ##########
data = load(open('./data/dataset_vectorized.pkl', 'rb'))
# x_train = []
# y_train = []
#
# for sample in data:
#     input_text = np.array(sample['article'])
#     x_train.append(input_text)
#     # x_train = np.array(x_train)
#     # x_train.reshape(-1, 1)
#
# for sample in data:
#     input_text = np.array(sample['summary'])
#     y_train.append(input_text)
#     # y_train = np.array(y_train)
#     # y_train.reshape(-1, 1)
#
# x_train = np.array(x_train)
# y_train = np.array(y_train)
# x_train = sequence.pad_sequences(x_train, maxlen=2500)
# y_train = sequence.pad_sequences(x_train, maxlen=2500)
# # x_train.reshape((9996,))
# # y_train.reshape((9996,))
#
# print(np.shape(x_train))
# print(np.shape(y_train))

# encoder_shape = x_train.shape
# decoder_shape = y_train.shape
# print(x_train.shape)
# print(decoder_shape)

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
    input_texts.append(input_text)
    target_texts.append(target_text)

    # 문자 집합 생성
    for char in input_text:
        if char not in input_characters:
            input_characters.add(char)

    for char in target_text:
        if char not in target_characters:
            target_characters.add(char)

    cnt += 1
    if cnt == 100: break

# 학습 파라미터의 개수
num_samples = len(input_texts)
input_characters = sorted(list(input_characters))
target_characters = sorted(list(target_characters))
num_encoder_tokens = len(input_characters)
num_decoder_tokens = len(target_characters)
max_encoder_seq_length = max([len(txt) for txt in input_texts])
max_decoder_seq_length = max([len(txt) for txt in target_texts])

print('Number of samples:', num_samples)
print('Number of unique input tokens:', num_encoder_tokens)
print('Number of unique output tokens:', num_decoder_tokens)
print('Max sequence length for inputs:', max_encoder_seq_length)
print('Max sequence length for outputs:', max_decoder_seq_length)

input_token_index = dict( [(char, i) for i, char in enumerate(input_characters)])
target_token_index = dict( [(char, i) for i, char in enumerate(target_characters)])

# 학습에 사용할 데이터를 담을 3차원 배열
encoder_input_data = np.zeros((num_samples, max_encoder_seq_length, num_encoder_tokens), dtype='float32')
decoder_input_data = np.zeros((num_samples, max_decoder_seq_length, num_decoder_tokens), dtype='float32')
decoder_target_data = np.zeros((num_samples, max_decoder_seq_length, num_decoder_tokens), dtype='float32')

# 문장을 문자 단위로 원 핫 인코딩하면서 학습용 데이터를 만듬
for i, (input_text, target_text) in enumerate(zip(input_texts, target_texts)):
    for t, char in enumerate(input_text):
        encoder_input_data[i, t, input_token_index[char]] = 1.
    for t, char in enumerate(target_text):
        decoder_input_data[i, t, target_token_index[char]] = 1.
        if t > 0:
            decoder_target_data[i, t - 1, target_token_index[char]] = 1.

# 숫자 -> 문자 변환용 사전
reverse_input_char_index = dict( (i, char) for char, i in input_token_index.items())
reverse_target_char_index = dict( (i, char) for char, i in target_token_index.items())

encoder_shape = (max_encoder_seq_length, num_encoder_tokens)
decoder_shape = (max_decoder_seq_length, num_decoder_tokens)

########## helper functions ##########
def encoder_decoder(articles, summaries):
    print('Encoder and Decoder LSTM...')

    """___encoder___"""
    encoder_inputs = Input(shape=(max_encoder_seq_length, num_encoder_tokens))
    # encoder_sequential = Sequential()(encoder_inputs)
    encoder_LSTM = LSTM(hidden_units, dropout_U=0.2, dropout_W=0.2, return_sequences=True, return_state=True)
    encoder_LSTM_rev = LSTM(hidden_units, dropout_U=0.05, dropout_W=0.05, return_sequences=True, return_state=True, go_backwards=True)

    encoder_outputs, state_h, state_c = encoder_LSTM(encoder_inputs)
    encoder_outputs_rev, state_h_rev, state_c_rev = encoder_LSTM_rev(encoder_inputs)

    state_h_final = Add()([state_h, state_h_rev])
    state_c_final = Add()([state_c, state_c_rev])
    encoder_outputs_final = Add()([encoder_outputs, encoder_outputs_rev])
    # print(encoder_outputs_final.shape)

    encoder_states = [state_h_final, state_c_final]

    """___decoder___"""
    decoder_inputs = Input(shape=(max_decoder_seq_length, num_decoder_tokens))
    decoder_LSTM = LSTM(hidden_units, return_sequences=True, dropout_U=0.2, dropout_W=0.2, return_state=True)
    decoder_outputs, _, _ = decoder_LSTM(decoder_inputs, initial_state=encoder_states)

    """___attention___"""

    attention = TimeDistributed(Dense(1, activation='tanh'))(encoder_outputs_final)
    attention = Flatten()(attention)
    # attention = Reshape((9999, -1))(attention)
    # decoder_outputs = Reshape((9999,-1))(decoder_outputs)
    print(decoder_outputs.shape)
    print(attention.shape)

    decoder_attention = TimeDistributed(Dense(1, activation='tanh'))(decoder_outputs)
    decoder_attention = Flatten()(decoder_attention)

    attention = Multiply()([decoder_outputs, attention])
    attention = Activation('softmax')(attention)

    attention = Permute((2, 1))(attention)

    """___output___"""
    decoder_dense = Dense((max_decoder_seq_length, num_decoder_tokens), activation='softmax')
    decoder_outputs = decoder_dense(attention)

    model = Model(inputs=[encoder_inputs, decoder_inputs], outputs=decoder_outputs)
    print(model.summary())

    rmsprop = RMSprop(lr=learning_rate, clipnorm=clip_norm)
    model.compile(loss='categorical_crossentropy', optimizer=rmsprop, metrics=['accuracy'])

    x_train, x_test, y_train, y_test = tts(articles, summaries, test_size=0.20)
    history = model.fit(x=[x_train, y_train], y=y_train, batch_size=batch_size, epochs=epochs, verbose=1,
                        validation_data=([x_test, y_test], y_test))

    print(model.summary())

    """___inference mode___"""
    encoder_model_inf = Model(encoder_inputs, encoder_states)

    decoder_state_input_H = Input(shape=(encoder_shape,))
    decoder_state_input_C = Input(shape=(encoder_shape,))
    decoder_state_inputs = [decoder_state_input_H, decoder_state_input_C]
    decoder_outputs, decoder_state_h, decoder_state_c = decoder_LSTM(decoder_inputs, initial_state=decoder_state_inputs)
    decoder_states = [decoder_state_h, decoder_state_c]
    decoder_outputs = decoder_dense(decoder_outputs)

    decoder_model_inf = Model([decoder_inputs] + decoder_state_inputs, [decoder_outputs] + decoder_states)

    scores = model.evaluate([x_test, y_test], y_test, verbose=1)

    print('LSTM test scores: ', scores)
    print(model.summary())

    return model, encoder_model_inf, decoder_model_inf, history


trained_model, encoder, decoder, history = encoder_decoder(input_texts, target_texts)
