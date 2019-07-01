
from utils.load_data import CSVReader
from utils.utils_Answer import *
from models.Decision_Tree_Answer import *
from models.NaiveBayesian_Answer import *


def main():
    data_reader = CSVReader('./data/tennis.csv', encoding='utf-8')

    dataframe = data_reader.df
    header = data_reader.header

    Y_data = dataframe['play']
    print('[Decision tree validation]\n')
    answer_Gini = 0.459184
    answer_Entropy = 0.940286
    answer_Feature = 'outlook'

    Dtree = Decision_Tree('entropy', max_depth=3)
    Dtree.fit(dataframe, 'Tennis')
    Feature, Type = Dtree.Find_Best_Feature(dataframe)

    print()
    print('Gini: \t', answer_Gini, '\tYour: ', round(Gini_index(Y_data), 6), '\t\t', (answer_Gini == round(Gini_index(Y_data), 6)))
    print('Entropy: ', answer_Entropy, '\tYour: ',round(Entropy(Y_data), 6), '\t\t',(answer_Entropy == round(Entropy(Y_data), 6)))
    print('Feature: ', answer_Feature, '\tYour: ',Feature, '\t\t',(answer_Feature == Feature))
    print()


    y = [1., 0., 1., 1., 0., 0.,
         1., 1., 1., 1., 0., 1.,
         1., 1., 1., 1., 0., 1.,
         1., 1., 1., 1., 0., 1.,
         1., 0., 1., 1., 1., 1.]

    Gini_y = 0.357778
    entropy_y = 0.783777

    print('Gini: \t\t', Gini_y , '\tYour: ', round(Gini_index(y), 6))
    print('Entropy: \t', entropy_y, '\tYour: ', round(Entropy(y), 6))

    print('\n[Naive Bayesian validation]\n')
    yes_idx = (Y_data == 'yes')
    yes_data = dataframe[yes_idx]

    prob_given_yes = {}
    prob_given_yes['humidity = high'] = 0.363636
    prob_given_yes['humidity = normal'] = 0.636364
    prob_given_yes['temp = cool'] = 0.333333
    prob_given_yes['temp = hot'] = 0.25
    prob_given_yes['temp = mild'] = 0.416667
    prob_given_yes['outlook = overcast'] = 0.416667
    prob_given_yes['outlook = rainy'] = 0.333333
    prob_given_yes['outlook = sunny'] = 0.25
    prob_given_yes['windy = False'] = 0.636364
    prob_given_yes['windy = True'] = 0.363636

    your_likelihood = {}

    NB = Gaussian_NaiveBayesian()
    NB.fit(yes_data, 'Tennis')
    NB.initialize_likelihood_prob(your_likelihood)
    NB.set_likelihood_prob(yes_data, your_likelihood)


    for keys in prob_given_yes.keys():
        print((prob_given_yes[keys] == your_likelihood[keys]),
              '\tAnswer: ', keys,':',prob_given_yes[keys], '\t\t', 'Your: ', keys,':',your_likelihood[keys])

    Gaussian_x = 0.241971
    x = round(Gaussian_prob(1, 0, 1), 6)

    print('\n\n[Gaussian prob test]')
    print('Answer: ', Gaussian_x, 'Your: ', x, '\t', (Gaussian_x == x) )

if __name__ == '__main__':
    main()