import tensorflow as tf
import numpy as np
import os

os.environ['TF_CPP_MIN_LOG_LEVLE'] = '2'
os.environ['CUDDA_VISIBLE_DEVICES'] = '2'
config = tf.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.19
session = tf.Session(config = config)

from tensorflow.contrib.keras.api.keras.datasets.cifar10 import load_data
(x_train, y_train), (x_test, y_test) = load_data()

def next_batch(num, data, labels):
    idx = np.arange(0, len(data))
    np.random.shuffle(idx)
    idx = idx[:num]
    data_shuffle = [data[i] for i in idx]
    labels_shuffle = [labels[i] for i in idx]

    return np.asarray(data_shuffle), np.asarray(labels_shuffle)

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
    b_fc2 = tf.Variable(tf.constant(0, 1, shape=[10]))
    logits = tf.matmul(h_fc1_drop, w_fc2) + b_fc2
    y_pred = tf.nn.softmax(logits)

    return y_pred, logits

x = tf.placeholder(tf.float32, shape = [None, 32, 32, 3])
y = tf.placeholder(tf.float32, shape = [None, 10])
keep_prob = tf.placeholder(tf.float32)
epsilon = 1e-3

(x_train, y_train), (x_test, y_test) = load_data()
y_train_one_hot = tf.squeeze(tf.one_hot(y_train, 10), axis = 1)
y_test_one_hot = tf.squeeze(tf.one_hot(y_test, 10), axis = 1)
y_pred, logits = build_CNN_classifier(x)
loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels = y, logits = logits))
train_step = tf.train.AdamOptimizer(1e-3).minimize(loss)
correct_prediction = tf.equal(tf.argmax(y_pred, 1), tf.argmax(y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    for i in range(10000):
        batch = next_batch(128, x_train, y_train_one_hot.eval())
        if i % 100 == 0:
            train_accuracy = accuracy.eval(feed_dict = {x: batch[0], y: batch[1], keep_prob: 1.0})
            loss_print = loss.eval(feed_dict = {x:batch[0], y: batch[1], keep_prob:1.0})
            print("epoch: %d, training data accuracy: %f, loss: %f" % (i, train_accuracy, loss_print))

        sess.run(train_step, feed_dict = {x: batch[0], y: batch[1], keep_prob: 0.7})

    test_accuracy = 0.0
    for i in range(10):
        test_batch = next_batch(1000, x_test, y_test_one_hot.eval())
        test_accuracy = test_accuracy + accuracy.eval(feed_dict = {x: test_batch[0], y: test_batch[1], keep_prob: 1.0})

    test_accuracy = test_accuracy / 100
    print("test data accuracy: %f" % test_accuracy)