/* Description:
 * a directed acyclic graph with N nodes
 * produce a shortest path between S and T
 * if no path is there, output -1
 * input:
 * N M S T
 * then M lines for all the arcs: Ui Vi Wi
 * 1<=Ui,Vi<=N 1<=Wi<=100
 * 30%: 2<=N<=100, 0<=M<=500
 * 60%: 2<=N<=1000, 0<=M<=20000
 * 100%: 2<=N<=100000, 0<=M<=1000000
*/
// ACed
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
    arcNode *next;
};

int main(void) {
    // input and initialization
    int N = 0, M = 0, S = 0, T = 0;
    cin >> N >> M >> S >> T;
    vector<arcNode*> head(N+1);
    // arc list of vexes, index starts from 1
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        arcNode* arc_u = new arcNode;
        arc_u->v = v; arc_u->w = w;
        arc_u->next = head[u];
        head[u] = arc_u;
    }
    vector<int> dis(N+1);
    // stores distance to S in dijkstra algorithm
    // index starts from 1
    fill(dis.begin(), dis.end(), INFINITE);
    dis[S] = 0; // shortest path starts from S
    vector<bool> visited(N+1);
    // if true, then vex i belongs to S(added to the tree);
    // index starts from 1
    priority_queue<P_int, vector<P_int>, greater<P_int>> Set;
    // the set of nodes already found shortest path
    Set.push(P_int(dis[S], S));

    // dijkstra algorithm
    while (!Set.empty()) {
        P_int top = Set.top(); Set.pop();
        int u_dis = top.first, u = top.second;
        if (visited[u]) continue;
        for (arcNode* p = head[u]; p; p = p->next) {
            int v = p->v, w = p->w;
            if (!visited[v] && dis[v] > dis[u]+w) {
                dis[v] = dis[u] + w;
                Set.push(P_int(dis[v], v));
            }
        }
    }
    if (dis[T] == INFINITE) printf("-1\n");
    else printf("%d\n", dis[T]);
    return 0;
}