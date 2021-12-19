/* Description:
 * given N clothes, you should remove M from tehm
 * so that the clothes will be dried faster
 * input:
 * N M
 * M < N <= 200,000
 * then for each line gives the coordinate Xi of one cloth
 * Xi <= 20,000,000
 * solution:
 * greedy algorithm:
 * record the information of the intervals, and their left and right intervals
 * always select the closest clothes(smallest intervals) to remove
 */

#include <iostream>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
#define INFINITE 0x7fffffff

typedef struct Interval {
    /* value: the value of this interval
     * left_value: the left interval's value
     * right_value: the right interval's value
     * left_ptr: pointer of the left interval
     * right_ptr: pointer of the right interval
    */
    int value;
    int left_value;
    int right_value;
    Interval* left_ptr;
    Interval* right_ptr;
    bool operator<(const Interval& rhs) const {
        return value > rhs.value;
    }
}Interval, *IntervalPtr;

int main(void) {
    // input and initialization
    int N = 0, M = 0;
    cin >> N >> M;
    //cout << N << " " << M << endl;
    int* coordinates = new int[N];
    for (int i = 0; i < N; i++)
        cin >> coordinates[i];
    priority_queue<Interval> intervals;
    IntervalPtr in1 = new Interval;
    IntervalPtr in2 = new Interval;
    IntervalPtr in3 = new Interval;
    in1->value = 1; in2->value = 0; in3->value = 2;

    in1->left_value = INFINITE;
    in1->right_value = 0;
    in1->left_ptr = NULL;
    in1->right_ptr = in2;

    in2->left_value = 1;
    in2->right_value = 2;
    in2->left_ptr = in1;
    in2->right_ptr = in3;

    in3->left_value = 0;
    in3->right_value = INFINITE;
    in3->left_ptr = in2;
    in3->right_ptr = NULL;

    cout << "in1's value: " << in1->value << endl;
    cout << "in2's value: " << in2->value << endl;
    cout << "in3's value: " << in3->value << endl;

    intervals.push(*in1);
    intervals.push(*in2);
    intervals.push(*in3);

    auto top = intervals.top();
    cout << "priority_queue's top's value: " << top.value << endl;
    intervals.pop();
    auto l_in = top.left_ptr;
    auto r_in = top.right_ptr;
    l_in->value = 5;
    r_in->value = 10;
    while (!intervals.empty()) {
        top = intervals.top();
        intervals.pop();
        cout << "priority_queue's top's value: " << top.value << endl;
    }
    

}