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

/*struct flock
{
	short_l_type;//锁的类型
	short_l_whence;//偏移量的起始位置
	short_l_start;//偏移量
	short_l_len;  //要锁的字节数
	short_l_pid;   //锁的属主进程ID
};
*/

int lock_set(int fd,struct flock* lock)
{
	if(fcntl(fd,F_SETLK,lock) == 0)
	{
		if(lock->l_type == F_RDLCK){
			printf("set read lock ,pid:%d\n", getpid());
		}else if(lock->l_type == F_WRLCK){
			printf("set write lock,pid:%d\n", getpid());
		}else if(lock->l_type == F_UNLCK){
			printf("release lock,pid:%d\n", getpid());
		}
	}
	else 
	{
		perror("lock operation fail\n");
		return -1;
	}
}


int lock_test(int fd,struct flock * lock)
{
	if(fcntl(fd,F_GETLK,lock) == 0)
	{
		if(lock->l_type == F_UNLCK)//可以上锁
		{
			printf("lock can be set in fd\n");
			return 0;
		}
		else                      //不能上锁
		{
			if(lock->l_type == F_RDLCK)//输出其已锁的类型
				printf("can`t set lock, read lock has been set by:%d\n", lock->l_pid);
			else if(lock->l_type == F_WRLCK) 
				printf("can`t set lock, write lock has been set by:%d\n", lock->l_pid);
			return -2;
		}
	}
	else
	{
		perror("get incompatible locks fail");
		return -1;
	}
}




int main(int argc,char *argv[])
{
	int 		  fd;
	int           ret;
	struct flock  lock;
	char          read_buf[32];

/**************************打开文件，并写文件******************************/
	if((fd = open("test2.cpp",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU)) == -1){
		my_err("open",__LINE__);
	}
	if(write(fd,"test lock",10) != 10){
		my_err("write",__LINE__);
	}


 /***********************初始化结构体******************************/   
    memset(&lock,0,sizeof(struct flock));
	lock.l_start  = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len    = 0;



/*******************上读锁****************************************/
	lock.l_type = F_RDLCK;
	if(lock_test(fd,&lock) == 0){
		lock.l_type = F_RDLCK;
		lock_set(fd,&lock);
	}


	lseek(fd,0,SEEK_SET);
	if((ret = read(fd,read_buf,10))<0){
		my_err("read",__LINE__);
	}
	read_buf[ret]='\0';
	printf("%s\n", read_buf);

	getchar();

/*****************上写锁*****************************/
	lock.l_type = F_WRLCK;
	if(lock_test(fd,&lock) == 0){
		lock.l_type = F_WRLCK;
		lock_set(fd,&lock);
	}


/**********************取消锁****************************/    
	lock.l_type = F_UNLCK;
	lock_set(fd,&lock);
	close(fd);
    return 0;
}







