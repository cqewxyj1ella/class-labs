/*Discription:
 *for every testcase in T cases: 
 *find whether there's a number occurs more than (N+1)/2 times
 *input:
 *first input T, shows there'll be T testcases(1<=T<=20)
 *then input 2*T lines, N and N numbers(1<=N<=1E+6, every array member: 1<=array[i]<=1E+8)
 *if there's a number satisfied the condition, output the number
 *else, output -1
*/
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

//use hash table algorithm: molde to create hash table; linklist to solve collision
#define hashM 29    //hashAddress = hash[key] mod hashM

typedef struct hashNode{
    int key;
    struct hashNode* next;
}hashNode, *hashList;


int main(void) {
    int T, N;

    scanf("%d", &T);
    int i, j;//loop variant

    for (i = 0; i < T; i++) {
        scanf("%d", &N);
        int* array = (int*)malloc(N*sizeof(int));
        memset(array, 0, N*sizeof(int));
        for (j = 0; j < N; j++)
            scanf("%d", &array[j]);
        //initiate and store the array

        hashList hashL = (hashList)malloc(hashM*sizeof(hashNode));
        memset(hashL, 0, hashM*sizeof(hashNode));
        //initiate the hash table

        for (j = 0; j < N; j++){
            hashList hashInsert = (hashList)malloc(sizeof(hashNode));
            int hashAddr = array[j] % hashM; //address of array[j]
            hashInsert->key = array[j];
            hashInsert->next = hashL[hashAddr].next;
            hashL[hashAddr].next = hashInsert;
            hashL[hashAddr].key++;
        }
        //insert the array numbers into the hash table

        for (j = 0; j < hashM; j++)
            if (hashL[j].key >= (N+1)/2)
                break;
        if (j >= hashM) {
            printf("-1\n");
            free(array);
            continue;
        }
        hashList p, q;
        int found; //flag for finding the value
        int cnt, value; //count for the same value
        while (hashL[j].key >= (N+1)/2)
        {
            for (p = &hashL[j], cnt = 0, value = hashL[j].next->key, found = 0; p != NULL; p = p->next)
                if (p->key == value)
                    cnt++;
            if (cnt < (N+1)/2) {//delete the first node in hashL[j] linklist
                q = hashL[j].next;
                hashL[j].next = q->next;
                free(q);
                hashL[j].key--;
            }
            else {
                found = 1;
                break;
            }
        }
        if (found)
            printf("%d\n", value);
        else printf("-1\n");
        free(array);
    }
    return 0;
}