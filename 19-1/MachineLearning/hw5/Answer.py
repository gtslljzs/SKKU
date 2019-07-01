from collections import OrderedDict
import numpy as np

np.random.seed(123)

def softmax(z):
    # Numerically stable softmax. Already implemented.
    z = z - np.max(z, axis=1, keepdims=True)
    _exp = np.exp(z)
    _sum = np.sum(_exp, axis=1, keepdims=True)
    sm = _exp / _sum
    return sm
 

def convolution2d(x, kernel, stride):
    """
    Convolution 2D : Do Convolution on 'x' with filter = 'kernel', stride = 'stride'
    입력 x에 대해 'kernel'을 filter로 사용하여 2D Convolution을 수행하시오.

    [Input]
    x: 2D data (e.g. image)
    - Shape : (Height, Width)

    kernel : 2D convolution filter
    - Shape : (Kernel size, Kernel size)

    stride : Stride size
    - dtype : int

    [Output]
    conv_out : convolution result
    - Shape : (Conv_Height, Conv_Width)
    - Conv_Height & Conv_Width can be calculated using 'Height', 'Width', 'Kernel size', 'Stride'
    """
    height, width = x.shape
    kernel_size = kernel.shape[0]
    conv_out = None
    # =============================== EDIT HERE ===============================
    
    f_height = int((height - kernel_size) / stride + 1)
    f_width = int((width - kernel_size) / stride + 1)
    conv_out = np.zeros((f_height, f_width))

    for i in range(f_height):
        for j in range(f_width):
            h = i * stride
            w = j * stride
            conv_out[i][j] = np.sum(x[h : h + kernel_size, w : w + kernel_size] * kernel)
    
    # =========================================================================
    return conv_out

class ReLU:
    """
    ReLU Function. ReLU(x) = max(0, x)
    Implement forward & backward path of ReLU.

    ReLU(x) = x if x > 0. 0 otherwise.
    Be careful. It's '>', not '>='.
    (ReLU in previous HW might be different.)
    """

    def __init__(self):
        # 1 (True) if ReLU input <= 0
        self.zero_mask = None

    def forward(self, z):
        """
        ReLU Forward.
        ReLU(x) = max(0, x)

        z --> (ReLU) --> out

        [Inputs]
            z : ReLU input in any shape.

        [Outputs]
            out : ReLU(z).
        """
        out = None
        # =============================== EDIT HERE ===============================

        self.zero_mask = (z < 0)
        out = np.copy(z)
        out[self.zero_mask] = 0
        
        # =========================================================================
        return out

    def backward(self, d_prev):
        """
        ReLU Backward.

        z --> (ReLU) --> out
        dz <-- (dReLU) <-- d_prev(dL/dout)

        [Inputs]
            d_prev : Gradients until now.
            d_prev = dL/dk, where k = ReLU(z).

        [Outputs]
            dz : Gradients w.r.t. ReLU input z.
        """
        dz = None
        # =============================== EDIT HERE ===============================

        d_prev[self.zero_mask] = 0
        dz = d_prev

        # =========================================================================
        return dz

    def update(self, learning_rate):
        # NOT USED IN ReLU
        pass

    def summary(self):
        return 'ReLU Activation'

################################################################################################################
#    ** ConvolutionLayer **                                                                                    #
#   Single Convolution Layer.                                                                                  #
#                                                                                                              #
#   Given input images,                                                                                        #
#   'Convolution Layer' do convolution on input with kernels and convolution options (stride, pad ...).        #
#                                                                                                              #
#   You need to implement forward and backward pass of single convolution layer.                               #
#   (This is NOT an entire CNN model.)                                                                         #
#                                                                                                              #
#    ** ConvolutionLayer **                                                                                    #
#   단일 합성곱 계층                                                                                              #
#                                                                                                              #
#   이미지 입력을 받아서, 합성곱 계층은 주어진 세팅과 kernel(self.W) 및 bias(self.b)를 활용하여#                         #
#   입력에 대하여 합성곱을 수행한다.                                                                                #
#                                                                                                              #
#   합성곱 계층의 Forward, Backward 함수를 구현하시오                                                               #
#   (CNN 모델 전체가 아닙니다.)                                                                         #
################################################################################################################


