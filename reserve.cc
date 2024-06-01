#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int id;
    char name[50];
    char schedule[200];
    Color color;
    struct Node *left, *right, *parent;
} Node;

typedef struct {
    Node* root;
    Node* nil;
} RBTree;

RBTree* createRBTree() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->nil = (Node*)malloc(sizeof(Node));
    tree->nil->color = BLACK;
    tree->root = tree->nil;
    return tree;
}

Node* createNode(RBTree* tree, int id, const char* name, const char* schedule) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->id = id;
    strcpy(newNode->name, name);
    strcpy(newNode->schedule, schedule);
    newNode->color = RED;
    newNode->left = newNode->right = newNode->parent = tree->nil;
    return newNode;
}

void leftRotate(RBTree* tree, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(RBTree* tree, Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != tree->nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void rbInsertFixup(RBTree* tree, Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void rbInsert(RBTree* tree, int id, const char* name, const char* schedule) {
    Node* z = createNode(tree, id, name, schedule);
    Node* y = tree->nil;
    Node* x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (z->id < x->id) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (z->id < y->id) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = tree->nil;
    z->right = tree->nil;
    z->color = RED;

    rbInsertFixup(tree, z);
}

void rbTransplant(RBTree* tree, Node* u, Node* v) {
    if (u->parent == tree->nil) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

Node* treeMinimum(RBTree* tree, Node* x) {
    while (x->left != tree->nil) {
        x = x->left;
    }
    return x;
}

void rbDeleteFixup(RBTree* tree, Node* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

void rbDelete(RBTree* tree, Node* z) {
    Node* y = z;
    Node* x;
    Color yOriginalColor = y->color;

    if (z->left == tree->nil) {
        x = z->right;
        rbTransplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        rbTransplant(tree, z, z->left);
    } else {
        y = treeMinimum(tree, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbTransplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK) {
        rbDeleteFixup(tree, x);
    }

    free(z);
}

Node* rbSearch(RBTree* tree, int id) {
    Node* current = tree->root;
    while (current != tree->nil) {
        if (id < current->id) {
            current = current->left;
        } else if (id > current->id) {
            current = current->right;
        } else {
            return current;
        }
    }
    return NULL;
}

void rbPrint(Node* root, Node* nil, int level) {
    if (root != nil) {
        rbPrint(root->right, nil, level + 1);
        printf("(%d, %s, %s, %s, level %d)\n", root->id, root->name, root->schedule,
               root->color == RED ? "RED" : "BLACK", level);
        rbPrint(root->left, nil, level + 1);
    }
}

int reservationID = 1;

void makeReservation(RBTree* tree, char* userName, char source, char destination, char* timeStr, int day) {
    char reservationDetails[100];
    sprintf(reservationDetails, "Reservation for %s: %c to %c on day %d at %s", userName, source, destination, day, timeStr);
    rbInsert(tree, reservationID++, userName, reservationDetails);
}


void deleteReservation(RBTree* tree, int id) {
    Node* node = rbSearch(tree, id);
    if (node != NULL) {
        rbDelete(tree, node);
        printf("Reservation %d deleted.\n", id);
    } else {
        printf("Reservation %d not found.\n", id);
    }
}