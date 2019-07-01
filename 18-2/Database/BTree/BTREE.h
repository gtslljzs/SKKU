#ifndef BTREE_H_
#define BTREE_H_
#include <stdbool.h>

typedef struct BTreeNode {
	int *keys;  // An array of keys
	struct BTreeNode **C; // An array of child pointers
	int n;     // Current number of keys
	bool leaf; // Is true when node is leaf. Otherwise false
} BTreeNode;

BTreeNode *root; // Pointer to root node
int t;  // B-tree degree

void BTreeInit(int _t); // Initializes tree as empty
BTreeNode* _createNode(bool _leaf);

void traverse(); // A function to traverse all nodes in a subtree rooted with this node
void _traverse(BTreeNode* present);

BTreeNode* search(int k); // function to search a key in this tree
BTreeNode* _search(BTreeNode* present, int k);

// insert part
void insertElement(int k); // The main function that inserts a new key in this B-Tree
void _insert(BTreeNode* present, int k, BTreeNode* parent);
int _splitChild(BTreeNode* present, BTreeNode* parent, int i);

// print B-tree
int _getLevel(BTreeNode* present);
void _getNumberOfNodes(BTreeNode* present, int* numNodes, int level);
void _mappingNodes(BTreeNode* present, BTreeNode ***nodePtr, int* numNodes, int level);
void printTree();

#endif /* BTREE_H_ */