class ConvolutionLayer:
    def __init__(self, in_channels, out_channels, kernel_size, stride=1, pad=0):
        self.W = np.random.randn(out_channels, in_channels, kernel_size, kernel_size)
        self.b = np.zeros(out_channels, dtype=np.float32)
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kernel_size = kernel_size
        self.stride = stride
        self.pad = pad

    def forward(self, x):
        """
        Convolution Layer Forward.
        합성곱 신경망 Forward.

        [Input]
        x: 4-D input batch data
        - Shape : (Batch size, In Channel, Height, Width)

        [Output]
        conv_out : convolution result
        - Shape : (Conv_Height, Conv_Width)
        - Conv_Height & Conv_Width can be calculated using 'Height', 'Width', 'Kernel size', 'Stride'

        이 부분은 구현이 필요없습니다.
        """
        self.x = x
        batch_size, in_channel, _, _ = x.shape
        conv = self.convolution(x, self.W, self.b, self.stride, self.pad)
        self.output_shape = conv.shape

        return conv

    def convolution(self, x, kernel, bias=None, stride=1, pad=0):
        """
        Convolution Operation.
        Add bias if bias is not none

        Use
        variables --> self.W, self.b, self.stride, self.pad, self.kernel_size
        function --> convolution2d (what you already implemented above.)

        위 변수와 함수를 활용하여 구현하세요.
        bias는 None이 아닐 때, 더해집니다.

        [Input]
        x: 4-D input batch data
        - Shape : (Batch size, In Channel, Height, Width)
        kernel: 4-D convolution filter
        - Shape : (Out Channel, In Channel, Kernel size, Kernel size)
        bias: 1-D bias
        - Shape : (Out Channel)
        - default : None
        stride : Stride size
        - dtype : int
        - default : 1
        pad: pad value, how much to pad
        - dtype : int
        - default : 0

        [Output]
        conv_out : convolution result
        - Shape : (Batch_size, Out Channel, Conv_Height, Conv_Width)
        - Conv_Height & Conv_Width can be calculated using 'Height', 'Width', 'Kernel size', 'Stride'
        """
        batch_size, in_channel, _, _ = x.shape

        if pad > 0:
            x = self.zero_pad(x, pad)

        _, _, height, width = x.shape
        out_channel, _, kernel_size, _ = kernel.shape
        assert x.shape[1] == kernel.shape[1]

        conv = None
        # =============================== EDIT HERE ===============================

        conv_height = int((height - kernel_size) / stride) + 1
        conv_width = int((width - kernel_size) / stride) + 1
        conv = np.zeros([batch_size, out_channel, conv_height, conv_width])
        
        for b in range(batch_size):
            for oc in range(out_channel):
                for ic in range(in_channel):
                    conv[b,oc,:] += convolution2d(x[b,ic,:], kernel[oc,ic,:], stride)
                    if bias[oc] != None:
                        conv[b,oc,:] += bias[oc]
        
        # =========================================================================
        return conv

    def backward(self, d_prev):
        """
        Convolution Layer Backward.
        Compute derivatives w.r.t x, W, b (self.x, self.W, self.b)

        x, W, b에 대한 gradient (dx, dW, db)를 구하시오.

        ** [HINT] **
        See lecture notes.
        "convolution2d" & "self.convolution" functions might be useful I guess...

        강의 노트를 보세요. (강의 노트에는 1 input channel, 1 output channel간의 gradient 계산)
        "convolution2d" & "self.convolution" 함수가 유용할지도 모릅니다...

        [Input]
        d_prev: Gradients value so far in back-propagation process.

        [Output]
        self.dx : Gradient values of input x (self.x)
        - Shape : (Batch size, channel, Heigth, Width)

        """
        batch_size, in_channel, height, width = self.x.shape
        out_channel, _, kernel_size, _ = self.W.shape

        if len(d_prev.shape) < 3:
            d_prev = d_prev.reshape(*self.output_shape)

        self.dW = np.zeros_like(self.W, dtype=np.float64)
        self.db = np.zeros_like(self.b, dtype=np.float64)
        dx = np.zeros_like(self.x, dtype=np.float64)
        # =============================== EDIT HERE ===============================
        
        x_pad = np.zeros_like(self.x)
        dx_pad = np.zeros_like(dx)
        x_pad = self.zero_pad(self.x, self.pad)
        dx_pad = self.zero_pad(dx, self.pad)
        _, _, d_prev_height, d_prev_width = d_prev.shape
        
        for b in range(batch_size):
            single_x_pad = x_pad[b]
            single_dx_pad = dx_pad[b]
            
            for h in range(d_prev_height):
                for w in range(d_prev_width):
                    for c in range(out_channel):
                        ver_start = h * self.stride
                        ver_end = ver_start + kernel_size
                        hor_start = w * self.stride
                        hor_end = hor_start + kernel_size

                        x_slice = single_x_pad[:, ver_start:ver_end, hor_start:hor_end]
                        single_dx_pad[:, ver_start:ver_end, hor_start:hor_end] += self.W[c, :, :, :] * d_prev[b, c, h, w]
                        self.dW[c, :, :, :] += x_slice * d_prev[b, c, h, w]
                        self.db[c] += d_prev[b, c, h, w]
            
            if self.pad > 0:
                dx[b, :, :, :] = single_dx_pad[:, self.pad:-self.pad, self.pad:-self.pad]
            else:
                dx[b, :, :, :] = single_dx_pad[:, :, :]
            
        # =========================================================================
        return dx

    def zero_pad(self, x, pad):
        """
        Zero padding
        Given x and pad value, pad input 'x' around height & width.

        입력 x에 대하여 좌우상하 'pad'만큼을 '0'으로 padding 하시오.

        [Input]
        x: 4-D input batch data
        - Shape : (Batch size, In Channel, Height, Width)

        pad: pad value. how much to pad on one side.
        e.g. pad=2 => pad 2 zeros on left, right, up & down.

        [Output]
        padded_x : padded x
        - Shape : (Batch size, In Channel, Padded_Height, Padded_Width)
        """
        padded_x = None
        batch_size, in_channel, height, width = x.shape
        # =============================== EDIT HERE ===============================

        padded_x = np.pad(x, ((0, 0), (0, 0), (pad, pad), (pad, pad)), 'constant', constant_values = (0, 0))

        # =========================================================================
        return padded_x

    def update(self, learning_rate):
        # Update weights
        self.W -= self.dW * learning_rate
        self.b -= self.db * learning_rate

    def summary(self):
        return 'Filter Size : ' + str(self.W.shape) + \
               ' Stride : %d, Zero padding: %d' % (self.stride, self.pad)

