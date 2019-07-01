#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define MALLOC(p, s) if (!((p) = malloc(s))) { \
                        fprintf(stderr, "Insufficient memory for malloc!"); \
                        exit(EXIT_FAILURE); \
                    }

// Declare edge structure and global variables
typedef struct Edge {
    int v1;
    int v2;
    int weight;
} Edge;
typedef Edge Data;
int num_of_vertex = 0;
int num_of_edge = 0;
int min_total_weight = 0;
Edge *e_arr;

// Helper functions
typedef int Priority(Data w1, Data w2);
int PQ_Weight_Dsc_Comp(Edge d1, Edge d2) {
    return d2.weight - d1.weight;
}
int PQ_Weight_Asc_Comp(Edge d1, Edge d2) {
    return d1.weight - d2.weight;
}
void Read_File(FILE *fp) {
    int i;

    fp = fopen("hw4_input.txt", "r");
    if (!fp) {
        puts("Cannot open the file!");
        exit(-1);
    }

    fscanf(fp, "%d", &num_of_edge);
    MALLOC(e_arr, sizeof(Edge) * num_of_edge);

    for (i = 0; i < num_of_edge; i++) {
        fscanf(fp, "%d %d %d", &e_arr[i].v1, &e_arr[i].v2, &e_arr[i].weight);

        if (e_arr[i].v1 > num_of_vertex)
            num_of_vertex = e_arr[i].v1;
        if (e_arr[i].v2 > num_of_vertex)
            num_of_vertex = e_arr[i].v2;
    }

    fclose(fp);
}
void Write_File(FILE *fp, int data) {
    fp = fopen("hw4_output.txt", "w");

    if (!fp) {
        puts("Cannot open the file!");
        exit(-1);
    }

    fprintf(fp, "%d", data);
    fclose(fp);
}

// Linked-List functions
typedef struct L_Node {
    int data;
    struct L_Node *next;
} Node;
typedef struct Linked_List {
    Node *head;
    Node *cur;
    Node *before;
    int num_of_data;
} List;
void L_Init(List *plist) {
    MALLOC(plist->head, sizeof(Node));
    plist->head->next = NULL;
    plist->num_of_data = 0;
}
void L_Insert(List *plist, int data) {
    Node *n_node = (Node *) malloc(sizeof(Node));
    n_node->data = data;

    if (plist->head->next == NULL) {
        n_node->next = plist->head->next;
        plist->head->next = n_node;
        (plist->num_of_data)++;
    } else {
        Node *s_node = plist->head;
        while (s_node->next != NULL) {
            s_node = s_node->next;
        }

        n_node->next = s_node->next;
        s_node->next = n_node;
        (plist->num_of_data)++;
    }
}
int L_First(List *plist, int *pdata) {
    if (plist->head->next == NULL)
        return FALSE;
    plist->before = plist->head;
    plist->cur = plist->head->next;

    *pdata = plist->cur->data;

    return TRUE;
}
int L_Next(List *plist, int *pdata) {
    if (plist->cur->next == NULL)
        return FALSE;

    plist->before = plist->cur;
    plist->cur = plist->cur->next;

    *pdata = plist->cur->data;
    return TRUE;
}
int L_Remove(List *plist, int data) {
    Node *r_node = plist->head;

    while (r_node->next->data != data) {
        r_node = r_node->next;
    }
    plist->before = r_node;
    r_node = r_node->next;
    plist->cur = r_node;
    plist->before->next = plist->cur->next;
    plist->cur = plist->before;

    free(r_node);
    (plist->num_of_data)--;
}

// Stack declares
typedef struct Array_Stack {
    int *s_arr;
    int top;
} Stack;
void S_Init(Stack *pstack, int num_vertex) {
    pstack->top = -1;
    MALLOC(pstack->s_arr, sizeof(int) * num_vertex);
}
int S_is_Empty(Stack *pstack) {
    if (pstack->top == -1)
        return TRUE;
    else
        return FALSE;
}
void S_Append(Stack *pstack, int data) {
    pstack->top += 1;
    pstack->s_arr[pstack->top] = data;
}
int S_Pop(Stack *pstack) {
    int r_idx;

    if (S_is_Empty(pstack)) {
        printf("Stack is empty!");
        exit(-1);
    }

    r_idx = pstack->top;
    pstack->top -= 1;

    return pstack->s_arr[r_idx];
}
int S_Peek(Stack *pstack) {
    if (S_is_Empty(pstack)) {
        printf("Stack is empty!");
        exit(-1);
    }

    return pstack->s_arr[pstack->top];
}

