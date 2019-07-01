class SGD:
    def __init__(self):
        pass

    def update(self, w, grad, lr):
        ww = w - lr * grad
        return ww