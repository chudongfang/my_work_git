/*************************************************************************
	> File Name: createthread.c
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年08月01日 星期一 14时46分15秒
 ************************************************************************/

#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
int *thread(void *arg)
{
    pthread_t newhid;
    newhid = pthread_self();
    printf("this is a new thread ,thread ID = %u\n",newhid);
    return NULL;
}

int main(int argc,char *argv[])
{
    pthread_t thid;
    printf("main thread ,ID is %u\n",pthread_self());
    if(pthread_create(&thid,NULL,(void *)thread,NULL) != 0){
        perror("pthread_create");
        exit(1);
    }
    sleep(1);
    exit(0);
}
