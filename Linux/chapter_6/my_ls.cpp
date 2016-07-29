/*************************************************************************
	> File Name: my_ls.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月19日 星期二 09时35分44秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>

#define PARAM_NONE 0
#define PARAM_A    1
#define PARAM_L    2
#define PARAM_I    4
#define PARAM_R    8
#define PARAM_T    16
#define PARAM_RR   32
#define MAXROWLEN  80

int g_leave_len = MAXROWLEN;
int g_maxlen;

/******************错误分析********************/
void my_err(const char *err_string,int line)
{
	fprintf(stderr, "line:%d ", line);
	perror(err_string);
	exit(1);
}


/*********************************************
 * 功能 ：输出文件信息  参数  -l
 * 参数1  文件信息
 * 参数2  文件名
 * 参数3  文件显示颜色
 *********************************************/
void display_attribute(struct stat buf,char * name,int filecolor)
{
    char colorname[NAME_MAX + 30];
	char buf_time[32];
	struct passwd *psd;
	struct group  *grp;
/****************文件类型**********************/
	if(S_ISLNK(buf.st_mode)){
		printf("l");
	} else if(S_ISREG(buf.st_mode)){
		printf("-");
	} else if(S_ISDIR(buf.st_mode)){
		printf("d");
	} else if(S_ISCHR(buf.st_mode)){
		printf("c");
	} else if(S_ISBLK(buf.st_mode)){
		printf("b");
	} else if(S_ISFIFO(buf.st_mode)){
		printf("f");
	} else if(S_ISSOCK(buf.st_mode)){
		printf("s");
	}


/********************文件权限**************************/ 

    //拥有者权限
    if(buf.st_mode & S_IRUSR)   
		printf("r");
	else 
		printf("-");
	if(buf.st_mode & S_IWUSR)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXUSR){
		printf("x");
    }
	else 
		printf("-");
	
    //组权限	
	if(buf.st_mode & S_IRGRP)
		printf("r");
	else 
		printf("-");

    if(buf.st_mode & S_IWGRP)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXGRP){
		printf("x");

    }
	else 
		printf("-");
	

    //其他用户权限
	if(buf.st_mode & S_IROTH)
		printf("r");
	else 
		printf("-");
	
    if(buf.st_mode & S_IWOTH)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXOTH){
		printf("x");
    }
	else 
		printf("-");
	

    printf("\t");	
    //通过用户和组id得到用户的信息和其所在组的信息
	psd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);

	printf("%4d ",buf.st_nlink);    //打印文件的硬链接数
	printf("%-8s",psd->pw_name);    //打印用户的名字
	printf("%-8s", grp->gr_name);   //打印用户组的名字

	printf("%6d", buf.st_size);     //打印文件大小
	strcpy(buf_time,ctime(&buf.st_mtime));//把时间转换成普通表示格式

	buf_time[strlen(buf_time)-1]='\0';    //去掉换行符
	printf("  %s", buf_time);//输出时间 
    sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
	printf(" %-s\n", colorname);
}





/*******************************
 * 功能 ：输出文件名
 * 参数1 文件名
 * 参数2 文件显示参数
 * *****************************/

void display_single(char *name,int filecolor)
{
	char colorname[NAME_MAX + 30];
	int i,len;
    //剩余空间不够，就进行换行 
	if(g_leave_len < g_maxlen){
		printf("\n");
		g_leave_len=MAXROWLEN;
	}
    //名字的长度
	len = strlen(name);
	len = g_maxlen - len;

	sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
	printf(" %-s", colorname);
	//输出若个个空格，补够一个单位
    for(i=0;i<len;i++)
		printf(" ");
	printf(" ");
    //更新剩余空间
	g_leave_len -= (g_maxlen+2);
}


/********************************************
 * 功能：输出带有i_node的文件信息
 * 参数1 文件信息
 * 参数2 文件名
 * 参数3 显示颜色
 * *****************************************/
void display_st_ino(struct stat buf,char *name,int filecolor)
{
	char colorname[NAME_MAX + 30];
	int i,len;
    //剩余空间不够，就进行换行 
	if(g_leave_len < g_maxlen+8){
		printf("\n");
		g_leave_len=MAXROWLEN;
	}

	printf("%d ", buf.st_ino);
    //名字的长度
	len = strlen(name);
	len = g_maxlen - len;

	sprintf(colorname,"\033[%dm%s\033[0m",filecolor,name);
	printf(" %-s", colorname);
	//输出若个个空格，补够一个单位
    for(i=0;i<len;i++)
		printf(" ");
	printf(" ");
    //更新剩余空间
	g_leave_len -= (g_maxlen+2+8);
}




