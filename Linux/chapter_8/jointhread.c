/*************************************************************************
	> File Name: jointhread.c
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年08月01日 星期一 15时38分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


void assisthread()
{
    printf("I am a helper\n");
    sleep(3);
    pthread_exit(110);

}

int main(int argc,char *argv[])
{
    pthread_t assistthid;
    int status;
    pthread_create(&assistthid,NULL ,(void *)assisthread,NULL);
    pthread_join(assistthid,(void *)&status);
    printf("assisthread's exit is caused %d\n",status);
    return 0;
}
