#include <cstdio>
#include <queue>
#include <limits>
using namespace std;

// Global variables
int ver_num, edge_num, smst_weight = 0;
class Edge {
public:
    int u, v, weight;
    Edge() { }
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
    int parent = -1;
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
        adj_list[from - 1].Insert(to - 1, weight);
        adj_list[to - 1].Insert(from - 1, weight);
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

void Prim(Graph* pg) {
    Table *Tab = new Table[ver_num];
    Edge *n_edge = new Edge;
    priority_queue < Edge, vector<Edge>, greater<Edge> > PQ;

    int parent, next;
    parent = 0;
    Tab[parent].idx = 0;
    Tab[parent].dist = 0;

    do {
        Tab[parent].visit = true;
        smst_weight += Tab[parent].dist;

        if (pg->adj_list[parent].First(&next)) {
            Tab[next].idx = next;
            n_edge->u = parent;
            n_edge->v = next;
            n_edge->weight = pg->adj_list[parent].cur->weight;
            if (Tab[next].dist > pg->adj_list[parent].cur->weight) {
                Tab[next].parent = parent;
                Tab[next].dist = pg->adj_list[parent].cur->weight;
                PQ.push(*n_edge);
            }

            while (pg->adj_list[parent].Next(&next)) {
                Tab[next].idx = next;
                n_edge->v = next;
                n_edge->weight = pg->adj_list[parent].cur->weight;

                if (Tab[next].dist > pg->adj_list[parent].cur->weight) {
                    Tab[next].parent = parent;
                    Tab[next].dist = pg->adj_list[parent].cur->weight;
                    PQ.push(*n_edge);
                }
            }
        }

        while (Tab[parent].visit && !PQ.empty()) {
            parent = PQ.top().v;
            PQ.pop();
        }
    } while (!PQ.empty());
}

int main(int argc, char** argv) {
    FILE *ifp = fopen(argv[1], "r");
    if (!ifp) {
        puts("Cannot open the input file!");
        return -1;
    }

    fscanf(ifp, "%d %d", &ver_num, &edge_num);
    Graph G(ver_num);
    Edge* n_edge = new Edge;
    for (int i = 0; i < edge_num; i++) {
        fscanf(ifp, "%d %d %d", &n_edge->u, &n_edge->v, &n_edge->weight);
        G.Add_Edge(n_edge->u, n_edge->v, n_edge->weight);
    }
    delete[] n_edge;
    fclose(ifp);

    Prim(&G);
    G.Destroy();

    FILE *ofp = fopen(argv[2], "w");
    if (!ofp) {
        puts("Cannot open the output file!");
        return -1;
    }

    if (edge_num < ver_num - 1)
        smst_weight = -1;
    fprintf(ofp, "%d", smst_weight);
    fclose(ofp);

    return 0;
}

