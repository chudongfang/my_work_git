/*************************************************************************
	> File Name: getpid.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月27日 星期三 16时49分34秒
 ************************************************************************/

#include<sys/types.h>
#include<unistd.h>
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

    pid_t pid;
    if((pid = fork()) == -1){
        printf("fork error\n");
        exit(1);
    }
    if(pid == 0)
        printf("getpid return %d\n",getpid());
    
    exit(0);
    return 0;
}
