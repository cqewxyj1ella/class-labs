//AC from xrs
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXNAMES 10000000
#define MAXNAMELENGTH 5

char wd[MAXNAMES + 1][MAXNAMELENGTH + 2];

#define HASHNUM 524287
#define HASHSIZE HASHNUM * MAXNAMELENGTH

struct hashdata{
    char name[MAXNAMELENGTH + 1] = {0};
    int nums = 0;
};
hashdata hash[HASHSIZE + 1];
int cnt = 0;
int n = 0;

//TODO: hash表的建立，维护和将查找换为hash查找，hash表采用线性散列
//todo:返回str转化得到的hash初始值
int HASH(char *str){
    int l = strlen(str);//?
    int n = 0;
    for(int i = 0; i< l; i++){
        n = (n * 256 + str[i]) % HASHNUM;
    }
  	//n += (HASHNUM * (l - 1)) % HASHSIZE;
    return n;
}

int Hash_add(char *str){
    int h = HASH(str);
    int i = 1;
    while(true){
        if (hash[h].name[0] != 0){
            if(strcmp(str, hash[h].name) == 0){
                break;
            } else {
                h = (h + i) % HASHSIZE;
                i++;
            }
        } else {
            memcpy(hash[h].name, str, sizeof(str[0]) * (strlen(str) + 1));
            break;
        }
    }
    hash[h].nums++;
    return 0;
}

int Hash_delete(char *str){
    int h = HASH(str);
    int i = 1;
    while(true){
        if (hash[h].name[0] != 0){
            if(strcmp(str, hash[h].name) == 0){
                if(hash[h].nums > 0) hash[h].nums--;
                return 0;
            } else {
                h = (h + i) % HASHSIZE;
                i++;
            }
        } else {
            return 1;
        }
    }
    return 1;
}

//todo:在hash表内查找指定的字符串是否存在,若不存在返回0，存在返回非0
int Hash_find(char *str){
    int h = HASH(str);
    int i = 1;
    while(true){
        if (hash[h].name[0] != 0){
            if(strcmp(str, hash[h].name) == 0){
                if(hash[h].nums > 0){
                    return 1;
                } else {
                    return 0;
                }
            } else {
                h = (h + i) % HASHSIZE;
                i++;
            }
        } else {
            return 0;
        }
    }
    return 0;
}

/*int find(int tgt, int begin, int end){
    if(begin > end) return 0;
    for(int i = begin; i <= end; i++){
        if(strcmp(wd[tgt], wd[i]) == 0) return 1;
    }
    return 0;
}*/

//当调用此函数时应确保hash表中存在begin-end的各个表项
//返回1-拓展，2-收缩，3-右移
int cont(int begin, int end){
    if(end >= n) return 0;
    if(begin > end) return 0;
    cnt++;
    //b,f
    if(Hash_find(wd[end + 1])){//f
        Hash_delete(wd[begin]);
        if(Hash_find(wd[begin])){//b //1,1
            int f = end + 1;
            Hash_add(wd[begin]);
            if(f<n){
                while(Hash_find(wd[f]) && f < n){
                    f++;
                    cnt++;
                }
            }
            Hash_delete(wd[begin]);
//printf("%d::%d->%d::%d!%d\n", begin, end, begin+1, end, f);
            return 2; // 1,1
        } else { // 0,1
//printf("%d::%d->%d::%d\n", begin, end, begin, end+1);
            Hash_add(wd[begin]);
            Hash_add(wd[end + 1]);
            return 1; //0,1
        }
    } else {
        Hash_delete(wd[begin]);
        if(Hash_find(wd[begin])){// 1,0
//printf("%d::%d->%d::%d\n", begin, end, begin+1, end);
            return 2; // 1,0
        } else {//0,0
//printf("%d::%d->%d::%d\n", begin, end, begin+1, end + 1);
            Hash_add(wd[end + 1]);
            return 3; // 0,0
        }
    }
    return 0;
}
//printf("%d::%d->%d::%d\n", begin, end, begin+1, end+1);

int main(){
    int k;
    scanf("%d %d", &n, &k);
    getc(stdin);
    for(int i = 0; i < n - 1; i++){
        int j = 0;
        do{
            wd[i][j] = getc(stdin);
        }while(wd[i][j++] != ' ');
        wd[i][j-1] = 0;
//printf("%d:%s;\n",i,wd[i]);
    }
    scanf("%s", wd[n-1]);
//printf("%d:%s;\n",n-1,wd[n-1]);
    int l = 1, i = 1;
    Hash_add(wd[0]);
    while(l < k){
        if(!Hash_find(wd[i])){
            l++;
        }
        Hash_add(wd[i]);
        i++;
    }
//printf("%d::%d->%d::%d\n", -1, -1, 0, i-1);
    int b = 0;
    int e = i - 1;
    int key = 0;
    do{
        key = cont(b, e);
//printf("%d,%d::%d\n",b,e,key);
        switch (key){
            case 1:
                e++;
                break;
            case 2:
                b++;
                break;
            case 3:
                b++;
                e++;
                break;
            default:
                break;
        }
    }while(key != 0);
    
    printf("%d", cnt);
    return 0;
}
