import numpy as np

class LogisticRegression:
    def __init__(self, num_features):
        self.num_features = num_features
        self.W = np.zeros((self.num_features, 1))

    def train(self, x, y, epochs, batch_size, lr, optim):
        final_loss = None   # loss of final epoch

        # Train should be done for 'epochs' times with minibatch size of 'batch size'
        # The function 'train' should return the loss of final epoch
        # Loss of an epoch is calculated as an average of minibatch losses

        # ========================= EDIT HERE ========================

        x_train = x
        y_train = y.reshape(-1, 1)

        # creating mini-batch
        x_batch = []
        y_batch = []
        num_batches = len(x_train) // batch_size
        if len(x_train) % batch_size != 0:
            num_batches += 1
        
        for i in range(num_batches):
            x_mini = x_train[i * batch_size : (i + 1) * batch_size]
            y_mini = y_train[i * batch_size : (i + 1) * batch_size]
            x_batch.append(x_mini)
            y_batch.append(y_mini)

        # training
        for i in range(epochs):
            cnt = i % (num_batches)
            
            # hypothesis
            h = self._sigmoid((x_batch[cnt] @ self.W))

            # distance loss function
            loss = 0
            for i in range(len(x_batch[cnt])):
                loss += np.power((h - y_batch[cnt])[i], 2)
            loss /= len(x_batch[cnt])
            
            # cross-entropy loss function
            # ce = 0
            # for i in range(len(x_batch[cnt])):
            #     ce += (y_batch[cnt][i] * np.log(h[i]) + (1 - y_batch[cnt][i]) * np.log(1 - h[i]))
            # print(ce)

            # calculating gradients
            grad = x_batch[cnt].T @ (h - y_batch[cnt]) / len(x_batch[cnt])

            # optimization
            self.W = optim.update(w = self.W, grad = grad, lr = lr)
        
        # calculating final loss
        final_loss = 0
        for i in range(len(x_batch[cnt])):
            final_loss += np.power((h - y_batch[cnt])[i], 2)
        final_loss /= len(x_batch[cnt])

        # calculating cross-entropy loss
        h = self._sigmoid((x_train @ self.W))
        ce = 0
        for i in range(len(x_train)):
            ce += (y_train[i] * np.log(h[i]) + (1 - y_train[i]) * np.log(1 - h[i]))
        print(ce)


        # ============================================================
        return final_loss

    def eval(self, x):
        threshold = 0.5
        pred = None

        # Evaluation Function
        # Given the input 'x', the function should return prediction for 'x'
        # The model predicts the label as 1 if the probability is greater or equal to 'threshold'
        # Otherwise, it predicts as 0

        # ========================= EDIT HERE ========================

        x_test = x
        pred = self._sigmoid((x_test @ self.W))
        
        for i in range(len(pred)):
            if pred[i] > threshold:
                pred[i] = 1
            else:
                pred[i] = 0

        # ============================================================

        return pred

    def _sigmoid(self, x):
        sigmoid = np.empty_like(x)

        # Sigmoid Function
        # The function returns the sigmoid of 'x'

        # ========================= EDIT HERE ========================

        for i in range(len(x)):
            sigmoid[i] = 1 / (1 + np.exp(-x[i]))

        # ============================================================
        return sigmoid