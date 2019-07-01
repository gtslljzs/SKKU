import numpy as np
from Answer import softmax, ReLU, Sigmoid, InputLayer, HiddenLayer, SigmoidOutputLayer, SoftmaxOutputLayer

"""
*** 이것은 MLP 구현이 정확한지 체크하기 위한 코드입니다. ***

- 꼼꼼하게 읽고  MLP_1.py, MLP_2.py를 실행하기'전에' 구현을 확인하세요.
- 이 파일의 코드는 수정하지 말고 Answer.py에서 수정하여 결과를 체크하세요.
- 코드의 실행 결과는 제시된 결과와 동일해야 합니다.
- 명시되어있는 기능 이외의 다른 기능을 추가하지마세요. 
  정답 결과와 같은 결과가 나오지 않을 수 있습니다.
  ex) 매 epoch에 데이터를 섞는다. (신경쓰지 않으셔도 됩니다. 물론 과제 이외에 개인적인 실험은 하셔도 좋습니다.)
  ex) Random seed를 설정한다/바꾼다. (이미 설정 해놓았습니다.)

*** This is the codes for you to check whether you implement PLA correctly ***

- Read carefully and check your codes *BEFORE* you go into MLP_1.py & MLP_2.py.
- Do NOT edit any code here, rather edit your codes in 'Answer.py'.
- Your codes are supposed to make same results as mine.
- Please do not code any other functionality other than what is said to do. 
  You will not get the same results, otherwise.
  ex) shuffle data every epoch (You don't need to care this. But you can try personally.)
  ex) set/change random seed (I've already done it.)

"""

print('===== ReLU Check =====')
"""
The results should be exactly same as below:
결과는 아래와 일치해야 합니다:

Forward: 
 [1. 0. 3.]
Backward: 
 [-10   0 -30]
"""
relu = ReLU()
temp2 = np.array([1, -0.1, 3])
temp3 = np.array([-10, -20, -30])

print('Forward: \n', relu.forward(temp2))
print('Backward: \n', relu.backward(temp3))
print()

print('===== Sigmoid Check =====')
"""
The results should be exactly same as below:
결과는 아래와 일치해야 합니다:

Forward: 
 [[0.26894142 0.88079708 0.62245933]
 [0.5        0.47502081 0.52497919]]
Backward: 
 [[0.         0.10499359 0.        ]
 [0.25       0.         0.24937604]]
"""

sigmoid = Sigmoid()
temp4 = np.array([[-1, 2, 0.5], [0.0, -0.1, 0.1]])
temp5 = np.array([[0, 1, 0], [1, 0, 1]])

print('Forward: \n', sigmoid.forward(temp4))
print('Backward: \n', sigmoid.backward(temp5))
print()

print('===== InputLayer Check =====')
"""
The results should be exactly same as below:
결과는 아래와 일치해야 합니다:

Forward: 
 [[2. 0.]
 [3. 0.]]
dW: 
 [[ 1.  0.]
 [-3.  0.]
 [-2.  0.]]
db: 
 [-1.  0.]
"""

input_layer = InputLayer(3, 2, ReLU)
input_layer.W = np.array([[-2.0, -2.0],
                          [2.0, -1.0],
                          [-3.5, -0.5]])
input_layer.b = np.array([1.0, -1.0])
temp6 = np.array([[-1, 3, 2],
                  [0.0, 1.0, 0.0]])
temp7 = np.array([[-1, 3], [0.0, 1.0]])
print('Forward: \n', input_layer.forward(temp6))
input_layer.backward(temp7)
print('dW: \n', input_layer.dW)
print('db: \n', input_layer.db)
print()

print('===== HiddenLayer Check =====')
"""
The results should be exactly same as below:
결과는 아래와 일치해야 합니다:

Forward: 
 [[1.03 0.  ]
 [0.   0.  ]]
Backward: 
 [[ 2.   -0.01]
 [ 1.25 -1.  ]]
dW: 
 [[ 1.  0.]
 [-3.  1.]]
db: 
 [-1.  1.]
"""
hidden_layer = HiddenLayer(2, 2)
hidden_layer.W = np.array([[-2.0, 1.25],
                           [0.01, -1.0]])
