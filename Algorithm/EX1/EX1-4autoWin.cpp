//partial ac
//auto window
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_NAME_LEN 5
#define HASH_PRIME 99981389
//26 + 26*256 + 26*(256^2) + 26*(256^3) + 26*(256^4)=806
typedef struct hashTable {
    char name[MAX_NAME_LEN+1];
    int nums;
} hashTable, *hashList;

int hashAlgorithm(char* string);    //match string to an int
bool hashFind(char* key, hashList table); //find key in the hash table
void hashAdd(char* key, hashList table);  //add key in the hash table
void hashDel(char* key, hashList table);  //delete key in the hash table
int moveWin(int L, int R, char** str, hashList table);//calculate how to move the window
void solve(void);                   //solve the problem
int subTroops;                      //global variable: answer
int n, k;                           //as problem describes
int winNames;                       //how many names within the window

int main(void) {
    /*
    const int DEBUGGING = 1; //get data from the standard input if DEBUGGING is 0; and from files if DEBUGGING is 1.
	if (!DEBUGGING) 
	{
		solve();
		return 0;
	}
	const int NUMBER_OF_TESTCASES = 4;
	for (int _index = 1; _index <= NUMBER_OF_TESTCASES; ++_index) 
	{
		char file_name[20];
		sprintf(file_name, "%d.in", _index);
		freopen(file_name, "r", stdin);
		sprintf(file_name, "%d.ans", _index);
		freopen(file_name, "w", stdout);
		int t1 = clock(); // #include <time.h>
		solve();
		t1 = clock() - t1;
		fprintf(stderr, "Testcase %2d: time consuming %8lf seconds\n", _index, (double)t1 / CLOCKS_PER_SEC);
		fclose(stdin);
		fclose(stdout);
	}
    return 0;
    */
   //freopen("1.in", "r", stdin);
   //freopen("1.ans", "w", stdout);
   solve();
   //fclose(stdin);
   //fclose(stdout);
   return 0;
}

int hashAlgorithm(char* string) {
    /*match string to an int
     *input is a string
     *return an int which the string match to
    */
    int ret = 0; //return value
    int len = strlen(string);
    int factor = 1;
    for (int i = 0; i < len; i++) {
        ret += factor * (string[i]- 'a' + 1);
        ret %= HASH_PRIME;
        factor *= 256;
    }
    return ret % HASH_PRIME;
}

bool hashFind(char* key, hashList table) {
    /*find key in the hash table
     *if has found, return 1;
     *else, return 0
    */
    int hashAddr = hashAlgorithm(key);
    int offset = 1; //linear offset address for hash collision
    while (true) {
        if (table[hashAddr].nums != 0) { //already has value
            if (strcmp(key, table[hashAddr].name) == 0)
                return true; //has found
            else {
                hashAddr = (hashAddr + offset) % HASH_PRIME;
                offset++;
            }
        }
        else { //an empty node
            return false;
        }
    }
    return false;
}

void hashAdd(char* key, hashList table) {
    /*add key in the hash table
     *if the key is already in the table, simply add times(table[key]++)
     *else, table[key]++ and table[0]++
     *table[0] is the total name types within the window
    */
    int hashAddr = hashAlgorithm(key);
    int offset = 1; //linear offset address for hash collision
    while (true) {
        if (table[hashAddr].nums != 0) { //already has value
            if (strcmp(key, table[hashAddr].name) == 0)
                break; //no nead to fill in a new node
            else {
                hashAddr = (hashAddr + offset) % HASH_PRIME;
                offset++;
            }
        }
        else { //an empty node
            memcpy(table[hashAddr].name, key, (strlen(key)+1)*sizeof(char));
            winNames++;
            break;
        }
    }
    table[hashAddr].nums++;
}

