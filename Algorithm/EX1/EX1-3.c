/*Discription:
 *find the converse couple in an array
 *input:
 *n(0<=n<=1E+6)
 *<n numbers>(0<=Ai<=1E+8)
 *n is the length of the array
 *if i<j but Ai>Aj, they are a converse couple
 *output the amount of couples
 *the output can be very large, so molde 10,000,007
 *
 *solution:
 *use merge sort to count exchange times
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INFINITE 0x7fffffff
int couple; //global variable to store exchange times

void merge(int* A, int p, int q, int r) {
    int n1 = q - p + 1; //A[p...q] is sorted
    int n2 = r - q;     //A[q+1...r] is sorted
    int* L = (int*)malloc((n1+1)*sizeof(int));  //copy the left subarray
    int* R = (int*)malloc((n2+1)*sizeof(int));  //copy the right subarray
    int i, j, k;  //loop variant
    for (i = 0; i < n1; i++)
        L[i] = A[p + i];
    for (i = 0; i < n2; i++)
        R[i] = A[q + i + 1];
    L[n1] = INFINITE;
    R[n2] = INFINITE;
    for (i = 0, j = 0, k = p; k <= r && i < n1 && j < n2; k++) {
        if (L[i] <= R[j])
            A[k] = L[i++];
        else {
            A[k] = R[j++];
            couple += n1 - i;
            couple %= 10000007;
        }
    }
    while (i < n1) {
        A[k++] = L[i++];
    }
    while (j < n2) {
        A[k++] = R[j++];
    }
}

void mergeSort(int*A, int p, int r) {
    if (p < r) {
        int q = (p+r)/2;
        mergeSort(A, p, q);
        mergeSort(A, q+1, r);
        merge(A, p, q, r);
    }
}

int main(void) {
    int n;
    scanf("%d", &n);
    int* A = (int*)malloc(n*sizeof(int));
    memset(A, -1, n*sizeof(int));
    int i, j; //loop variant
    for (i = 0; i < n; i++)
        scanf("%d", &A[i]);
    //input the array

    couple = 0;
    mergeSort(A, 0, n-1);
    printf("%d\n", couple%10000007);
    //system("pause");
    return 0;
}