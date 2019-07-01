import numpy as np




"""
  =========================================
|| COPY Codes from the previous assignment ||
  =========================================
"""

class SGD:
    # ========================= EDIT HERE =========================
    def __init__(self, gamma, epsilon):
        pass

    def update(self, w, grad, lr):
        # print(w.shape)
        # print(grad.shape)
        w -= lr * grad
        
        return w
    # =============================================================

class Momentum:
    # ========================= EDIT HERE =========================
    def __init__(self, gamma, epsilon):
        self.gamma = gamma
        self.itr = 0

    def update(self, w, grad, lr):
        if self.itr == 0:
            self.v = np.zeros_like(grad, dtype = np.float32)
            self.itr += 1
        
        v = self.gamma * self.v + grad
        w -= lr * v

        return w
    # =============================================================

class RMSProp:
    # ========================= EDIT HERE =========================
    def __init__(self, gamma, epsilon):
        self.gamma = gamma
        self.epsilon = epsilon
        self.itr = 0

    def update(self, w, grad, lr):
        if self.itr == 0:
            self.v = np.zeros_like(grad, dtype = np.float32)
            self.itr += 1

        v = (self.gamma * self.v) + (1 - self.gamma) * (grad * grad)
        w -= lr * (grad / np.sqrt(self.v + self.epsilon))

        return w
    # =============================================================