void hashDel(char* key, hashList table) {
    /*delete key in the hash table
     *if key appears many times in the window, simply delete it;
     *else(key appears only once), total name types--
    */
    int hashAddr = hashAlgorithm(key);
    int offset = 1; //linear offset address for hash collision
    while (true) {
        if (table[hashAddr].nums != 0) { //already has value
            if (strcmp(key, table[hashAddr].name) == 0) {
                table[hashAddr].nums--;
                if (!table[hashAddr].nums) {
                    memset(table[hashAddr].name, 0, MAX_NAME_LEN+1);
                    winNames--;
                }
                return;
            } //this is the node to delete
            else {
                hashAddr = (hashAddr + offset) % HASH_PRIME;
                offset++;
            }
        }
        else { //no node to delete
            return;
        }
    }
    return;
}

int moveWin(int L, int R, char** str, hashList table) {
    //calculate how to move the window
    if (L > R) return 0;
    if (R >= n) return 0;
    if (winNames == k)
        subTroops++;
    if (R+1 == n) {
        hashDel(str[L], table);
        while (L <= R && hashFind(str[L], table)) {
            subTroops++;
            L++;
            hashDel(str[L], table);
        }
        return 0;
    }
    //else: R+1!=n
    if (hashFind(str[R+1], table)) {
        hashDel(str[L], table); 
        if (hashFind(str[L], table)) {
            int la = R + 1; //look ahead
            while (la < n && hashFind(str[la], table)) {
                subTroops++;
                la++;
            }
            return 2;
        }//1,1
        else {
            hashAdd(str[L], table);
            hashAdd(str[R+1], table);
            return 1;
        }//0,1
    }//?,1
    else {
        hashDel(str[L], table);
        if (hashFind(str[L], table)) {
            return 2;
        }//1,0
        else {
            hashAdd(str[R+1], table);
            return 3;
        }//0,0
    }//?,0
    return 0;
}

void solve(void) {

    //int n, k;
    scanf("%d%d", &n, &k);
    //printf("%d %d\n", n, k);
    /*
    int* hashStr = (int*)malloc(n*sizeof(int));
    memset(hashStr, 0, n*sizeof(int));
    char* inputStr = (char*)malloc(10*sizeof(char));    
    for (int i = 0; i < n; i++) {
        memset(inputStr, 0, 10*sizeof(char));
        scanf("%s", inputStr);
        hashStr[i] = hashAlgorithm(inputStr);
    }
    */
    char** names = (char**)malloc((n+1)*sizeof(char*));
    for (int i = 0; i < n+1; i++) {
        names[i] = (char*)malloc((MAX_NAME_LEN+1)*sizeof(char));
        memset(names[i], 0, (MAX_NAME_LEN+1)*sizeof(char));
    }
    //initiation

    for (int i = 0; i < n; i++) {
        scanf("%s", names[i]);
    }
    //input the string and hash them to int array hashStr[]
    //for (int i = 0; i < n; i++)
    //    printf("str[%d]: %d\n", i, hashStr[i]);

    //int* hashTable_Times = (int*)malloc(HASH_PRIME*sizeof(int));
    //memset(hashTable_Times, 0, HASH_PRIME*sizeof(int));
    hashList hashTable_Times = (hashList)malloc(HASH_PRIME*sizeof(hashTable));
    memset(hashTable_Times, 0, HASH_PRIME*sizeof(hashTable));
    //hashTable[i]: hashStr[i] occurs how many times in the current window
    subTroops = 0;
    winNames = 0;
    int winL = 0, winR = k-1; //window left-most and right-most
    for (int i = winL; i <= winR; i++)
        hashAdd(names[i], hashTable_Times);
    while (winNames < k) {
        winR++;
        hashAdd(names[winR], hashTable_Times);
    }
    //find the least window(initiate window)

    //printf("find the initiate window.\n");    
    while (winR < n) {
        //printf("winL: %d; winR: %d\n", winL, winR);
        int moveType;
        moveType = moveWin(winL, winR, names, hashTable_Times);
        if (moveType == 0)
            break;
        switch (moveType) {
        case 1:
            winR++;
            break;
        case 2:
            winL++;
            break;
        case 3:
            winL++;
            winR++;
            break;
        default:
            break;
        }
        //printf("moveType: %d\n", moveType);
        //printf("subTroops: %d\n", subTroops);
    }
    printf("%d\n", subTroops);
    
    
}