/*discription:
 *build a data library, the user can either insert or find data
 *input form:
 *INSERT KEY or
 *FIND KEY or
 *EXIT(abort)
 *if FIND KEY is in the library, return its key;
 *else, return the biggest key which is just smaller than its kay
 *0<=KEY<=1E+9
 *no more than 2 000 000 data
 *first sight: use hash table, and traverse the whole table if not found
 *second sight(classmate's advise): red-black tree
 *third sight: use arrays instead of struct
 *fourth sight: other unecessary assignment will cost time......
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_L 10
#define MAX_DATA_SIZE 2000002
#define NIL 0
#define INIT_ROOT 1

enum color_set{red, black};

typedef struct RBTree{
    /*
     * red-black tree node
     * key is for sorting value
     * left is for the left child
     * right is for the right child
     * parent is the node's parent
     * color is either red or black
     * length is the current size of the RBTree
    */
    int* key;
    int* left;
    int* right;
    int* parent;
    enum color_set* color;
    int length;
    int root;
}RBTreeNode, * RBTreeList;

void RBTreeInit(RBTreeList T) {
    T->key = (int*)malloc(MAX_DATA_SIZE*sizeof(int));
    memset(T->key, 0, MAX_DATA_SIZE*sizeof(int));
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

void TreeInsert(RBTreeList T, int id) {

    T->key[T->length] = id;
    int y = NIL;
    int x = T->root;

    if (T->length == INIT_ROOT) { //this id is the root of the tree
        T->parent[T->length] = NIL;
        //parent[NIL] = root;
        T->left[NIL] = INIT_ROOT;
        T->right[NIL] = INIT_ROOT;
    }
    else {
        while (x != NIL) {
            y = x;
            if (id < T->key[x])
                x = T->left[x];
            else
                x = T->right[x];
        }
        T->parent[T->length] = y;
        if (id < T->key[y])
            T->left[y] = T->length;
        else
            T->right[y] = T->length;
    }

    T->left[T->length] = NIL;
    T->right[T->length] = NIL;
}

void leftRotate(RBTreeList T, int x) {

    int y = T->right[x];
    if (y == NIL) {
        printf("leftRotate wrong! x's right is nil.\n");
        exit(-1);
    }
    T->parent[y] = T->parent[x];
    if (x == T->left[T->parent[x]])
        T->left[T->parent[x]] = y;
    else
        T->right[T->parent[x]] = y;
    T->parent[x] = y;
    T->right[x] = T->left[y];
    T->parent[T->left[y]] = x;
    T->left[y] = x;
    if (x == T->root) {
        T->root = y;
        T->left[NIL] = y;
        T->right[NIL] = y;
    }
}

void rightRotate(RBTreeList T, int y) {

    int x = T->left[y];
    if (x == NIL) {
        printf("rightRotate wrong! y's left is nil.\n");
        exit(-1);
    }
    T->parent[x] = T->parent[y];
    if (y == T->left[T->parent[y]])
        T->left[T->parent[y]] = x;
    else
        T->right[T->parent[y]] = x;
    T->parent[y] = x;
    T->left[y] = T->right[x];
    T->parent[T->right[x]] = y;
    T->right[x] = y;
    if (y == T->root) {
        T->root = x;
        T->left[NIL] = x;
        T->right[NIL] = x;
    }
}

void RBInsert(RBTreeList T, int id) {

    TreeInsert(T, id);

    int x = T->length; //the current inserted node

    T->color[x] = red;
    while (x != T->root && T->color[T->parent[x]] == red) { //collision
        if (T->parent[x] == T->left[T->parent[T->parent[x]]]) {
            int uncle = T->right[T->parent[T->parent[x]]]; //that is y
            if (T->color[uncle] == red) { //case 1
                T->color[T->parent[x]] = black;
                T->color[uncle] = black;
                T->color[T->parent[T->parent[x]]] = red;
                x = T->parent[T->parent[x]];
            }
            else { //uncle is black
                if (x == T->right[T->parent[x]]) { //case 2
                    //x is a right child
                    x = T->parent[x];
                    leftRotate(T, x);
                }
                //case 3: x is a left child
                T->color[T->parent[x]] = black;
                T->color[T->parent[T->parent[x]]] = red;
                rightRotate(T, T->parent[T->parent[x]]);
            }
        }
        else { //x->parent == x->parent->parent->right
            int uncle = T->left[T->parent[T->parent[x]]]; //that is y
            if (T->color[uncle] == red) { //case 1
                T->color[T->parent[x]] = black;
                T->color[uncle] = black;
                T->color[T->parent[T->parent[x]]] = red;
                x = T->parent[T->parent[x]];
            }
            else { //uncle is black
                if (x == T->left[T->parent[x]]) { //case 2
                    //x is a left child
                    x = T->parent[x];
                    rightRotate(T, x);
                }
                //case 3: x is a right child
                T->color[T->parent[x]] = black;
                T->color[T->parent[T->parent[x]]] = red;
                leftRotate(T, T->parent[T->parent[x]]);
            }
        }
    }
    
    T->color[T->root] = black;
    T->length++;
}

int findRBTree(RBTreeList T, int id) {
    //if id is in the tree, return id; else return the just-smaller-than-id's key

    int x;
    int ret = -1; //the biggest element which is smaller than id
    for (x = T->root; x < T->length; ) {
        if (T->key[x] == id)
            return id;
        else if (T->key[x] > id) {
            if (T->left[x] != NIL)
                x = T->left[x];
            else {
                return ret;
            }
        }
        else { //key[x] < id
            //if (key[x] > ret)
                ret = T->key[x];
            if (T->right[x] != NIL)
                x = T->right[x];
            else
                //return key[x];
                return ret;
        }
    }
    
}

int main(void) {

    RBTreeList T = (RBTreeList)malloc(sizeof(RBTreeNode));
    RBTreeInit(T);
    char* string = (char*)malloc(MAX_L*sizeof(char));
    memset(string, 0, MAX_L*sizeof(char));
    int key;

    while (1) {
        scanf("%s", string);
        if (!strcmp(string, "EXIT"))
            break;
        else if (!strcmp(string, "INSERT")) {
            scanf("%d", &key);
            RBInsert(T, key);
        }
        else if (!strcmp(string, "FIND")) {
            scanf("%d", &key);
            printf("%d\n", findRBTree(T, key));
        }
        else {
            printf("wrong input format!\n");
            break;
        }
    }
    return 0;
}