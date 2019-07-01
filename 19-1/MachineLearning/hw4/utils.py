import os
import numpy as np


###############################################
# -- DO NOT EDIT ANY SINGLE CHARACTER HERE -- #
###############################################

def load_iris(data_path):
    iris_path = os.path.join(data_path, 'iris')

    x_train = np.load(os.path.join(iris_path, 'iris_train_x.npy'))
    y_train = np.load(os.path.join(iris_path, 'iris_train_y.npy'))
    x_test = np.load(os.path.join(iris_path, 'iris_test_x.npy'))
    y_test = np.load(os.path.join(iris_path, 'iris_test_y.npy'))

    y_train[y_train == 1] = -1
    y_test[y_test == 1] = -1
    y_train[y_train == 0] = 1
    y_test[y_test == 0] = 1

    return x_train, y_train, x_test, y_test

def load_fashion_mnist(data_path):
    cifar_path = os.path.join(data_path, 'fashion_mnist')

    x_train = np.load(os.path.join(cifar_path, 'fashion_train_x.npy'))
    y_train = np.load(os.path.join(cifar_path, 'fashion_train_y.npy'))
    x_test = np.load(os.path.join(cifar_path, 'fashion_test_x.npy'))
    y_test = np.load(os.path.join(cifar_path, 'fashion_test_y.npy'))

    # Flatten X
    x_train = x_train.reshape(len(x_train), -1)
    x_test = x_test.reshape(len(x_test), -1)

    # Y as one-hot
    y_train = np.eye(10)[y_train]
    y_test = np.eye(10)[y_test]

    return x_train, y_train, x_test, y_test

def print_xor_hidden(x, h1, h2):
    print('\t\t  H1   H2')
    print(x[0], ' ', '%.2f' % h1[0], '%.2f' % h2[0])
    print(x[1], ' ', '%.2f' % h1[1], '%.2f' % h2[1])
    print(x[2], ' ', '%.2f' % h1[2], '%.2f' % h2[2])
    print(x[3], ' ', '%.2f' % h1[3], '%.2f' % h2[3])