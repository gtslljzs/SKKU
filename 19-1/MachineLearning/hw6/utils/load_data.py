import pandas as pd
import numpy as np
from random import sample

class CSVReader:
    def __init__(self, FileName, encoding):
        self.df = pd.read_csv(FileName, engine='python', encoding=encoding)
        self.header = self.df.columns.values
        self.data = self.df.values

    def pack_data(self):
        data = pd.DataFrame(self.data, columns=self.header)

        return data

def Train_Test_split(data_name, df, ratio):

    n = len(df)
    Test_idx = sample(range(n), int(ratio * n))
    Train_idx = np.delete(range(n), Test_idx)

    train = df.loc[Train_idx]
    test = df.loc[Test_idx]

    train.reset_index()
    test.reset_index()

    train.to_csv('./data/%s_Train.csv'% (data_name), mode='w', index=False)
    test.to_csv('./data/%s_Test.csv' %(data_name), mode='w', index=False)


def split_to_x_y(df, feature):

    input_feature = feature[:-1]
    class_label = feature[-1]

    X = df[input_feature]
    Y = df[class_label]

    return X, Y