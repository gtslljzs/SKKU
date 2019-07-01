#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node {
    int id;
    char* name;
    int score;
    char grade;
    struct node* next;
} Node;

typedef struct list {
    int size;
    Node* head;
    Node* tail;
} List;

List* initList();
void insertNode(List* plist, int id, char* name, int score, char grade);
char calGrade(int score);
void printInfo(List* list);
void searchByName(List* plist, char* name);
void searchByID(List* plist, int id);
void changeScoreByName(List* plist, char* name);
void changeScoreByID(List* plist, int id);
void calMeanVar(List* plist);

int main(void) {
    
    List* students = initList();
    FILE* input = fopen("student.dat", "r");

    while (1) {
        int id;
        char* name = (char*)malloc(sizeof(char*));
        int score;
        char grade;

        if (fscanf(input, "%d %s %d", &id, name, &score) == EOF) break;

        grade = calGrade(score);
        insertNode(students, id, name, score, grade);
    }

    fclose(input);
    printInfo(students);
    calMeanVar(students);

}

List* initList() {
    List* plist = (List*)malloc(sizeof(List));
    plist->size = 0;
    plist->head = plist->tail = NULL;

    return plist;
}

void insertNode(List* _plist, int _id, char* _name, int _score, char _grade) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->id = _id;
    temp->name = _name;
    temp->score = _score;
    temp->grade = _grade;
    temp->next = NULL;

    if (_plist->tail == NULL) _plist->head = _plist->tail = temp;
    else {
        _plist->tail->next = temp;
        _plist->tail = temp;
    }

    _plist->size++;
}

char calGrade(int _score) {
    char grade;

    if (_score >= 90) grade = 'A';
    else if (_score >= 80) grade = 'B';
    else if (_score >= 70) grade = 'C';
    else if (_score >= 60) grade = 'D';
    else grade = 'F';

    return grade;
}

void printInfo(List* _plist) {
    Node* p = _plist->head;
    
    for( ; p; p = p->next)
        printf("%d %s %d %c\n", p->id, p->name, p->score, p->grade);
}

void calMeanVar(List* _plist) {
    float mu;
    float var;
    Node* p = _plist->head;

    for (; p; p = p->next)
        mu += (float)p->score;
    mu /= (float) _plist->size;

    p = _plist->head;
    for (; p; p = p->next)
        var += (float) pow(p->score - mu, 2);
    var = sqrt(var / (_plist->size - 1));

    printf("average = %f\n", mu);
    printf("standard deviation = %f\n", var);
}

void searchByName(List* _plist, char* _name) {
    Node* p = _plist->head;
    
    for( ; p; p = p->next) {
        if (p->name == _name) {
            printf("%d %s %d %c\n", p->id, p->name, p->score, p->grade);
            
            return;
        }
    }

    printf("no such student\n");
}

void searchByID(List* _plist, int _id) {
    Node* p = _plist->head;
    
    for( ; p; p = p->next) {
        if (p->id == _id) {
            printf("%d %s %d %c\n", p->id, p->name, p->score, p->grade);
            
            return;
        }
    }

    printf("no such student\n");
}

void changeScoreByName(List* _plist, char* _name) {
    Node* p = _plist->head;
    
    for( ; p; p = p->next) {
        if (p->name == _name) {
            printf("input score: ");
            scanf("%d", &p->score);
            p->grade = calGrade(p->score);
            
            return;
        }
    }

    printf("no such student\n");
}

void changeScoreByID(List* _plist, int _id) {
    Node* p = _plist->head;
    
    for( ; p; p = p->next) {
        if (p->id == _id) {
            printf("input score: ");
            scanf("%d", &p->score);
            p->grade = calGrade(p->score);
            
            return;
        }
    }

    printf("no such student\n");
}