import numpy as np
from utils.Answer import evaluation_test2

label = np.array([[0, 0, 1, 0, 1, 0, 0, 1, 1, 0],                                   # Q1
                [0, 1, 1, 0, 1, 1, 1, 0, 0, 0],                                     # Q2
                [1, 1, 1, 1, 0, 0, 0, 0, 1, 0],                                     # Q3
                [0, 1, 0, 0, 0, 1, 0, 1, 0, 0],                                     # Q4
                [0, 1, 0, 1, 0, 0, 0, 1, 0, 0]                                      # Q5
                ])
hypo = np.array([[0.16, 0.55, 0.7, 0.18, 0.13, 0.23, 0.36, 0.21, 0.74, 0.94],       # Q1_hypothesis
                [0.21, 0.19, 0.11, 0.87, 0.25, 0.38, 0.08, 0.84, 0.69, 0.51],       # Q2_hypothesis
                [0.1, 0.68, 0.89, 0.59, 0.9, 0.92, 0.51, 0.93, 0.51, 0.86],         # Q3_hypothesis
                [0.31, 0.86, 0.81, 0.93, 0.64, 0.45, 0.43, 0.09, 0.56, 0.62],       # Q4_hypothesis
                [0.74, 0.1, 0.52, 0.2, 0.31, 0.02, 0.13, 0.24, 0.83, 0.19]          # Q5_hypothesis
                ])


result = evaluation_test2(label, hypo, at=5)


real_result = {}
real_result['MAP  @5'] = 0.123000
real_result['nDCG @5'] = 0.239278

for key in result.keys():
    print('your: ', key, '\t:\t %.6f' % result[key], 'Answer: ', key, '\t:\t %.6f' % real_result[key])

real_result = {}
real_result['MAP  @7'] = 0.229508
real_result['nDCG @7'] = 0.400956

result = evaluation_test2(label, hypo, at=7)

for key in result.keys():
    print('your: ', key, '\t:\t %.6f' % result[key], 'Answer: ', key, '\t:\t %.6f' % real_result[key])
