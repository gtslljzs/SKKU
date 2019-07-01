#include <iostream>
#include <cstdio>
#include <cmath>
#include <queue>
#include <stack>

using namespace std;

int row, col;
int limit, max_cost;
int max_height = 0;
struct cor {
    int x, y;
};
int** H;

int Dist(cor cur, cor next) {
    int cur_h, next_h;
    cur_h = H[cur.x][cur.y];
    next_h = H[next.x][next.y];
    
    if (cur_h > next_h)
        return cur_h - next_h;
    else
        return next_h - cur_h;
}

int Cost(cor cur, cor next) {
    if (H[cur.x][cur.y] == H[next.x][next.y])
        return 0;
    else
        return (int) pow(H[next.x][next.y] - H[cur.x][cur.y], 2) + 1;
}

void Return() {
    cor start = {0, 0};
    cor cur, next;
    max_height = H[start.x][start.y];
    bool visited[row][col];
    int cost[row][col];
    int n_cost;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            visited[i][j] = false;
            cost[i][j] = 0;
        }
    }
    
    queue<cor> q;
    q.push(start);
    cor dn, de, ds, dw;
    
    while (!q.empty()) {
        cur = q.front();
        q.pop();
        visited[cur.x][cur.y] = true;
        
        if (H[cur.x][cur.y] > max_height && cost[cur.x][cur.y] <= max_cost)
            max_height = H[cur.x][cur.y];
        
        dn = {cur.x - 1, cur.y};
        de = {cur.x, cur.y + 1};
        ds = {cur.x + 1, cur.y};
        dw = {cur.x, cur.y - 1};
        
        if (dn.x >= 0 && Dist(cur, dn) <= limit) {
            if (!visited[dn.x][dn.y] && cost[dn.x][dn.y] == 0) {
                cost[dn.x][dn.y] = cost[cur.x][cur.y] + Cost(cur, dn);
                q.push(dn);
            } else if (cost[dn.x][dn.y] > cost[cur.x][cur.y] + Cost(cur, dn)) {
                cost[dn.x][dn.y] = cost[cur.x][cur.y] + Cost(cur, dn);
                q.push(dn);
            }
        }
        
        if (de.y < col && Dist(cur, de) <= limit) {
            if (!visited[de.x][de.y] && cost[de.x][de.y] == 0) {
                cost[de.x][de.y] = cost[cur.x][cur.y] + Cost(cur, de);
                q.push(de);
            } else if (cost[de.x][de.y] > cost[cur.x][cur.y] + Cost(cur, de)) {
                cost[de.x][de.y] = cost[cur.x][cur.y] + Cost(cur, de);
                q.push(de);
            }
        }
        
        if (ds.x < row && Dist(cur, ds) <= limit) {
            if (!visited[ds.x][ds.y] && cost[ds.x][ds.y] == 0) {
                cost[ds.x][ds.y] = cost[cur.x][cur.y] + Cost(cur, ds);
                q.push(ds);
            } else if (cost[ds.x][ds.y] > cost[cur.x][cur.y] + Cost(cur, ds)) {
                cost[de.x][ds.y] = cost[cur.x][cur.y] + Cost(cur, ds);
                q.push(ds);
            }
        }
        
        if (dw.y >= 0 && Dist(cur, dw) <= limit) {
            if (!visited[dw.x][dw.y] && cost[dw.x][dw.y] == 0) {
                cost[dw.x][dw.y] = cost[cur.x][cur.y] + Cost(cur, dw);
                q.push(dw);
            } else if (cost[dw.x][dw.y] > cost[cur.x][cur.y] + Cost(cur, dw)) {
                cost[dw.x][dw.y] = cost[cur.x][cur.y] + Cost(cur, dw);
                q.push(dw);
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
    
    fscanf(ifp, "%d %d", &row, &col);
    fscanf(ifp, "%d %d", &limit, &max_cost);
    
    H = new int* [row];
    for (int i = 0; i < row; i++)
        H[i] = new int [col];
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            fscanf(ifp, "%d", &H[i][j]);
        }
    }
    
    fclose(ifp);
    
    Return();
    
    FILE* ofp = fopen(argv[2], "w");
    if (!ofp) {
        cout << "Cannot open the output file!";
        return -1;
    }
    
    fprintf(ofp, "%d", max_height);
    fclose(ofp);
    
    return 0;
}