################################################################################################################
#    ** Max-Pooling Layer **                                                                                   #
#   Single Max-Pooling Layer.                                                                                  #
#                                                                                                              #
#   Given input images,                                                                                        #
#   'Max-Pooling Layer' max_pool (or subsample) maximum value in certain region of input                       #
#                                                                                                              #
#   You need to implement forward and backward pass of single max-pooling layer.                               #
#   (This is NOT an entire model.)                                                                             #
#                                                                                                              #
#   이미지를 입력으로 받아,                                                                                        #
#   'Max-Pooling Layer'는 stride를 해가며 해당하는 구역에서 가장 큰 값을 뽑는다.                                       #
#                                                                                                              #
#   Max-pooling layer의 Forward와 Backward를 구현하시오.                                                          #
################################################################################################################

class MaxPoolingLayer:
    def __init__(self, kernel_size, stride=2):
        self.kernel_size = kernel_size
        self.stride = stride

    def forward(self, x):
        """
        Max-Pooling Layer Forward. Pool maximum value by striding kernel.

        If image size is not divisible by pooling size (e.g. 4x4 image, 3x3 pool, stride=2),
        only pool from valid region, not go beyond the input image.
        4x4 image, 3x3 pool, stride=2 => 1x1 out
        (* Actually you should set kernel/pooling size, stride and pad properly, so that this does not happen.)

        만야 이미지 크기가 pooling 크기로 나누어 떨어지지 않는다면 (e.g. 4x4 image, 3x3 pool, stride=2),
        padding 없이 유효한 범위 내에서만 수행한다.
        4x4 image, 3x3 pool, stride=2 => 1x1 out
        (* 사실 애초에 kernel 크기, stride, pooling, padding을 이런 상황이 없이 세팅하는 것이 좋다.)

        [Input]
        x: 4-D input batch data
        - Shape : (Batch size, In Channel, Height, Width)

        [Output]
        pool_out : max_pool result
        - Shape : (Batch_size, Out Channel, Pool_Height, Pool_Width)
        - Pool_Height & Pool_Width can be calculated using 'Height', 'Width', 'Kernel size', 'Stride'
        """
        max_pool = None
        batch_size, channel, height, width = x.shape
        # Where it came from x. (1 if it is pooled, 0 otherwise.)
        # Might be useful when backward
        self.mask = np.zeros_like(x)
        # =============================== EDIT HERE ===============================

        # Define the dimensions of the output
        self.x = x
        out_height = int(1 + (height - self.kernel_size) / self.stride)
        out_width = int(1 + (width - self.kernel_size) / self.stride)
        out_channel = channel

        # Initialize output matrix
        max_pool = np.zeros((batch_size, out_channel, out_height, out_width))
        
        # Start max pooling
        for b in range(batch_size):
            for h in range(out_height):
                for w in range(out_width):
                    for c in range(out_channel):

                        ver_start = h * self.stride
                        ver_end = ver_start + self.kernel_size
                        hor_start = w * self.stride
                        hor_end = hor_start + self.kernel_size

                        # Max pooling
                        x_slice = x[b, c, ver_start:ver_end, hor_start:hor_end]
                        max_pool[b, c, h, w] = np.max(x_slice)

                        # Create the mask
                        self.mask[b, c, ver_start:ver_end, hor_start:hor_end] = x_slice == np.max(x_slice)

        # =========================================================================
        self.output_shape = max_pool.shape
        return max_pool


    def backward(self, d_prev=1):
        """
        Max-Pooling Layer Backward.
        In backward pass, Max-pool distributes gradients to where it came from in forward pass.

        [Input]
        d_prev: Gradients value so far in back-propagation process.
        - Shape can be varies since either Conv. layer or FC-layer can follow.
            (Batch_size, Channel, Height, Width)
            or
            (Batch_size, FC Dimension)

        [Output]
        d_max : max_pool gradients
        - Shape : (batch_size, channel, height, width) - same shape as input x
        """
        d_max = None
        if len(d_prev.shape) < 3:
            d_prev = d_prev.reshape(*self.output_shape)
        batch, channel, height, width = d_prev.shape
        # =============================== EDIT HERE ===============================
        
        # Initialize d_max
        d_max = np.zeros_like(self.mask)

        for b in range(batch):
            for h in range(height):
                for w in range(width):
                    for c in range(channel):

                        ver_start = h * self.stride
                        ver_end = ver_start + self.kernel_size
                        hor_start = w * self.stride
                        hor_end = hor_start + self.kernel_size
                        
                        d_max[b, c, ver_start:ver_end, hor_start:hor_end] = np.multiply(d_prev[b, c, h, w], self.mask[b, c, ver_start:ver_end, hor_start:hor_end])

        # =========================================================================
        return d_max

    def update(self, learning_rate):
        # NOT USED IN MAX-POOL
        pass

    def summary(self):
        return 'Pooling Size : ' + str((self.kernel_size, self.kernel_size)) + \
               ' Stride : %d' % (self.stride)