// Heap functions
typedef struct Heap {
    Priority *pc;
    int num_of_data;
    Data *h_arr;
} Heap;
void H_Init(Heap *pheap, int size, Priority pc) {
    pheap->num_of_data = 0;
    MALLOC(pheap->h_arr, sizeof(Data) * size);
    pheap->pc = pc;
}
int H_is_Empty(Heap *pheap) {
    if (pheap->num_of_data == 0)
        return TRUE;
    else
        return FALSE;
}
int Get_Parent_Idx(int idx) {
    return idx / 2;
}
int Get_LChild_Idx(int idx) {
    return idx * 2;
}
int Get_RChild_Idx(int idx) {
    return idx * 2 + 1;
}
int Get_Hi_Pri_Child_Idx(Heap *pheap, int idx) {
    if (Get_LChild_Idx(idx) > pheap->num_of_data)
        return 0;
    else if (Get_LChild_Idx(idx) == pheap->num_of_data)
        return Get_LChild_Idx(idx);
    else {
        if (pheap->pc(pheap->h_arr[Get_LChild_Idx(idx)], pheap->h_arr[Get_RChild_Idx(idx)]) < 0)
            return Get_LChild_Idx(idx);
        else
            return Get_RChild_Idx(idx);
    }
}
void H_Insert(Heap *pheap, Data data) {
    int idx = pheap->num_of_data + 1;

    while (idx != 1) {
        if (pheap->pc(data, pheap->h_arr[Get_Parent_Idx(idx)]) < 0) {
            pheap->h_arr[idx] = pheap->h_arr[Get_Parent_Idx(idx)];
            idx = Get_Parent_Idx(idx);
        } else
            break;
    }

    pheap->h_arr[idx] = data;
    pheap->num_of_data += 1;
}
Data H_Delete(Heap *pheap) {
    Data d_data = pheap->h_arr[1];
    Data last_data = pheap->h_arr[pheap->num_of_data];

    int parent_idx = 1;
    int child_idx;

    while (child_idx = Get_Hi_Pri_Child_Idx(pheap, parent_idx)) {
        if (pheap->pc(last_data, pheap->h_arr[child_idx]) < 0)
            break;

        pheap->h_arr[parent_idx] = pheap->h_arr[child_idx];
        parent_idx = child_idx;
    }

    pheap->h_arr[parent_idx] = last_data;
    pheap->num_of_data -= 1;

    return d_data;
}

// Priority-Queue functions
typedef Heap PQueue;
void PQ_Init(PQueue *ppq, int size, Priority pc) {
    H_Init(ppq, size, pc);
}
int PQ_is_Empty(PQueue *ppq) {
    return H_is_Empty(ppq);
}
void PQ_Append(PQueue *ppq, Data data) {
    H_Insert(ppq, data);
}
Data PQ_Pop(PQueue *ppq) {
    return H_Delete(ppq);
}

