import numpy as np
from Answer import convolution2d, ReLU, ConvolutionLayer, MaxPoolingLayer, FCLayer, SoftmaxLayer, CNN_Classifier
from etc.check_value import *
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from PIL import Image

np.random.seed(428)

################################################################################################################
#                                                                                                              #
#                               This is a file to check your implementation.                                   #
#                               - Comment parts where you don't want to check.                                 #
#                               - Do not change any codes here.                                                #
#                               - Check your implementation before run this codes.                             #
#                                                                                                              #
################################################################################################################
#                                                                                                              #
#                               이 파일은 구현을 확인하기 위한 파일입니다.                                            #
#                               - 확인하고 싶지 않은 부분은 주석처리하세요.                                           #
#                               - 이 파일의 코드를 수정하여 실행하지 마세요. 결과가 다를 수 있습니다.                     #
#                               - 실행 전 구현을 한 후 확인하세요.                                                  #
#                                                                                                              #
################################################################################################################

# print('============================ 1. Convolution 2D ============================')
# # **** Computation Check ****
# # Check numerical difference

# height = 5
# width = 5
# x = np.random.rand(height, width)
# kernel = np.array([[-1, 1, 3],
#                    [0.5, 4, 0],
#                    [1, 2, -5]])
# conv_out = convolution2d(x, kernel, stride=1)
# correct = correct_conv2d_out
# diff = correct - conv_out

# # Difference should be 0.0 (or small in order of 1e-8)
# # 차이는 0.0 혹은 1e-8 단위 이하로 작아야 함

# print('Convolution 2D Function Check')
# print('Difference : ', diff.sum())
# print()

# # **** Convolution on Image ****
# # Your image should be same as './image/conv_img.png'

# # Read sample image
# img_size = 100
# puppy = mpimg.imread('./img/puppy.jpg')

# # Convert image to gray scale (2D)
# gray_puppy = Image.fromarray(puppy).convert('L').resize((img_size, img_size))
# gray_puppy = np.array(gray_puppy, dtype=np.float32)

# # Kernel 1. Edge detection
# kernel_1 = np.array([[-1, -1, -1],
#                      [-1, 8, -1],
#                      [-1, -1, -1]])
# # Kernel 2. Sharpen
# kernel_2 = np.array([[0, -1, 0],
#                      [-1, 5, -1],
#                      [0, -1, 0]])

# conv_1 = convolution2d(gray_puppy, kernel_1, stride=1)
# conv_2 = convolution2d(gray_puppy, kernel_2, stride=1)

# fig, (ax0, ax1, ax2) = plt.subplots(1, 3)
# ax0.imshow(puppy)
# ax1.imshow(conv_1, cmap='gray')
# ax2.imshow(conv_2, cmap='gray')

# ax0.set_title('Original Image')
# ax1.set_title('Edge Detection')
# ax2.set_title('Sharpen')

# ax0.get_xaxis().set_visible(False)
# ax0.get_yaxis().set_visible(False)
# ax1.get_xaxis().set_visible(False)
# ax1.get_yaxis().set_visible(False)
# ax2.get_xaxis().set_visible(False)
# ax2.get_yaxis().set_visible(False)

# plt.show()

# # ===========================================================================


# print('================================ 2. ReLU ==================================')
# """
# * Correct ReLU *

# Forward: 
#  [1. 0. 3.]
# Backward: 
#  [-10   0 -30]
# """
# relu = ReLU()
# temp2 = np.array([1, -0.1, 3], dtype=np.float32)
# temp3 = np.array([-10, -20, -30], dtype=np.float32)
# print('ReLU Check')
# print('Forward: \n', relu.forward(temp2))
# print('Backward: \n', relu.backward(temp3))
# print()

# # ===========================================================================



print('=========================== 3. Convolution Layer ==========================')
# Convolution with stride 1, no padding
in_channel = 1
out_channel = 3
kernel_size = 3
stride = 1
pad = 0

conv_layer = ConvolutionLayer(in_channel, out_channel, kernel_size, stride, pad)

# Forward
x = np.ones((2, in_channel, 10, 10))    # test input
W = np.array([[1, -1, 2],               # weight
              [-10, 1.5, 2.0],
              [1.5, 2.0, -3.5]])
b = np.arange(0, out_channel)           # bias

conv_layer.W[0, 0] = W                  # Set same weight for all input channel
conv_layer.W[1, 0] = W
conv_layer.W[2, 0] = W
conv_layer.b = b

conv_out = conv_layer.forward(x)
correct = correct_conv_out
diff = correct - conv_out

# Difference should be zero
print('Convolution Layer Forward Check')
print('Difference : ', diff.sum())
print()

conv_dx = conv_layer.backward(conv_out)
conv_dW = conv_layer.dW
conv_db = conv_layer.db

print('Convolution Layer Backward Check')
print('dx Difference : ', (conv_dx - correct_conv_dx).sum())
print('dW Difference : ', (conv_dW - correct_conv_dW).sum())
print('db Difference : ', (conv_db - correct_conv_db).sum())
print()
# ===========================================================================


# print('============================ 4. Pooling Layer =============================')
# kernel_size = 2
# max_pool = MaxPoolingLayer(kernel_size, stride=kernel_size)
# x = np.array([[5,   1,  3, 10],
#               [-1,  0,  4, -1],
#               [-10, -1, 11, 1],
#               [0,   1,  -5, 3]], dtype=np.float32).reshape(1, 1, 4, 4)
# pooled = max_pool.forward(x)
# d_pool = max_pool.backward(np.ones_like(pooled))

