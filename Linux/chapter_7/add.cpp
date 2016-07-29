/*************************************************************************
	> File Name: add.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月28日 星期四 11时16分31秒
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
int main(int argc,char *argv[])
{
    int a,b;
    printf("please input tow numbers :");
    scanf("%d %d",&a,&b);
    printf("%d",a+b);
    for(int i=0;i<argc;i++)
        puts(argv[i]);
    return 0;
}
