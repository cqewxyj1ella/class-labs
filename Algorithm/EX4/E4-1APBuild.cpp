/* Description:
 * build APs among several viliges(on a line)
 * given the amount of viliges and the radios of AP
 * and the coordinates of viliges
 * choose least amount of APs to cover all the viliges
 * solution:
 * greedy choice
*/
// ACed
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// compare function for qsort()
int myCmp(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

int main(void) {
    // input and initialization
    int N = 0, R = 0;
    scanf("%d%d", &N, &R);
    int* coordinates = (int*)malloc(N*sizeof(int));
    memset(coordinates, 0, N*sizeof(int));
    bool* built = (bool*)malloc(N*sizeof(bool));
    memset(built, false, N*sizeof(bool));
    for (int i = 0; i < N; i++)
        scanf("%d", &coordinates[i]);
    qsort(coordinates, N, sizeof(coordinates[0]), myCmp);

    int APs = 0;
    for (int i = 0; i < N;) {
        int selected = i;
        while(coordinates[selected] <= coordinates[i] + R)
            selected++;
        // now X[selected] > X[i] + R

        // built an AP here
        selected--;
        built[selected] = true;
        APs++;
        
        // look forward and skip viliges whithin the new-built AP's range
        i = selected;
        while(coordinates[i] <= coordinates[selected] + R)
            i++;
        // now i is no more in the new-built AP's range, and should consider a newer AP
    }
    printf("%d\n", APs);
    return 0;
}