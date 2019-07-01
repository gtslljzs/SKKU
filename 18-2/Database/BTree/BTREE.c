#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "BTREE.h"

int compare (const void *first, const void *second)
{
    if (*(int*)first > *(int*)second)
        return 1;
    else if (*(int*)first < *(int*)second)
        return -1;
    else 
        return 0;
}

void BTreeInit(int _t)
{
	root = NULL;  t = _t - 1;
}


void traverse()
{
	if (root != NULL) _traverse(root);
}


BTreeNode* search(int k)
{
	return (root == NULL) ? NULL : _search(root, k);
}


BTreeNode* _createNode(bool _leaf)
{
	BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
	int i;

	// Copy the given minimum degree and leaf property
	newNode->leaf = _leaf;

	// Allocate memory for maximum number of possible keys
	// and child pointers
	newNode->keys = (int*)malloc(sizeof(int) * (t+1));
	newNode->C = (BTreeNode**)malloc(sizeof(BTreeNode*)*(t+2));

	// Initialize child
	for (i = 0; i < t + 2; i++)
		newNode->C[i] = NULL;

	// Initialize the number of keys as 0
	newNode->n = 0;

	return newNode;
}


void _traverse(BTreeNode* present)
{
	// There are n keys and n+1 children, travers through n keys and first n children
	int i;
	for (i = 0; i < present->n; i++)
	{
		// If this is not leaf, then before printing key[i],
		// traverse the subtree rooted with child C[i].
		if (present->leaf == false)
			_traverse(present->C[i]);

		printf(" ");
		printf("%d", present->keys[i]);
	}

	// Print the subtree rooted with last child
	if (present->leaf == false)
		_traverse(present->C[i]);
}


BTreeNode* _search(BTreeNode* present, int k)
{
	// Find the first key greater than or equal to k
	int i = 0;
	while (i < present->n && k > present->keys[i])
		i++;

	// If the found key is equal to k, return this node
	if (i < present->n && present->keys[i] == k)
		return present;

	// If key is not found here and this is a leaf node
	if (present->leaf == true)
		return NULL;

	// Go to the appropriate child
	return _search(present->C[i], k);
}


void insertElement(int k)
{ 
	// Find key in this tree, and If there is a key, it prints error message.
	if (search(k) != NULL)
	{
		printf("The tree already has %d \n", k);
		return;
	}

	// If tree is empty
	if (root == NULL)
	{
		// Allocate memory for root
		root = _createNode(true);
		root->keys[0] = k;  // Insert key
		root->n = 1;  // Update number of keys in root
	}
	else // If tree is not empty
		_insert(root, k, NULL);
}


void _insert(BTreeNode* present, int k, BTreeNode* parent)
{
	// Implement your code
	// ................
	int i, temp;
	present = root;

  	// when root is leaf and root is full
	if(present->leaf == true && present->n == t) {
		temp = _splitChild(present, NULL, -1);
		present = root;

		// find where to insert
		for(i = 0; i < (present->n); i++) {
			if((k > present->keys[i]) && (k < present->keys[i + 1])) {
				i++;
				break;
			} else if(k < present->keys[0]) {
				break;
			} else {
				continue;
			}
		}
		present = present->C[i];
	}

	// when root is not leaf
	else {
		// go to leaf node
		while(present->leaf == false) {
			for(i = 0; i < (present->n); i++) {
				if((k > present->keys[i]) && (k < present->keys[i + 1])) {
					i++;
					break;
				} else if(k < present->keys[0]) {
					break;
				} else {
					continue;
				}
			}

			// when leaf node is full, split
			if((present->C[i])->n == t) {
				temp = _splitChild(present->C[i], present, i);
				present->keys[present->n] = temp;
				present->n++;
				continue;
			} else {
				present = present->C[i];
			}
		}
	}
 	
 	present->keys[present->n] = k;
 	qsort(present->keys, present->n, sizeof(int), compare);
 	present->n++;

}


