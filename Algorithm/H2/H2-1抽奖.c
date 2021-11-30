/*Discription:
 *input:
 *n k
 *<n numbers>
 *n is for the length of the array
 *you should find the kth member in the array
*/
#include<stdio.h>
#include<stdlib.h>

int RandomizedPartition(int* A, int p, int r) {
/*partition the array A and return the pivot
 *numbers smaller than the pivot will be placed before it
 *bigger than the pivot will be placed after it
 *A: the object array
 *p: start of the subarray
 *r: end of the subarray
 */
    int pivot = A[r];
    int i = p - 1;//pointer: less than the pivot
    int j = 0;  //loop variant
    int exc;    //exchange value
    for (j = p; j < r; j++) {
        if (A[j] <= pivot) {
            i++;
            exc = A[i];
            A[i] = A[j];
            A[j] = exc;
        }
    }
    A[r] = A[i+1];
    A[i+1] = pivot;
    return i+1;
}

int RandomizedSelect(int* A, int p, int r, int i) {
/*select and return the ith smallest number in the array A
 *A: the object array
 *p: start of the subarray
 *r: end of the subarray
 *i: to find the ith smallest number in the subarray A[p-r]
 */
    if (p == r) return A[p];
    int q = RandomizedPartition(A, p, r);               //iteration
    int k = q - p + 1;
    if (i == k) return A[q];
    if (i < k) return RandomizedSelect(A, p, q-1, i);   //recursion
    else return RandomizedSelect(A, q+1, r, i-k);       //recursion
}

int main(void) {
    int n = 0, k = 0;
    scanf("%d%d", &n, &k);
    int* A = (int*)malloc(n * sizeof(int));
    memset(A, 0, n*sizeof(int));
    //initialization

    int i = 0;  //loop variant
    for (i = 0; i < n; i++)
        scanf("%d", &A[i]);
    //input array A

    printf("%d\n", RandomizedSelect(A, 0, n-1, k));
    //solve and print
}