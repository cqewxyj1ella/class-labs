/* Description:
 * given N cities(nodes) and M roads(arcs)
 * every road has weight and no direction
 * choose some roads from M so that every city is accessible
 * but the total weight is minimum
 * solution:
 * minimum spanning tree
 * use c++ priority queue instead of FIB heap(duplicated node will be marked as visted[])
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
#define INFINITE 0x7fffffff
typedef pair<int, int> P_int; // store key(distance) and index of a vex

struct arcNode {
    int v, w;
    arcNode* next;
};

int main(void) {
    // input and initialization
    int N = 0, M = 0;
    cin >> N >> M;
    vector<arcNode*> vexes(N+1);
    // arc list of vexes, index starts from 1
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        // insert to u's arclist
        arcNode* arc_u = new arcNode;
        arc_u->v = v; arc_u->w = w;
        arcNode* head_u = vexes[u];
        arc_u->next = head_u;
        vexes[u] = arc_u;
        // insert to v's arclist
        arcNode* arc_v = new arcNode;
        arc_v->v = u; arc_v->w = w;
        arcNode* head_v = vexes[v];
        arc_v->next = head_v;
        vexes[v] = arc_v;
    }
    vector<int> key(N+1); 
    // stores key value in Prim algorithm, index starts from 1
    fill(key.begin(), key.end(), INFINITE);
    key[1] = 0; // spans the tree start from the 1st vex
    vector<bool> visited(N+1);
    // if true, then vex i belongs to S(added to the tree);
    // index starts from 1
    int added_num = 0; // added how many nodes to the MST
    priority_queue<P_int, vector<P_int>, greater<P_int>> MST_Q;
    MST_Q.push(P_int(key[1], 1));

    // Prim algorithm
    int tree_weight = 0;
    while (added_num < N && !MST_Q.empty()) {
        P_int top = MST_Q.top();
        int u_key = top.first, u = top.second;
        MST_Q.pop();
        if (visited[u]) continue;
        visited[u] = true;
        added_num++;
        tree_weight += u_key;

        for (arcNode* p = vexes[u]; p; p = p->next) {
            int v = p->v, w = p->w;
            if (!visited[v] && key[v] > w) {
                key[v] = w;
                MST_Q.push(P_int(key[v], v));
            }
        }
    }
    cout << tree_weight << endl;
}