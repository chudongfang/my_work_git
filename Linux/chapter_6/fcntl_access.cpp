/*************************************************************************
	> File Name: file_read.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月18日 星期一 09时24分34秒
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
	int ret;
	int access_mode;
	int fd;

	if((fd = open("test1.cpp",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU)) == -1){
		my_err("open",__LINE__);
	}

	if((ret = fcntl(fd, F_SETFL, O_APPEND))<0){//设置最加
		my_err("fcntl",__LINE__);
	}

	printf("O_APPEND=%d\n",O_APPEND );
	
	printf("只读O_RDONLY=%d\n",O_RDONLY );
	printf("只写O_WRONLY=%d\n",O_WRONLY );
	printf("读写O_RDWR=%d\n",O_RDWR );

	if((ret = fcntl(fd,F_GETFL,0))<0){//得到其打开状态
		my_err("fcntl",__LINE__);
	}

	printf("ret=%d\n",ret);
	access_mode=ret & 3;//得到其文件打开方式，取其后两位
	printf("%d\n", access_mode);
	if(access_mode == O_RDONLY){
		printf("example_1 access mode:read only\n");
	}else if(access_mode == O_WRONLY){
		printf("example_1 access mode:write only\n");
	}else if(access_mode == O_RDWR){
		printf("example_1 access mode:read + write\n");
	}

	printf("ret=%d\n",ret);

	if(ret & O_APPEND){//判断其是否是追加
		printf(" ,append\n");
	}
	printf("O_APPEND=%d\n",O_APPEND);
	if(ret & O_NONBLOCK){//判断是否锁定
		printf(" ,nonblock\n");
	}
	printf("O_NONBLOCK=%d\n", O_NONBLOCK);

	if(ret & O_SYNC){
		printf(", sync\n");
	}
	printf("O_SYNC=%d\n", O_SYNC);
	
	printf("\n");



    return 0;
}
