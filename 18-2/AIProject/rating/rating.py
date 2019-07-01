import os
import tensorflow as tf
import numpy as np
from PIL import Image
import csv

os.environ['TF_CPP_MIM_LOG_LEVEL'] = '2'
os.environ['CUDA_VISIBLE_DEVICES'] = '2'
config = tf.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.19
session = tf.Session(config = config)

# minmax-scaling을 위한 함수
def MinMaxScaler(data):
    numerator = data - np.min(data, 0)
    denominator = np.max(data, 0) - np.min(data, 0)
    # noise term prevents the zero division
    return numerator / (denominator + 1e-5)

def next_batch(num, data, img, labels):
    idx = np.arange(0, len(data))
    np.random.shuffle(idx)
    idx = idx[:num]
    data_shuffle = [data[i] for i in idx]
    img_shuffle = [img[i] for i in idx]
    labels_shuffle = [labels[i] for i in idx]

    return np.asarray(data_shuffle), np.asarray(img_shuffle), np.asarray(labels_shuffle)

def batch_norm_wrapper(inputs, is_training, decay=0.999):
    scale = tf.Variable(tf.ones([inputs.get_shape()[-1]]))
    beta = tf.Variable(tf.zeros([inputs.get_shape()[-1]]))
    pop_mean = tf.Variable(tf.zeros([inputs.get_shape()[-1]]), trainable=False)
    pop_var = tf.Variable(tf.ones([inputs.get_shape()[-1]]), trainable=False)
    if is_training:
        batch_mean, batch_var = tf.nn.moments(inputs, [0])
        train_mean = tf.assign(pop_mean, pop_mean * decay + batch_mean * (1 - decay))
        train_var = tf.assign(pop_var, pop_var * decay + batch_var * (1 - decay))

        with tf.control_dependencies([train_mean, train_var]):
            return tf.nn.batch_normalization(inputs, batch_mean, batch_var, beta, scale, epsilon)
    else:
        return tf.nn.batch_normalization(inputs, pop_mean, pop_var, beta, scale, epsilon)

""" 이미지 처리를 위한 CNN """
def build_CNN_classifier(image):
    x_image = image

    w_conv1 = tf.Variable(tf.truncated_normal(shape = [3, 3, 3, 32], stddev = 5e-2))
    b_conv1 = tf.Variable(tf.constant(0.1, shape = [32]))
    h_conv1 = tf.nn.relu(tf.nn.conv2d(x_image, w_conv1, strides = [1, 1, 1, 1], padding = 'SAME') + b_conv1)

    w_conv2 = tf.Variable(tf.truncated_normal(shape=[3, 3, 32, 64], stddev=5e-2))
    b_conv2 = tf.Variable(tf.constant(0.1, shape=[64]))
    h_conv2 = tf.nn.relu(tf.nn.conv2d(h_conv1, w_conv2, strides=[1, 1, 1, 1], padding='SAME') + b_conv2)

    h_pool1 = tf.nn.max_pool(h_conv2, ksize = [1, 3, 3, 1], strides = [1, 2, 2, 1], padding = 'SAME')
    h_drop1 = tf.nn.dropout(h_pool1, keep_prob)

    w_conv3 = tf.Variable(tf.truncated_normal(shape=[3, 3, 64, 64], stddev=5e-2))
    b_conv3 = tf.Variable(tf.constant(0.1, shape=[64]))
    h_conv3 = tf.nn.relu(tf.nn.conv2d(h_drop1, w_conv3, strides=[1, 1, 1, 1], padding = 'SAME') + b_conv3)

    w_conv4 = tf.Variable(tf.truncated_normal(shape=[3, 3, 64, 128], stddev=5e-2))
    b_conv4 = tf.Variable(tf.constant(0.1, shape=[128]))
    h_conv4 = tf.nn.relu(tf.nn.conv2d(h_conv3, w_conv4, strides=[1, 1, 1, 1], padding='SAME') + b_conv4)

    h_pool2 = tf.nn.max_pool(h_conv4, ksize=[1, 3, 3, 1], strides=[1, 2, 2, 1], padding = 'SAME')
    h_drop2 = tf.nn.dropout(h_pool2, keep_prob)

    w_conv5 = tf.Variable(tf.truncated_normal(shape=[3, 3, 128, 128], stddev=5e-2))
    b_conv5 = tf.Variable(tf.constant(0.1, shape=[128]))
    h_conv5 = tf.nn.relu(tf.nn.conv2d(h_drop2, w_conv5, strides=[1, 1, 1, 1], padding = 'SAME') + b_conv5)

    w_fc1 = tf.Variable(tf.truncated_normal(shape=[8 * 8 * 128, 384], stddev=5e-2))
    b_fc1 = tf.Variable(tf.constant(0.1, shape=[384]))
    h_conv5_flat = tf.reshape(h_conv5, [-1, 8 * 8 * 128])
    h_batch_norm = tf.matmul(h_conv5_flat, w_fc1) + b_fc1
    batch_norm = batch_norm_wrapper(h_batch_norm, True)
    h_fc1 = tf.nn.relu(batch_norm)
    h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

    w_fc2 = tf.Variable(tf.truncated_normal(shape=[384, 10], stddev=5e-2))
    b_fc2 = tf.Variable(tf.constant(0.1, shape=[10]))
    h_fc2 = tf.matmul(h_fc1_drop, w_fc2) + b_fc2

    w_fc3 = tf.Variable(tf.truncated_normal(shape=[10, 4], stddev=5e-2))
    b_fc3 = tf.Variable(tf.constant(0.1, shape=[4]))
    h_fc3 = tf.matmul(h_fc2, w_fc3) + b_fc3
    img_output = tf.nn.relu(h_fc3)

    return img_output

