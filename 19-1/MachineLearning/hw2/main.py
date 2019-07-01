import numpy as np
from utils import _initialize, optimizer, plot_prediction

np.random.seed(428)

# ========================= EDIT HERE =========================
# 1. Choose DATA : Digit, Iris
# 2. Adjust Hyperparameters
# 3. Choose Optimizer : SGD / Momentum / RMSProp

# DATA
DATA_NAME = 'Digit' #None

# HYPERPARAMETERS
batch_size = 256 #None
num_epochs = 50 #None
learning_rate = 1e-5 #None
epsilon = 1e-2 #None
gamma = 9e-1 #None
show_plot = True    # show prediction sample images for DIGIT dataset

# OPTIMIZER
OPTIMIZER = 'RMSProp' #None
# =============================================================
assert DATA_NAME in ['Digit', 'Iris']
assert OPTIMIZER in ['SGD', 'Momentum', 'RMSProp']

# Load dataset, model and evaluation metric
train_data, test_data, softmax_classifier, accuracy = _initialize(DATA_NAME)
train_x, train_y = train_data
if DATA_NAME == 'Digit':
    train_x, mean_img = train_x

num_data, num_features = train_x.shape
num_label = int(train_y.max()) + 1
print('# of Training data : %d \n' % num_data)

# Make model & optimizer
model = softmax_classifier(num_features, num_label)
optim = optimizer(OPTIMIZER, gamma=gamma, epsilon=epsilon)

# TRAIN
loss = model.train(train_x, train_y, num_epochs, batch_size, learning_rate, optim)
print('Training Loss at last epoch: %.4f' % loss)

# EVALUATION
test_x, test_y = test_data
pred = model.eval(test_x)

acc = accuracy(pred, test_y)
print(OPTIMIZER, ' Accuracy on Test Data : %.2f' % acc)

# Visualize predictions for Digit dataset
if show_plot and DATA_NAME == 'Digit':
    test_x = test_x[:, 1:] + mean_img
    plot_prediction(test_x, pred, test_y)