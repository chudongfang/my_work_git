/*************************************************************************
	> File Name: ice_creamworld.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年08月02日 星期二 08时22分42秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include <algorithm>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
int parent[1005];

int find_set(int x);


int main(int argc,char *argv[])
{
    int n,m,i,j,x,y,x1,y1;
    
    while(scanf("%d %d",&n,&m) != EOF)
    {
        int flag=0;
        for(i=0;i<n;i++)
            parent[i]=i;
        for(i=0;i<m;i++)
        {
            scanf("%d %d",&x,&y);
            x1 = find_set(x);
            y1 = find_set(y);
            if(x1 != y1)
                parent[x1] = y1;
            else   
                flag++;
        }

        printf("%d\n",flag);
    }

    return 0;
}

int find_set(int x){
    if(parent[x] != x){
        parent[x] = find_set(parent[x]);
    }
    return parent[x];
}

