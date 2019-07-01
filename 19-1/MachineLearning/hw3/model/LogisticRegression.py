import numpy as np

class LogisticRegression:
    def __init__(self, num_features):
        self.num_features = num_features
        self.W = np.zeros((self.num_features, 1))

    def train(self, x, y, batch_size, epochs, lr, optim):
        loss_memory = [] # loss of every epoch
        epoch_memory = []
        num_data = len(x)

        for epoch in range(epochs):
            st = 0
            batch_losses = []
            while st < num_data:
                ed = min(st + batch_size, num_data)

                batch_x = x[st:ed]
                batch_y = y[st:ed]
                batch_y = np.expand_dims(batch_y, 1)

                act = np.matmul(batch_x, self.W)
                hyp = self._sigmoid(act)
                error = hyp - batch_y

                loss = batch_y * np.log(hyp) + (1 - batch_y) * np.log(1 - hyp)
                loss = - np.mean(loss)
                batch_losses.append(loss)



                # Compute Gradient
                grad = np.mean(batch_x * error, axis=0, keepdims=True).T

                # Update Weights
                self.W = optim.update(self.W, grad, lr)

                st += batch_size

            loss_memory.append(sum(batch_losses) / len(batch_losses))
            epoch_memory.append(epoch)
        return loss_memory, epoch_memory

    def predict(self, x, threshold=0.5):
        hyp = self._sigmoid(np.matmul(x, self.W))

        pred = np.zeros_like(hyp, dtype=np.int32)
        pred[hyp >= threshold] = 1

        return hyp, pred

    def _sigmoid(self, x):
        return 1 / (1 + np.exp(-x))