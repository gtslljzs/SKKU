import numpy as np

"""
DO NOT EDIT ANY PARTS OTHER THAN "EDIT HERE" !!! 

[Description]
__init__ - Initialize necessary variables for optimizer class
input   : gamma, epsilon
return  : X

update   - Update weight for one minibatch
input   : w - current weight, grad - gradient for w, lr - learning rate
return  : updated weight 
"""

class SGD:
    def __init__(self, gamma, epsilon):
        # ========================= EDIT HERE =========================
        pass


        # =============================================================

    def update(self, w, grad, lr):
        updated_weight = None
        # ========================= EDIT HERE =========================

        updated_weight = w
        for i in range(len(w)):
            updated_weight[i] -= lr * grad[i]

        # =============================================================
        return updated_weight

class Momentum:
    def __init__(self, gamma, epsilon):
        # ========================= EDIT HERE =========================
        
        self.gamma = gamma
        self.itr = 0
        self.v = []


        # =============================================================

    def update(self, w, grad, lr):
        updated_weight = None
        # ========================= EDIT HERE =========================

        if self.itr == 0:
            self.v = np.zeros((len(grad), 1))
            self.itr += 1

        updated_weight = w
        for i in range(len(w)):
            self.v[i] = self.gamma * self.v[i] + grad[i]
            updated_weight[i] -= lr * self.v[i]

        # =============================================================
        return updated_weight


class RMSProp:
    # ========================= EDIT HERE =========================
    def __init__(self, gamma, epsilon):
        # ========================= EDIT HERE =========================
        
        self.gamma = gamma
        self.epsilon = epsilon
        self.itr = 0
        self.v = []

        # =============================================================

    def update(self, w, grad, lr):
        updated_weight = None
        # ========================= EDIT HERE =========================

        if self.itr == 0:
            self.v = np.zeros((len(grad), 1))
            self.itr += 1

        updated_weight = w
        for i in range(len(w)):
            self.v[i] = self.gamma * self.v[i] + (1 - self.gamma) * np.power(grad[i], 2)
            updated_weight[i] -= lr * (grad[i] / np.sqrt(self.v[i] + self.epsilon))
        
        # =============================================================
        return updated_weight