#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;

struct cor {
    double x;
    double y;
};
int ctz, shlt;
int time, spd;
int live;
int* c_set;
int* s_set;
bool* matched;
int* depth;
vector< vector< int > > graph;

double Dist( cor u, cor v ) {
    return sqrt( pow( u.x - v.x, 2 ) + pow( u.y - v.y, 2 ) );
}

void BFS() {
    queue<int> Q;
    int cur;
    
    for ( int i = 0; i < ctz; i++ ) {
        if ( !matched[i] ) {
            depth[i] = 0;
            Q.push( i );
        } else
            depth[i] = -1;
    }
    
    while ( !Q.empty() ) {
        cur = Q.front();
        Q.pop();
        
        for ( int i = 0; i < graph[cur].size(); i++ ) {
            if ( s_set[i] != -1 && depth[s_set[i]] == -1 ) {
                depth[s_set[i]] = depth[i] + 1;
                Q.push( s_set[i] );
            }
        }
    }
}
bool DFS( int cur ) {
    for ( int i = 0; i < graph[cur].size(); i++ ) {
        if ( s_set[i] == -1 || ( depth[s_set[i]] == depth[cur] + 1 && DFS( s_set[i] ) ) ) {
            matched[cur] = true;
            c_set[cur] = i;
            s_set[i] = cur;
            return true;
        }
    }
    
    return false;
}
void Runaway() {
    while ( true ) {
        BFS();
        
        int path = 0;
        for ( int i = 0; i < ctz; i++ ) {
            if ( !matched[i] && DFS(i))
                path++;
        }
        
        if ( path == 0 )
            break;
        
        live += path;
    }
}

int main( int argc, char** argv ) {
    FILE* ifp = fopen( argv[1], "r" );
    if ( !ifp ) {
        cout << "Cannot open the input file!";
        return -1;
    }
    
    fscanf( ifp, "%d %d", &ctz, &shlt );
    fscanf( ifp, "%d %d", &time, &spd );
    
    cor* C;
    cor* S;
    C = new cor [ctz];
    S = new cor [shlt];
    matched = new bool [ctz];
    depth = new int [ctz];
    c_set = new int [ctz];
    s_set = new int [shlt];
    graph = vector < vector < int > > ( ctz );
    
    for ( int i = 0; i < ctz; i++ )
        fscanf( ifp, "%lf %lf", &C[i].x, &C[i].y );
    fill( c_set, c_set + ctz, -1 );
    
    for ( int i = 0; i < shlt; i++ )
        fscanf( ifp, "%lf %lf", &S[i].x, &S[i].y );
    fill( s_set, s_set + shlt, -1 );
    
    fclose( ifp );
    
    for ( int i = 0; i < ctz; i++ ) {
        for ( int j = 0; j < shlt; j++ ) {
            if ( Dist( C[i], S[j] ) <= time * spd )
                graph[i].push_back(j);
        }
    }
    
    delete [] C;
    delete [] S;
    
    Runaway();
    
    delete [] matched;
    delete [] depth;
    delete [] c_set;
    delete [] s_set;
    
    FILE* ofp = fopen( argv[2], "w" );
    if ( !ofp ) {
        cout << "Cannot open the output file!";
        return -1;
    }
    
    fprintf( ofp, "%d", ctz - live );
    fclose( ofp );
    
    return 0;
}
