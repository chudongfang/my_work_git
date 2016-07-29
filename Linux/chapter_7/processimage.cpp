#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char const *argv[],char **environ)
{
	int i;
	printf("I am a process image!\n");
	printf("My pid = %d,parentpid = %d\n", getpid(),getppid());
	printf("My uid = %d,parentuid = %d\n", getuid(),getgid());
	for(i=0;i<argc;i++)
		printf("argv[%d]:%s\n", i,argv[i]);
	return 0;
}