// Graph functions
typedef struct Graph {
    int num_v;
    int num_e;
    List *adj_list;
    int *visit_info;
    PQueue d_pq;
    PQueue a_pq;
} Graph;
void G_Init(Graph *pgraph, int nv, int ne) {
    int i;

    MALLOC(pgraph->adj_list, sizeof(List) * nv);
    pgraph->num_v = nv;
    pgraph->num_e = 0;

    for (i = 0; i < nv; i++) {
        L_Init(&(pgraph->adj_list[i]));
    }

    MALLOC(pgraph->visit_info, sizeof(int) * pgraph->num_v);
    memset(pgraph->visit_info, 0, sizeof(int) * pgraph->num_v);

    PQ_Init(&(pgraph->d_pq), ne + 1, PQ_Weight_Dsc_Comp);
    PQ_Init(&(pgraph->a_pq), ne + 1, PQ_Weight_Asc_Comp);
}
void G_Destroy(Graph *pgraph) {
    if (pgraph->adj_list != NULL)
        free(pgraph->adj_list);

    if (pgraph->visit_info != NULL)
        free(pgraph->visit_info);
}
void Add_Edge(Graph *pgraph, int from_v, int to_v, int weight) {
    Edge edge = {from_v, to_v, weight};

    L_Insert(&(pgraph->adj_list[from_v]), to_v);
    L_Insert(&(pgraph->adj_list[to_v]), from_v);
    pgraph->num_e += 1;

    PQ_Append(&(pgraph->d_pq), edge);
}
void Show_Edge(Graph *pgraph) {
    int i;
    int vx;

    for (i = 0; i < pgraph->num_v; i++) {
        printf("Vertex connected with %d: ", i + 1);

        if (L_First(&(pgraph->adj_list[i]), &vx)) {
            printf("%d ", vx + 1);
            while (L_Next(&(pgraph->adj_list[i]), &vx))
                printf("%d ", vx + 1);
        }
        printf("\n");
    }
}
int Visit_Vertex(Graph *pgraph, int visit_v) {
    if (pgraph->visit_info[visit_v] == 0) {
        pgraph->visit_info[visit_v] = 1;

        return TRUE;
    }

    return FALSE;
}
void Show_DFS_Vertex(Graph *pgraph, int size, int start_v) {
    Stack stack;
    int visit_v = start_v - 1;
    int next_v;

    S_Init(&stack, size);
    Visit_Vertex(pgraph, visit_v);
    S_Append(&stack, visit_v);

    while (L_First(&(pgraph->adj_list[visit_v]), &next_v) == TRUE) {
        int visit_flag = FALSE;

        if (Visit_Vertex(pgraph, next_v) == TRUE) {
            S_Append(&stack, visit_v);
            visit_v = next_v;
            visit_flag = TRUE;
        } else {
            while (L_Next(&(pgraph->adj_list[visit_v]), &next_v) == TRUE) {
                if (Visit_Vertex(pgraph, next_v) == TRUE) {
                    S_Append(&stack, visit_v);
                    visit_v = next_v;
                    visit_flag = TRUE;
                    break;
                }
            }
        }

        if (visit_flag == FALSE) {
            if (S_is_Empty(&stack) == TRUE) {
                break;
            } else {
                visit_v = S_Pop(&stack);
            }
        }
    }
    memset(pgraph->visit_info, 0, sizeof(int) * pgraph->num_v);
}

