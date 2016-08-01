/*************************************************************************
	> File Name: condition.c
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年08月01日 星期一 17时02分36秒
 ************************************************************************/
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
pthread_mutex_t  mutex;//锁
pthread_cond_t cond;    //条件变量

void *thread1(void  *arg){
    pthread_cleanup_push(pthread_mutex_unlock,&mutex);
    
    while(1){
        printf("thread1 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread1 applied the condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(4);
    }
    pthread_cleanup_pop(&mutex);
}

void *thread2(void *arg)
{

    while(1){
        printf("thread2 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        printf("thread2 applied the condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main(int argc,char *argv[])
{
    pthread_t tid1,tid2;
    printf("condition variable study\n");
    pthread_mutex_init(&mutex , NULL);
    pthread_cond_init(&cond,NULL);
    pthread_create (&tid1,NULL,(void *)thread1,NULL);
    pthread_create (&tid2,NULL,(void *)thread2,NULL);
    
    do{
        pthread_cond_signal(&cond);
    }while(1);
    
    sleep(50);
    exit(0);
}
