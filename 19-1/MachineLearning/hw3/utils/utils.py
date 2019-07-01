import os
import numpy as np
from model.LogisticRegression import LogisticRegression
from optim.SGD import SGD
import csv
np.random.seed(503)


def load_class_data(path, filename, target_at_front, to_binary=False, normalize=False, excludes=None, shuffle=False):
    if excludes is None:
        excludes = []

    fullpath = os.path.join(path, filename)

    with open(fullpath, 'r') as f:
        lines = f.readlines()
    lines = [s.strip().split(',') for s in lines]

    header = lines[0]
    raw_data = lines[1:]
    num_feat = len(raw_data[0])
    feat_to_idx = [{} for _ in range(num_feat)]

    data = []
    for d in raw_data:
        line = []
        for i, f in enumerate(d):
            if i in excludes:
                continue

            try:
                line.append(float(f))
            except:
                if f in feat_to_idx[i]:
                    f_idx = feat_to_idx[i][f]
                else:
                    f_idx = len(feat_to_idx[i])
                    feat_to_idx[i][f] = f_idx
                line.append(f_idx)
        data.append(line)

    data = np.array(data, dtype=np.float32)
    if target_at_front:
        x, y = data[:, 1:], data[:, 0].astype(np.int32)
    else:
        x, y = data[:, :-1], data[:, -1].astype(np.int32)

    num_data = x.shape[0]
    if normalize:
        mins = np.expand_dims(np.min(x, axis=0), 0).repeat(num_data, 0)
        maxs = np.expand_dims(np.max(x, axis=0), 0).repeat(num_data, 0)
        x = (x - mins) / maxs

    # Add 1 column for bias
    bias = np.ones((x.shape[0], 1), dtype=np.float32)
    x = np.concatenate((x, bias), axis=1)

    if to_binary:
        y[y > 1] = 1

    if shuffle:
        perm = np.random.permutation(num_data)
        x = x[perm]
        y = y[perm]

    return x, y

def ContraceptData(path, filename): #3
    return load_class_data(path, filename, target_at_front=False, normalize=True, shuffle=True)

def HeartData(path, filename):
    return load_class_data(path, filename, target_at_front=False, normalize=True, shuffle=True)

def YeastData(path, filename):
    return load_class_data(path, filename, target_at_front=False, shuffle=True)

def optimizer(optim_name, epsilon=None, gamma=None):
    if optim_name == 'SGD':
        optim = SGD(gamma=gamma, epsilon=epsilon)
    else:
        raise NotImplementedError
    return optim

config = {
    'Contracept': ('Contracept', LogisticRegression),
    'Heart': ('heart', LogisticRegression),
    'Yeast':('Yeast', LogisticRegression)
}

def _initialize(data_name):
    dir_name, model = config[data_name]
    path = os.path.join('./data', dir_name)

    if data_name == 'Contracept':
        train_x, train_y = ContraceptData(path, 'train.csv')
        test_x, test_y = ContraceptData(path, 'test.csv')

    elif data_name == 'Heart':
        train_x, train_y = HeartData(path, 'train.csv')
        test_x, test_y = HeartData(path, 'test.csv')

    elif data_name == 'Yeast':
        train_x, train_y = YeastData(path, 'train.csv')
        test_x, test_y = YeastData(path, 'test.csv')
    else:
        raise NotImplementedError

    return (train_x, train_y), (test_x, test_y), model


def load_rank_data(DATA_NAME, DATA_TYPE):
    filename = (DATA_TYPE == 'pred') and 'pred.csv' or 'true.csv'

    FullPath = os.path.join('./data', DATA_NAME)
    FullPath = os.path.join(FullPath, filename)

    with open(FullPath, 'r') as f:
        data = []
        rdr = csv.reader(f)
        for line in rdr:
            data.append(line)
    data = np.array(data, dtype=np.float32)

    return data

