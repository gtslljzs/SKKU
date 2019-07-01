import numpy as np
from utils.utils import _initialize
from optim.SGD import SGD
from utils.Answer import evaluation_test1
from model.LogisticRegression import LogisticRegression


import matplotlib.pyplot as plt

np.random.seed(503)

# ========  You can setting =========
DATA_NAME = 'Yeast'
#====================================

# HYPERPARAMETERS
batch_size = 32
num_epochs = 100
learning_rate = 0.01

# OPTIMIZER
OPTIMIZER = 'SGD'

assert DATA_NAME in ['Contracept', 'Heart', 'Yeast']
assert OPTIMIZER == 'SGD'

# Load dataset, model and evaluation metric
train_data, test_data, logistic_regression = _initialize(DATA_NAME)
train_x, train_y = train_data

num_data, num_features = train_x.shape
num_class = len(np.unique(train_y))

optim = SGD()

model = LogisticRegression(num_features)
loss, epoch = model.train(train_x, train_y, batch_size, num_epochs, learning_rate, optim)

test_x, test_y = test_data
hypo, pred = model.predict(test_x)
pred = np.squeeze(pred)

# ========  Edit here =========
result = evaluation_test1(test_y, pred, at=90)
# =============================

for key in result.keys():
    print(key, '\t\t\t:\t\t %.6f' % result[key])
