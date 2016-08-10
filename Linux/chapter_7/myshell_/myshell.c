/*************************************************************************
	> File Name: meshell.c
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月28日 星期四 09时31分43秒
 ************************************************************************/
#include <pwd.h>
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
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#define  MAX_SIZE 256

#define normal          0  //正常命令
#define out_redirect    1  //重定向输出
#define in_redirect     2  //重定向输入
#define have_pipe       3  //管道命令

void do_cmd(int argcout,char arglist[100][256],int his_count,char history[][256]);
int find_command(char *command);           
void explain_input(char *buf,int *argcout,char arglist[100][256]);
void get_input(char *buf,char *path,int *his_count,char history[][256]);


struct passwd *pw;  //用户信息结构体



int main(int argc,char *argv[])
{

	int i;
    
    char history[256][256];//存储历史
    int his_count=0;       //历史的数量
	
    int argcout = 0;        //命令计数器
	char arglist[100][256]; //储存命令
	char **arg = NULL;      
	char *buf  = NULL;      //临时字符数组
	char path [MAX_SIZE];   //进程所在路径
    memset(history,0,sizeof(history));

	buf = (char *)malloc(256);   
	
	if(buf == NULL){
		perror("malloc failed");
		exit(-1);
	}
	


	while(1){
		memset(buf,0,256);             
		memset(path,0,MAX_SIZE);  
		if (getcwd( path, MAX_SIZE) == NULL)    //得到当前路径
		{
    		printf("read path error!\n");
    		exit(-1);
		}
		//获取当前目录绝对路径，即去掉程序名

		get_input(buf,path,&his_count,history);	                 //输出路径提示，并得到输入命令   

		while(strlen(buf) == 0)                  
		{ 
		 	get_input(buf,path,&his_count,history);                //无输入，继续接受输入		
		}	
		
		if( strcmp(buf,"exit\n") == 0 || strcmp(buf,"logout\n") == 0)  //exit 和 logout 退出shell
			break;
		
		for(i=0;i<100;i++)                   //初始化命令         
		{
			arglist[i][0]='\0';           
		}

		argcout = 0;                         //命令个数为0
		explain_input(buf,&argcout,arglist); //把输入解析成命令 

		do_cmd(argcout,arglist,his_count,history);             //对命令进行分析，操作
	}

	if(buf != NULL){
		free(buf);
		buf = NULL;
	}

    exit(0);
}



/****************************************
 * 函数功能： 实现提示，接受输入，tab 补全，记录输入历史
 * 
 * 函数参数： 
 *  参数1 ：输入
 *  参数2 ：解析后的命令
 ***************************************************/

void get_input(char *buf,char *path,int *his_count,char history[][256])
{

    int count=0,i,len;
	uid_t  uid;
    char  *temp;
  	char  str[500];
    uid = getuid ();  //得到uid   
    pw = getpwuid (uid);    //得到用户信息
    if (!pw)  
    {  
        printf ("Couldn't find out about user %d./n", (int)uid);  
        return ;  
    }  
	sprintf(str,"\033[;31m %s@\033[0m\033[;31mmyshell\033[0m\033[;32m %s\033[0m\033[;33m$\033[0m",pw -> pw_name,path);    //加上颜色，合成提示语句str
    temp = (char* )malloc(sizeof(256));  //malloc 申请空间，用于readline 注意： readline 返回值只能用malloc申请的指针接受
    memset(temp,0,sizeof(temp));  //初始化空间
    temp = readline(str);         //调用readline库函数 实现补全   ,调用方法在另一篇博客
    strcpy(buf, temp);            //把接受到的存入到buf里面

    //历史记录，如果输入的全部是空格或是换行，则不存储 
    len = strlen(buf);           
    for(i=0;i < len; i++)
        if(buf[i] == ' '||buf[i] == '\n') 
            count++;
    
    if(count != len)
    {
        //如果输入的命令与前一个重复，则不把其记录到history里面
        if(*his_count != 0){
            if(strcmp(history[*his_count-1],buf) != 0){
                strcpy(history[*his_count],buf);
                *his_count = *his_count + 1;
                add_history(buf);
            }
        }
        else
        {
            strcpy(history[*his_count],buf);
            *his_count = *his_count + 1;
            add_history(buf);
        }
    }
    
    free(temp);
}

/*************************************************
 * 函数功能 ：根据输入的内容解析出命令
 * 函数参数：
 *  1 ：输入的内容
 *  2 ：命令个数
 *  3 ：存储命令数组
 *************************************************/

void explain_input(char *buf,int *argcout,char arglist[100][256])
{
	int number = 0 ;
	char *p = buf;
	char *q = buf;
	while(1){
		if(p[0] == '\n' || p[0] == '\0')
			break;
		if(p[0] == ' ')
			p++;
		else{
			q=p;
			number = 0;
			while( (q[0] != ' ') && q[0] != '\0' &&q[0] != '\n'){
				number++;//代表单个命令的长度
				q++;
			}
			strncpy(arglist[*argcout],p,number+1);   //把单个命令解析出来
			arglist[*argcout][number] = '\0';         //最后加上结尾
			*argcout = *argcout + 1; 
			p=q;
		}
	}
}

/*****************************************************
 * 函数功能 ：命令分类，并执行命令
 * 参数：
 *  1：命令个数
 *  2：存储命令数组
 * **************************************************/
