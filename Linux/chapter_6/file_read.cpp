/*************************************************************************
	> File Name: file_read.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月18日 星期一 09时24分34秒
 ************************************************************************/

#include<sys/types.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
typedef long long ll;

void my_err(const char * err_string,int line)
{
    fprintf(stderr,"line %d:\t",line);//错误输出，输出到终端
    perror(err_string);               //错误函数提示
    exit(1);
}

int my_read(int fd)
{
    int len;
    int ret;
    int i;
    char read_buf[64];

    if(lseek(fd,0,SEEK_END) == -1){
        my_err("lseek",__LINE__);
    }
    
    if((len = lseek(fd,0,SEEK_CUR)) == -1){//利用lseek计算文件大小
        my_err("lseek",__LINE__);
    }
    
    if(lseek(fd,0,SEEK_SET) == -1){
        my_err("lseek",__LINE__);
    }
    
    printf("len:%d\n",len);
    
    if((ret = read(fd,read_buf,len))<0){  //读入数据
        my_err("read",__LINE__);
    }

    for(int i=0;i<len;i++)                //输出数据
        printf("%c",read_buf[i]);
    printf("\n");

    return ret;
}




int main(int argc,char *argv[])
{
    int fd;
    char write_buf[32]="Hellow my vacation!";
    
    if((fd=open("test.cpp",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU)) ==-1) {//可写可读打开
        my_err("open",__LINE__);
    }else{
        printf("creat file success!");
    } 
    
    if(write(fd,write_buf,strlen(write_buf)) != strlen(write_buf)){//写入
        my_err("write",__LINE__);
    }
    
    my_read(fd);//读文件

    printf("_____________________________\n");
    if(lseek(fd,20,SEEK_END) == -1){    //中间隔了20字节的数据
        my_err("lseek",__LINE__);
    }
    if(write(fd,write_buf,strlen(write_buf)) != strlen(write_buf)){//写入数据 
        my_err("write",__LINE__);
    }

    my_read(fd);
    
    close(fd);          //关闭文件 

    return 0;
}
