/*************************************************************************
	> File Name: studyuid.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月27日 星期三 17时35分46秒
 ************************************************************************/
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#define INF 0x3f3f3f3f
using namespace std;

extern int errno;

typedef long long ll;
int main(int argc,char *argv[])
{
    int fd;
    setuid(5000);
    setgid(10);
    printf("uid study:\n");
    printf("process's   uid =%d euid=%d\n",getuid(),geteuid());
    printf("process's   gid =%d egid=%d\n",getgid(),getegid());
    if((fd = open("test.c",O_RDWR)) == -1){
        printf("Open failure errno is %d :%s \n",errno,strerror(errno));
        exit(1);
    }
    else{
        printf("open successfully!\n");
    }
    
    close(fd);
    exit(0);
}
