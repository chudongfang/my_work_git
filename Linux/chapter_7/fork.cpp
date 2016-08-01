/*************************************************************************
	> File Name: fork.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月29日 星期五 20时32分06秒
 ************************************************************************/
#include <unistd.h>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include <algorithm>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
void bomp();
int main(int argc,char *argv[])
{
    bomp();
    return 0;
}


void bomp()
{
    fork();
    bomp();
}
