#include <cstdio>
int n, oper_num;
int* parent;

int Find_Set(int* parent, int data) {
    while (parent[data] != data)
        data = parent[data];

    return data;
}
void Union_Set(int a, int b) {
    a = Find_Set(parent, a);
    b = Find_Set(parent, b);

    if (a != b)
        parent[b] = a;
}

int main(int argc, char** argv) {
    FILE* ifp = fopen(argv[1], "r");
    FILE* ofp = fopen(argv[2], "w");
    fscanf(ifp, "%d %d", &n, &oper_num);

    parent = new int [n + 1];
    for (int i = 0; i < n + 1; i++)
        parent[i] = i;

    int inst, a, b;
    for (int i = 0; i < oper_num; i++) {
        fscanf(ifp, "%d %d %d", &inst, &a, &b);
        if (!inst) {
            if (Find_Set(parent, a) != Find_Set(parent, b))
                Union_Set(a, b);
        } else {
            if (Find_Set(parent, a) == Find_Set(parent, b))
                fprintf(ofp, "Y\n");
            else
                fprintf(ofp, "N\n");
        }
    }

    fclose(ifp);
    fclose(ofp);

    return 0;
}