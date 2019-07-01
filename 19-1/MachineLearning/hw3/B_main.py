import numpy as np
import csv

from utils.Answer import evaluation_test2
from utils.utils import load_rank_data


np.random.seed(503)
# ===  You can setting here  ===
DATA_NAME = 'Product'
# ==============================
assert DATA_NAME in ['Product', 'Movielens']

pred = load_rank_data(DATA_NAME, 'pred')
true = load_rank_data(DATA_NAME, 'true')


# ========  You can setting here  =========
result = evaluation_test2(true, pred, at=75)
# =========================================
for key in result.keys():
    print(key, '\t:\t %.6f' % result[key])