// Kruskal algorithm functions
void Remove_Way_Edge(Graph *pgraph, int from_v, int to_v) {
    int edge;

    if (L_First(&(pgraph->adj_list[from_v]), &edge)) {
        if (edge == to_v) {
            L_Remove(&(pgraph->adj_list[from_v]), to_v);
            return;
        }

        while (L_Next(&(pgraph->adj_list[from_v]), &edge)) {
            if (edge == to_v) {
                L_Remove(&(pgraph->adj_list[from_v]), to_v);
                return;
            }
        }
    }
}
void Remove_Edge(Graph *pgraph, int from_v, int to_v) {
    Remove_Way_Edge(pgraph, from_v, to_v);
    Remove_Way_Edge(pgraph, to_v, from_v);
    (pgraph->num_e)--;
}
void Recover_Edge(Graph *pgraph, int from_v, int to_v, int weight) {
    L_Insert(&(pgraph->adj_list[from_v]), to_v);
    L_Insert(&(pgraph->adj_list[to_v]), from_v);
    (pgraph->num_e)++;
}
int Is_Conn_Vertex(Graph *pgraph, int v1, int v2, int num_vertex) {
    Stack stack;
    int visit_v = v1;
    int next_v;

    S_Init(&stack, num_vertex);
    Visit_Vertex(pgraph, visit_v);
    S_Append(&stack, visit_v);

    while (L_First(&(pgraph->adj_list[visit_v]), &next_v) == TRUE) {
        int visit_flag = FALSE;

        if (next_v == v2) {
            memset(pgraph->visit_info, 0, sizeof(int) * pgraph->num_v);
            return TRUE;
        }

        if (Visit_Vertex(pgraph, next_v) == TRUE) {
            S_Append(&stack, visit_v);
            visit_v = next_v;
            visit_flag = TRUE;
        } else {
            while (L_Next(&(pgraph->adj_list[visit_v]), &next_v) == TRUE) {
                if (next_v == v2) {
                    memset(pgraph->visit_info, 0, sizeof(int) * pgraph->num_v);
                    return TRUE;
                }

                if (Visit_Vertex(pgraph, next_v) == TRUE) {
                    S_Append(&stack, visit_v);
                    visit_v = next_v;
                    visit_flag = TRUE;
                    break;
                }
            }
        }

        if (visit_flag == FALSE) {
            if (S_is_Empty(&stack) == TRUE)
                break;
            else
                visit_v = S_Pop(&stack);
        }
    }

    memset(pgraph->visit_info, 0, sizeof(int) * pgraph->num_v);
    return FALSE;
}
void D_Kruskal_MST(Graph *pgraph, int num_edge, int num_vertex) {
    Edge rcv_e_arr[num_edge];
    Edge edge;
    int e_idx = 0;
    int i;

    while (!PQ_is_Empty(&(pgraph->d_pq))) {
        edge = PQ_Pop(&(pgraph->d_pq));
        Remove_Edge(pgraph, edge.v1, edge.v2);

        if (!Is_Conn_Vertex(pgraph, edge.v1, edge.v2, num_vertex)) {
            Recover_Edge(pgraph, edge.v1, edge.v2, edge.weight);
            rcv_e_arr[e_idx++] = edge;
        } else {
            PQ_Append(&(pgraph->a_pq), edge);
        }
    }

    for (i = 0; i < e_idx; i++) {
        PQ_Append(&(pgraph->d_pq), rcv_e_arr[i]);
    }
}
void I_Kruskal_MST(Graph *pgraph, int num_vertex) {
    Edge edge;
    while (!PQ_is_Empty(&(pgraph->a_pq))) {
        edge = PQ_Pop(&(pgraph->a_pq));
        if (!Is_Conn_Vertex(pgraph, edge.v1 - 1, edge.v2 - 1, num_vertex)) {
            Add_Edge(pgraph, edge.v1 - 1, edge.v2 - 1, edge.weight);
        }
    }
}
int Total_Edge_Weight(Graph *pgraph) {
    int total_weight = 0;

    PQueue copy_pq = pgraph->d_pq;
    Edge edge;

    while (!PQ_is_Empty(&copy_pq)) {
        edge = PQ_Pop(&copy_pq);
        total_weight += edge.weight;
    }

    return total_weight;
}

void SMST(void) {
    int i;
    int j = 0;

    //------------------------------------------------------------------------------------------------------------------
    // Making MST
    Graph G1;
    G_Init(&G1, num_of_vertex, num_of_edge);

    for (i = 0; i < num_of_edge; i++) {
        Add_Edge(&G1, e_arr[i].v1 - 1, e_arr[i].v2 - 1, e_arr[i].weight);
    }

    D_Kruskal_MST(&G1, num_of_edge, num_of_vertex);

    //------------------------------------------------------------------------------------------------------------------
    // Making SMST
    int size = G1.a_pq.num_of_data;
    int w_arr[size];
    int total_weight;

    while (!PQ_is_Empty(&G1.a_pq)) {
        Graph G2;
        G_Init(&G2, num_of_vertex, num_of_edge);

        Add_Edge(&G2, G1.a_pq.h_arr[1].v1, G1.a_pq.h_arr[1].v2, G1.a_pq.h_arr[1].weight);
        PQ_Pop(&G1.a_pq);

        for (i = 0; i < num_of_edge; i++) {
            PQ_Append(&G2.a_pq, e_arr[i]);
        }

        I_Kruskal_MST(&G2, num_of_vertex);
        total_weight = Total_Edge_Weight(&G2);

        w_arr[j] = total_weight;
        j++;
    }

    min_total_weight = w_arr[0];
    for (i = 1; i < size; i++) {
        if (w_arr[i] < min_total_weight)
            min_total_weight = w_arr[i];
    }
}

int main(void) {
    clock_t start, end;
    double result;

    FILE *fp;
    Read_File(fp);

    printf("edge: %d\n", num_of_edge);
    printf("vertex: %d", num_of_vertex);

    start = clock();
    SMST();
    end = clock();

    FILE *fp2;
    Write_File(fp2, min_total_weight);

    result = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\nActual running time: %f\n", result);

    return 0;
}