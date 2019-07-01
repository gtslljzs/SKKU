from utils.load_data import CSVReader, split_to_x_y
import numpy as np
from models.Decision_Tree_Answer import Decision_Tree
from models.NaiveBayesian_Answer import Gaussian_NaiveBayesian

np.random.seed(100)

from random import sample

def main():

    # ====   Edit this  =====
    data_name = 'Heart'
    # data_name = 'Carseats'
    # ======================
    assert data_name in ['Heart', 'Carseats']

    data_reader = CSVReader('./data/%s_Train.csv' % data_name, 'utf-8')
    Train = data_reader.df.dropna()

    data_reader = CSVReader('./data/%s_Test.csv' % data_name, 'utf-8')
    Test = data_reader.df.dropna()

    Test_x, Test_y = split_to_x_y(Test, Test.columns.values)
    Test_y = Test_y.values

    # ====   Edit this  =====
    depth = 3

    impurity_metric = 'entropy'
    # impurity_metric = 'gini'
    # ======================
    assert impurity_metric in ['gini', 'entropy']


    # Dtree = Decision_Tree(impurity_metric, max_depth=depth)
    # Dtree.fit(Train, data_name)
    
    # pred = Dtree.predict(Test_x)
    

    # Accuracy = np.sum((pred == Test_y))/len(Test_y)
    # print('Accuracy: ', round(Accuracy,4))

    Nb = Gaussian_NaiveBayesian()
    Nb.fit(Train, data_name)

    pred = Nb.predict(Test_x)

    Accuracy = np.sum((pred == Test_y)) / len(Test_y)
    print('Accuracy: ', round(Accuracy,4))
    

if __name__ == '__main__':
    main()
