#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int coef;
    int exp;
    struct Node* next;
} Node;

typedef struct List {
    int size;
    Node* head;
    Node* tail;
} List;

List* initList();
void printList(List* plist);
void insertNode(List* plist, int _coef, int _exp);
void addition(List* plist1, List* plist2, List* plist3);
void calculate(List* plist, int _x);

int main() {

    List* a;
    List* b;
    List* result;
    int x;

    a = initList();
    b = initList();
    result = initList();

    insertNode(a, 3, 12);
    insertNode(a, 2, 8);
    insertNode(a, 1, 0);

    insertNode(b, 8, 12);
    insertNode(b, -3, 10);
    insertNode(b, 10, 6);

    printList(a);
    printList(b);

    addition(a, b, result);
    printList(result);

    printf("input x: ");
    scanf("%d", &x);

    calculate(result, x);

    free(a); free(b); free(result);

}

List* initList() {
    
    List* plist = (List*)malloc(sizeof(List));
    plist->size = 0;
    plist->head = plist->tail = NULL;

    return plist;
}

void insertNode(List* plist, int _coef, int _exp) {

    Node* temp = (Node*)malloc(sizeof(Node));
    temp->coef = _coef;
    temp->exp = _exp;
    temp->next = NULL;

    if (plist->tail == NULL) {
        plist->head = plist->tail = temp;
    } else {
        plist->tail->next = temp;
        plist->tail = temp;
    }

    plist->size++;
}

void printList(List* plist) {
    Node* p = plist->head;
    
    printf("polynomial = ");
    for( ; p; p = p->next) {
        if (p->next != NULL) printf("%dx^%d + ", p->coef, p->exp);
        else printf("%dx^%d", p->coef, p->exp);
    }

    printf("\n");
}

void addition(List* plist1, List* plist2, List* plist3) {

    Node* a = plist1->head;
    Node* b = plist2->head;
    int sum = 0;

    while (a && b) {
        
        if (a->exp == b->exp) {

            sum = a->coef + b->coef;
            if (sum != 0) insertNode(plist3, sum, a->exp);
            
            a = a->next;
            b = b->next;

        } else if (a->exp > b->exp) {

            insertNode(plist3, a->coef, a->exp);
            a = a->next;

        } else {

            insertNode(plist3, b->coef, b->exp);
            b = b->next;

        }
    }

    for ( ; a; a = a->next) insertNode(plist3, a->coef, a->exp);
    for ( ; b; b = b->next) insertNode(plist3, b->coef, b->exp);
}

void calculate(List* plist, int _x) {

    Node* temp = plist->head;
    int result = 0;

    if (temp != NULL) {

        for ( ; temp; temp = temp->next)
            result += temp->coef * pow(_x, temp->exp);
        
    }

    printf("result: %d\n", result);
}