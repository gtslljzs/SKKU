import time
import numpy as np
from utils import load_small_mnist
from Answer import CNN_Classifier, ConvolutionLayer, MaxPoolingLayer, FCLayer, SoftmaxLayer, ReLU
import matplotlib.pyplot as plt

np.random.seed(428)

x_train, y_train, x_test, y_test = load_small_mnist('./data')

num_train, in_channnel, height, width = x_train.shape
num_class = y_train.shape[1]

CNN = CNN_Classifier()

# =============================== EDIT HERE ===============================
###########################################################################
#                                                                         #
#   Build CNN Architecture and do experiment.                             #
#   You can add layer as below examples.                                  #
#   NOTE THAT, layers are executed in the order that you add.             #
#   It might take more than 5 minutes for an epoch.                       #
#   Enjoy.                                                                #
#                                                                         #
#   CNN 모델을 구성하고 실험하세요                                             #
#   레이어는 아래 예제와 같이 추가할 수 있습니다.                                 #
#   주의하세요! 레이어들은 추가한 순서대로 실행됩니다.                             #
#   Layer에 따라, 구현에 따라 다르지만 1 Epoch에 5분 이상 걸려도 정상입니다.        #
#   ^^                                                                    #
#                                                                         #
###########################################################################

# Add Layers, Layer 추가 예제
# 아래 예시를 참고하여 과제에 주어진 대로 (혹은 과제와 별개로 원하는 대로) Layer를 추가하세요.

# Convolution Layer
CNN.add_layer('Conv-1', ConvolutionLayer(in_channels=in_channnel, out_channels=8, kernel_size=3, stride=1, pad=1))

# ReLU Layer
CNN.add_layer('ReLU-1', ReLU())
CNN.add_layer('Conv-2', ConvolutionLayer(in_channels=8, out_channels=8, kernel_size=3, stride=1, pad=1))
CNN.add_layer('ReLU-2', ReLU())

# Max-pool Layer
CNN.add_layer('Max-Pool', MaxPoolingLayer(kernel_size=2, stride=2))

# FC Layer
CNN.add_layer('FC-1', FCLayer(input_dim=1568, output_dim=500))
CNN.add_layer('FC-2', FCLayer(input_dim=500, output_dim=5))

# Softmax Layer
# 이 layer는 항상 마지막에 추가
CNN.add_layer('Softmax Layer', SoftmaxLayer())

# Model Architecture 출력
CNN.summary()

# Hyper-parameters
num_epochs = 5
learning_rate = 1e-3
print_every = 1

# =========================================================================

batch_size = 128
train_accuracy = []
test_accuracy = []

best_epoch = -1
best_acc = -1

print('Training Starts...')
num_batch = int(np.ceil(num_train / batch_size))
for epoch in range(1, num_epochs + 1):
    start = time.time()
    epoch_loss = 0.0
    for b in range(0, len(x_train), batch_size):
        x_batch = x_train[b: b + batch_size]
        y_batch = y_train[b: b + batch_size]

        loss = CNN.forward(x_batch, y_batch)
        epoch_loss += loss

        CNN.backward()
        CNN.update(learning_rate)
        print('[%4d / %4d]\t batch loss : %.4f' % (b, num_train, loss))

    epoch_loss /= num_batch
    end = time.time()
    diff = end - start
    print('Epoch %d took %.2f seconds\n' % (epoch, diff))

    if epoch % print_every == 0:
        print('[EPOCH %d] Loss = %.5f\n' % (epoch, epoch_loss))

        print('Evaluate model accuracy...')
        # TRAIN ACCURACY
        prob = CNN.predict(x_train)
        pred = np.argmax(prob, -1).astype(int)
        true = np.argmax(y_train, -1).astype(int)

        correct = len(np.where(pred == true)[0])
        total = len(true)
        train_acc = correct / total
        train_accuracy.append(train_acc)

        # TEST ACCURACY
        prob = CNN.predict(x_test)
        pred = np.argmax(prob, -1).astype(int)
        true = np.argmax(y_test, -1).astype(int)

        correct = len(np.where(pred == true)[0])
        total = len(true)
        test_acc = correct / total
        test_accuracy.append(test_acc)


        print('Train Accuracy = %.3f' % train_acc + ' // ' + 'Test Accuracy = %.3f' % test_acc)

        if best_acc < test_acc:
            print('Best Accuracy updated (%.4f => %.4f)' % (best_acc, test_acc))
            best_acc = test_acc
            best_epoch = epoch

print('Training Finished...!!')
print('Best acc : %.2f at epoch %d' % (best_acc, best_epoch))


# =============================== EDIT HERE ===============================
###########################################################################
#                                                                         #
#   Draw a plot of train/test accuracy                                    #
#   X-axis : Epoch, Y-axis : train_accuracy & test_accuracy               #
#   Draw train_acc-epoch, test_acc-epoch graph in ine plot                #
#                                                                         #
#   Train/test 정화도의 plot을 그리세요                                       #
#   X-축 : Epoch, Y-축 : train_accuracy & test_accuracy                    #
#   train_acc-epoch, test_acc-epoch의 그래프를 한 plot에 같이 그리세요         #
#                                                                         #
###########################################################################

x_axis = list(range(len(test_accuracy)))
plt.plot(x_axis, train_accuracy, 'b-', label='Train Acc.')
plt.plot(x_axis, test_accuracy, 'r-', label='Test Acc.')
plt.title('Train & Test Accuracy')
plt.xlabel('Epochs')
plt.ylabel('Accuracy')
plt.legend()
plt.show()

# =========================================================================
plt.show()