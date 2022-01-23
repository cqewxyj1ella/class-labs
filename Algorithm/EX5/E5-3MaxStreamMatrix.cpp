/* Description:
 * there are N nodes in a transport system
 * directed graph, there are conveyors between nodes
 * each conveyor has life time L, which transports L goods before broken-out
 * need to transport from S to T, how many goods at most?
 * input:
 * N M S T
 * then M lines are conveyors: Ui Vi Li
 * 1<=Ui,Vi<=N, 1<=Li<=100
 * 40%: 2<=N<=50, 2<=M<=500
 * 100%: 2<=N<=500, 2<=M<=20000
 * data structure: 
 * sparse graph, so use arc-link
 * algorithm:
 * Ford-Fulkerson
 * use BFS to find a path from S to T
 * instead of link list, use matrix on arcs
 * attention!!!!!
 * the most tricked thing is that there might be multi arcs between node u and v
 * so the weight of (u,v) should += new arc's weight(instead of change its value directly)
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

const int MAX_N = 505;
const int NIL = -1;
const int INFINITE = 0x7fffffff;
struct Graph {
    /* a structure to describe graph in networks
     * vexnum: the number of nodes
     * arcs: a matrix of arcs
     * rc: a matrix of residual graph(remain capacity)
    */
    int vexnum;
    int arcs[MAX_N][MAX_N];
    int rc[MAX_N][MAX_N];
};

bool bfs(const Graph G, const int S, const int T, int parent[]) {
    /* operate BFS on graph G, return true if find a path from S to T
     * and return false otherwise
     * can trace back to S using parent array
     * only look at arcs in residual graph: f[][]
    */
    int N = G.vexnum;
    vector<bool> visited(N+1);
    queue<int> q; // a queue about G's vex index
    for (int i = 1; i <= N; i++) {
        parent[i] = NIL;
    }
    visited[S] = true;
    q.push(S);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 1; i <= N; i++) {
            if (!visited[i] && G.rc[u][i]) {
                visited[i] = true;
                parent[i] = u;
                //if (i == T) break;
                q.push(i);
            }
        }
    }
    
    return visited[T];
}

int fordFulkerson(Graph &G, const int S, const int T) {
    int N = G.vexnum;
    int max_flow = 0;
    int* parent = (int*)malloc((N+1)*sizeof(int));
    while (bfs(G, S, T, parent)) {
        // there is an argument path in the residual graph from S to T
        // find the minimum value of delta f
        int min_f = INFINITE;
        for (int v = T; v != S; v = parent[v]) {
            int u = parent[v];
            int cf = G.rc[u][v];
            if (cf < min_f) min_f = cf;
        }
        max_flow += min_f;
        // update the argument path
        for (int v = T; v != S; v = parent[v]) {
            int u = parent[v];
            G.rc[u][v] -= min_f;
            G.rc[v][u] += min_f;
            int a = 1;
        }
    }
    return max_flow;
}

int main(void) {
    // input and initialization
    int N = 0, M = 0, S = 0, T = 0;
    cin >> N >> M >> S >> T;
    Graph graph;
    graph.vexnum = N;
    for (int i = 1; i <= M; i++) {
        int u, v, L;
        cin >> u >> v >> L;
        graph.arcs[u][v] += L;
        // initialize the residual graph
        graph.rc[u][v] += L;
    }

    int max_flow = fordFulkerson(graph, S, T);
    cout << max_flow << endl;
}
