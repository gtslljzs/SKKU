# -*- coding: utf-8 -*-

from pickle import dump
from pickle import load
import pandas as pd
import csv

from utils import clean_text
from utils import _romanize
from utils import make_dict
from utils import seq2vec
from utils import _word2vec

########## 데이터 전처리 ##########
# train_article = pd.read_csv('./data/DATASET/train_X.csv', header=None)
# train_summary = pd.read_csv('./data/DATASET/train_Y.csv', header=None)
# test_article = pd.read_csv('./data/DATASET/test_X.csv', header=None)
# test_summary = pd.read_csv('./data/DATASET/test_Y.csv', header=None)
#
# # 텍스트 정제
# clean_article = clean_text(train_article)
# clean_summary = clean_text(train_summary)
# clean_test_article = clean_text(test_article)
# clean_test_summary = clean_text(test_summary)

#
# train_dataset_clean = list()
# for i, text in enumerate(clean_article):
#     train_dataset_clean.append({'article':text, 'summary':clean_summary[i]})
# dump(train_dataset_clean, open('./data/train_dataset_clean.pkl','wb'))
#
# test_dataset_clean = list()
# for i, text in enumerate(clean_test_article):
#     test_dataset_clean.append({'article':text, 'summary':clean_test_summary[i]})
# dump(test_dataset_clean, open('./data/test_dataset_clean.pkl', 'wb'))


# 텍스트 영어화
# roman_article = _romanize(clean_article)
# roman_summary = _romanize(clean_summary)
# dataset_romanized = list()
# for i, text in enumerate(roman_article):
#     dataset_romanized.append({'article':text, 'summary':roman_summary[i]})
# dump(dataset_romanized, open('./data/dataset_romanized.pkl', 'wb'))

# max_len = 0
# avg_len = 0
# for i in clean_article:
#     length = len(i)
#     avg_len += length
#     if length > max_len:
#         max_len = length
#
# print('max: ', max_len)
# print('avg: ', (int)(avg_len / 9996))
#
# max_len = 0
# avg_len = 0
# for i in clean_summary:
#     length = len(i)
#     avg_len += length
#     if length > max_len:
#         max_len = length
#
# print('max: ', max_len)
# print('avg: ', (int)(avg_len/9996))



# 단어 사전 만들기
train_data = load(open('./data/train_dataset_clean.pkl', 'rb'))
test_data = load(open('./data/test_dataset_clean.pkl', 'rb'))
x_train = []
y_train = []
x_test = []
y_test = []

for sample in train_data:
    input_text = sample['article']
    target_text = sample['summary']
    x_train.append(input_text)
    y_train.append(target_text)

for sample in test_data:
    input_text = sample['article']
    target_text = sample['summary']
    x_test.append(input_text)
    y_test.append(target_text)

embedding = _word2vec(x_train)
print(embedding.wv)


# print(len(x_train))
# print(x_train[0])
# print(len(y_train))
# print(y_train[0])
# print(len(x_test))
# print(x_test[0])
# print(len(y_test))
# print(y_test[0])
#
# with open('train.tsv', 'w', encoding='utf-8', newline='') as f:
#     wr = csv.writer(f, delimiter='\t')
#     train = list()
#     for i in range(len(x_train)):
#         temp = [x_train[i], y_train[i]]
#         wr.writerow(temp)
#         train.append(temp)
#     # print(train[0])
#     f.close()
#
# with open('test.tsv', 'w', encoding='utf-8', newline='') as f:
#     wr = csv.writer(f, delimiter='\t')
#     test = list()
#     for i in range(len(x_test)):
#         temp = [x_test[i], y_test[i]]
#         wr.writerow(temp)
#         test.append(temp)
#     # print(test[0])
#     f.close()

# dataset = pd.read_csv("train.tsv", delimiter='\t', header=None)
# print(dataset)



# with open('train_story.txt', 'w') as f:
#     for text in x_train:
#         f.write(text + '\n')
#
#     f.close()
#
# with open('train_summ.txt', 'w') as f:
#     for text in y_train:
#         f.write(text + '\n')
#
#     f.close()
#
# with open('eval_story.txt', 'w') as f:
#     for text in x_test:
#         f.write(text + '\n')
#
#     f.close()
#
# with open('eval_summ.txt', 'w') as f:
#     for text in y_test:
#         f.write(text + '\n')
#
#     f.close()

# dict = make_dict(y_train)
# dump(dict, open('./data/dict.pkl', 'wb'))

# Word embedding

# vec_article = seq2vec(x_train, padding=True, size=500)
# vec_summary = seq2vec(y_train, padding=True, size=500)
# vec_article, vec_summary = word2vec(data, padding=True, size=500)
# print(vec_article.shape)
# print(vec_summary.shape)
#
# dataset_vectorized = list()
# for i, text in enumerate(vec_article):
#     dataset_vectorized.append({'article':text, 'summary':vec_summary[i]})
# dump(dataset_vectorized, open('./data/dataset_vectorized.pkl','wb'))

# dic = load(open('./data/dict.pkl', 'rb'))
# print(dic)

