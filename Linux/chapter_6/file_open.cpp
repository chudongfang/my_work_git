/*************************************************************************
	> File Name: file_open.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月18日 星期一 09时24分41秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<algorithm>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
int main(int argc,char *argv[])
{
    int fd;
    
    //if((fd=open("test.c",O_CREAT|O_EXCL, S_IRUSR|S_IWUSR)) == -1){
    if((fd=creat("test.c",S_IRWXO)) == -1){       //Creat函数会直接把其覆盖
        //perror("open");
        printf("open:%s with errno :%d\n",strerror(errno),errno);
        exit(1);
    }
    else{
        printf("Creat File success\n");
    }
    close(fd);
    return 0;
}
