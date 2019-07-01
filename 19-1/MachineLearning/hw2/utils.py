import os
import numpy as np
import matplotlib.pyplot as plt
from model.SoftmaxClassifier import SoftmaxClassifier
from optim.Optimizer import SGD, Momentum, RMSProp


# ================================================== #
#       DO NOT EDIT ANY SINGLE CHARACTER HERE.       #
# IT MIGHT CHANGE THE WHOLE TRAINING PROCESS BADLY.  #
# ================================================== #

def load_class_data(path, filename, target_at_front, normalize=False, shuffle=False):
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

    if shuffle:
        perm = np.random.permutation(num_data)
        x = x[perm]
        y = y[perm]

    return x, y

def DigitData(path, filename):
    x, y = load_class_data(path, filename, target_at_front=True, normalize=False, shuffle=False)
    mean_img = x[:, 1:].mean()
    x[:, 1:] -= mean_img
    return (x, y), mean_img

def IrisData(path, filename):
    x, y = load_class_data(path, filename, target_at_front=False, normalize=False, shuffle=True)
    return (x, y)

def RMSE(h, y):
    if len(h.shape) > 1:
        h = h.squeeze()
    se = np.square(h - y)
    mse = np.mean(se)
    rmse = np.sqrt(mse)
    return rmse

def Accuracy(h, y):
    """
    h : (N, ), predicted label
    y : (N, ), correct label
    """
    if len(h.shape) == 1:
        h = np.expand_dims(h, 1)
    if len(y.shape) == 1:
        y = np.expand_dims(y, 1)

    total = h.shape[0]
    correct = len(np.where(h==y)[0])
    accuracy = correct / total

    return accuracy

def plot_prediction(test_x, pred, true):
    num_data = test_x.shape[0]
    test_x = test_x.reshape(num_data, 28, 28)

    pred = pred.squeeze().tolist()
    true = true.squeeze().tolist()

    sample_num = 10
    sample_idx = np.random.permutation(num_data)[-sample_num:]

    sample_x = [test_x[s] for s in sample_idx]
    sample_pred = [pred[s] for s in sample_idx]
    sample_true = [true[s] for s in sample_idx]

    fig, axes = plt.subplots(2, 5, figsize=(10, 10), subplot_kw={'xticks': [], 'yticks': []})

    fig.subplots_adjust(hspace=0.3, wspace=0.05)
    for i, (ax, xx) in enumerate(zip(axes.flat, sample_x)):
        img = xx / 255.
        ax.imshow(img, cmap='gray')
        ax.set_title('True: %s / Pred: %s' % (sample_true[i], sample_pred[i]), fontsize=10)
    plt.show()

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
    'Digit': ('digit', SoftmaxClassifier, Accuracy),
    'Iris': ('iris', SoftmaxClassifier, Accuracy)
}

def _initialize(data_name):
    dir_name, model, metric = config[data_name]
    path = os.path.join('./data', dir_name)

    if data_name == 'Digit':
        (train_x, train_y), mean_img = DigitData(path, 'train.csv')
        (test_x, test_y), _ = DigitData(path, 'test.csv')
        train_x = (train_x, mean_img)
    elif data_name == 'Iris':
        train_x, train_y = IrisData(path, 'train.csv')
        test_x, test_y = IrisData(path, 'test.csv')
    else:
        raise NotImplementedError

    return (train_x, train_y), (test_x, test_y), model, metric