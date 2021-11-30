//AC

/* Description:
 * for A(x1, y1) and B(x2, y2), if x1 <= x2 && y1 <= y2, then A <= B
 * given a set S, find the max subset of S so that all the elements
 * in it have partial order relations
 * use LIS algorithm
 * dynamic programing
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int N = 10010;
typedef struct {
    int x;
    int y;
} coordinate, *coordinates;

bool partialLE(const coordinates a, const coordinates b) {
    if (a->x <= b->x && a->y <= b->y)
        return true;
    else return false;
}

int main(void) {
    int n;
    scanf("%d", &n);
    coordinates Co = (coordinates)malloc(n*sizeof(coordinate));
    memset(Co, 0, n*sizeof(coordinate));
    //to store the coordinates from input
    int* length = (int*)malloc(n*sizeof(int));
    memset(length, 0, n*sizeof(int));
    //length[i] stores the max length of the string ended with i
    int max_size = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &Co[i].x, &Co[i].y);
        //store the coordinate
        length[i] = 1;
        for (int j = 0; j < i; j ++) {
            if (partialLE(Co+j, Co+i) && length[j]+1 > length[i])
                length[i] = length[j] + 1;
        }
        if (length[i] > max_size)
            max_size = length[i];
    }
    printf("%d\n", max_size);
    return 0;
}