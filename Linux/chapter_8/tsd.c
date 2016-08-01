/*************************************************************************
	> File Name: tsd.c
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年08月01日 星期一 16时11分15秒
 ************************************************************************/
#include<string.h>
#include<pthread.h>
#include<stdio.h>

pthread_key_t key;

void * thread2(void *arg)
{
    int tsd = 5;
    printf("thread %u is running \n",pthread_self());
    
    pthread_setspecific(key,(void *) tsd);
    printf("thread %u return %d\n",pthread_self(),pthread_getspecific(key));
}

void * thread1(void *arg)
{
    int tsd = 0;
    int status;
    pthread_t thid2;
    
    printf("thread %u is runnig\n",pthread_self());
    pthread_setspecific(key,(void *)tsd);
    pthread_create(&thid2,NULL,thread2,NULL);
    pthread_join(thid2,&status);

    printf("thread %u return %d\n",pthread_self(),pthread_getspecific(key));
}

int main(int argc,char *argv[])
{
    pthread_t thid1;
    int status;
    printf("main thread begins running \n");
    pthread_key_create(&key ,NULL);
    pthread_create(&thid1,NULL,thread1,NULL);
    pthread_join(thid1,&status);
    printf("main exit\n");
    return 0;
}
