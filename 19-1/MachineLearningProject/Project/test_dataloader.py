import os, sys, time, random
import h5py
import numpy as np
import test_utils

class TokenList:
    def __init__(self, token_list):
        self.id2t = ['<PAD>', '<UNK>', '<S>', '</S>'] + token_list
        self.t2id = {v:k for k, v in enumerate(self.id2t)}

    def id(self, x):    return self.t2id.get(x, 1)
    def token(self, x): return self.id2t[x]
    def num(self):      return len(self.id2t)
    def start_id(self): return 2
    def end_id(self):   return 3


def make_seq2seq_dict(fn=None, min_freq=5, delimiter=' ', dict_file=None):
    if dict_file is not None and os.path.exists(dict_file):
        print('loading ', dict_file)
        lst = test_utils.load_list(dict_file)
        midpos = lst.index('<@@@>')
        itokens = TokenList(lst[:midpos])
        otokens = TokenList(lst[midpos+1:])

        return itokens, otokens

    data = test_utils.load_csv(fn)
    wdicts = [{}, {}]

    for ss in data:
        for seq, wd in zip(ss, wdicts):
            for w in seq.split(delimiter):
                wd[w] = wd.get(w,0) + 1

    wlists = []
    for wd in wdicts:
        wd = test_utils.freq_dict2list(wd)
        wlist = [x for x, y in wd if y >= min_freq]
        wlists.append(wlist)

    print(wlists[0])
    print(wlists[1])

    print('seq 1 words: ', len(wlists[0]))
    print('seq 2 words: ', len(wlists[1]))

    itokens = TokenList(wlists[0])
    otokens = TokenList(wlists[1])

    if dict_file is not None:
        test_utils.save_list(wlists[0] + ['<@@@>'] + wlists[1], dict_file)

    return itokens, otokens


def make_seq2seq_data(fn=None, itokens=None, otokens=None, delimiter=' ', h5_file=None, max_len=200):
    if h5_file is not None and os.path.exists(h5_file):
        print('loading', h5_file)

        with h5py.File(h5_file) as dfile:
            X, Y = dfile['X'][:], dfile['Y'][:]

        return X, Y

    data = test_utils.load_csvg(fn)
    Xs = [[], []]

    for ss in data:
        for seq, xs in zip(ss, Xs):
            xs.append(list(seq.split(delimiter)))

    X, Y = pad_to_longest(Xs[0], itokens, max_len), pad_to_longest(Xs[1], otokens, max_len)

    if h5_file is not None:
        with h5py.File(h5_file, 'w') as dfile:
            dfile.create_dataset('X', data=X)
            dfile.create_dataset('Y', data=Y)

    return X, Y

def pad_to_longest(xs, tokens, max_len=999):
    longest = min(len(max(xs, key=len)) + 2, max_len)
    X = np.zeros((len(xs), longest), dtype='int32')
    X[:, 0] = tokens.start_id()

    for i, x in enumerate(xs):
        x = x[: max_len - 2]
        for j, z in enumerate(x):
            X[i, 1+j] = tokens.id(z)

        X[i, 1+len(x)] = tokens.end_id()

    return X
