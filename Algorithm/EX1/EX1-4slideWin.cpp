//time limit exceeded

/*Discription:
 *find the sub troops that has just k names nonredundant
 *input:
 *n k(0<k<n<=10E+8)
 *<n word(name_i)>(0<len(name_i)<5)
 *n is the length of the troop
 *output the amount of sub troops
 *
 *solution:
 *slide window
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NAME_LEN 5

int main(void) {

    int n, k;
    int i, j; //loop variant
    scanf("%d%d", &n, &k);
    char** names = (char**)malloc((n+1)*sizeof(char*));
    for (i = 0; i < n+1; i++) {
        names[i] = (char*)malloc((MAX_NAME_LEN+1)*sizeof(char));
        memset(names[i], 0, (MAX_NAME_LEN+1)*sizeof(char));
    }
    //initiation

    for (i = 0; i < n; i++) {
        scanf("%s", names[i]);
    }
    /*
    for (i = 0; i < n; i++) {
        printf("names[%d]: %s\n", i, names[i]);
    }
    */ //debug
    int subTroops = 0;      //amount of the sub troops satisfie the condition
    int winSize = k;        //window size = winR - winL + 1
    int diffNames;          //different names within the window; window always move right-forward
    int winL = 0, winR = winL + winSize - 1;
    //the left and right side of the window
    int* firstWinDiffNames = (int*)malloc((n+1)*sizeof(int*));
    memset(firstWinDiffNames, 0, (n+1)*sizeof(int));
    //to store the first window has how many diffrent names
    diffNames = 0;      //at least there'll be 1 different name
    int diffFlag = 1;
    for (i = winL; i <= winR; i++) {
        diffFlag = 1;
        for (j = winL; j < i; j++) {
            if (strcmp(names[j], names[i]) == 0) {//different names
                diffFlag = 0;
                break;
            }
        }
        if (diffFlag)
            diffNames++;
    }
    if (diffNames == k)
        subTroops++;
    firstWinDiffNames[k] = diffNames;
    //calculate the very 1st window
    for (; winSize <= n; winSize++) {

        if (winSize > k) {
            winL = 0; winR = winL + winSize - 1;
            diffFlag = 1;
            for (i = 0; i < winR; i++) {
                if (strcmp(names[i], names[winR]) == 0) {
                    diffFlag = 0;
                    break;
                }
            }
            if (diffFlag) {
                diffNames = firstWinDiffNames[winSize-1] + 1;
            }
            else {
                diffNames = firstWinDiffNames[winSize-1];
            }
            firstWinDiffNames[winSize] = diffNames;
            //to see the influence of names[winR]
            
            if (diffNames == k)
                subTroops++;
        }   //calculate the 1st window if winSize > k
        
        for (winL++, winR++; winR < n; winL++, winR++) {
            diffFlag = 1;
            //supose the just deleted name(names[winL-1] contributes to the diffNames)
            for (i = winL; i < winR; i++) {
                if (strcmp(names[i], names[winL-1]) == 0) {
                    diffFlag = 0; //at least there's 1 name the same as names[winL-1]
                    break;
                }
            }
            if (diffFlag)
                diffNames--; //because names[winL-1] is deleted
            //see the influence of names[winL-1]

            diffFlag = 1;
            for (i = winL; i < winR; i++) {
                if (strcmp(names[i], names[winR]) == 0) {
                    diffFlag = 0; //at least there's 1 name the same as names[winR]
                    break;
                }
            }
            if (diffFlag)
                diffNames++; //because names[winR] is added
            //see the influence of names[winR]

            if(diffNames == k)
                subTroops++;
        }   //calculate the remain windows
        
    }
    /*
    while (winL < n && winR < n) {
        if (diffNames == k)
            subTroops++;
        if (diffNames <= k) {
            winR++;
            int diffFlag = 1;
            //supose the new name(names[winR]) is different from all the names within the window
            for (i = winL; i < winR; i++) {
                if (strcmp(names[i], names[winR]) == 0) {
                    diffFlag = 0; //there's a name the same as the new name
                    break;
                }
            }
            if (diffFlag)
                diffNames++;

        }
        if (diffNames > k) {
            winL++; //window smaller
            int sameFlag = 0;
            //supose the just deleted name(names[winL-1]) is different from all the names within the window
            for (i = winL; i <= winR; i++) {
                if (strcmp(names[i], names[winL-1]) == 0) {
                    sameFlag = 1;
                    break;
                }
            }
            if (!sameFlag)
                diffNames--;
        }
    }
    */
    printf("%d\n", subTroops);
    system("pause");
    return 0;
}