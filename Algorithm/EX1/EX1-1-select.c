#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int RandomizedPartition(int* A, int p, int r) {
/*partition the array A and return the pivot
 *numbers smaller than the pivot will be placed before it
 *bigger than the pivot will be placed after it
 *A: the object array
 *p: start of the subarray
 *r: end of the subarray
 */
    int i, j;
    int pivot = A[p], pivoti = p;;
    if (A[(p+r)/2]>=A[p] && A[(p+r)]<=A[r]) {
        pivot = A[(p+r)/2];
        pivoti = (p+r)/2;
    }
    else if (A[r]>=A[p] && A[r]<=A[(p+r)]) {
        pivot = A[r];
        pivoti = r;
    }
    //find the pivot

    //int pivot = A[(p+r)/2];
    A[pivoti] = A[r];
    A[r] = pivot;
    
    int exc;    //exchange value
    for (i = p - 1, j = p; j < r; j++) {
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
    int T, N;

    scanf("%d", &T);
    int i, j;//loopvariant

    for (i = 0; i < T; i++) {
        scanf("%d", &N);
        int* array = (int*)malloc(N*sizeof(int));
        memset(array, 0, N*sizeof(int));
        for (j = 0; j < N; j++)
            scanf("%d", &array[j]);
        //initiate and store the array

       int medium = RandomizedSelect(array, 0, N-1, (N+1)/2); //find the medium
       int cnt = 0; //cnt for medium occur times
       int is_succed = 0; //flag to tell whether medium occurs more than (N+1)/2 times
       for (j = 0; j < N; j++){
           if (array[j] == medium) cnt++;
           if (cnt >= (N+1)/2){
               is_succed = 1;
               break;
           }
       }
       if (is_succed) printf("%d\n", medium);
       else printf("-1\n");
       //check whther the condition meets and output result
       free(array);
    }
    return 0;
}