################################################################################################################
#    ** Fully-Connected Layer **                                                                               #
#   Single Fully-Connected Layer.                                                                              #
#                                                                                                              #
#   Given input features,                                                                                      #
#   FC layer linearly transform input with weights (self.W) & bias (self.b)                                    #
#                                                                                                              #
#   입력 특징을 받아,                                                                                             #
#   FC Layer는 weight와 bias를 활용하여 특징을 변형한다.                                                            #
#                                                                                                              #
#   You need to implement forward and backward pass                                                            #
#   This FC Layer works same as one in HW-4, so you can copy your codes if you need any.                       #
#   이 FC Layer는 HW-4의 Hidden Layer와 동일 혹은 비슷하게 동장하므로 가져올 코드가 있다면 참조해도 좋다.                  #
#                                                                                                              #
################################################################################################################

class FCLayer:
    def __init__(self, input_dim, output_dim):
        # Weight Initialization
        self.W = np.random.randn(input_dim, output_dim) / np.sqrt(input_dim / 2)
        self.b = np.zeros(output_dim)

    def forward(self, x):
        """
        FC Layer Forward.
        Use variables : self.x, self.W, self.b

        [Input]
        x: Input features.
        - Shape : (Batch size, In Channel, Height, Width)
        or
        - Shape : (Batch size, input_dim)

        [Output]
        self.out : fc result
        - Shape : (Batch size, output_dim)
        """
        if len(x.shape) > 2:
            batch_size = x.shape[0]
            x = x.reshape(batch_size, -1)
        self.x = x
        # =============================== EDIT HERE ===============================
        
        self.out = np.dot(self.x, self.W) + self.b
        
        # =========================================================================
        return self.out

    def backward(self, d_prev):
        """
        FC Layer Backward.
        Use variables : self.x, self.W

        [Input]
        d_prev: Gradients value so far in back-propagation process.

        [Output]
        dx : Gradients w.r.t input x
        - Shape : (batch_size, input_dim) - same shape as input x
        """
        self.dW = np.zeros_like(self.W, dtype=np.float64)   # Gradient w.r.t. weight (self.W)
        self.db = np.zeros_like(self.b, dtype=np.float64)   # Gradient w.r.t. bias (self.b)
        dx = np.zeros_like(self.x, dtype=np.float64)        # Gradient w.r.t. input x
        # =============================== EDIT HERE ===============================

        dx = np.dot(d_prev, self.W.T)
        self.dW = np.dot(self.x.T, d_prev)
        self.db = np.sum(d_prev, axis=0)

        # =========================================================================
        return dx

    def update(self, learning_rate):
        self.W -= self.dW * learning_rate
        self.b -= self.db * learning_rate

    def summary(self):
        return 'Input -> Hidden : %d -> %d ' % (self.W.shape[0], self.W.shape[1])

