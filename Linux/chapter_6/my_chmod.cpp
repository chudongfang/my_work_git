/*************************************************************************
	> File Name: my_chmod.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月18日 星期一 08时20分54秒
 ************************************************************************/
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    int mode;
    int mode_u;//文件所属用户权限       user
    int mode_g;//与文件所有者同组的权限 group
    int mode_o;//其他用户操作权限       other
    char *path;
     
    if(argc<3){                      //如果用户输入不到3个参数，提醒用户输入相应参数
        printf("%s <mode num> <target file>\n",argv[0]);
        exit(0);
    }
    
    mode=atoi(argv[1]);//把字符转换成整形

    if(mode>777||mode<0){    //如果mode超界，返回错误
        printf("mode num error!");
        exit(0);
    }

    mode_u = mode/100;    //拆分
    mode_g = mode%100/10;
    mode_o = mode%10;
    mode   = mode_u*8*8+mode_g*8+mode_o; //把八进制转化为对应的十进制
    path   = argv[2];                    //路径赋值
    
    if( chmod(path,mode) == -1){      //调用系统函数
        perror("chmod error");
        exit(1);
    }

    return 0;
}
