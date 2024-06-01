#ifndef RESERVE_H
#define RESERVE_H

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int id;
    char name[50];
    char schedule[200];
    Color color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct {
    Node* root;
    Node* nil;
} RBTree;

RBTree* createRBTree();
void rbInsert(RBTree* tree, int id, const char* name, const char* schedule);
void rbDelete(RBTree* tree, Node* z);
Node* rbSearch(RBTree* tree, int id);
void rbPrint(Node* root, Node* nil, int level);

void makeReservation(RBTree* tree, char* userName, char source, char destination, char* timeStr, int day);
void deleteReservation(RBTree* tree, int id);

#endif // RESERVE_H
