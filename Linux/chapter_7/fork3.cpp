#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	pid_t pid;

	pid = fork();
    if(pid == 0)  sleep(4);
	switch(pid){
		case 0:
			while(1){
				printf(" A backgroud process ,PID:	%d\n,parent ID :%d\n ",getpid(),getppid());
				sleep(3);
			}
		case -1:
			perror("perror creation failed!\n");
			exit(-1);
		default :
			printf("I am parent process ,my pid is %d\n", getpid());
			exit(0);
	}
	return 0;
}
