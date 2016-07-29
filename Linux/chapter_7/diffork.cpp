#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int globvar=5;

int main(void)
{
	pid_t pid;

	int  var = 1,i;


	//pid=fork();
	pid  =vfork();
    switch(pid){
	case 0:
		i=3;
        printf("%d",getppid());
		while(i-- > 0){
			printf("Child process is running \n");
			globvar++;
			var++;
			sleep(1);
		}
		printf("Child's globvar = %d,var = %d\n", globvar,var);
		break;
		
	case -1:
		perror("process creation failed!\n");
        break;

	default :
        printf("var=%d\n",var);
		i=5;
		while(i-- > 0){
			printf("parent process is running\n");
			globvar++;
			var++;
		    printf("parent's globvar =%d,var = %d\n", globvar,var);
			sleep(1);
		}
		printf("parent's globvar =%d,var = %d\n", globvar,var);
	}
	exit(0);
}
