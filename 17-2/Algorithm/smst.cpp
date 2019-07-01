#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;

class Edge {
public:
    int u, v, weight;
    Edge () { }

    bool operator > (const Edge& edge) const {
        return weight > edge.weight;
    }

    bool operator < (const Edge& edge) const {
        return weight < edge.weight;
    }
};
Edge* edges;
Edge* not_added;
int ver_num, edge_num;
int mst_weight = 0;
int smst_weight = std::numeric_limits<int>::max();
int max_lv;
int* mst_set;
int* depth;
int** parent;
int** weight;
bool* visited;
vector< vector< pair<int, int> > > graph;

int Find_Set(int* set, int data) {
    while (set[data] != data) {
        data = set[data];
    }

    return data;
}
void Union_Set(int* set, int a, int b) {
    if (a > b) {
        int tmp = b;
        b = a;
        a = tmp;
    }
    a = Find_Set(set, a);
    b = Find_Set(set, b);

    if (a != b)
        set[b] = a;
}
void MST() {
    int cnt = 0;
    int a, b;

    for (int i = 0; i < edge_num; i++) {
        if (Find_Set(mst_set, edges[i].u) != Find_Set(mst_set, edges[i].v)) {
            Union_Set(mst_set, edges[i].u, edges[i].v);
            graph[edges[i].u].push_back(make_pair(edges[i].v, edges[i].weight));
            graph[edges[i].v].push_back(make_pair(edges[i].u, edges[i].weight));
            mst_weight += edges[i].weight;
        } else {
            not_added[cnt] = edges[i];
            cnt++;
        }
    }
}
void Setting(int cur, int ans) {
    int next;

    if (visited[cur])
        return;
    visited[cur] = true;

    for (int i = 0; i < graph[cur].size(); i++) {
        next = graph[cur][i].first;
        if (next == ans)
            continue;

        parent[next][0] = cur;
        weight[next][0] = graph[cur][i].second;
        depth[next] = depth[cur] + 1;
        Setting(next, cur);
    }
}
void SMST() {
    for (int j = 0; j < max_lv - 1; j++) {
        for (int i = 1; i < ver_num; i++) {
            int tmp = parent[i][j];
            if (parent[i][j] == -1)
                continue;
            parent[i][j + 1] = parent[tmp][0];
            weight[i][j + 1] = max(weight[i][j], weight[tmp][0]);
        }
    }

    parent[0][0] = 0;
    int dif;
    int l_weight;
    int tmp_smst = 0;

    // trace back parents
    for (int i = 0; i < edge_num - ver_num + 1; i++) {
        int x = not_added[i].u;
        int y = not_added[i].v;
        int w = not_added[i].weight;
        int tmp;

        if (depth[x] < depth[y]) {
            tmp = x;
            x = y;
            y = tmp;
        }

        dif = depth[x] - depth[y];
        if (dif) {
            l_weight = weight[x][dif - 1];
            x = parent[x][dif - 1];

            while (x != y) {
                int y_weight = weight[y][0];
                int x_weight = weight[x][0];
                int tmp_weight = max(x_weight, y_weight);
                l_weight = max(l_weight, tmp_weight);

                x = parent[x][0];
                y = parent[y][0];
            }
        } else {
            l_weight = 0;

            while (x != y) {
                int y_weight = weight[y][0];
                int x_weight = weight[x][0];
                int tmp_weight = max(x_weight, y_weight);
                l_weight = max(l_weight, tmp_weight);

                x = parent[x][0];
                y = parent[y][0];
            }
        }

        tmp_smst = mst_weight - l_weight + w;
        if (tmp_smst == mst_weight)
            continue;
        if (smst_weight > tmp_smst)
            smst_weight = tmp_smst;
    }
}

int main(int argc, char** argv) {
    FILE *ifp = fopen(argv[1], "r");
    if (!ifp) {
        puts("Cannot open the input file!");
        return -1;
    }

    fscanf(ifp, "%d %d", &ver_num, &edge_num);

    if (edge_num < ver_num - 1) {
        FILE *ofp = fopen(argv[2], "w");
        if (!ofp) {
            puts("Cannot open the output file!");
            return -1;
        }
        fprintf(ofp, "%d", -1);
        fclose(ofp);
        return 0;
    }

    max_lv = (int) log2(ver_num) + 100;
    edges = new Edge[edge_num];
    not_added = new Edge[edge_num - ver_num + 1];

    Edge* n_edge = new Edge;
    for (int i = 0; i < edge_num; i++) {
        fscanf(ifp, "%d %d %d", &n_edge->u, &n_edge->v, &n_edge->weight);
        n_edge->u--;
        n_edge->v--;
        edges[i] = *n_edge;
    }
    fclose(ifp);
    delete[] n_edge;

    // allocating
    sort(edges, edges + edge_num);
    graph = vector< vector< pair< int, int> > > (ver_num);
    mst_set = new int [ver_num];
    depth = new int [ver_num];
    visited = new bool [ver_num];
    memset(visited, false, sizeof(bool) * ver_num);
    for (int i = 0; i < ver_num; i++) {
        mst_set[i] = i;
        depth[i] = 0;
    }
    parent = new int* [ver_num];
    weight = new int* [ver_num];
    for (int i = 0; i < ver_num; i++) {
        parent[i] = new int[max_lv];
        weight[i] = new int[max_lv];
        memset(parent[i], -1, sizeof(int) * max_lv);
        memset(weight[i], -1, sizeof(int) * max_lv);
    }

    MST(); // making MST
    Setting(0, 0); // making parent and weight table by DFS

    SMST();

    FILE* ofp = fopen(argv[2], "w");
    if (smst_weight == std::numeric_limits<int>::max())
        smst_weight = -1;

    fprintf(ofp, "%d", smst_weight);
    fclose(ofp);

    return 0;
}

