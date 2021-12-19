/* Description:
 * given N tasks, and their reach time ri, processing time pi
 * when a task finishes procedure, that its finish time fi
 * fi-ri is its reply time
 * the purpose is to minimizing the total reply time
 * constraints:
 * preemptive task procedure
 * time slides are integers
 * solution:
 * greedy choice: always choose the task which has lest left time
*/
// ACed
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <iostream>
using namespace std;

typedef struct task{
    /* r is for the reach time of the task
     * p is for the processing time of the task
     * p can also denote remain time of the task
     * operator< re-write the '<' operator so that priority queue puts the smallest p on the top
    */
    int r;
    int p;
    bool operator<(const task& rhs) const {
        return p > rhs.p;
    }
}task, *taskList;

int myCmp_r(const void* a, const void* b) {
    return (*(task*)a).r - (*(task*)b).r;
}

int main(void) {
    // input and initialization
    int N = 0;
    scanf("%d", &N);
    taskList tasks = (taskList)malloc(N*sizeof(task));
    memset(tasks, 0, N*sizeof(tasks[0]));
    for (int i = 0; i < N; i++)
        scanf("%d%d", &tasks[i].r, &tasks[i].p);

    qsort(tasks, N, sizeof(tasks[0]), myCmp_r);
    priority_queue<task> PQueue;
    PQueue.push(tasks[0]);
    int next_task_indx = 1;
    int curr_time = tasks[0].r;
    int response_time = 0; // total response time

    while(!PQueue.empty()) {
        auto curr_task = PQueue.top(); // current task
        PQueue.pop();
        if (next_task_indx >=N || curr_time + curr_task.p <= tasks[next_task_indx].r) {
            curr_time += curr_task.p; //process current task till end
            response_time += curr_time - curr_task.r;
        }
        else {
            // next task will come during processing current task
            curr_task.p -= tasks[next_task_indx].r - curr_time;
            curr_time = tasks[next_task_indx].r;
            PQueue.push(curr_task);
            PQueue.push(tasks[next_task_indx]);
            next_task_indx++;
        }
        if (PQueue.empty() && next_task_indx < N) {
            PQueue.push(tasks[next_task_indx]);
            curr_time = tasks[next_task_indx].r;
            next_task_indx++;
        }
    }
    cout << response_time << endl;
    return 0;
    //system("pause");
}