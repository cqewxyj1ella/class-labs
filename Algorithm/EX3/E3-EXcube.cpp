/* Dynamic Programming
 * ACed
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
using namespace std;
#define N 2000

typedef struct {
    /* struct for dynamic programing
     * continuous: from [0][0] to [i][j], how many cubes(just on the line)
     * cotiColumn: continuous column cubes
     * contiRow: continuous row cubes
     * total: total cubes in the [i][j]
    */
    int continuous;
    int contiColumn;
    int contiRow;
    int total;
} dpNode;
// initialized to be zero

int minOf3(const int a, const int b, const int c) {
    // return the minimum value in a, b and c
    if (b < c)
        return (a < b) ? a : b;
    else
        return (a < c) ? a : c;
    //TODO: if (a<b && a<c) return a;
    
}


int main(void) {

    // initiation and input
    int n = 0, m = 0;
    scanf("%d%d", &n, &m);
    //printf("n: %d, m: %d\n", n, m);
    // create dp matrix
    dpNode** Find = (dpNode**)malloc((N+1)*sizeof(dpNode*));
    for (int i = 0; i <= N+1; i++) {
        Find[i] = (dpNode*)malloc((N+1)*sizeof(dpNode));
        memset(Find[i], 0, (N+1)*sizeof(dpNode));
    }
    // create input matrix
    int** floor = (int**)malloc((N+1)*sizeof(int*));
    for (int i = 0; i <= N+1; i++) {
        floor[i] = (int*)malloc((N+1)*sizeof(int));
        memset(floor[i], 0, (N+1)*sizeof(int));
    }
    // handle input
    for(int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%d", &floor[i][j]);
            // cout << floor[i][j] << endl;
        }
    }
    //cout << "input success" << endl;
    // dynamic programing
    // if loop index begins at 0, then i-1 and j-1 should be wrong(== -1)
    for (int j = 1; j <= m; j++) {
        //cout << 1 << " " << j << endl;
        Find[1][j].total = Find[1][j-1].total + (1-floor[1][j]);
        Find[1][j].continuous = 1 - floor[1][j];
        Find[1][j].contiColumn = 1 - floor[1][j];
        Find[1][j].contiRow = (floor[1][j] == 1) ? 0 : Find[1][j-1].contiRow + 1;
        /*
        cout << Find[1][j].continuous << " " << 
                Find[1][j].contiColumn << " " <<
                Find[1][j].contiRow << " " <<
                Find[1][j].total << endl;
        */
    }
    
    for (int i = 2; i <= n; i++) {
        //cout << i << " " << 1 << endl;
        Find[i][1].total = Find[i-1][1].total + (1-floor[i][1]);
        Find[i][1].continuous = 1 - floor[i][1];
        Find[i][1].contiColumn = (floor[i][1] == 1) ? 0 : Find[i-1][1].contiColumn + 1;
        Find[i][1].contiRow = 1 - floor[i][1];

        /*
        cout << Find[1][j].continuous << " " << 
                Find[1][j].contiColumn << " " <<
                Find[1][j].contiRow << " " <<
                Find[1][j].total << endl;
        */

        for (int j = 2; j <= m; j++) {
            //cout << i << " " << j << endl;
            if (floor[i][j] == 0) {
                Find[i][j].continuous = minOf3(Find[i-1][j-1].continuous, 
                    Find[i-1][j].contiColumn, Find[i][j-1].contiRow) + 1;
                Find[i][j].contiColumn = Find[i-1][j].contiColumn + 1;
                Find[i][j].contiRow = Find[i][j-1].contiRow + 1;
            }
            else { // floor_i_j = 1
                Find[i][j].continuous = 0;
                Find[i][j].contiColumn = 0;
                Find[i][j].contiRow = 0;
            }
            Find[i][j].total = Find[i][j-1].total + Find[i-1][j].total
                - Find[i-1][j-1].total + Find[i][j].continuous;
            /*
            cout << Find[1][j].continuous << " " << 
                Find[1][j].contiColumn << " " <<
                Find[1][j].contiRow << " " <<
                Find[1][j].total << endl;
            */
        }
    }
    printf("%d\n", Find[n][m].total);
    //system("pause");
    
    return 0;
}