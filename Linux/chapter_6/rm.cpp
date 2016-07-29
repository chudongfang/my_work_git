/*************************************************************************
	> File Name: rm.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月18日 星期一 20时33分26秒
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
    if(argc<3){
        printf("my_mv <old file> <new file>\n");
        exit(0);
    }

    if(rename(argv[1],argv[2]) < 0){
        perror("rename");
        exit(1);
    }

    return 0;
}
