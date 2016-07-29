/*************************************************************************
	> File Name: mynice.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月28日 星期四 08时50分24秒
 ************************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <sys/resource.h>	
#include <sys/wait.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
int main(int argc,char *argv[])
{
	pid_t pid;
	int stat_val=0;
	int oldpri,newpri;
	printf("nice study\n");
	pid = fork();
	switch(pid)
	{
	case 0:
		printf("child :curpid = %d parentpid = %d\n", pid,getppid());
		oldpri = getpriority(PRIO_PROCESS,0);
		printf("old priortity\n");
		newpri = nice(17);
        for(int i=1;i<=10;i++){
            printf("child\n");
            sleep(1);
        }
		printf("newpri= %d\n",newpri);
		exit(0);
	case -1:
		perror("process creat fail!");
		break;
	default :
        printf("parent=%d",getpriority(PRIO_PROCESS,0));
		printf("parent is running childpid =  %d parentpid\n", pid,getpid());
        
        for(int i=1;i<=10;i++)
        {
            
            printf("parent\n");
            sleep(1);
        }
		break;
	}
	wait(&stat_val);
    exit(0);
}
