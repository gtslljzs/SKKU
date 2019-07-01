import os
import numpy as np
from models.LinearRegression import LinearRegression
from models.LogisticRegression import LogisticRegression
from optim.Optimizer import SGD, Momentum, RMSProp

# =================================================================== #
#                   DO NOT EDIT ANY SINGLE CHARACTER                  #
# =================================================================== #


def load_reg_data(path, filename, target_at_front, normalize=False, shuffle=False):
    fullpath = os.path.join(path, filename)

    with open(fullpath, 'r') as f:
        lines = f.readlines()
    lines = [s.strip().split(',') for s in lines]

    header = lines[0]
    data = lines[1:]

    data = np.array([[float(f) for f in d] for d in data], dtype=np.float32)
    if target_at_front:
        x, y = data[:, 1:], data[:, 0]
    else:
        x, y = data[:, :-1], data[:, -1]

    num_data = x.shape[0]
    if normalize:
        mins = np.expand_dims(np.min(x, axis=0), 0).repeat(num_data, 0)
        maxs = np.expand_dims(np.max(x, axis=0), 0).repeat(num_data, 0)
        x = (x - mins) / maxs

    # Add 1 column for bias
    bias = np.ones((x.shape[0], 1), dtype=np.float32)
    x = np.concatenate((bias, x), axis=1)

    if shuffle:
        perm = np.random.permutation(num_data)
        x = x[perm]
        y = y[perm]

    return x, y

def load_class_data(path, filename, target_at_front, to_binary=False, normalize=False, excludes=None):
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
    x = np.concatenate((bias, x), axis=1)

    if to_binary:
        y[y > 1] = 1

    return x, y

def ConcreteData(path, filename):
    return load_reg_data(path, filename, target_at_front=False, normalize=True)

def GraduateData(path, filename):
    return load_reg_data(path, filename, target_at_front=False, normalize=True)

def DigitData(path, filename):
    x, y = load_class_data(path, filename, target_at_front=True, excludes=[1, 2, 3, 4, 5, 7, 8, 9], normalize=False, to_binary=True)
    x = x / 256
    return (x, y)

def TitanicData(path, filename):
    return load_class_data(path, filename, target_at_front=True, excludes=[2])

def RMSE(h, y):
    if len(h.shape) > 1:
        h = h.squeeze()
    se = np.square(h - y)
    mse = np.mean(se)
    rmse = np.sqrt(mse)
    return rmse

def Accuracy(h, y):
    if len(y.shape) == 1:
        y = np.expand_dims(y, 1)

    total = h.shape[0]
    correct = len(np.where(h==y)[0])
    accuracy = correct / total

    return accuracy

def optimizer(optim_name, epsilon=None, gamma=None):
    if optim_name == 'SGD':
        optim = SGD(gamma=gamma, epsilon=epsilon)
    elif optim_name == 'Momentum':
        optim = Momentum(gamma=gamma, epsilon=epsilon)
    elif optim_name == 'RMSProp':
        optim = RMSProp(gamma=gamma, epsilon=epsilon)
    else:
        raise NotImplementedError
    return optim

config = {
    'Concrete': ('concrete', LinearRegression, RMSE),
    'Graduate': ('graduate', LinearRegression, RMSE),
    'Titanic': ('titanic', LogisticRegression, Accuracy),
    'Digit': ('digit', LogisticRegression, Accuracy)
}

def _initialize(data_name):
    dir_name, model, metric = config[data_name]
    path = os.path.join('./data', dir_name)

    if data_name == 'Concrete':
        train_x, train_y = ConcreteData(path, 'train.csv')
        test_x, test_y = ConcreteData(path, 'test.csv')
    elif data_name == 'Graduate':
        train_x, train_y = GraduateData(path, 'train.csv')
        test_x, test_y = GraduateData(path, 'test.csv')
    elif data_name == 'Digit':
        train_x, train_y = DigitData(path, 'train.csv')
        test_x, test_y = DigitData(path, 'test.csv')
    elif data_name == 'Titanic':
        train_x, train_y = TitanicData(path, 'train.csv')
        test_x, test_y = TitanicData(path, 'test.csv')
    else:
        raise NotImplementedError

    return (train_x, train_y), (test_x, test_y), model, metric


if __name__ == '__main__':
    print('DATASET TEST START\n')
    graduate_x, graduate_y = GraduateData(os.path.join('./data', 'graduate'), 'train.csv')
    concrete_x, concrete_y = ConcreteData(os.path.join('./data', 'concrete'), 'train.csv')
    digit_x, digit_y = DigitData(os.path.join('./data', 'digit'), 'train.csv')
    titanic_x, titanic_y = TitanicData(os.path.join('./data', 'titanic'), 'train.csv')

    print('GRADUATE X, Y : ', graduate_x.shape, ',', graduate_y.shape)
    print('CONCRETE X, Y : ', concrete_x.shape, ',', concrete_y.shape)
    print('DIGIT X, Y : ', digit_x.shape, ',', digit_y.shape)
    print('TITANIC X, Y : ', titanic_x.shape, ',', titanic_y.shape)

    print('\nDATASET TEST FINISHED\n')


    print('OPTIMIZER TEST START\n')

    SGD = optimizer('SGD', 1, 1)
    Momentum = optimizer('Momentum', 1, 1)
    RMSProp = optimizer('RMSProp', 1, 1)

    print('OPTIMIZER TEST FINISHED\n')
