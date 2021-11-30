/*Discription:
 *input:
 *n
 *<n numbers>
 *n is the length of the array
 *sort the array and output the result
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef long T;

T Partition(T* A, T p, T r) {
/*partition the array A be a pivot element
 *numbers smaller than the pivot will be placed before it
 *bigger than it will be placed after it
 *A: the array to be partitioned
 *p: start of the array
 *r: end of the array
 */
    T pivot = A[(p+r)/2];
    A[(p+r)/2] = A[r];
    A[r] = pivot;
    //to find a reasonable pivot!
    T i = p - 1;//pointer: smaller than the pivot
    T j;    //loop variant
    T exc;  //to store the exchanged value
    for (j = p; j <= r-1; j++) {
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

void QuickSort(T* A, T p, T r) {
/*sort the array A using quick sort algorithm
 *A: the array to be sorted
 *p: start of the array
 *r: end of the array
 */
    if (p < r) {
        T q = Partition(A, p, r);
        QuickSort(A, p, q-1);
        QuickSort(A, q+1, r);
    }
}

int main(void) {
    T n;
    scanf("%ld", &n);
    T* A = (T*)malloc(n*sizeof(T));
    memset(A, 0, n*sizeof(T));
    //initialization

    T i;  //loop variant
    for (i = 0; i < n; i++)
        scanf("%ld", &A[i]);
    //input array A
    
    QuickSort(A, 0, n-1);
    for (i = 0; i < n; i++)
        printf("%ld ", A[i]);
    printf("\n");
    //sort and print
    return 0;

}