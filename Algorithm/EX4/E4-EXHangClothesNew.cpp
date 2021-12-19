/* Description:
 * given N clothes, you should remove M from tehm
 * so that the clothes will be dried faster
 * input:
 * N M
 * M < N <= 200,000
 * then for each line gives the coordinate Xi of one cloth
 * Xi <= 20,000,000
 * solution:
 * greedy algorithm but another method:
 * don't remove clothes from the string, but hang them on
 * loop for the min interval d: 0 <= d <= x[max]-x[min]
 * (binary research for d's loop)
 * if after loop, hanged clothes are more than N-M
 * then d can be smaller;
 * else d must be bigger
 */
// ACed
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main(void) {
    // input and initialization
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N = 0, M = 0;
    cin >> N >> M;
    int* coordinates = (int*)malloc(N*sizeof(int));
    memset(coordinates, 0, N*sizeof(coordinates[0]));
    for (int i = 0; i < N; i++)
        cin >> coordinates[i];
    sort(coordinates, coordinates+N);

    int low = 0;
    int high = coordinates[N-1] - coordinates[0];
    // these two are the binary search pointers
    // 0 <= d <= x[max] - x[min]
    while (low <= high) {
        int d = (low + high) / 2; // binary search pointer: min distance of two clothes
        int leftCloth = coordinates[0]; // the coordinate of the left cloth of coor[i]
        int hangedClothes = 1; // hanged the leftmost cloth

        for (int i = 1; i < N; i++) {
            if (coordinates[i] - leftCloth >= d) {
                // it's safe to hange on the ith cloth
                hangedClothes++;
                leftCloth = coordinates[i];
            }
            // else: the interval
        }

        // renew the binary search pointers
        if (hangedClothes >= N-M) // hanged more than command, d can be smaller
            low = d + 1;
        else // hanged less than command, d should be bigger
            high = d - 1;
    }
    cout << high << endl; // that is also d, because d = (low+high)/2 and low == high
}
