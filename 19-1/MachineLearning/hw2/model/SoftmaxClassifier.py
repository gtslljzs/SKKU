import numpy as np

class SoftmaxClassifier:
    def __init__(self, num_features, num_label):
        self.num_features = num_features
        self.num_label = num_label
        self.W = np.zeros((self.num_features, self.num_label))

    def train(self, x, y, epochs, batch_size, lr, optimizer):
        """
        N : # of training data
        D : # of features
        C : # of classes

        [INPUT]
        x : (N, D), input data (first column is bias for all data)
        y : (N, )
        epochs: (int) # of training epoch to execute
        batch_size : (int) # of minibatch size
        lr : (float), learning rate
        optimizer : (Python class) Optimizer

        [OUTPUT]
        final_loss : (float) loss of last training epoch

        [Functionality]
        Given training data, hyper-parameters and optimizer, execute training procedure.
        Training should be done in minibatch (not the whole data at a time)
        Procedure for one epoch is as follow:
        - For each minibatch
            - Compute probability of each class for data
            - Compute softmax loss
            - Compute gradient of weight
            - Update weight using optimizer
        * loss of one epoch = Mean of minibatch losses
        (minibatch losses = [0.5, 1.0, 1.0, 0.5] --> epoch loss = 0.75)

        """
        print('========== TRAINING START ==========')
        final_loss = None   # loss of final epoch
        num_data, num_feat = x.shape
        losses = []
        for epoch in range(1, epochs + 1):
            batch_losses = []   # list for storing minibatch losses

        # ========================= EDIT HERE ========================

            # making batches
            x_batch = []
            y_batch = []
            num_batches = len(x) // batch_size
            if len(x) % batch_size != 0:
                num_batches += 1
            for i in range(num_batches):
                x_mini = x[i * batch_size : (i + 1) * batch_size]
                y_mini = y[i * batch_size : (i + 1) * batch_size]
                x_batch.append(x_mini)
                y_batch.append(y_mini)
            
            # training
            for i in range(num_batches):
                x_train = x_batch[i]
                y_train = y_batch[i]
                
                # hypothesis
                f = np.dot(x_train, self.W)
                softmax = self._softmax(f)
                
                # calculating loss
                loss = self.softmax_loss(softmax, y_train)
                batch_losses.append(loss)

                # updating weights
                grads = self.compute_grad(x_train, self.W, softmax, y_train)
                self.W = optimizer.update(self.W, grads, lr)

            
        # ============================================================
            epoch_loss = sum(batch_losses) / len(batch_losses)  # epoch loss
            # print loss every 10 epoch
            if epoch % 10 == 0:
                print('Epoch %d : Loss = %.4f' % (epoch, epoch_loss))
            # store losses
            losses.append(epoch_loss)
        final_loss = losses[-1]

        return final_loss

    def eval(self, x):
        """

        [INPUT]
        x : (N, D), input data

        [OUTPUT]
        pred : (N, ), predicted label for N test data

        [Functionality]
        Given N test data, compute probability and make predictions for each data.
        """
        pred = None
        # ========================= EDIT HERE ========================

        pred = []
        f = np.dot(x, self.W)
        softmax = self._softmax(f)

        for i in range(len(softmax)):
            pred.append(np.argmax(softmax[i]))
        pred = np.array(pred)

        # ============================================================
        return pred

    def softmax_loss(self, prob, label):
        """
        N : # of minibatch data
        C : # of classes

        [INPUT]
        prob : (N, C), probability distribution over classes for N data
        label : (N, ), label for each data

        [OUTPUT]
        softmax_loss : scalar, softmax loss for N input

        [Functionality]
        Given probability and correct label, compute softmax loss for N minibatch data
        """
        softmax_loss = 0.0
        # ========================= EDIT HERE ========================
        prob += 1e-9
        loss = - np.log(prob)
        for i in range(len(label)):
            softmax_loss += loss[i][label[i]]

        # ============================================================
        return softmax_loss

    def compute_grad(self, x, weight, prob, label):
        """
        N : # of minibatch data
        D : # of features
        C : # of classes

        [INPUT]
        x : (N, D), input data
        weight : (D, C), Weight matrix of classifier
        prob : (N, C), probability distribution over classes for N data
        label : (N, ), label for each data. (0 <= c < C for c in label)

        [OUTPUT]
        gradient of weight: (D, C), Gradient of weight to be applied (dL/dW)

        [Functionality]
        Given input (x), weight, probability and label, compute gradient of weight.
        """
        grad_weight = np.zeros_like(weight, dtype=np.float32) # (D, C)
        # ========================= EDIT HERE ========================

        n = label.shape[0]
        for i in range(n):
            prob[i][label[i]] -= 1

        grad = np.dot(x.T, prob) / n
        grad_weight = grad

        # ============================================================
        return grad_weight


    def _softmax(self, x):
        """
        [INPUT]
        x : (N, C), score before softmax

        [OUTPUT]
        softmax : (same shape with x), softmax distribution over axis-1

        [Functionality]
        Given an input x, apply softmax function over axis-1 (classes).
        """
        softmax = None
        # ========================= EDIT HERE ========================

        x -= np.max(x, axis = 1, keepdims = True)
        sum_x = np.sum(np.exp(x), axis = 1, keepdims = True)
        softmax = np.exp(x) / sum_x
        
        # ============================================================
        return softmax