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

    int length = T->length;
    int root = T->root;
    int* key = T->key;
    int* left = T->left;
    int* right = T->right;
    int* parent = T->parent;
    enum color_set* color = T->color;

    key[length] = id;
    int y = NIL;
    int x = root;

    if (length == INIT_ROOT) { //this id is the root of the tree
        parent[length] = NIL;
        //parent[NIL] = root;
        left[NIL] = INIT_ROOT;
        right[NIL] = INIT_ROOT;
    }
    else {
        while (x != NIL) {
            y = x;
            if (id < key[x])
                x = left[x];
            else
                x = right[x];
        }
        parent[length] = y;
        if (id < key[y])
            left[y] = length;
        else
            right[y] = length;
    }

    left[length] = NIL;
    right[length] = NIL;
}

void leftRotate(RBTreeList T, int x) {

    int* key = T->key;
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

    int* key = T->key;
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

void RBInsert(RBTreeList T, int id) {

    TreeInsert(T, id);

    int x = T->length; //the current inserted node
    int root = T->root;
    int* key = T->key;
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
}

int findRBTree(RBTreeList T, int id) {
    //if id is in the tree, return id; else return the just-smaller-than-id's key
    int root = T->root;
    int* key = T->key;
    int* left = T->left;
    int* right = T->right;
    int* parent = T->parent;
    int length = T->length;

    int x;
    int ret = -1; //the biggest element which is smaller than id
    for (x = root; x < length; ) {
        if (key[x] == id)
            return id;
        else if (key[x] > id) {
            if (left[x] != NIL)
                x = left[x];
            else {
                return ret;
            }
        }
        else { //key[x] < id
            //if (key[x] > ret)
                ret = key[x];
            if (right[x] != NIL)
                x = right[x];
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