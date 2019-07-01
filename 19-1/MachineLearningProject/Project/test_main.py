import os, sys
import test_dataloader as dd
# from keras.optimizers import  *
# from keras.callbacks import  *

itokens, otokens = dd.make_seq2seq_dict('./data/train_article.txt', dict_file='./data/dict.txt')

print('seq 1 words: ', itokens.num())
print('seq 2 words: ', otokens.num())

from transformer import Transformer, LRSchedulerPerStep

