/*Discription:
 *2 lines' input:
 *N T(5<=N<=1E+6, 0<=T<=9)
 *<a numerical string>(after division, 0<=A[i]<=1E+8)
 *N is the length of the string, T is the division character
 *you should divide the string by T and sort it, and output the result.
*/
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int Partition(int* A, int p, int r) {
/*partition the array A be a pivot element
 *numbers smaller than the pivot will be placed before it
 *bigger than it will be placed after it
 *A: the array to be partitioned
 *p: start of the array
 *r: end of the array
 */
    int pivot = A[(p+r)/2];
    A[(p+r)/2] = A[r];
    A[r] = pivot;
    int i = p - 1;//pointer: smaller than the pivot
    int j;    //loop variant
    int exc;  //to store the exchanged value
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

void QuickSort(int* A, int p, int r) {
/*sort the array A using quick sort algorithm
 *A: the array to be sorted
 *p: start of the array
 *r: end of the array
 */
    //initial call: QuickSort(A, 0, n-1);
    if (p < r) {
        int q = Partition(A, p, r);
        QuickSort(A, p, q-1);
        QuickSort(A, q+1, r);
    }
}

int main(void) {
    int N, T;
    scanf("%d%d", &N, &T);
    char* string = (char*)malloc((N+10)*sizeof(char));
    memset(string, 0, (N+10)*sizeof(char));
    scanf("%s", string);
    int* A = (int*)malloc(N*sizeof(int));
    memset(A, -1, N*sizeof(int));
    //initialization and input

    int arrayValue;
    int i, Ai; //loop variant
    for (i = 0, Ai = 0; i < N; i++) {
        arrayValue = 0;
        if (string[i] == T+48) continue;
        while (string[i]!=T+48 && string[i]!='\0')
        {
            //printf("string[%d]%c\n", i, string[i]);
            arrayValue = arrayValue*10 + string[i] - 48;
            i++;
        } 
        A[Ai++] = arrayValue;
    }
    //tranverse string to int array
    int arrayL = Ai; //the amount of A[]'s elements
    QuickSort(A, 0, arrayL-1);
    for (Ai = 0; A[Ai] != -1; Ai++)
        printf("%d ",A[Ai]);

    free(string);
    free(A);
    return 0;
}