import numpy as np

class LinearRegression:
    def __init__(self, num_features):
        self.num_features = num_features
        self.W = np.zeros((self.num_features, 1))

    def train(self, x, y, epochs, batch_size, lr, optim):
        final_loss = None   # loss of final epoch

        # Training should be done for 'epochs' times with minibatch size of 'batch_size'
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
            h = (x_batch[cnt] @ self.W)

            # loss function
            loss = 0
            for i in range(len(x_batch[cnt])):
                loss += np.power((h - y_batch[cnt])[i], 2)
            loss /= len(x_batch[cnt])

            # calculating gradients
            grad = x_batch[cnt].T @ (h - y_batch[cnt]) / len(x_batch[cnt])

            # optimization
            self.W = optim.update(w = self.W, grad = grad, lr = lr)

        # calculating final loss
        final_loss = 0
        for i in range(len(x_batch[cnt])):
            final_loss += np.power((h - y_batch[cnt])[i], 2)
        final_loss /= len(x_batch[cnt])


        # ============================================================
        return final_loss

    def eval(self, x):
        pred = None

        # Evaluation Function
        # Given the input 'x', the function should return prediction for 'x'

        # ========================= EDIT HERE ========================

        x_test = x
        pred = (x_test @ self.W)

        # ============================================================
        return pred