# print('Difference pool out: ', (correct_pooled - pooled).sum())
# print('Difference d_pool: ', (correct_d_pool - d_pool).sum())
# print()

# # ===========================================================================


# print('=============================== 5. FC Layer ===============================')
# fc_layer = FCLayer(2, 2)
# fc_layer.W = np.array([[-2.0, 1.25],
#                            [0.01, -1.0]])
# fc_layer.b = np.array([-1.0, 1.0])
# temp6 = np.array([[-1, 3],
#                   [0.0, 1.0]])
# temp7 = np.array([[-1, 3],
#                   [0.0, 1.0]])

# fc_forward = fc_layer.forward(temp6)
# fc_backward = fc_layer.backward(temp7)
# fc_dW = fc_layer.dW
# fc_db = fc_layer.db

# # Difference should be
# # => in the order of 1e-15 for forward
# # => 0.0 for Backward, dW, db
# print('Forward difference: ', (correct_fc_forward - fc_forward).sum())
# print('Backward difference: ', (correct_fc_backward - fc_backward).sum())
# print('dW difference: ', (correct_fc_dW - fc_dW).sum())
# print('db difference: ', (correct_fc_db - fc_db).sum())
# print()

# # ===========================================================================


# print('============================= 6. Softmax Layer ============================')
# softmax_layer = SoftmaxLayer()

# # Forward
# """
# - Correct Softmax Forward -

# [[2.48168930e-02 6.72851458e-01 3.02331649e-01]
#  [9.99831219e-01 1.23388975e-04 4.53922671e-05]
#  [1.38389653e-87 3.72007598e-44 1.00000000e+00]
#  [3.33333333e-01 3.33333333e-01 3.33333333e-01]]
# """
# x = np.array([[-2., 1.3, 0.5],
#               [-1., -10., -11.],
#               [-100, 0, 100],
#               [0, 0, 0]])
# sm = softmax_layer.forward(x)
# print(' Softmax ')
# print('[Answer]')
# print(correct_sm)
# print('[Submit]')
# print(sm)
# print()

# y = np.array([[1., 0., 0., 0.],
#               [0., 1., 0., 0.],
#               [0., 0., 1., 0.]])
# y_hat = np.array([[0.25, 0.25, 0.25, 0.25],
#                   [0.9, 0.033, 0.033, 0.033],
#                   [0.0, 0.0, 1.0, 0.0]])

# # Cross-entropy Loss
# # Backward

# # For d_softmax, difference should be very small a

# ce_loss = softmax_layer.ce_loss(y_hat, y)
# d_softmax = softmax_layer.backward(d_prev=1)
# print('CE Loss Difference : ', correct_ce_loss - ce_loss)
# print()

# print(' d_softmax ')
# print('[Answer]')
# print(correct_d_softmax)
# print('[Submit]')
# print(d_softmax)
# print()

# # ===========================================================================



print('============================= 7. CNN Classifier ===========================')
img_size = 8
output_dim = 2
learning_rate = 0.01

puppy = mpimg.imread('./img/puppy.jpg')
# Convert image to gray scale (2D)
gray_puppy = Image.fromarray(puppy).convert('L').resize((img_size, img_size))
gray_puppy = np.array(gray_puppy, dtype=np.float32)

kitten = mpimg.imread('./img/kitten.jpg')
# Convert image to gray scale (2D)
gray_kitten = Image.fromarray(kitten).convert('L').resize((img_size, img_size))
gray_kitten = np.array(gray_kitten, dtype=np.float32)


sample_x = np.zeros((2, 1, img_size, img_size))
sample_x[0, 0] += gray_puppy
sample_x[1, 0] += gray_kitten

sample_y = np.zeros((2, output_dim), dtype=np.float32)
sample_y[0, 0] = 1  # 0 is puppy
sample_y[1, 1] = 1  # 1 is kitten


CNN = CNN_Classifier()
CNN.add_layer('Conv - 1', ConvolutionLayer(in_channels=1, out_channels=3, kernel_size=2, stride=1, pad=0))
CNN.add_layer('ReLU - 1', ReLU())
CNN.add_layer('Conv - 2', ConvolutionLayer(in_channels=3, out_channels=3, kernel_size=2, stride=1, pad=0))
CNN.add_layer('ReLU - 2', ReLU())
CNN.add_layer('Max-Pool', MaxPoolingLayer(kernel_size=2, stride=2))
CNN.add_layer('FC - 1', FCLayer(27, 2))
CNN.add_layer('Softmax Layer', SoftmaxLayer())

loss = CNN.forward(sample_x, sample_y)

CNN.backward()
CNN.update(learning_rate=learning_rate)

# print(loss)
correct_loss = 11.51292546492023

# Should be zero
print('Loss difference: ', correct_loss - loss)

# Should be zero
print('FC - 1 dW, db difference: ',
      (correct_cnn_fc_dW - CNN.layers['FC - 1'].dW).sum(), '/', (correct_cnn_fc_db - CNN.layers['FC - 1'].db).sum())

# Should be different in the order of 1e-9
print('Conv - 1 dW, db difference: ',
      (correct_cnn_conv1_dW - CNN.layers['Conv - 1'].dW).sum(), '/', (correct_cnn_conv1_db - CNN.layers['Conv - 1'].db).sum())
# ===========================================================================