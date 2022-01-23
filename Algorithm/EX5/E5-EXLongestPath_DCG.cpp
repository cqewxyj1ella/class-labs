/* Description:
 * given a directed graph, find the max subgraph such that
 * any node u, v it contains has relations: either u connected to v or v connected to u
 * input:
 * N M
 * then every line is u v (u->v) for M lines
 * solution:
 * find the longest path in the graph
 * can use topological sort
 * but this graph is not garanteed to be a DAG
 * so use Tarjan algorithm to find the strongly connected subgraph
 * and abstract the subgraph as a single node(contains many node)
 * the new graph must be stored, so every original edge should be visited
 * then apply topological sort
 * T(N,M) = O(N+M), S(N,M) = O(N+M)
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

const int MAX_N = 5005;
const int MAX_M = 100005;
int N = 0, M = 0;
int orig_head[MAX_N];   // before abstraction, head array for nodes
int abstract_head[MAX_N]; // after abstraction, head array for nodes
int in_set[MAX_N];      // for each node, which set is it in
int contains[MAX_N];    // for each set, how many nodes it contains
int set_num = 0;            // how many sets have been recorded
typedef struct {
    int u;
    int v;
    int next;
} Edge;
Edge orig_e[MAX_M];     // before abstraction
Edge abstract_e[MAX_M]; // after abstraction
int inDegree[MAX_N];    // every node's indegree
int dis[MAX_N]; // the longest distance from every node
// both 2 arrays above are for abstracted graph
int top = 0;    // top pointer of edge

void add_arc(const int u, const int v, Edge* e, int* head) {
    // add an arc u->v
    ++top;  // top != 0
    e[top].u = u;
    e[top].v = v;
    e[top].next = head[u];
    head[u] = top;
}

int dfn[MAX_N];
int low[MAX_N];
vector<int> stack;
bool in_stack[MAX_N];   // if node i is in the stack
int cnt = 0;
void tarjan(int u) {
    // apply tarjan algorithm on original graph to find strong connectivities
    // and store them in 2 arrays: in_set and contains
    // initialization
    dfn[u] = low[u] = ++cnt;
    stack.push_back(u);
    in_stack[u] = true;
    // traverse node u's neighbors
    for (int i = orig_head[u]; i != 0; i = orig_e[i].next) {
        if (!dfn[orig_e[i].v]) {
            // hasn't been visited
            tarjan(orig_e[i].v);
            low[u] = min(low[u], low[orig_e[i].v]);
        }
        else if (in_stack[orig_e[i].v]) {
            // already visited and in stack(v is u's ancestor)
            low[u] = min(low[u], dfn[orig_e[i].v]);
        }
    }
    // after traverse
    if (dfn[u] == low[u]) {
        set_num++;
        //cout << "this is a strong connectivity: " << set_num << endl;
        int cur; // current node
        do {
            //cur = *(stack.end()-1); // the last element of stack
            cur = stack.back();
            stack.pop_back();
            //cout << cur << endl;
            in_set[cur] = set_num;
            contains[set_num]++;
            in_stack[cur] = false;
        } while (u != cur);
        //cout << "there are " << contains[set_num] << " nodes in the set" << endl;
    }

}

void abstract_graph(void) {
    // astract all the strong connectivity as one node in the new graph
    // also calculate the indegree of the new graph
    top = 0;
    
    for (int u = 1; u <= N; u++) {
        // traverse every node's arcs
        // amortized analysis: O(E)'s time
        for (int i = orig_head[u]; i != 0; i = orig_e[i].next) {
            int v = orig_e[i].v;
            if (in_set[u] != in_set[v]) {
                // u and v are in different set
                add_arc(in_set[u], in_set[v], abstract_e, abstract_head);
                inDegree[in_set[v]]++;
            }
        }
    }
    
   /*
   // another method: according to the edge
   for (int i = 1; i <= M; i++) {
       int u = orig_e[i].u, v = orig_e[i].v;
       if (in_set[u] != in_set[v]) {
           add_arc(in_set[u], in_set[v], abstract_e, abstract_head);
           inDegree[in_set[v]]++;
       }
   }
   */
}

void topoSort(void) {
    // apply topological sort to the abstrated graph
    stack.clear();
    // initialization
    for (int i = 1; i <= set_num; i++) {
        if (inDegree[i] == 0) {
            stack.push_back(i);
            dis[i] = contains[i];
        }
    }
    // topo sort
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (int i = abstract_head[u]; i != 0; i = abstract_e[i].next) {
            int v = abstract_e[i].v;
            inDegree[v]--;
            //dis[v] = max(dis[v], dis[u]+contains[v]);
            dis[v] = dis[u] + contains[v];
            if (inDegree[v] == 0)
                stack.push_back(v);
        }
    }
}

int main(void) {
    // input and initialization
    memset(orig_head, 0, MAX_N*sizeof(orig_head[0]));
    memset(abstract_head, 0, MAX_N*sizeof(abstract_head[0]));
    memset(in_set, 0, MAX_N*sizeof(in_set[0]));
    memset(contains, 0, MAX_N*sizeof(contains[0]));
    memset(orig_e, 0, MAX_M*sizeof(orig_e[0]));
    memset(abstract_e, 0, MAX_M*sizeof(abstract_e[0]));
    memset(inDegree, 0, MAX_N*sizeof(inDegree[0]));
    memset(dis, 0, MAX_N*sizeof(dis[0]));
    memset(dfn, 0, MAX_N*sizeof(dfn[0]));
    memset(low, 0, MAX_N*sizeof(low[0]));
    stack.clear();
    cin >> N >> M;  // global variable
    for (int i = 1; i <= M; i++) {
        int u = 0, v = 0;
        cin >> u >> v;
        add_arc(u, v, orig_e, orig_head);
    }
    // find strong connectivity and abstract graph
    for (int i = 1; i <= N; i++) {
        if (!dfn[i])
            tarjan(i);
    }
    abstract_graph();
    // test the abstract_graph
    /*
    cout << "after abstration" << endl;
    for (int u = 1; u <= set_num; u++) {
        cout << "indegree of u: " << inDegree[u] << endl;
        for (int i = abstract_head[u]; i != 0; i = abstract_e[i].next) {
            cout << abstract_e[i].u << " " << abstract_e[i].v << endl;
        }
    }
    */
    // topological sort the abstracted graph
    topoSort();
    // calculate the answer
    int answer = 0;
    for (int i = 1; i <= set_num; i++) {
        answer = max(answer, dis[i]);
    }
    cout << answer << endl;

}