void do_cmd(int argcout,char arglist[100][256],int his_count,char history[][256])
{
	int flag=0;
	int how =0;
	int backgroud = 0;
	int status;
	int i;
	int fd;
	char * arg[argcout + 4];
	char * argnext[argcout + 4];
	char *file;
	char home[256];
	pid_t pid;
	

	for(i=0;i<argcout;i++)
		arg[i]=(char *)arglist[i];    

	arg[argcout] = NULL;	

    //解析是否是后台运行
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
		//解析重定向输入
        if(strcmp(arg[i],">") == 0){
			flag++;
			how = out_redirect;
			if (arg[i+1] == NULL)
				flag++;
		}
        //解析重定向输出
		if( strcmp(arg[i],"<") == 0){
			flag++;
			how = in_redirect;
			if(i == 0)
				flag++;
		}
        //解析管道命令
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

/**************解析重定向输出文件名*******************/
	if( how == out_redirect){
		for(i=0;arg[i]!=NULL;i++){
			if(strcmp(arg[i],">") == 0){
				file = arg[i+1];
				arg[i] = NULL;
			}
		}
	}
/*******************解析重定向输入文件名**********************/
	if(how == in_redirect){
		for(i=0; arg[i] != NULL;i++){
			if(strcmp(arg[i],"<") == 0){
				file = arg[i+1];
				arg[i] = NULL;
			}
		}
	}

/***************解析管道命令参数*******************/
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
  // 如果命令为history 则输出历史。
    if(strcmp(arg[0],"history") == 0&& argcout == 1){
        printf("tolal num %d :\n",his_count);
        for(i=0;i<his_count;i++){
            printf("%d\t%s\n",i+1,history[i]);
        }
        goto loop;
    }


/*********************CD命令****************************/
//cd命令为内嵌命令，，因为chdir只能改变其本身进行的目录 
 
    if(strcmp(arg[0],"cd") == 0){
		sprintf(home,"/home/%s/",pw->pw_name);
        if(argcout == 1 || argcout == 2 && strcmp(arg[1],"~") == 0) //特殊情况判断
		{
		    chdir(home);
            goto loop;
		}
        if(chdir(arg[1]) == -1){
            perror("chdir");
        }
        goto  loop;
	}

//fork一个子进程，执行参数
	if( (pid = fork()) < 0){
		printf("fork error\n");
		return ;
	}
	
	switch(how){
/***************************普通命令*************************/		
	case 0:
        //子进程
		if(pid == 0){
			if( !(find_command(arg[0])) ){
				printf("%s :command not found !\n",arg[0]);
				exit(0);
			}
			execvp(arg[0],arg);
			exit(0);
		}
		break;
		
/************************重定向输入命令**************************/
	case 1:
		if(pid == 0){
			if( !(find_command(arg[0])) ){
				printf("%s :command not found !\n",arg[0]);
				exit(0);
			}
            //打开一个文件
			fd = open(file,O_RDWR | O_CREAT | O_TRUNC,0644);
			//把屏幕输出流给文件
            dup2(fd,1);

			execvp(arg[0],arg);//加载参数命令
			exit(0);
		}
		break;
/**********************重定向输出命令**************************/        
	case 2:
		if(pid == 0){
			if( !(find_command(arg[0])) ){
				printf("%s :command not found !\n",arg[0]);
				exit(0);
			}
            //打开文件
			fd = open(file,O_RDONLY);
			//把键盘输入流给文件
            dup2(fd,0);
			execvp(arg[0],arg);
            close(fd);
			exit(0);
		}
		break;
/**********************管道命令*************************/
	case 3:
		if(pid == 0){
			int pid2;
			int status2;
			int fd2;

            //fork一个子进程，运行管道符前命令
			if( (pid2 = fork()) < 0){
				printf("fork2 error\n");
				return ;
			}
			else if(pid2 == 0){
				if(!(find_command)(arg[0])){
					printf("%s :command not found !\n",arg[0]);
					exit(0);
				}
                
                
                //打开文件，将屏幕输出流1 给 fd2，其向屏幕输出的数据就存储到文件当中
                if((fd2 = open("/tmp/tempfile",O_WRONLY | O_CREAT | O_TRUNC ,0644)) == -1)
                    printf("open /tmp/tempfile failed!");
				dup2(fd2,1);
                execvp(arg[0],arg);
                close(fd2);
                exit(0);
			}

			
            //等待管道符前面命令执行完
            if(waitpid(pid2,&status2,0) == -1){
				printf("wait for child process error\n");
			}

            //管道后命令开始执行
            if(!(find_command)(argnext[0])){
					printf("%s :command not found !\n",argnext[0]);
					exit(0);
			}
           
            if((fd2 = open("/tmp/tempfile",O_RDONLY)) == -1) {
                perror("Open");
            }
			dup2(fd2,0);

			execvp(argnext[0],argnext);
			if(remove("/tmp/tempfile") == -1)//移除文件
				perror("remove");
			exit(0);

		}
		break;
		
	default :
		break;
	}
	
/**********************杀死父进程，后端运行********************************/    
    if(backgroud ==1  && pid != 0){
        printf("[process id %d]\n",pid);
        exit(0);
    }

    if(backgroud == 1 && pid)
        goto loop;

/************************父进程等待子进程退出******************************/    
    
    if(waitpid(pid,&status,0) == -1){
        printf("wait for child process error!\n");
    }


	loop: ;

}


int find_command(char *command)
{
	DIR *dp;
	struct dirent*  dirp;
	char *path[]={"./","/bin","/usr/bin",NULL};//在一下路径下，找可执行程序

	if(strncmp(command,"./",2) == 0)
		command =command + 2;
	int i=0;
	while(path[i]!=NULL){
		if((dp = opendir(path[i])) == NULL){//打开目录
			printf("can not open /bin\n");
		}
		while((dirp = readdir(dp)) != NULL){       //比较是否有相同的文件
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



