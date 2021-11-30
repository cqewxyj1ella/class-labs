//partial ac but I don't think I have any differences from the online code...

/* E2-EX dangerous items partition
 * partition the vexes in 2 set(finally)
 * sort the arcs, and handle them from big->small
 * the max self-to-self dangerous weight is stored seperately
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>

#define MAX_N 100000
#define MAX_M 1000000

struct arcNode {
    //a node for arcs
    int i;
    int j;
    int weight;
} arcs[MAX_M+1];

struct vexNode{
    //a node for vexes
    int set; //the set it belongs to
    bool polarity;  //it has different polarity from its neighbor vex
} vexes[MAX_N+1];

int myCmp(const void* a, const void* b) {
    //for qsort function
    //sort the struct by weight as: small->big
    return (*(arcNode*) a).weight > (*(arcNode*) b).weight ? 1 : -1;
}

int parent[MAX_N+1];
bool polarityReverse[MAX_N+1];

int findSet(int set) {
    if (set != parent[set]) { //this is not the root of the set
        parent[set] = findSet(parent[set]);
        polarityReverse[set] ^= polarityReverse[parent[set]];
        //route compress in the union, so that next time we can find the root more quickly
    }
    return parent[set]; //that's set itself, the root
}

int findPolarityReverse(int set) {
    if (set != parent[set])
        polarityReverse[set] ^= findPolarityReverse(parent[set]);
        //also route compress
    return polarityReverse[set];
}

int realPolarity(int x) {
    //return the vex x's real polarity
    return (findPolarityReverse(vexes[x].set) ^ vexes[x].polarity);
}

int main(void) {
    
    //it seems that global variant will be automatically set to 0
    //so no need for initialization

    int n = 0, m = 0;
    scanf("%d%d", &n, &m);
    int max_self_to_self_dangerous_weight = 0;
    int edge_size = 0; //apart from all the self-to-self edges
    for (int k = 0; k < m; k++) {
        int i, j, weight;
        scanf("%d%d%d", &i, &j, &weight);
        if (i != j) { //store the edge to the arc-list
            arcs[edge_size].i = i;
            arcs[edge_size].j = j;
            arcs[edge_size].weight = weight;
            edge_size++;
        }
        else { //it's a self-to-self edge
            if (weight > max_self_to_self_dangerous_weight)
                max_self_to_self_dangerous_weight = weight;
        }
    }
    for (int i = 0; i <= n; i++)
        parent[i] = i;
    //all the union has root for itself

    qsort(arcs, edge_size, sizeof(arcNode), myCmp);
    //sort the edges by weight: small->big

    int set_size = 0;
    for (edge_size -= 1; edge_size >= 0; edge_size--) {
        //from big to small(by weight), partition the neighbor vexes on edge to different sets
        int i = arcs[edge_size].i;
        int j = arcs[edge_size].j;
        //current i, j
        if (!vexes[i].set) { //i hasn't been partitioned to a set
            if (!vexes[j].set) { //j also hasn't been partitioned
                set_size++;
                vexes[i].set = set_size;
                vexes[j].set = set_size;
                vexes[i].polarity = i < j;
                vexes[j].polarity = i >= j;
                //so i and j have different polarity and are in the same set now
            }
            else { //j is already in a set
                //add i to the set which j belongs to
                vexes[i].set = findSet(vexes[j].set);
                //find j's root set as i's set
                vexes[i].polarity = !realPolarity(j);
                //so i is in the j's set and has different polarity from j now
            }
        }
        else { //i is already in a set
            if (!vexes[j].set) { //j hasn't been partitioned to a set
                //add j to the set which i belongs to
                vexes[j].set = findSet(vexes[i].set);
                vexes[j].polarity = !realPolarity(i);
                //so j is in the i's set and has different polarity from i now
            }
            else { //i and j have both been partitioned in a set
                int pol_i = realPolarity(i);
                int pol_j = realPolarity(j);
                int set_i = findSet(vexes[i].set);
                int set_j = findSet(vexes[j].set);
                std::cout << set_i << ' ' << set_j << std::endl;
                std::cout << pol_i << ' ' << pol_j << std::endl;
                //find their parents' set
                /*
                if (pol_i == pol_j) {
                    if (set_i < set_j) {
                        polarityReverse[set_j] ^= 1;
                        parent[set_j] = set_i;
                    }
                    else {
                        polarityReverse[set_i] ^= 1;
                        parent[set_i] = set_j;
                    }
                }
                else {
                    if (set_i < set_j)
                        parent[set_j] = set_i;
                    else 
                        parent[set_i] = set_j;
                }
                */
                
                if (set_i == set_j) { //they are in the same set
                    if (pol_i != pol_j); //safe
                    else break; //this is the necessary edge and must be printed
                    //note! we search from the big->small, so all the smaller weight edges needn't be considered
                }
                else { //in different set->merge
                    if (set_i < set_j)
                        parent[set_j] = set_i;
                    else
                        parent[set_i] = set_j;
                    if (pol_i != pol_j); //safe
                    else { //pol_i == pol_j
                        if (parent[set_i] == set_j)
                            polarityReverse[set_i] ^= 1;
                        else
                            polarityReverse[set_j] ^= 1;
                    }
                }
                
            }
        }
    }
    int output = (max_self_to_self_dangerous_weight > arcs[edge_size].weight) ? max_self_to_self_dangerous_weight : arcs[edge_size].weight;
    printf("%d\n", output);
    return 0;
}
