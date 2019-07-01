import numpy as np
from utils.Answer import evaluation_test1


label = np.array([0, 1, 1, 1, 1, 0, 0, 1, 1, 0])
pred  = np.array([0, 1, 0, 0, 1, 1, 1, 0, 0, 1])
hypo = np.array([0.43, 0.77, 0.31, 0.03, 0.52, 0.66, 0.90, 0.26, 0.44, 0.74])
result = evaluation_test1(label, pred, at=5)

real_result = {}
real_result['Accuracy '] = 0.300000
real_result['Precision'] = 0.400000
real_result['Recall   '] = 0.333333
real_result['F_measure'] = 0.363636


for key in result.keys():
    print('your: ', key, '\t:\t %.6f' % result[key], '\tanswer: ', '\t:\t %.6f' % real_result[key])


