#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
using namespace std;

int stone_num, goal;
int total_dist = 0;
const double max_dist = 3;
typedef struct Stone {
    int xpos;
    int ypos;
    bool operator< (const Stone& stone) const {
        return ypos < stone.ypos;
    }
} Stone;
typedef struct Table {
    double dist = numeric_limits<double>::max();
    int idx = -1;
    int parent = -1;
    int ypos = -1;
    bool visit = false;
    bool operator> (const Table& tab) const {
        return dist > tab.dist;
    }
} Table;
bool Compare_x(Stone s1, Stone s2) {
    return s1.xpos < s2.xpos;
}
bool Compare_y(Stone s1, Stone s2) {
    return s1.ypos < s2.ypos;
}

// Linked-List class
typedef struct L_Node {
    double dist;
    int idx;
    int xpos;
    int ypos;
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

    void Insert(int idx, double dist, int xpos, int ypos) {
        Node* n_node = new Node;
        n_node->idx = idx;
        n_node->dist = dist;
        n_node->xpos = xpos;
        n_node->ypos = ypos;

        if (head->next == NULL) {
            n_node->next = head->next;
            head->next = n_node;
            data_num++;
        } else {
            Node* s_node = head;
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

    void Add_Edge(int from, int to, double dist, int xpos, int ypos) {
        adj_list[from].Insert(to, dist, xpos, ypos);
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

double Dist(Stone* p, Stone* q) {
    return fabs(sqrt(pow(p->xpos - q->xpos, 2) + (pow(p->ypos - q->ypos, 2))));
}
void Make_Graph(Graph* pg, Stone* ref) {
    Stone n_stone, target;
    double dist;

    for (int i = 0; i < stone_num + 1; i++) {
        n_stone.xpos = ref[i].xpos;
        n_stone.ypos = ref[i].ypos;
        for (int j = i + 1; ref[j].xpos <= n_stone.xpos + 2  && j < stone_num + 1; j++) {
            target.xpos = ref[j].xpos;
            target.ypos = ref[j].ypos;
            dist = Dist(&n_stone, &target);
            if (dist < max_dist) {
                pg->Add_Edge(i, j, dist, target.xpos, target.ypos);
                pg->Add_Edge(j, i, dist, n_stone.xpos, n_stone.ypos);
            }
        }
    }
}
void Shortest_Path(Graph* pg) {
    Table *T = new Table[stone_num + 1];
    priority_queue<Table, vector<Table>, greater<Table> > PQ;
    int parent, next;

    parent = 0;
    T[parent].dist = 0;
    T[parent].idx = parent;
    T[parent].ypos = 0;
    for (int i = 0; i < stone_num + 1; i++) {
        T[parent].visit = true;
        if (T[parent].ypos >= goal) {
            total_dist = (int) floor(T[parent].dist + 0.5);
            break;
        }

        if (pg->adj_list[parent].First(&next)) {
            T[next].idx = next;
            if (T[next].dist > T[parent].dist + pg->adj_list[parent].cur->dist) {
                T[next].dist = T[parent].dist + pg->adj_list[parent].cur->dist;
                T[next].parent = parent;
                T[next].ypos = pg->adj_list[parent].cur->ypos;
                PQ.push(T[next]);
            }
            while (pg->adj_list[parent].Next(&next)) {
                T[next].idx = next;
                if (T[next].dist > T[parent].dist + pg->adj_list[parent].cur->dist) {
                    T[next].dist = T[parent].dist + pg->adj_list[parent].cur->dist;
                    T[next].parent = parent;
                    T[next].ypos = pg->adj_list[parent].cur->ypos;
                    PQ.push(T[next]);
                }
            }
        }

        while (T[parent].visit && !PQ.empty()) {
            parent = PQ.top().idx;
            PQ.pop();
        }
    }
}

int main(int argc, char** argv) {
    FILE *ifp = fopen(argv[1], "r");
    if (!ifp) {
        puts("Cannot open the input file!");
        return -1;
    }
    fscanf(ifp, "%d %d", &stone_num, &goal);
    Stone* n_stone = new Stone;
    Stone* stones = new Stone[stone_num + 1];
    n_stone->xpos = 0;
    n_stone->ypos = 0;
    stones[0] = *n_stone;
    for (int i = 1; i <= stone_num; i++) {
        fscanf(ifp, "%d %d", &n_stone->xpos, &n_stone->ypos);
        stones[i] = *n_stone;
    }
    fclose(ifp);

    delete[] n_stone;
    stable_sort(stones, stones + stone_num + 1, Compare_y);
    stable_sort(stones, stones + stone_num + 1, Compare_x);
    Graph G(stone_num + 1);
    Make_Graph(&G, stones);
    Shortest_Path(&G);

    FILE* ofp = fopen(argv[2], "w");
    if (!ofp) {
        puts("Cannot open the output file!");
        return -1;
    }
    fprintf(ofp, "%d", total_dist);
    fclose(ofp);
    return 0;
}