hidden_layer.b = np.array([-1.0, 1.0])
temp6 = np.array([[-1, 3],
                  [0.0, 1.0]])
temp7 = np.array([[-1, 3],
                  [0.0, 1.0]])
print('Forward: \n', hidden_layer.forward(temp6))
print('Backward: \n', hidden_layer.backward(temp7))
print('dW: \n', hidden_layer.dW)
print('db: \n', hidden_layer.db)
print()

print('===== SigmoidOutputLayer Check =====')
"""
The results should be exactly same as below:
결과는 아래와 일치해야 합니다:

Binary Cross-entropy Loss: 
 15.581616346746953
Predict: 
 [[0.90437415]
 [0.00750045]]
Forward: 
 0.05402041769476528
Backward: 
 [[-0.05371304  0.10152596 -0.14933889]
 [ 0.004213   -0.00796322  0.01171345]]
dW: 
 [[-0.05306324]
 [ 0.0574135 ]
 [ 0.00375022]]
db: 
 [-0.0440627]
"""
sigmoid_output_layer = SigmoidOutputLayer(3, 1)
sigmoid_output_layer.W = np.array([[1.1234, -2.1234, 3.1234]]).T
sigmoid_output_layer.b = np.array([-1])

temp_y = np.array([0.0, 0.5, 1.0])
temp_y_hat = np.array([1, 1, 0])
print('Binary Cross-entropy Loss: \n', sigmoid_output_layer.binary_ce_loss(temp_y_hat, temp_y))

temp8 = np.array([[1, -1, 0], [-1.4, 2.56, 1]])
print('Predict: \n', sigmoid_output_layer.predict(temp8))

temp_y = np.array([[1], [0]])
print('Forward: \n', sigmoid_output_layer.forward(temp8, temp_y))
print('Backward: \n', sigmoid_output_layer.backward(d_prev=1))
print('dW: \n', sigmoid_output_layer.dW)
print('db: \n', sigmoid_output_layer.db)
print()

print('===== SoftmaxOutputLayer Check =====')
"""
The results should be exactly same as below:
결과는 아래와 일치해야 합니다:

Cross-entropy loss: 
 56.797098960473136
Predict: 
 [[0.03511903 0.25949646 0.70538451]
 [0.00108758 0.00270191 0.99621051]]
Forward: 
 4.631404434078592
Backward: 
 [[ 0.25645564  0.50146461  1.92169975]
 [ 2.7215362  -0.314622   10.96489546]]
dW: 
 [[-5.43791927e-04  4.98649045e-01 -4.98105254e-01]
 [ 5.43791927e-04 -4.98649045e-01  4.98105254e-01]
 [ 2.71895963e-04 -2.49324523e-01  2.49052627e-01]]
db: 
 [-0.48189669 -0.36890082  0.85079751]
"""

softmax_output_layer = SoftmaxOutputLayer(3, 3)
softmax_output_layer.W = np.array([[1.12, -2.34, 3.12], [-1, 0.5, -0.13], [1.1, -11, 11]])
softmax_output_layer.b = np.array([-1, 1, 2])

temp_y = np.array([[0, 1, 0], [1, 0, 0], [0, 0, 1]])
temp_y_hat = np.array([[0.5, 0.0, 0.1], [0.3, 0.2, 2.1], [3.5, 1.0, 1.1]])
print('Cross-entropy loss: \n', softmax_output_layer.ce_loss(temp_y_hat, temp_y))

temp8 = np.array([[0, 0, 0], [-1, 1, 0.5]])
print('Predict: \n', softmax_output_layer.predict(temp8))

temp_y = np.array([[1, 0, 0], [0, 1, 0]])
print('Forward: \n', softmax_output_layer.forward(temp8, temp_y))
print('Backward: \n', softmax_output_layer.backward(d_prev=1))
print('dW: \n', softmax_output_layer.dW)
print('db: \n', softmax_output_layer.db)
print()