/*************************************************************************
	> File Name: meshell.c
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月28日 星期四 09时31分43秒
 ************************************************************************/
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>	
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#define normal          0
#define out_redirect    1
#define in_redirect     2
#define have_pipe       3

void print_prompt();
int find_command(char *command);
void get_input(char *buf);
void explain_input(char *buf,int *argcout,char arglist[100][256]);
void do_cmd(int argcout,char arglist[100][256]);
void myshell();

int main(int argc,char *argv[])
{
	myshell();
}

void myshell()
{
	int i;
	int argcout = 0;
	char arglist[100][256];
	char **arg = NULL;
	char *buf  = NULL;

	buf = (char *)malloc(256);
	if(buf == NULL){
		perror("malloc failed");
		exit(-1);
	}
	while(1){
		memset(buf,0,256);
		print_prompt();
		get_input(buf);
		if( strcmp(buf,"exit\n") == 0 || strcmp(buf,"logout\n") == 0)
			break;
		for(i=0;i<100;i++)
		{
			arglist[i][0]='\0';
		}

		argcout = 0;
		explain_input(buf,&argcout,arglist);
		do_cmd(argcout,arglist);
	}
	if(buf != NULL){
		free(buf);
		buf = NULL;
	}
    exit(0);
}



void print_prompt()
{
	printf("mtshell$ ");
}

void get_input(char *buf)
{
	int len = 0;
	int ch;
	ch = getchar();
	while(len < 256 && ch != '\n'){
		buf[len++] = ch;
		ch = getchar();
	}
	if(len == 256){
		printf("command is too long \n");
		exit(-1);
	}
	buf[len] = '\n';
	len++;
	buf[len] = '\0';
}


void explain_input(char *buf,int *argcout,char arglist[100][256])
{
	int number = 0 ;
	char *p = buf;
	char *q = buf;
	while(1){
		if(p[0] == '\n')
			break;
		if(p[0] == ' ')
			p++;
		else{
			q=p;
			number = 0;
			while( (q[0] != ' ') && q[0] != '\n'){
				number++;
				q++;
			}
			strncpy(arglist[*argcout],p,number+1);
			arglist[*argcout][number] = '\0';
			*argcout = *argcout + 1;
			p=q;
		}
	}
}

void do_cmd(int argcout,char arglist[100][256])
{
	int closefa=0;
	int flag=0;
	int how =0;
	int backgroud = 0;
	int status;
	int i;
	int fd;
	char * arg[argcout + 1];
	char * argnext[argcout + 1];
	char *file;
	pid_t pid;
	for(i=0;i<argcout;i++)
		arg[i]=(char *)arglist[i];

	arg[argcout] = NULL;	

	for(i=0;i < argcout;i++){
		if(strncmp(arg[i],"&",1) == 0){
			if(i == argcout - 1){
				backgroud = 1;
				arg[argcout - 1] =NULL;
				break;
			}
			else {
				printf("wrong command\n");
				return ;
			}
		}
	}

	for(i=0 ;arg[i] != NULL; i++){
		if(strcmp(arg[i],">") == 0){
			flag++;
			how = out_redirect;
			if (arg[i+1] == NULL)
				flag++;
		}
		if( strcmp(arg[i],"<") == 0){
			flag++;
			how = in_redirect;
			if(i == 0)
				flag++;
		}
		if( strcmp(arg[i],"|") == 0){
			flag++;
			how = have_pipe;
			if(arg[i+1] == NULL)
				flag++;
			if( i == 0)
				flag++;
		}

	}

	if(flag > 1){
		printf("wrong command !\n");
		return ;
	}


	if( how == out_redirect){
		for(i=0;arg[i]!=NULL;i++){
			if(strcmp(arg[i],">") == 0){
				file = arg[i+1];
				arg[i] = NULL;
			}
		}
	}
	if(how == in_redirect){
		for(i=0; arg[i] != NULL;i++){
			if(strcmp(arg[i],"<") == 0){
				file = arg[i+1];
				arg[i] = NULL;
			}
		}
	}

	if(how == have_pipe){
		for( i=0; arg[i]!=NULL ;i++){
			if(strcmp(arg[i],"|") == 0){
				arg[i] = NULL;
				
				int j;
				for(j=i+1;arg[j]!=NULL;j++){
					argnext[j-i-1] = arg[j];
				}
				argnext[j-i-1] = arg[j];

				break;
			}
		}
	}

	if( (pid = fork()) < 0){
		printf("fork error\n");
		return ;
	}
	switch(how){
		case 0:
		if(pid == 0){
			if(strcmp(arg[0],"cd") == 0){
				puts(arg[1]);
				chdir(arg[1]);
				closefa=1;
				myshell();
				exit(0);
			}
			if( !(find_command(arg[0])) ){
				printf("%s :command not found !\n",arg[0]);
				exit(0);
			}
			execvp(arg[0],arg);
			exit(0);
		}
		if(pid == 1 && closefa == 1)
		{
			exit(0);
		}
		break;
		case 1:
		if(pid == 0){
			if( !(find_command(arg[0])) ){
				printf("%s :command not found !\n",arg[0]);
				exit(0);
			}
			fd = open(file,O_RDWR | O_CREAT | O_TRUNC,0644);
			dup2(fd,1);
			execvp(arg[0],arg);
			exit(0);
		}
		break;
		case 2:
		if(pid == 0){
			if( !(find_command(arg[0])) ){
				printf("%s :command not found !\n",arg[0]);
				exit(0);
			}
			fd = open(file,O_RDONLY);
			dup2(fd,0);
			execvp(arg[0],arg);
			exit(0);
		}
		break;

		case 3:
		if(pid == 0){
			int pid2;
			int status2;
			int fd2;
			if( (pid2 = fork()) < 0){
				printf("fork2 error\n");
				return ;
			}
			else if(pid2 == 0){
				if(!(find_command)(arg[0])){
					printf("%s :command not found !\n",arg[0]);
					exit(0);
				}
				fd2 = open("/tmp/youdonotknowfile",
							O_WRONLY | O_CREAT | O_TRUNC ,0644);
				dup2(fd2,1);
				execvp(arg[0],arg);
				exit(0);
			}

			if(waitpid(pid2,&status2,0) == -1){
				printf("wait for child process error\n");
			}

			if(!(find_command)(argnext[0])){
					printf("%s :command not found !\n",arg[0]);
					exit(0);
			}
			fd2 = open("/temp/youdonotknowfile",O_RDONLY);
			dup2(fd2,0);
			execvp(argnext[0],argnext);

			if(remove("/temp/youdonotknowfile"))
				printf("remove error\n");
			exit(0);
		}
		break;
		
		default :
		break;
	}
}


int find_command(char *command)
{
	DIR *dp;
	struct dirent*  dirp;
	char *path[]={"./","/bin","/usr/bin",NULL};

	if(strncmp(command,"./",2) == 0)
		command =command + 2;
	int i=0;
	while(path[i]!=NULL){
		if((dp = opendir(path[i])) == NULL){
			printf("can not open /bin\n");
		}
		while((dirp = readdir(dp)) != NULL){
			if(strcmp(dirp -> d_name,command) == 0){
				closedir(dp);
				return 1;
			}
		}
		closedir(dp);
		i++;
	}
	return 0;
}



