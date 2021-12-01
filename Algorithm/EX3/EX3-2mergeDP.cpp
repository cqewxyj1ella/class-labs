/* Description:
 * given n arrays' lengths
 * you have only a merge function that can sort the neighboring arrays
 * like matrix multiplying, calculate the minimum steps used in merging progress
 * sulution: dynamic programing
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INFINITE 2147483647

int main(void) {
    
    int n;
    scanf("%d", &n); //problem size
    int* array_size = (int*)malloc(n*sizeof(int));
    memset(array_size, 0, n*sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &array_size[i]);
    } //input all

    int** dp_matrix = (int**)malloc(n*sizeof(int*));
    for (int i = 0; i < n; i++) {
        dp_matrix[i] = (int*)malloc(n*sizeof(int));
        memset(dp_matrix[i], 0, n*sizeof(int));
    }
    //initialize an n*n matrix for dynamic programing

    for (int l = 2; l <= n; l++) {
        for (int i = 0; i < n-l+1; i++) {
            int j;
            j = i+l-1; dp_matrix[i][j] = INFINITE;
            int array_sum = 0;
            int m_i_j = dp_matrix[i][j];
            for (int k = i; k <= j; k++) {
                array_sum += array_size[k];
                int q = dp_matrix[i][k] + dp_matrix[k+1][j];
                if (q < m_i_j)
                    m_i_j = q;
            }
            dp_matrix[i][j] = m_i_j + array_sum;
        }
    }
    printf("%d\n", dp_matrix[0][n-1]);
   /*
    int a = 0x7fffffff;
    printf("%d\n", a);
    //test for inifite int number
    */
    system("pause");
    return 0;
}