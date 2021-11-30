/*discription:
 *build a data library v2, the user can either insert or find data
 *input form:
 *INSERT ID STR or
 *FIND STR or
 *EXIT(abort)
 *if FIND STR is in the library, return its ID;
 *else, return -1
 *0<=KEY<=1E+9
 *STR is no longer than 9 ch
 *no more than 100 000 data
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_L 10
#define HASH_PRIME 100003

typedef struct hashNode{
    int id;
    char str[MAX_L];
    struct hashNode* next;
}hashNode, *hashList;

int hashAlgorithm(const char* str);    //match string(no longer than 9) to an int
int hashFind(const char* str, hashList table); //find str in the hash table
void hashAdd(const char* str, const int id, hashList table); //add key to the hash table

int hashAlgorithm(const char* str) {
    //given a str, return its address in the hash table
    int ret = 0; //return value
    int len = strlen(str);
    int factor = 1;
    for (int i = 0; i < len; i++) {
        ret += factor * str[i];
        ret %= HASH_PRIME;
        factor *= 256;
    }
    return ret % HASH_PRIME;
}

int hashFind(const char* str, hashList table) {
    /*find str in the hash table
     *if has found, return its id;
     *else, return -1
    */
    int hashAddr = hashAlgorithm(str);
    hashList p = NULL;
    for (p = &table[hashAddr]; p; p = p->next) {
        if (strcmp(p->str, str) == 0)
            return p->id;
    }
    return -1;
}

void hashAdd(const char* str, const int id, hashList table) {
    //add str to the hash table
    int hashAddr = hashAlgorithm(str); //hash address
    hashList p = (hashList)malloc(sizeof(hashNode));
    memset(p, 0, sizeof(hashNode));
    strcpy(p->str, str);
    p->id = id;

    p->next = table[hashAddr].next;
    table[hashAddr].next = p;
}

int main(void) {
    hashList hashL = (hashList)malloc(HASH_PRIME*sizeof(hashNode)); //hash table
    memset(hashL, 0, HASH_PRIME*sizeof(hashNode));
    char* string = (char*)malloc(MAX_L*sizeof(char)); //command string and str_id
    memset(string, 0, MAX_L*sizeof(char));
    int id; //id
    while(1) {
        scanf("%s", string);
        if (!strcmp(string, "EXIT"))
            break;
        else if (!strcmp(string, "INSERT")) {
            scanf("%d%s", &id, string);
            hashAdd(string, id, hashL);
        }
        else if (!strcmp(string, "FIND")) {
            scanf("%s", string);
            int ret = hashFind(string, hashL);
            printf("%d\n", ret);
        }
        else {
            printf("wrong input format!\n");
            break;
        }
    }
    //system("pause");
    return 0;
}