int _splitChild(BTreeNode* present, BTreeNode* parent, int i)
{
	// Implement your code.
	// ................
	int j, mid;
  	BTreeNode *new_parent, *new_right, *y; 

	new_right = _createNode(true);

	// if parent is null = present is root
	if(i == -1) {

		// split by median
		mid = present->keys[t / 2];
		present->keys[t / 2] = 0;
		present->n--;

		// make new parent
		new_parent = _createNode(false);

		// set present to left leaf node
		present->leaf = true;
		for(j = t / 2 + 1; j < t; j++) {

			// new_right takes right parts of present
			new_right->keys[j - (t / 2 + 1)] = present->keys[j];
			new_right->C[j - (t / 2 + 1)] = present->C[j];
			new_right->n++;

			// present takes left parts of its own
			present->keys[j] = 0;
			present->n--;
		}

		// set all childrent of present to null
		for(j = 0; j < t + 1; j++) {
		present->C[j] = NULL;
		}

		new_parent->keys[0] = mid;
		new_parent->C[new_parent->n] = present;
		new_parent->C[new_parent->n + 1] = new_right;
		new_parent->n++;

		// set new parent to root
		root = new_parent;
  	}

 	// if parent already exists
	else {
		y = parent->C[i];
		mid = y->keys[t / 2];
		y->keys[t / 2] = 0;
		y->n--;

		for(j = t / 2 + 1; j < t; j++) {

		// new_right takes right parts of present
		new_right->keys[j - (t / 2 + 1)] = y->keys[j];
		new_right->n++;
		y->keys[j] = 0;
		y->n--;
		}

		parent->C[i] = y;
		parent->C[i + 1] = new_right; 
	}

	return mid;
}


// This code related to print the B-tree
int _getLevel(BTreeNode* present)
{
	int i;
	int mapresentLevel = 0;
	int temp;
	if(present == NULL) return mapresentLevel;
	if(present->leaf == true)
		return mapresentLevel+1;

	for (i = 0; i < present->n+1; i++)
	{
		temp = _getLevel(present->C[i]);

		if (temp > mapresentLevel)
			mapresentLevel = temp;
	}

	return mapresentLevel + 1;
}

// This code related to print the B-tree
void _getNumberOfNodes(BTreeNode* present, int* numNodes, int level)
{
	int i;
	if (present == NULL) return;

	if (present->leaf == false)
	{
		for (i = 0; i < present->n+1; i++)
			_getNumberOfNodes(present->C[i], numNodes, level + 1);
	}
	numNodes[level] += 1;
}

// This code related to print the B-tree
void _mappingNodes(BTreeNode* present, BTreeNode ***nodePtr, int* numNodes, int level)
{
	int i;
	if (present == NULL) return;

	if (present->leaf == false)
	{
		for (i = 0; i < present->n+1; i++)
			_mappingNodes(present->C[i], nodePtr, numNodes, level + 1);
	}

	nodePtr[level][numNodes[level]] = present;
	numNodes[level] += 1;
}

// This code related to print the B-tree
void printTree()
{
	int level;
	int *numNodes;
	int i,j,k;

	level = _getLevel(root);
	numNodes = (int *)malloc(sizeof(int) * (level));
	memset(numNodes, 0, level * sizeof(int));

	_getNumberOfNodes(root, numNodes, 0);

	BTreeNode ***nodePtr;
	nodePtr = (BTreeNode***)malloc(sizeof(BTreeNode**) * level);
	for (i = 0; i<level; i++) {
		nodePtr[i] = (BTreeNode**)malloc(sizeof(BTreeNode*) * numNodes[i]);
	}

	memset(numNodes, 0, level * sizeof(int));
	_mappingNodes(root, nodePtr, numNodes, 0);

	for (i = 0; i < level; i++) {	
		for (j = 0; j < numNodes[i]; j++) {
			printf("[");
			
			for (k = 0; k < nodePtr[i][j]->n; k++)
				printf("%d ", nodePtr[i][j]->keys[k]);
			
			printf("] ");
		}
		printf("\n");
	}

	for (i = 0; i<level; i++) {
		free(nodePtr[i]);
	}
	free(nodePtr);
}