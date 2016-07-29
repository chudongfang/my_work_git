#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[],char ** environ)
{
	int *a;
	pid_t pid;

	pid = fork();
	switch(pid){
		case -1:
		perror("process creation failed\n");
		exit(1);
		case 0:
		printf("child process is runnig \n");

		printf("My pid = %d,parentpid = %d\n", getpid(),getppid());
		printf("My uid = %d,gid= %d\n", getuid(),getgid());
		execve("processimage",argv,environ);
		printf("process never go to here!\n");
		exit(0);
		default:
		printf("parent process is runming \n");
		break;
	}
	wait(NULL);
	exit(0);
	return 0;
}
