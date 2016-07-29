/*************************************************************************
	> File Name: my_cdvc.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月19日 星期二 08时08分11秒
 ************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
typedef long long ll;

void my_err(const char * err_string,int line)
{
    fprintf(stderr,"line %d:\t",line);//错误输出，输出到终端
    perror(err_string);               //错误函数提示
    exit(1);
}

int main(int argc,char *argv[])
{

	char buf[PATH_MAX+1];
	if(argc < 2){
		printf("my_cd <target path>\n");
		exit(1);
	}

	if(chdir(argv[1]) < 0){
		my_err("chdir",__LINE__);
	}

	if(getcwd(buf,512) < 0){
		my_err("getcwd",__LINE__);
	}
	printf("%s\n", buf);
    return 0;
}
