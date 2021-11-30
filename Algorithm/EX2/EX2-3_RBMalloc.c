#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_L 10
#define MAX_DATA_SIZE 1000002
#define NIL 0
#define INIT_ROOT 1

enum color_set{red, black};

typedef struct RBTree{
    /*
     * red-black tree node
     * to present an interval in the malloc space
     * a is the left side of the interval
     * b is the right side of the interval
     * left is for the left child
     * right is for the right child
     * parent is the node's parent
     * color is either red or black
     * length is the current size of the RBTree
    */
    int* a;
    int* b;
    int* left;
    int* right;
    int* parent;
    enum color_set* color;
    int length;
    int root;
}RBTreeNode, * RBTreeList;

void RBTreeInit(RBTreeList T) {
    T->a = (int*)malloc(MAX_DATA_SIZE*sizeof(int));
    memset(T->a, 0, MAX_DATA_SIZE*sizeof(int));
    T->b = (int*)malloc(MAX_DATA_SIZE*sizeof(int));
    memset(T->b, 0, MAX_DATA_SIZE*sizeof(int));
    T->left = (int*)malloc(MAX_DATA_SIZE*sizeof(int));
    memset(T->left, 0, MAX_DATA_SIZE*sizeof(int));
    T->right = (int*)malloc(MAX_DATA_SIZE*sizeof(int));
    memset(T->right, 0, MAX_DATA_SIZE*sizeof(int));
    T->parent = (int*)malloc(MAX_DATA_SIZE*sizeof(int));
    memset(T->parent, 0, MAX_DATA_SIZE*sizeof(int));
    T->color = (enum color_set*)malloc(MAX_DATA_SIZE*sizeof(enum color_set));
    memset(T->color, black, MAX_DATA_SIZE*sizeof(enum color_set));
    T->root = INIT_ROOT;
    T->length = T->root; //<array>[0] is nil
}

int TreeInsert(RBTreeList T, int Lin, int Rin) {
    //T is a RBTree, Lin is the left of the interval and Rin is the right side
    int length = T->length;
    int root = T->root;
    int* a = T->a;
    int* b = T->b;
    int* left = T->left;
    int* right = T->right;
    int* parent = T->parent;
    enum color_set* color = T->color;

    //a[length] = Lin;
    //b[length] = Rin;
    int y = NIL;
    int x = root;

    if (length == INIT_ROOT) { //this id is the root of the tree
        //it's safe to insert an interval
        parent[length] = NIL;
        //parent[NIL] = root;
        left[NIL] = INIT_ROOT;
        right[NIL] = INIT_ROOT;
    }
    else {
        while (x != NIL) {
            y = x;
            if (Rin < a[x])
                x = left[x];
            else if (Lin > b[x])
                x = right[x];
            else //overlap interval !
                return -1;
        }
        parent[length] = y;
        if (Rin < a[y])
            left[y] = length;
        else if (Lin > b[y])
            right[y] = length;
        else
            return -1;
    }
    a[length] = Lin;
    b[length] = Rin;
    left[length] = NIL;
    right[length] = NIL;
    return 0;
}

void leftRotate(RBTreeList T, int x) {

    int* left = T->left;
    int* right = T->right;
    int* parent = T->parent;
    enum color_set* color = T->color;
    int root = T->root;

    int y = right[x];
    if (y == NIL) {
        printf("leftRotate wrong! x's right is nil.\n");
        exit(-1);
    }
    parent[y] = parent[x];
    if (x == left[parent[x]])
        left[parent[x]] = y;
    else
        right[parent[x]] = y;
    parent[x] = y;
    right[x] = left[y];
    parent[left[y]] = x;
    left[y] = x;
    if (x == root) {
        root = y;
        left[NIL] = y;
        right[NIL] = y;
        T->root = root;
    }
}

void rightRotate(RBTreeList T, int y) {

    int* left = T->left;
    int* right = T->right;
    int* parent = T->parent;
    enum color_set* color = T->color;
    int root = T->root;

    int x = left[y];
    if (x == NIL) {
        printf("rightRotate wrong! y's left is nil.\n");
        exit(-1);
    }
    parent[x] = parent[y];
    if (y == left[parent[y]])
        left[parent[y]] = x;
    else
        right[parent[y]] = x;
    parent[y] = x;
    left[y] = right[x];
    parent[right[x]] = y;
    right[x] = y;
    if (y == root) {
        root = x;
        left[NIL] = x;
        right[NIL] = x;
        T->root = root;
    }
}

int RBInsert(RBTreeList T, int Lin, int Rin) {

    if (TreeInsert(T, Lin, Rin) != 0)
        return -1;

    int x = T->length; //the current inserted node
    int root = T->root;
    int* left = T->left;
    int* right = T->right;
    int* parent = T->parent;
    enum color_set* color = T->color;

    color[x] = red;
    while (x != root && color[parent[x]] == red) { //collision
        if (parent[x] == left[parent[parent[x]]]) {
            int uncle = right[parent[parent[x]]]; //that is y
            if (color[uncle] == red) { //case 1
                color[parent[x]] = black;
                color[uncle] = black;
                color[parent[parent[x]]] = red;
                x = parent[parent[x]];
            }
            else { //uncle is black
                if (x == right[parent[x]]) { //case 2
                    //x is a right child
                    x = parent[x];
                    leftRotate(T, x);
                }
                //case 3: x is a left child
                color[parent[x]] = black;
                color[parent[parent[x]]] = red;
                rightRotate(T, parent[parent[x]]);
            }
        }
        else { //x->parent == x->parent->parent->right
            int uncle = left[parent[parent[x]]]; //that is y
            if (color[uncle] == red) { //case 1
                color[parent[x]] = black;
                color[uncle] = black;
                color[parent[parent[x]]] = red;
                x = parent[parent[x]];
            }
            else { //uncle is black
                if (x == left[parent[x]]) { //case 2
                    //x is a left child
                    x = parent[x];
                    rightRotate(T, x);
                }
                //case 3: x is a right child
                color[parent[x]] = black;
                color[parent[parent[x]]] = red;
                leftRotate(T, parent[parent[x]]);
            }
        }
    }
    
    color[root] = black;
    T->length++;
    return 0;
}

int main(void) {

    RBTreeList T = (RBTreeList)malloc(sizeof(RBTreeNode));
    RBTreeInit(T);
    int n = 0;
    scanf("%d", &n);
    int i; //loop variant
    for (i = 0; i < n; i++) {
        int a = 0, b = 0;
        scanf("%d%d", &a, &b);
        if (RBInsert(T, a, b) != 0)
            printf("-1\n");
        else
            printf("0\n");
    }
    return 0;
}