################################################################################################################
#    ** Softmax Layer **                                                                                       #
#   Softmax Layer applies softmax (WITHOUT any weights or bias)                                                #
#   Softmax Layer는 softmax만을 적용한다. (weights나 bias가 전혀 없이!!!)                                           #
#                                                                                                              #
#   Given an score,                                                                                            #
#   'Softmax Layer' applies softmax to make probability distribution. (Not log softmax or else...)             #
#   'Softmax Layer' softmax를 적용하여 class에 대한 확률분포를 만든다. (log softmax 나 다른 것이 아닌 softmax)          #
#                                                                                                              #
#   BE CAREFUL!!!!!                                                                                            #
#   This is different from 'SoftmaxOutputLayer' in HW-4                                                        #
#   This layer doesn't have any weights or bias. It only applies softmax!!!                                    #
#   (HW4 'SoftmaxOutputLayer' is separated into 'FCLayer' & 'SoftmaxLayer' in HW5)                             #
#                                                                                                              #
#   중요!!!!!                                                                                                   #
#   이 Layer는 HW-4의 SoftmaxOutputLayer와 다릅니다.                                                              #
#   이 Layer는 Weight나 Bias없이 단순 softmax를 적용하는 layer입니다.                                                #
#   (간단히 말하면 HW-4의 'SoftmaxOutputLayer'가 HW-5에서 'FC Layer'와 'SoftmaxLayer'로 분리되었습니다.)                    #
#                                                                                                              #
#   You need to implement forward and backward pass                                                            #
#   (This is NOT an entire model.)                                                                             #
################################################################################################################

