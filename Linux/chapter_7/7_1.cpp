/*************************************************************************
	> File Name: 7_1.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月27日 星期三 09时20分57秒
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
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
	printf("Process Creation Study\n");
	pid = fork();
	switch(pid){
		case 0:
		printf("Child progress is running ,Curpid is %d ,parentpid is %d\n",pid,getppid());
		break;
		case -1:
		perror("progress creadtion failed!\n");
		break;
		default:
		printf("Parent progress is running ,Childpid is %d,parentpid is %d\n", pid,getppid());
		break;

	}		
	exit(0);
    return 0;
}
