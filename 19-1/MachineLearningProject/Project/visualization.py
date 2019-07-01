from sklearn.manifold import TSNE
import matplotlib as mpl
import matplotlib.pyplot as plt
import gensim
import gensim.models as gsmodel
import pandas as pd

mpl.rcParams['axes.unicode_minus'] = False
model_name = 'embedding'
model = gsmodel.word2vec.Word2Vec.load('embedding.model')

vocab = list(model.wv.vocab)
X = model[vocab]

print(len(X))
print(X[0][:10])

tsne = TSNE(n_components=2)

# 100개의 단어에 대해서만 시각화
X_tsne = tsne.fit_transform(X[:100, :])

df = pd.DataFrame(X_tsne, index=vocab[:100], columns=['x', 'y'])
print(df.shape)
print(df.head(10))

fig = plt.figure()
fig.set_size_inches(40, 20)
ax = fig.add_subplot(1, 1, 1)

ax.scatter(df['x'], df['y'])

for word, pos in df.iterrows():
    ax.annotate(word, pos, fontsize=30)

plt.show()
