/*************************************************************************
	> File Name: wait.c
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月27日 星期三 16时15分49秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc,char *argv[])
{
	pid_t  pid;
	char  *msg;
	int k;
	int exit_code;

	pid = fork();
	switch(pid){
	case 0:
		msg = "Child process is running";
		k=5;
		exit_code=37;
		break;
	case -1:
		perror("process creation failed\n");
		break;
	default :
		exit_code=0;
		break;
	}

	if(pid != 0){
		int stat_var;
		pid_t child_pid;

		child_pid = wait(&stat_var);
		printf("child process has exited,pid = %d\n",child_pid);
		if(WIFEXITED(stat_var)){
			printf("child exited with code %d\n", WEXITSTATUS(stat_var));

		}
		else{
			printf("child exited abnormally\n");
		}
	}
	else{
		while(k-- >0){
			puts(msg);
			sleep(1);
		}
	}

	exit(exit_code);
    return 0;
}
