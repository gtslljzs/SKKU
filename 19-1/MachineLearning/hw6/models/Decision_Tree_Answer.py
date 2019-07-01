from utils.utils_Answer import impurity_func, Finding_split_point
import numpy as np
import warnings

warnings.simplefilter(action='ignore', category=FutureWarning)

Heart_Category_feature_idx = [1, 2, 5, 6, 8, 10, 11, 12]
Carseats_Category_feature_idx = [6, 9]
Tennis_Category_feature_idx = [0, 1, 2, 3]

class Node():

    def __init__(self, dataFrame, criterion, depth, split_feature=''):
        self.df = dataFrame
        self.depth = depth
        self.split_feature = split_feature

        self.impurity = round(impurity_func(dataFrame.values[:, -1], criterion), 6)
        self.child = {}
        self.is_leaf = False
        self.label = None


class Decision_Tree():

    def __init__(self, criterion, max_depth=5):

        self.criterion = criterion
        self.max_depth = max_depth

        assert criterion in ['gini', 'entropy']


# ========================              Training  part              ======================================
    def fit(self, Train_data, data_name):
        self.root = Node(Train_data, self.criterion, 0)
        self.Category_feature_idx = (data_name == 'Heart') \
                                    and Heart_Category_feature_idx or \
                                    (data_name == 'Carseats') \
                                    and Carseats_Category_feature_idx \
                                    or Tennis_Category_feature_idx
        self.branch_name = ''
        assert data_name in ['Heart', 'Carseats', 'Tennis']

        self.Generate_tree(self.root)

    # You don't need to touch
    def Generate_tree(self, node):

        '''
        Grow the tree from root node until arrival the maximum depth or completely split.
        split criterion is the lower the impurity.

        root 노드로부터 트리가 최대 깊이에 도달하거나 완전히 분리될 때까지
        tree를 성장시킵니다.
        분리하는 기준은 impurity를 가장 낮추는 feature입니다.

        '''

        """
        (when tree_depth == maximum_depth or data is separated completely), 
        Set current node to leaf_node and decide to class label.

        tree_depth == maximum_depth or 데이터가 완전 분리일 때 
        현재 node를 leaf_node로 설정 및 label 값 결정 
        """
        df = node.df
        if node.depth == self.max_depth:
            self.set_leaf_node(node)
            return

        if node.impurity == 0.:
            self.set_leaf_node(node)
            return

        """
        For the feature that the highest information gain,
                split the dataset and construct the child node

        정보 이득이 가장 높은 feature에 대해서 data 분할 진행
        """

        Best_feature, feature_type = self.Find_Best_Feature(df)
        self.make_child(node, Best_feature, feature_type)

        """
        recursive call for child node

        자식 노드에 대해 재귀적 호출
        """
        for key in node.child:
            self.branch_name = Best_feature + '(' + str(key) + ')'
            self.Generate_tree(node.child[key])

    def Find_Best_Feature(self, df):
        """
            This function retuns the feature that can split the data.
            in which, the impurity is the least.

            you should implement the part of calculating for
            impurity (entropy or gini_index) for given feature.변경하다


            이 함수는 불순도가 가장 작게 데이터를 분할할 수 있는 feature를
            반환하는 함수입니다.

            여러분이 구현할 내용은 주어진 feature의 impurity(entropy or gini_index)를
            계산하는 것입니다.


            [Parameter]:
                df : [dataFrame (got by pandas) ][N x D] : Training_data

            [Variables] :
                header : [list of string ] [1 x D]   the set of attribute_name, last element is for output_feature.
                input_feature : [list of string] [1 x (D-1)] the set of attribute name except for last feature(output)
                Y_data : [column vector of label] [N x 1] label_data, To get the (vector or matrix) not dataFrame,
                          you can write   ' data  = df.values '
                self.Category_feature_idx : [list of integers]: the set of idx only for Category feature.(referred top of the code.)
                split_value : in numeric_data, You just divide the data two part.
                              one is the less than split_value, the other is no less than split_value.

            [Objects]: (the part of implement)
                impurity : (float) entropy or gini_index got by splitting data given attribute.
                impurity_list [list of float] [1 x D] : the list which store the all impurity in order.

            [return]:
                Best_feature : (string) feature_name
                feature_type : (string) the type of feature ('Category' or 'Numeric')
        """

        header = df.columns.values
        input_feature = header[:-1]
        output_feature = header[-1]
        Y_data = df[output_feature].values
        
        impurity_list = []

        # for all features in DataFrame,
        # print(df)
        # print(header)

        for idx, h in enumerate(input_feature):
        # ============       Edit here      ==================
            # Category Feature Case
            if idx in self.Category_feature_idx:
                impurity = 0
                attr, counts = np.unique(df[h], return_counts=True)
                for i in range(len(attr)):
                    temp = df[df[h] == attr[i]]
                    temp_y = temp[output_feature].values
                    impurity += (counts[i] / np.sum(counts)) * impurity_func(temp_y, self.criterion)

            # Numeric Feature Case
            else:
                impurity = 0
                split_value = Finding_split_point(df, h, self.criterion)
                low = df[df[h] < split_value]
                high = df[df[h] >= split_value]
                low_y = low[output_feature].values
                high_y = high[output_feature].values

                impurity = (len(low) / len(df[h]) * impurity_func(low_y, self.criterion)) \
                            + (len(high) / len(df[h]) * impurity_func(high_y, self.criterion))

        #=====================================================
            impurity_list.append(np.round(impurity, 6))

        idx = np.argmin(impurity_list)
        Best_feature = input_feature[idx]
        feature_type = idx in self.Category_feature_idx and 'Category' or 'Numeric'

        return Best_feature, feature_type

    # You don't need to touch
    def make_child(self, node, Best_feature, feature_type):
        '''
        if attribute type is Category,
        For each attribute X, child_node is assigned to the data that has x-value.
        the child node is managed by dictionary. (dict key: 'feature_name = value')('outlook = sunny')

        attribute가 카테고리일 경우
        각각의 attribute 값 x에 대해서 x를 value로 갖는 데이터가 자식노드에 할당됩니다.
        자식노드는 dictionary에 의해 관리됩니다. (dict key: 'feature_name = value')('outlook = sunny')

        elif attribute type is Numeric
        Search for all value x which is the best split value. and the node has 2 child node.
        one is (data < x), the other is (data >= x).
        (dict key: 'feature_name < x' or 'feature_name >= x') ('Age' < 20)

        attribute가 수치값인 경우
        모든 수치값 x에 대해서 data를 가장 잘 나누는 split value를 탐색 후 2개의 자식노드로 분할합니다.
        하나는 (data < x) 다른 하나는 (data >= x).
        (dict key: 'feature_name < x' or 'feature_name >= x') ('Age' < 20)

        [Parameter]
            node : current Node (class object)
            Best_feature: 'the feature that can makes the impurity the least' (string)
            feature type: 'Category' or 'Numeric' (string)
        [return]
            nothing.
        '''

        df = node.df
        col_data = df[Best_feature]
        node.split_feature = Best_feature

        print('\nBest_Feature: ', Best_feature)

        if feature_type == 'Category':
            distinct_data = np.unique(col_data)

            for i, d in enumerate(distinct_data):
                if type(d) == np.float64 or type(d) == np.float32:
                    d = int(d)
                print('\t' * node.depth, 'parent: ', self.branch_name, '\tDepth:', node.depth, '\t', i, 'th branch: ',d)
                child_df = df[(col_data == d)]
                # print(child_df)
                child_node = Node(child_df, self.criterion, node.depth + 1, '%s = %s' % (Best_feature, d))
                node.child['%s'%d] = child_node

        elif feature_type == 'Numeric':
            split_value = Finding_split_point(df, Best_feature, self.criterion)
            less_idx = (col_data < split_value)
            idx_set = [less_idx, ~less_idx]

            print('\t' * node.depth, 'parent: ', self.branch_name, '\tDepth: ', node.depth, '\tsplit point: ',
                  split_value)
            for i, idx in enumerate(idx_set):
                child_df = df[idx]
                # print(child_df)
                inequal_symbol = (i == 0) and '<' or '>='
                child_node = Node(child_df, self.criterion, node.depth + 1,
                                  '%s %s %s' % (Best_feature, inequal_symbol, split_value))
                node.child['%s %.1f' % (inequal_symbol, split_value)] = child_node
        return

    # You don't need to touch
    def set_leaf_node(self, node):
        '''
        set the leaf_flag -> True
        set the label value. ('Yes' or 'No')

        [Parameter]
            node: current node(class object)
        [return]
            Nothing
        '''
        df = node.df
        header = df.columns.values
        output_feature = header[-1]
        Y = df[output_feature].values

        node.is_leaf = True
        node.label = (np.sum(Y == 'Yes') > np.sum(Y == 'No')) and 'Yes' or 'No'

        print('\t' * node.depth, 'parent: ', self.branch_name, '\t ', 'Leaf node \t Depth: ', node.depth, '\tLabel: ', node.label)
        return
#==============================================================================================================

#===============================                    Test part                    ===============================

    # You don't need to touch
    def predict(self, Test_x):
        pred = np.full(len(Test_x), None)
        feature = Test_x.columns.values

        for i, idx in enumerate(Test_x.index):
            tuple = Test_x.loc[idx]
            pred[i] = self.Get_label(tuple, feature)
        return pred

    def Get_label(self, tuple, feature):
        cur_node = self.root

        while cur_node.is_leaf == False:
            f = cur_node.split_feature
            feature_idx = np.where(feature == f)[0][0]

            if feature_idx in self.Category_feature_idx:
                key = tuple[f]
                if type(key) == np.float64 or type(key) == np.float32:
                    key = str(int(key))
            else:
                for keys in cur_node.child.keys():
                    split_value = float(keys.split(' ')[1])
                    key = (tuple[f] < split_value) and '< %.1f' % ((split_value)) or '>= %.1f' % ((split_value))
            try:
                cur_node = cur_node.child[key]
            except KeyError:
                cur_node.label = 'Yes'
                break

        return cur_node.label
#==============================================================================================================