class SoftmaxLayer:
    def __init__(self):
        # No parameters
        pass

    def forward(self, x):
        """

        Softmax Layer Forward.
        Apply softmax (not log softmax or others...) on axis-1

        Use 'softmax' function above in this file.

        [Input]
        x: Score to apply softmax
        - Shape: (N, C)

        [Output]
        y_hat: Softmax probability distribution.
        - Shape: (N, C)

        """
        y_hat = None
        # =============================== EDIT HERE ===============================

        self.y_hat = softmax(x)

        # =========================================================================
        return self.y_hat

    def backward(self, d_prev=1):
        """

        Softmax Layer Backward.
        Gradients w.r.t input score.

        That is,
        Forward  : softmax prob = softmax(score)
        Backward : dL / dscore => 'dx'

        Compute dx (dL / dscore).
        Check loss function in HW5 pdf file.

        """
        batch_size = self.y.shape[0]
        dx = None
        # =============================== EDIT HERE ===============================

        dx = (self.y_hat - self.y) / batch_size * d_prev

        # =========================================================================
        return dx

    def ce_loss(self, y_hat, y):
        """

        Compute Cross-entropy Loss.
        Use epsilon (eps) for numerical stability in log.
        Epsilon 값을 계산의 안정성을 위해 log에 사용하세요.

        Check loss function in HW5 pdf file.
        Loss Function 을 과제 파일에서 확인하세요.

        [Input]
        y_hat: Probability after softmax.
        - Shape : (Batch_size, # of class)

        y: One-hot true label
        - Shape : (Batch_size, # of class)

        [Output]
        self.loss : cross-entropy loss
        - Single float

        """
        self.loss = None
        eps = 1e-10
        self.y_hat = y_hat
        self.y = y
        # =============================== EDIT HERE ===============================
        
        # print('y: ', y.shape)
        # print(self.y)
        # print('y_hat: ')
        # print(self.y_hat)
        self.loss = -np.sum(np.log(y_hat + eps) * y) / y.shape[0]
        # print('loss: ', self.loss)
        # =========================================================================
        return self.loss

    def update(self, learning_rate):
        # Not used in softmax layer.
        pass

    def summary(self):
        return 'Softmax layer'

################################################################################################################
#    ** CNN Classifier **                                                                                      #
#   This is an class for entire CNN classifier.                                                                #
#   All the functions and variables are already implemented.                                                   #
#   Look at the codes below and see how the codes work.                                                        #
#                                                                                                              #
#    ** CNN Classifier **                                                                                      #
#   이 코드들은 CNN Classifier 통합 모델을 위한 코드입니다.                                                          #
#   필요한 모든 변수와 함수들은 이미 구현이 되어있습니다.                                                              #
#   아래 코드를 보면서 모델이 어떻게 동작하는지 확인하세요.                                                             #
#                                                                                                              #
#                              <<< DO NOT CHANGE ANY THING HERE>>>                                             #
#                                                                                                              #
################################################################################################################

class CNN_Classifier:
    def __init__(self):
        self.layers = OrderedDict()
        self.softmax_layer = None
        self.loss = None
        self.pred = None

    def predict(self, x):
        # Outputs model softmax score
        for name, layer in self.layers.items():
            x = layer.forward(x)
        x = self.softmax_layer.forward(x)
        return x

    def forward(self, x, y):
        # Predicts and Compute CE Loss
        self.pred = self.predict(x)
        self.loss = self.softmax_layer.ce_loss(self.pred, y)
        return self.loss

    def backward(self):
        # Back-propagation
        d_prev = 1
        d_prev = self.softmax_layer.backward(d_prev)
        for name, layer in list(self.layers.items())[::-1]:
            d_prev = layer.backward(d_prev)

    def update(self, learning_rate):
        # Update weights in every layer with dW, db
        for name, layer in self.layers.items():
            layer.update(learning_rate)

    def add_layer(self, name, layer):
        # Add Neural Net layer with name.
        if isinstance(layer, SoftmaxLayer):
            if self.softmax_layer is None:
                self.softmax_layer = layer
            else:
                raise ValueError('Softmax Layer already exists!')
        else:
            self.layers[name] = layer

    def summary(self):
        # Print model architecture.
        print('======= Model Summary =======')
        for name, layer in self.layers.items():
            print('[%s] ' % name + layer.summary())
        print('[Softmax Layer] ' + self.softmax_layer.summary())
        print()