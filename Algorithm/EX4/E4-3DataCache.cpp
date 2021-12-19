/* Discription:
 * stimulate cache replacement
 * given the amount of memory accesses(N), and the size of cache(K)
 * then give N lines denote N accesses
 * N <= 10000, K <= 1000
 * purpose:
 * find a replacement algorithm so that the MISS times are least
 * solution:
 * greedy algorithm: always replace the farmost memory to be accessed
 * data structure:
 * 1) c++ priority queue
 * 2) fibnaci heap
 */
// ACed
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
#define MEM_SIZE 10010
#define MAX_ACCESS_TIME 10001

typedef struct Cache {
    /* addr is the corresponding cached memory address
     * next_access_time is the next time to access the same cached memory
    */
    int addr;
    int next_access_time;
    bool operator<(const Cache& rhs) const {
        return next_access_time < rhs.next_access_time;
    }
}Cache, *CachePtr;

struct Memory {
    /* cached is to indicate whether this memory space has been cached
     * queue is to record every access time, 
     * and each time this memory space is accessed, the queue will pop_front
     * so the front of the queue is just next access time for this space
    */
    bool cached;
    queue<int> access_time_q;
}mem[MEM_SIZE];

int main(void) {
    // input and initialization
    int N = 0, K = 0;
    scanf("%d%d", &N, &K);
    int* access = (int*)malloc(N*sizeof(int));
    memset(access, 0, N*sizeof(access[0]));
    for (int i = 0; i < N; i++) {
        int access_mem = 0;
        scanf("%d", &access_mem);
        access[i] = access_mem;
        mem[access_mem].access_time_q.push(i);
    }
    priority_queue<Cache> cache;

    // scan the access list and process
    int cache_size = K;
    int cached_num = 0;
    int miss_times = 0;
    for (int i = 0; i < N; i++) {
        int access_mem = access[i];
        mem[access_mem].access_time_q.pop(); // accessed

        if (mem[access_mem].cached) { // HIT
            cached_num++;
            cache_size++;
            // (because will put one new entry in cache
            // to store the next access time of this memory block)
        }
        else { // MISS
            miss_times++; // missed
            if (cached_num < cache_size) { // not filled yet
                cached_num++;
            }
            else { // filled -> replacememt
                // replace the cached memory which is farmost to be accessed
                // discard one cache
                auto replaced = cache.top();
                cache.pop();
                mem[replaced.addr].cached = false;
            }
        }

        // make the common part outside of if-else block: add new cache entry to the priority queue
        CachePtr insert_cache = (CachePtr)malloc(sizeof(Cache));
        insert_cache->addr = access_mem;
        if (!mem[access_mem].access_time_q.empty())
            insert_cache->next_access_time = mem[access_mem].access_time_q.front();
        else 
            insert_cache->next_access_time = MAX_ACCESS_TIME;
        cache.push(*insert_cache);

        mem[access_mem].cached = true; // accessed
    }

    cout << miss_times << endl;
}
