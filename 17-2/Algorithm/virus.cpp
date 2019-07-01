#include <cstdio>
#include <limits>
#include <queue>
using namespace std;

int com_num, dpn, first, total_inf, total_time;
class Edge {
public:
    int u, v, weight;
    Edge () { }
    void print() {
        printf("(%d %d)\n", u, v);
    }
    bool operator> (const Edge& edge) const {
        return weight > edge.weight;
    }
};
typedef struct Table {
    int idx = -1;
    int dist = std::numeric_limits<int>::max();
    int prev = -1;
    bool visit = false;
    bool operator> (const Table& tab) const {
        return dist > tab.dist;
    }
} Table;

// Linked-List class
typedef struct L_Node {
    int idx;
    int weight;
    struct L_Node* next;
} Node;
class List {
public:
    Node *head;
    Node *cur;
    int data_num;
    List() {
        head = new Node;
        head->next = NULL;
        data_num = 0;
    }

    void Insert(int idx, int weight) {
        Node *n_node = new Node;
        n_node->idx = idx;
        n_node->weight = weight;

        if (head->next == NULL) {
            n_node->next = head->next;
            head->next = n_node;
            data_num++;
        } else {
            Node *s_node = head;
            while (s_node->next != NULL)
                s_node = s_node->next;

            n_node->next = s_node->next;
            s_node->next = n_node;
            data_num++;
        }
    }

    bool First(int *idx) {
        if (head->next == NULL)
            return false;

        cur = head->next;
        *idx = cur->idx;

        return true;
    }

    bool Next(int *idx) {
        if (cur->next == NULL)
            return false;

        cur = cur->next;
        *idx = cur->idx;

        return true;
    }
};

// Graph class
class Graph {
private:
    int ver_num;
public:
    List* adj_list;
    Graph(int v_n) {
        ver_num = v_n;
        adj_list = new List[ver_num];
    }

    void Destroy() {
        if (adj_list != NULL)
            delete[] adj_list;
    }

    void Add_Edge(int from, int to, int weight) {
        adj_list[from].Insert(to, weight);
    }

    void Show_Edge() {
        int vertex;

        for (int i = 0; i < ver_num; i++) {
            printf("Vertex connected with %d: ", i + 1);

            if (adj_list[i].First(&vertex)) {
                printf("%d ", vertex + 1);
                while (adj_list[i].Next(&vertex))
                    printf("%d ", vertex + 1);
            }
            printf("\n");
        }
    }
};

void Infection(Graph* pg) {
    int parent, next;
    Table *T = new Table[com_num];
    priority_queue< Table, vector<Table>, greater<Table> > PQ;

    parent = first - 1;
    T[parent].idx = parent;
    T[parent].dist = 0;
    do {
        T[parent].visit = true;
        if (pg->adj_list[parent].First(&next)) {
            T[next].idx = next;
            if (T[next].dist > T[parent].dist + pg->adj_list[parent].cur->weight) {
                T[next].dist = T[parent].dist + pg->adj_list[parent].cur->weight;
                T[next].prev = parent;
                PQ.push(T[next]);
            }

            while (pg->adj_list[parent].Next(&next)) {
                T[next].idx = next;
                if (T[next].dist > T[parent].dist + pg->adj_list[parent].cur->weight) {
                    T[next].dist = T[parent].dist + pg->adj_list[parent].cur->weight;
                    T[next].prev = parent;
                    PQ.push(T[next]);
                }
            }
        }

        while (T[parent].visit && !PQ.empty()) {
            parent = PQ.top().idx;
            PQ.pop();
        }
    } while (!PQ.empty());

    int cnt = 0;
    Table n_T = T[0];
    for (int i = 1; i < com_num; i++) {
        if (T[i].visit) {
            cnt++;
            if (T[i].dist > n_T.dist)
                n_T = T[i];
        }
    }
    
    total_inf = cnt;
    if (cnt == 0)
        n_T.dist = 0;
    total_time = n_T.dist;
}

int main(int argc, char** argv) {
    FILE *ifp = fopen(argv[1], "r");
    if (!ifp) {
        puts("Cannot open the input file!");
        return -1;
    }

    fscanf(ifp, "%d %d %d", &com_num, &dpn, &first);
    Graph G(com_num);
    int from, to, weight;
    for (int i = 0; i < dpn; i++) {
        fscanf(ifp, "%d %d %d", &to, &from, &weight);
        G.Add_Edge(from - 1, to - 1, weight);
    }

    Infection(&G);
    G.Destroy();

    FILE* ofp = fopen(argv[2], "w");
    if (!ofp) {
        puts("Cannot open the output file!");
        return -1;
    }
    fprintf(ofp, "%d %d", total_inf, total_time);
    fclose(ofp);

    return 0;
}

