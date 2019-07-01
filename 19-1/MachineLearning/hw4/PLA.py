import numpy as np
from utils import load_iris
from Answer import Perceptron

"""
Main code to run Perceptron Learning Algorithm
"""

x_train, y_train, x_test, y_test = load_iris('./data')

num_features = x_train.shape[1]
perceptron = Perceptron(num_features)

# =============== EDIT HERE ===============
# Try one of two train algorithm at a time (stochastic or batch)
# Test acc. should be 1.0 at both case

learning_rate = 0.001

perceptron.stochastic_train(x_train, y_train, learning_rate)
# perceptron.batch_train(x_train, y_train, learning_rate)

# =========================================

pred = perceptron.forward(x_test)
correct = len(np.where(pred.reshape(-1) == y_test)[0])
total = len(y_test)

print('Test Accuracy : %.2f' % (correct / total))