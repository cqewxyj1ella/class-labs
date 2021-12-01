/* Discription
 * given a knapsack which can contain items weighted at W
 * given n kinds of items to be put into it
 * output the max value amount in the knapsack
 * 1 <= n <= 200; 1 <= W <= 10000
 * 1 <= wi, vi <= 1000
 * 1 <= numi <= 10000
 * input format:
 * n W
 * (for every item):
 * wi vi numi
 * (weight, value, selectable nums)
 * greedy algorithm: partial accepted
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
    /* weight: the weight of the item
     * value: the value of the item(importance)
     * amount: in total we only have these amount of items
     * v_per_w: value per weight
    */
    int weight;
    int value;
    int amount;
    float v_per_w;
} item, *itemList;

int myCmp(const void* a, const void* b) {
    return (*(itemList)a).v_per_w < (*(itemList)b).v_per_w ? 1 : -1;
}

int main(void) {
    int n = 0, Weight = 0;
    scanf("%d%d", &n, &Weight);
    itemList items = (itemList)malloc(n*sizeof(item));
    memset(items, 0, n*sizeof(item));
    //initiate item array
    for (int i = 0; i < n; i++) {
        scanf("%d%d%d", &items[i].weight, &items[i].value, &items[i].amount);
        items[i].v_per_w = (float)items[i].value / (float)items[i].weight;
    }
    qsort(items, n, sizeof(items[0]), myCmp);
    //input and sort by v_per_w: from big to small

    int in_value = 0, left_weight = Weight;
    //the amount of value of the items already in knapsack
    //and the left volume in the knapsack
    int i; //loop variant
    for (i = 0; i < n; i++) {
        if (items[i].amount > left_weight) //already filled
            break;
        int needed = left_weight / items[i].weight; //needed amount
        if (needed >= items[i].amount) { //command >= supply
            //empty the available amount
            in_value += items[i].amount * items[i].value;
            left_weight -= items[i].amount * items[i].weight;
        }
        else { //command < supply
            //only give the needed items[i]
            in_value += needed * items[i].value;
            left_weight -= needed * items[i].weight;
        }
    }
    int needed_i = Weight / items[i].weight;
    if (items[i].value * needed_i > in_value)
        in_value = items[i].value * needed_i;
    printf("%d\n", in_value);
    //system("pause");
    return 0;

}