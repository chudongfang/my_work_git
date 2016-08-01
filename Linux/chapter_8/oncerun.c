/*************************************************************************
	> File Name: oncerun.c
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年08月01日 星期一 15时07分11秒
 ************************************************************************/
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
pthread_once_t once = PTHREAD_ONCE_INIT;
void run(void){
    printf("Function run is running !\n");
}

void thread1(void *arg){
    pthread_t thid = pthread_self();
    printf("Current progreming ID is %u\n",thid);
    pthread_once(&once,run);
    printf("thread ends\n");
}

void thread2(void *arg){
    pthread_t thid = pthread_self();
    printf("Current programing ID is %u\n",pthread_self());
    pthread_once(&once ,run);
    printf("ends\n");
}



int main(int argc,char *argv[])
{
    pthread_t thid1,thid2;
    pthread_create(&thid1,NULL,thread1,NULL);
    pthread_create(&thid2,NULL,thread2,NULL);
    sleep(3);
    printf("main end\n");
    exit(0);
}