/**********************************************
 * 功能：根据参数及路径输出单个文件信息
 * 参数1 参数信息
 * 参数2 单个文件路径
 * ********************************************/
void display(int flag,char * pathname)
{
    int filecolor=37;
	int    i,j;
	struct stat  buf;//文件信息buf
	char   name[NAME_MAX + 1];
	for(i=0,j=0; i<strlen(pathname);i++)
	{
		if(pathname[i] == '/')
		{
			j=0;
			continue;
		}
		name[j++]=pathname[i];
	}
	name[j]='\0';


	if(lstat(pathname,&buf) == -1){
		my_err("stat",__LINE__);
	}

	if(S_ISLNK(buf.st_mode)){

	} else if(S_ISREG(buf.st_mode)){

	} else if(S_ISDIR(buf.st_mode)){
        filecolor=34;
	} else if(S_ISCHR(buf.st_mode)){
        filecolor=33;
	} else if(S_ISBLK(buf.st_mode)){
        filecolor=33;
	} else if(S_ISFIFO(buf.st_mode)){

	} else if(S_ISSOCK(buf.st_mode)){

	}

    if(filecolor == 37&&
    		( (buf.st_mode & S_IXUSR)||
    		  (buf.st_mode & S_IXGRP)||
    		  (buf.st_mode & S_IXOTH)   )  ){
    	filecolor=32;
    }
    	



/********************文件权限**************************/ 

    
	switch(flag)
	{
		case PARAM_NONE:
			if(name[0]!='.'){
				display_single(name,filecolor);
			}
			break;

		case PARAM_I:
			if(name[0]!='.'){
				display_st_ino(buf,name,filecolor);
			}
			break;


		case PARAM_A:
			display_single(name,filecolor);
			break;

		case PARAM_L:
			if(name[0] != '.'){
				display_attribute(buf, name,filecolor);
			}
			break;

		case PARAM_A+PARAM_L:
			display_attribute(buf,name,filecolor);
			break;

		case PARAM_A+PARAM_I:
			display_st_ino(buf,name,filecolor);
			break;

		case PARAM_L+PARAM_I:
			if(name[0] != '.'){
				printf("%d ",buf.st_ino);
				display_attribute(buf, name,filecolor);
			}
			break;

		case PARAM_A+PARAM_L+PARAM_I:
			{
				printf("%d ",buf.st_ino);
				display_attribute(buf,name,filecolor);
			}
			break;
				
		default:
			break;
	}
}



