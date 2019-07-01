import numpy as np
from Answer import sign, Perceptron

"""
*** 이것은 Perceptron 구현이 정확한지 체크하기 위한 코드입니다. ***

- 꼼꼼하게 읽고  PLA.py를 실행하기'전에' 구현을 확인하세요.
- 이 파일의 코드는 수정하지 말고 Answer.py에서 수정하여 결과를 체크하세요.
- 코드의 실행 결과는 제시된 결과와 동일해야 합니다.
- 명시되어있는 기능 이외의 다른 기능을 추가하지마세요. 
  정답 결과와 같은 결과가 나오지 않을 수 있습니다.
  ex) 매 epoch에 데이터를 섞는다. (신경쓰지 않으셔도 됩니다. 물론 과제 이외에 개인적인 실험은 하셔도 좋습니다.)

*** This is the codes for you to check whether you implement PLA correctly ***

- Read carefully and check your codes *BEFORE* you go into PLA.py.
- Do NOT edit any code here, rather edit your codes in 'Answer.py'.
- Your codes are supposed to make same results as mine.
- Please do not code any other functionality other than what is said to do. 
  You will not get the same results, otherwise.
  ex) shuffle data every epoch (You don't need to care this. But you can try personally.)
  
"""

"""
sign(temp1) should be exactly same as below:
sign(temp1) 의 결과는 아래와 일치해야 합니다:
[[ 1. -1.  1.]
 [-1.  1.  1.]]
"""
temp1 = np.array([[0.1, -2, 123], [-123, 11, 0.0]])
print(sign(temp1))

perceptron = Perceptron(3)
temp_w = np.array([[1.0, -2.0, 3.0]]).T
temp_b = np.array([1.0])
perceptron.W = temp_w
perceptron.b = temp_b

"""
forward(temp2), forward(temp3) should be exactly same as below:
forward(temp2), forward(temp3) 의 결과는 아래와 일치해야 합니다:
[[1.]]
[[-1.]]
"""

temp2 = np.array([[10, -10, 1]])
print(perceptron.forward(temp2))

temp3 = np.array([[-0.1, 0.9, -1.0]])
print(perceptron.forward(temp3))