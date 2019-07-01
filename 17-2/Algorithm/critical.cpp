#include <iostream>
#include <cstdio>
#include <queue>
#include <vector>

using namespace std;

int ver_num, edge_num;
int total_time = 0;
struct Table {
    int in_deg = 0;
    int t_time = 0;
    int c_time = 0;
    int prev = -1;
};
Table* T;
vector< vector<int> > graph;
queue<int> q;

void C_Time() {
    int cur, next;
    
    for (int i = 0; i < ver_num; i++) {
        cur = q.front();
        q.pop();
        T[cur].c_time = T[cur].t_time + T[cur].c_time;
        
        if (T[cur].c_time > total_time)
            total_time = T[cur].c_time;
        
        for (int j = 0; j < graph[cur].size(); j++) {
            next = graph[cur][j];
            
            T[next].in_deg--;
            if (T[next].in_deg == 0)
                q.push(next);
            
            if (T[cur].c_time > T[next].c_time) {
                T[next].c_time = T[cur].c_time;
                T[next].prev = cur;
            }
        }
    }
}

int main(int argc, char** argv) {
    FILE* ifp = fopen(argv[1], "r");
    if (!ifp) {
        cout << "Cannot open the input file!";
        return -1;
    }
    
    fscanf(ifp, "%d %d", &ver_num, &edge_num);
    T = new Table [ver_num];
    for (int i = 0; i < ver_num; i++)
        fscanf(ifp, "%d", &T[i].t_time);
    
    int u, v;
    graph = vector< vector<int> > (ver_num);
    for (int i = 0; i < edge_num; i++) {
        fscanf(ifp, "%d %d", &u, &v);
        u--;
        v--;
        graph[u].push_back(v);
        T[v].in_deg++;
    }
    
    fclose(ifp);
    
    for (int i = 0; i < ver_num; i++) {
        if (T[i].in_deg == 0)
            q.push(i);
    }
    
    if (q.empty()) {
        total_time = -1;
        FILE* ofp = fopen(argv[2], "w");
        if (!ofp) {
            cout << "Cannot open the output file!";
            return -1;
        }
        fprintf(ofp, "%d", total_time);
        fclose(ofp);
        
        return 0;
    }
    
    C_Time();
    
    FILE* ofp = fopen(argv[2], "w");
    if (!ofp) {
        cout << "Cannot open the output file!";
        return -1;
    }
    fprintf(ofp, "%d", total_time);
    fclose(ofp);
    
    return 0;
}