/****************************************
* 功能：输出目录信息
* 参数1 参数信息
* 参数2 目录文件路径
* *************************************/
void display_dir(int flag_param,char *path)
{
	DIR   *dir;
	long   t;
	int flag_param_temp;
	struct stat buf;
	struct dirent  *ptr;
	int    count = 0;
	char   filename[256][PATH_MAX+1],temp[PATH_MAX+10];
	long   filetime[256][1];
	flag_param_temp=flag_param;
	dir = opendir(path);

	if(dir == NULL){
		my_err("opendir",__LINE__);
	}
	//解析文件个数，及文件名的最长值
    while((ptr = readdir(dir))!= NULL){
		if(g_maxlen < strlen(ptr->d_name))
			g_maxlen = strlen(ptr->d_name);
		count++;
	}
	
    closedir(dir);

	if(count>256)
        printf("%d :too many files under this dir",__LINE__);
	
    

    int i,j,len = strlen(path);
	dir = opendir(path);
    //得到该目录下的所有文件的路径	
    for(i=0;i < count; i++)
    {
		ptr = readdir(dir);
		if(ptr == NULL){
			my_err("opendir",__LINE__);
		}

		strncpy(filename[i],path,len);
		filename[i][len] = '\0';
		strcat(filename[i],ptr->d_name);
		filename[i][len+strlen(ptr->d_name)] = '\0';
    }

	closedir(dir);



    //插入排序

    if(flag_param & PARAM_T)//根据时间排序
    {
    	for(i=0;i<count;i++)
    	{
    		stat(filename[i],&buf);
    		filetime[i][0]=buf.st_mtime;
    	}

    	for(i=0;i<count;i++)
		{
			for(j=i;j<count;j++)
			{
				if(filetime[i][0]<filetime[j][0])
				{
					t 		      =filetime[i][0];
					filetime[i][0]=filetime[j][0];
					filetime[j][0]=t;
					strcpy(temp,filename[i]);
					strcpy(filename[i],filename[j]);
					strcpy(filename[j],temp);
				}
			}
		}
		flag_param -= PARAM_T;
    }
    else//根据名字排序
    {
    	for(i=0;i<count;i++)
		{
			for(j=i;j<count;j++)
			{
				if(strcmp(filename[i],filename[j])>0)
				{
					strcpy(temp,filename[i]);
					strcpy(filename[i],filename[j]);
					strcpy(filename[j],temp);
				}
			}
		}
    }
	

    //输出文件

    if(flag_param & PARAM_R)//逆序
    {
    	flag_param -= PARAM_R;
    	if(flag_param & PARAM_RR)//递归输出
		{
			flag_param -= PARAM_RR;
    		for (i = count-1; i >= 0; i--)
			{
				stat(filename[i],&buf);
				if(S_ISDIR(buf.st_mode))
				{

					len=strlen(filename[i]);
					if(filename[i][len-1]=='.' && filename[i][len-2]=='/'
						||filename[i][len-1] == '.' && filename[i][len-2]=='.'&&filename[i][len-3]=='/') 
						continue;
					
					printf("\n\n%s :\n",filename[i]);
					len=strlen(filename[i]);
					filename[i][len]='/';
					filename[i][len+1]='\0';		
					display_dir(flag_param_temp,filename[i]);
				}
				else
					display(flag_param,filename[i]);
			}
		}
		else
		{
			for (i = count-1; i >= 0; i--)
			{
				display(flag_param,filename[i]);
			}

		}
    }
    else
    {
    	if(flag_param & PARAM_RR)
		{
			flag_param -= PARAM_RR;
    		for (i = 0; i < count; i++)
			{
				stat(filename[i],&buf);
				if(S_ISDIR(buf.st_mode))
				{
					len=strlen(filename[i]);
					if(filename[i][len-1]=='.' && filename[i][len-2]=='/'
						||filename[i][len-1] == '.' && filename[i][len-2]=='.'&&filename[i][len-3]=='/') 
						continue;
					
					printf("\n\n%s :\n",filename[i]);
					len=strlen(filename[i]);
					filename[i][len]='/';
					filename[i][len+1]='\0';		
					display_dir(flag_param_temp,filename[i]);
				}
				else
					display(flag_param,filename[i]);
			}
		}
		else
		{
			for (i = 0; i < count; i++)
			{
				display(flag_param,filename[i]);
			}

		}
    }
	if((flag_param & PARAM_L) == 0) 
		printf("\n");
}






int main(int argc, char const *argv[])
{
	int i,j,k,num;
	char path[PATH_MAX+1];
	char param[32];
	int  flag_param = PARAM_NONE;
	struct stat  buf;

	j=0;
	num=0;
    //解析其参数
	for(i=1; i<argc;i++)
	{
		if(argv[i][0] == '-')
		{
			for(k=1;k<strlen(argv[i]);k++,j++)
			{
				param[j] = argv[i][k];
			}
			num++;
		}
	}
    
    
    //利用位运算确定具体参数
	for(i=0;i<j;i++)
    {
		if(param[i] == 'a'){
			flag_param |=PARAM_A;//利用位运算
			continue;
        }
        else if(param[i] == 'l'){
			flag_param |= PARAM_L;
			continue;
		} 
		else if(param[i] == 'i'){
			flag_param |= PARAM_I;
			continue;
		} 
		else if(param[i] == 'r'){
			flag_param |= PARAM_R;
			continue;
		} 
		else if(param[i] == 't'){
			flag_param |= PARAM_T;
			continue;
		}else if(param[i] == 'R'){
			flag_param |= PARAM_RR;
			continue;
		}    
		else{
			printf("my_ls: incalid option -%c\n", param[i]);
			exit(1);
		}
	}
    
	param[j] = '\0';
    //默认为当前路径
	if((num+1) == argc)
    {
		strcpy(path,"./");
		path[2] = '\0';
		display_dir(flag_param,path);
		return 0;
	}
    
	i =1;
	do{	
        if(argv[i][0] == '-')
        {
			i++;
			continue;
		} 
        else 
        {
            //得到具体路径
			strcpy(path, argv[i]);
			if(stat(path,&buf) == -1)
				my_err("stat",__LINE__);

	    	//判断是否为目录文件	
            if(S_ISDIR(buf.st_mode))
			{
                //如果目录最后忘记了/则加上
				if(path[strlen(argv[i]) - 1] != '/')
				{
					path[strlen(argv[i])]  = '/';
					path[strlen(argv[i])+1] = '\0';
				}
				else 
					path[strlen(argv[i])] = '\0';
			
                display_dir(flag_param,path);//按照目录输出
				i++;
			}
            else
			{
                //按照文件输出
				display(flag_param,path);
				i++;
			}
		}
	}while (i < argc);
    return 0;
}






