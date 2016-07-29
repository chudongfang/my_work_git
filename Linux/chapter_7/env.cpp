#include <stdio.h>
#include <malloc.h>

//extern  char **environ;
int main(int argc, char const *argv[],char **envp)
{
	int i;
	printf("Argument\n");
	for(i=0;i<argc;i++){
		printf("argv[%d] is %s\n", i,argv[i]);
	}
    printf("evironment:\n");
	for(i=0;envp[i]!=NULL;i++){
		printf("%s\n", envp[i]);
	}
	return 0;
}