train_image = np.load('/assets/movie_train_image.npy')
x_train = []
y_train = []

x_train_adult = []
x_train_collection = []
x_train_budget = []
x_train_genre =[]
x_train_original_language = []
x_train_original_title = []
x_train_overview = []
x_train_popularity = []
x_train_company = []
x_train_country = []
x_train_date = []
x_train_revenue = []
x_train_runtime = []
x_train_spoken_language = []
x_train_status = []
x_train_title = []

y_train_rating = []
y_train_count = []

languages = []
status = []

""" 데이터 전처리 """
with open('/assets/movie_train.csv', newline = '', encoding = 'utf-8') as csvfile:
    movies = csv.reader(csvfile, delimiter = ',')

    for row in movies:

        if row[20] == 'The Cake Eaters':
            continue

        # A열 - 성인 영화 여부
        if row[0] == 'False':
            x_train_adult.append(0)
        else:
            x_train_adult.append(1)

        # B열 - 시리즈 정보

        # C열 - 예산
        x_train_budget.append(row[2])

        # D열 - 장르

        # H열 - 원작 언어
        flag = 0
        for lang in languages:
            if row[7] in lang:
                x_train_original_language.append(languages.index(row[7]))
                flag = 1
                break
        if flag == 0:
            languages.append(row[7])
            x_train_original_language.append(languages.index(row[7]))

        # I열 - 원작 제목

        # J열 - 줄거리

        # K열 - 인기도
        x_train_popularity.append(row[10])

        # M열 - 영화 제작 회사

        # N열 - 영화 제작 국가

        # O열 - 개봉 날짜

        # P열 - 수익
        x_train_revenue.append(row[15])

        # Q열 - 상영 시간
        x_train_runtime.append(row[16])

        # R열 - 영화에서 사용되는 언어
        temp = row[17].split(', ')
        temp = temp[0].split(': ')
        temp = temp[1].split('\'')[1]
        flag = 0
        for lang in languages:
            if temp in lang:
                x_train_spoken_language.append(languages.index(temp))
                flag = 1
                break
        if flag == 0:
            languages.append(temp)
            x_train_spoken_language.append(languages.index(temp))

        # S열
        flag = 0
        for st in status:
            if row[18] in st:
                x_train_status.append(status.index(row[18]))
                flag = 1
                break
        if flag == 0:
            status.append(row[18])
            x_train_status.append(status.index(row[18]))

        x_train.append([x_train_adult[-1], x_train_budget[-1], x_train_original_language[-1],
                        x_train_popularity[-1], x_train_revenue[-1], x_train_runtime[-1],
                        x_train_spoken_language[-1], x_train_status[-1]])
        y_train.append(row[22:])

# print(x_train.__len__())
# print(x_train[0])
# print(x_train[0][3])
# print(max(x_train_budget))
# print(x_train_original_language.__len__())
# print(x_train_status.__len__())
# print(max(x_train_status))
# # print(y_train.__len__())
# print(y_train[0])
# print(train_image.shape)

''' combining model'''
def build_predictor(x_train, img_output, y_train):
    X = x_train.append(img_output)

    W1 = tf.Variable(tf.random_normal([12, 64], dtype = 'float'), name = 'weight')
    b1 = tf.Variable(tf.random_normal([64], dtype = 'float'), name = 'bias')
    h1 = tf.matmul(X, W1) + b1
    h1 = tf.nn.relu(h1)

    W2 = tf.Variable(tf.random_normal([64, 128], dtype = 'float'))
    b2 = tf.Variable(tf.random_normal([128], dtype = 'float'))
    h2 = tf.matmul(h1, W2) + b2
    h2 = tf.nn.relu(h2)
    h2 = tf.nn.dropout(h2)

    W3 = tf.Variable(tf.random_normal([128, 36], dtype = 'float'))
    b3 = tf.Variable(tf.random_normal([36], dtype = 'float'))
    h3 = tf.matmul(h2, W3) + b3
    h3 = tf.nn.relu(h3)
    h3 = tf.nn.dropout(h3)

    W4 = tf.Variable(tf.random_normal([36, 2], dtype = 'float'))
    b4 = tf.Variable(tf.random_normal([2], dtype = 'float'))
    hypothesis = tf.matmul(h3, W4) + b4

    cost = tf.reduce_mean(tf.square(tf.reduce_sum(hypothesis - y_train)))
    train = tf.train.AdamOptimizer(1e-3).minimize(cost)

    return train

''' global variables '''
train_image_batch = tf.placeholder(tf.float32, shape = [None, 300, 200, 3])
x_train_batch = tf.placeholder(tf.float32, shape = [None, 8])
y_train_batch = tf.placeholder(tf.float32, shape = [None, 2])
keep_prob = tf.placeholder(tf.float32)
epsilon = 1e-3

img_output = build_CNN_classifier(train_image_batch)
train = build_predictor(x_train_batch, img_output, y_train_batch)

''' training '''
with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())

    for i in range(10000):
        batch = next_batch(128, x_train, img_output, y_train)