import numpy as np

def Accuracy(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the accuracy for prediction.
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - Acc : (scalar, float), Computed accuracy score
    # ========================= EDIT HERE =========================
    Acc = 0

    if len(label.shape) == 1:
        label = np.expand_dims(label, 1)
    if len(pred.shape) == 1:
        pred = np.expand_dims(pred, 1)

    total = label.shape[0]
    correct = len(np.where(label==pred)[0])
    Acc = correct / total

    # =============================================================
    return Acc

def Precision(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the Precision for prediction.
    #         you should consider that label = 1 is positive. 0 is negative
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - precision : (scalar, float), Computed precision score
    # ========================= EDIT HERE =========================
    precision = 0

    if len(label.shape) == 1:
        label = np.expand_dims(label, 1)
    if len(pred.shape) == 1:
        pred = np.expand_dims(pred, 1)

    relevant = len(np.where(pred == 1)[0])
    if relevant == 0:
        return 1
    
    true_positive = 0
    for i in range(len(label)):
        if label[i] == 1 and pred[i] == 1:
            true_positive += 1
    
    precision = true_positive / relevant

    # =============================================================
    return precision

def Recall(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the Recall for prediction.
    #         you should consider that label = 1 is positive. 0 is negative
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - recall : (scalar, float), Computed recall score
    # ========================= EDIT HERE =========================
    recall = 0

    if len(label.shape) == 1:
        label = np.expand_dims(label, 1)
    if len(pred.shape) == 1:
        pred = np.expand_dims(pred, 1)

    relevant = len(np.where(label == 1)[0])
    if relevant == 0:
        return 1
    
    true_positive = 0
    for i in range(len(label)):
        if label[i] == 1 and pred[i] == 1:
            true_positive += 1
    
    recall = true_positive / relevant

    # =============================================================
    return recall

def F_measure(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the F-measure score for prediction.
    #         you can erase the code. (F_score = 0.)
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - F_score : (scalar, float), Computed F-score score
    # ========================= EDIT HERE =========================
    F_score = 0

    precision = Precision(label, pred)
    recall = Recall(label, pred)

    if precision + recall == 0:
        return 1
    
    F_score = (2 * precision * recall) / (precision + recall)

    # =============================================================
    return F_score

def MAP(label, hypo, at = 10):
    ########################################################################################
    # TODO : Complete the code to calculate the MAP for prediction.
    #         Notice that, hypo is the real value array in (0, 1)
    #         MAP (at = 10) means MAP @10
    #         [Input]
    #         - label : (N, K), Correct label with 0 (incorrect) or 1 (correct)
    #         - hypo  : (N, K), Predicted score between 0 and 1
    #         - at: (int), # of element to consider from the first. (TOP-@)
    #         [output]
    #         - Map : (scalar, float), Computed MAP score
    # ========================= EDIT HERE =========================
    Map = 0.
    
    queries = len(label)
    query_len = len(label[0])
    indices = np.array(-hypo).argsort()
    
    ranks = np.zeros_like(label)
    for i in range(queries):
        ranks[i] = label[i][indices[i]]

    relevant = []
    for i in range(queries):
        cnt = 0

        for j in range(query_len):
            if ranks[i][j] == 1:
                cnt += 1
        
        relevant.append(cnt)
    
    precisions = np.zeros_like(hypo)
    for i in range(queries):
        tp = 0

        for j in range(query_len):
            if ranks[i][j] == 1:
                tp += 1
                precisions[i][j] = tp / (j + 1)
            else:
                precisions[i][j] = tp / (j + 1)

    for i in range(queries):
        precision = 0

        for j in range(at):
            if ranks[i][j] == 1:
                precision += precisions[i][j]
        
        precision /= relevant[i]
        Map += (precision / queries)

    # =============================================================
    return Map

def nDCG(label, hypo, at = 10):
    ########################################################################################
    # TODO : Complete the each code to calculate the nDCG for prediction.
    #         you can erase the code. (dcg, idcg, ndcg = 0.)
    #         Notice that, hypo is the real value array in (0, 1)
    #         nDCG (at = 10 ) means nDCG @10
    #         [Input]
    #         - label : (N, K), Correct label with 0 (incorrect) or 1 (correct)
    #         - hypo  : (N, K), Predicted score between 0 and 1
    #         - at: (int), # of element to consider from the first. (TOP-@)
    #         [output]
    #         - Map : (scalar, float), Computed nDCG score


    def DCG(label, hypo, at=10):
        # ========================= EDIT HERE =========================
        dcg = []

        queries = len(label)
        indices = np.array(-hypo).argsort()
    
        ranks = np.zeros_like(label)
        for i in range(queries):
            ranks[i] = label[i][indices[i]]

        for i in range(queries):
            temp = 0

            for j in range(at):
                temp += ranks[i][j] / np.log2(j + 2)

            dcg.append(temp)

        # =============================================================
        return dcg

    def IDCG(label, hypo, at=10):
        # ========================= EDIT HERE =========================
        idcg = []

        queries = len(label)

        perfect_rank = np.zeros_like(label)
        for i in range(queries):
            perfect_rank[i] = np.sort(label[i])[::-1]
        
        for i in range(queries):
            temp = 0

            for j in range(at):
                temp += perfect_rank[i][j] / np.log2(j + 2)
            
            idcg.append(temp)

        # =============================================================
        return idcg
    # ========================= EDIT HERE =========================
    ndcg = 0.

    queries = len(label)
    ndcgs = []
    dcgs = DCG(label, hypo, at)
    idcgs = IDCG(label, hypo, at)

    for i in range(queries):
        ndcgs.append(dcgs[i] / idcgs[i])

    ndcg = np.sum(ndcgs) / queries

    # =============================================================
    return ndcg

# =============================================================== #
# ===================== DO NOT EDIT BELOW ======================= #
# =============================================================== #

def evaluation_test1(label, pred, at = 10):
    result = {}

    result['Accuracy '] = Accuracy(label, pred)
    result['Precision'] = Precision(label, pred)
    result['Recall   '] = Recall(label, pred)
    result['F_measure'] = F_measure(label, pred)

    return result

def evaluation_test2(label, hypo, at = 10):
    result = {}

    result['MAP  @%d'%at] = MAP(label, hypo, at)
    result['nDCG @%d'%at] = nDCG(label, hypo, at)

    return result
