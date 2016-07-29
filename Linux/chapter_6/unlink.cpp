/*************************************************************************
	> File Name: unlink.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月18日 星期一 21时08分37秒
 ************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
typedef long long ll;

void my_err(const char * err_string,int line)
{
    fprintf(stderr,"line %d:\t",line);//错误输出，输出到终端
    perror(err_string);               //错误函数提示
    exit(1);
}

int main(int argc,char *argv[])
{
	int fd;
	char buf[32];

	if((fd = open("temp5.cpp",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU)) < 0){
		my_err("open",__LINE__);
	}

	if(unlink("temp5.cpp") < 0){
		my_err("unlink",__LINE__);
	}

	printf("file unlinked\n");

	if(write(fd,"temp5.cpp",5) < 0){
		my_err("write",__LINE__);
	}
    getchar();

    return 0;
}
