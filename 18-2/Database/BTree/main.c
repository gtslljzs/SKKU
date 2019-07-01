#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BTREE.h"

int main(void)
{
	BTreeInit(5); // Max degree 5
	insertElement(1); insertElement(2); insertElement(3); insertElement(4);
	printTree();
	printf("\n");

	printf("====== split ======\n");
	insertElement(5); // split
	printTree();
	printf("\n");

	printf("====== balanced tree ======\n");
	insertElement(6); insertElement(7); insertElement(8); insertElement(9);
	insertElement(10); insertElement(11); insertElement(12); insertElement(13); 
	insertElement(14); insertElement(15); insertElement(16); insertElement(17);
	printTree();
	printf("\n");
	return 0;
}