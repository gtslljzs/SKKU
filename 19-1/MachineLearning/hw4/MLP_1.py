# XOR
import numpy as np
from Answer import Sigmoid, InputLayer, SigmoidOutputLayer
from utils import print_xor_hidden

"""
** MLP Class is already implemented **
** Adjust Hyperparameters and experiment **
"""

class MLP:
    def __init__(self, input_size, hidden_size, output_size):

        self.input_layer = InputLayer(input_size, hidden_size, Sigmoid)
        self.output_layer = SigmoidOutputLayer(hidden_size, output_size)

    def predict(self, x):
        x = self.input_layer.forward(x)
        pred = self.output_layer.predict(x)
        pred[pred >= 0.5] = 1
        pred[pred < 0.5] = 0
        return pred

    def loss(self, x, y):
        x = self.input_layer.forward(x)
        loss = self.output_layer.forward(x, y)
        return loss

    def gradient(self):
        d_prev = 1
        d_prev = self.output_layer.backward(d_prev=d_prev)
        self.input_layer.backward(d_prev)

    def update(self, learning_rate):
        self.input_layer.W -= self.input_layer.dW * learning_rate
        self.input_layer.b -= self.input_layer.db * learning_rate
        self.output_layer.W -= self.output_layer.dW * learning_rate
        self.output_layer.b -= self.output_layer.db * learning_rate


# =============== EDIT HERE ===============
num_epochs = 10000
learning_rate = 5e-1
print_every = 1000
# =========================================

x = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
y = np.array([[0.0], [1.0], [1.0], [0.0]])

model = MLP(input_size=2, hidden_size=2, output_size=1)
for i in range(1, num_epochs + 1):
    loss = model.loss(x, y)
    model.gradient()
    model.update(learning_rate)

    if i % print_every == 0:
        print('[EPOCH %d] Loss = %f' % (i, loss))

print('\n==== [TEST] ====')
pred = model.predict(x)
for i in range(len(x)):
    print('Pred : %d, Answer %d' % (pred[i], y[i]))
print()

print('Hidden Node Values')
h = model.input_layer.forward(x)
h1, h2 = h[:, 0], h[:, 1]
print_xor_hidden(x, h1, h2)
