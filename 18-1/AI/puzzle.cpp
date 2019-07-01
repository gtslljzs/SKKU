#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
using namespace std;


class Tree {
public:
    struct Node {
        Node* parent;
        Node* child1;
        Node* child2;
        Node* child3;
        Node* child4;
        int* state;
        int cost, coste, heuri;
        int blank;
        int lev;
    };
    
    struct cmp : public binary_function< Node*, Node*, bool > {
        bool operator() ( const Node* a, const Node* b ) const {
            return a->cost > b->cost;
        }
    };
    
    priority_queue< Node*, vector< Node* >, cmp > pq;
    vector< int > start;
    Node* root;
    bool goal;
    int visit;
    int open;

public:
    Tree() {
        root = NULL;
        visit = 0;
        open = -0;
    }
    bool isEmpty() const {
        return root == NULL;
    }
    void init( char** );
    void setRoot( vector< int > );
    void mdHeuri( Node* );
    void oopHeuri( Node* );
    bool movPuzzle( Node*, Node*, char );
    bool isGoal( Node* );
    void searchTree();
    void build( char** );
};

void Tree::init( char** file ) {
    int tmp;
    goal = false;
    FILE* fp = fopen( file[1], "r" );
    
    while ( fscanf( fp, "%d", &tmp ) != EOF )
        start.push_back( tmp );
    
    fclose( fp );
}
void Tree::mdHeuri( Node* puzzle ) {
    int h;
    int tmp;
    puzzle-> heuri = 0;
    
    for ( int i = 0; i < 9; i++ ) {
        tmp = puzzle->state[i] - 1;
        h = abs( ( tmp % 3 ) - ( i % 3 ) ) + abs( ( tmp / 3 ) - ( i / 3 ) );
        puzzle->heuri += h;
    }
}
void Tree::oopHeuri( Node* puzzle ) {
    puzzle->heuri = 0;
    
    for ( int i = 0; i < 9; i++ )
        if ( puzzle->state[i] != i + 1 )
            puzzle->heuri++;
}
void Tree::setRoot( vector< int > start ) {
    Node* tmp = new Node;
    tmp->state = new int[9];
    
    for ( int i = 0; i < 9; i++ )
        tmp->state[i] = start[i];
    
    tmp->parent = NULL;
    tmp->child1 = NULL;
    tmp->child2 = NULL;
    tmp->child3 = NULL;
    tmp->child4 = NULL;
    
    tmp->lev = 0;
    tmp->coste = 0;
    //mdHeuri( tmp );
    oopHeuri( tmp );
    tmp->cost = tmp->coste + tmp->heuri;
    
    for ( int i = 0; i < 9; i++ ) {
        if ( tmp->state[i] == 9 )
            tmp->blank = i;
    }
    
    if ( isEmpty() ) {
        root = tmp;
        pq.push( root );
        open++;
    }
}
bool Tree::movPuzzle( Node* cur, Node* next, char dir ) {
    int tmp;
    int bPos = cur->blank;
    next->state = new int[9];
    
    for ( int i = 0; i < 9; i++ )
        next->state[i] = cur->state[i];
    
    switch ( dir ) {
        case 'u':
            next->blank = bPos - 3;
            if ( cur->parent != NULL )
                if ( next->blank == cur->parent->blank )
                    return false;
            
            tmp = next->state[bPos];
            next->state[bPos] = next->state[bPos - 3];
            next->state[bPos -3] = tmp;
            cur->child1 = next;
            break;
        case 'd':
            next->blank = bPos + 3;
            if ( cur->parent != NULL )
                if ( next->blank == cur->parent->blank )
                    return false;
            
            tmp = next->state[bPos];
            next->state[bPos] = next->state[bPos + 3];
            next->state[bPos + 3] = tmp;
            cur->child2 = next;
            break;
        case 'l':
            next->blank = bPos - 1;
            if ( cur->parent != NULL )
                if ( next->blank == cur->parent->blank )
                    return false;
            
            tmp = next->state[bPos];
            next->state[bPos] = next->state[bPos - 1];
            next->state[bPos - 1] = tmp;
            cur->child3 = next;
            break;
        case 'r':
            next->blank = bPos + 1;
            if ( cur->parent != NULL )
                if ( next->blank == cur->parent->blank )
                    return false;
            
            tmp = next->state[bPos];
            next->state[bPos] = next->state[bPos + 1];
            next->state[bPos + 1] = tmp;
            cur->child4 = next;
            break;
    }
    
    return true;
}
bool Tree::isGoal( Node* cur ) {
    for ( int i = 0; i < 9; i++ )
        if ( cur->state[i] != i + 1 )
            return false;
    
    for ( int i = 0; i < 9; i++ )
        printf( "%d ", cur->state[i] );
    cout << endl;
    cout << "visit: " << visit << endl;
    cout << "open: " << open << endl;
    cout << "len: " << cur->lev << endl;
    
    goal =  true;
    return true;
}
void Tree::searchTree() {
    Node* cur = pq.top();
    pq.pop();
    visit++;
    
    if ( isGoal( cur ) )
        return;
    
    Node* c1 = new Node;
    Node* c2 = new Node;
    Node* c3 = new Node;
    Node* c4 = new Node;
    
    if ( cur->blank / 3 != 0 && movPuzzle( cur, c1, 'u' ) ) {
        c1->lev = cur->lev + 1;
        c1->coste = cur->coste + 1;
        //mdHeuri( c1 );
        oopHeuri( c1 );
        c1->cost = c1->coste + c1->heuri;
        
        c1->parent = cur;
        c1->child1 = NULL;
        c1->child2 = NULL;
        c1->child3 = NULL;
        c1->child4 = NULL;
        pq.push( c1 );
        open++;
    }
    
    
    if ( cur->blank / 3 != 2 && movPuzzle( cur, c2, 'd' ) ) {
        c2->lev = cur->lev + 1;
        c2->coste = cur->coste + 1;
        //mdHeuri( c2 );
        oopHeuri( c2 );
        c2->cost = c2->coste + c2->heuri;
        
        c2->parent = cur;
        c2->child1 = NULL;
        c2->child2 = NULL;
        c2->child3 = NULL;
        c2->child4 = NULL;
        pq.push( c2 );
        open++;
    }
    
    if ( cur->blank % 3 != 0 && movPuzzle( cur, c3, 'l' ) ) {
        c3->lev = cur->lev + 1;
        c3->coste = cur->coste + 1;
        //mdHeuri( c3 );
        oopHeuri( c3 );
        c3->cost = c3->coste + c3->heuri;
        
        c3->parent = cur;
        c3->child1 = NULL;
        c3->child2 = NULL;
        c3->child3 = NULL;
        c3->child4 = NULL;
        pq.push( c3 );
        open++;
    }
    
    if ( cur->blank % 3 != 2 && movPuzzle( cur, c4, 'r' ) ) {
        c4->lev = cur->lev + 1;
        c4->coste = cur->coste + 1;
        //mdHeuri( c4 );
        oopHeuri( c4 );
        c4->cost = c4->coste + c4->heuri;
        
        c4->parent = cur;
        c4->child1 = NULL;
        c4->child2 = NULL;
        c4->child3 = NULL;
        c4->child4 = NULL;
        pq.push( c4 );
        open++;
    }
}
void Tree::build( char** file ) {
    init( file );
    setRoot( start );
    while ( !goal )
        searchTree();
}

int main( int argc, char** argv ) {
    Tree T;
    T.build( argv );
    
    return 0;
}
