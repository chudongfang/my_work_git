/*************************************************************************
	> File Name: show_file.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月19日 星期二 08时23分04秒
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
using namespace std;
typedef long long ll;

int my_readir(const char *path)
{
	DIR *dir;
	struct dirent *ptr;
	if((dir = opendir(path)) == NULL){
		perror("opendir");
		return -1;
	}

	while((ptr = readdir(dir)) != NULL){
		printf("file name :%s\n",ptr->d_name);
	}

	closedir(dir);

	return 0;
}


int main(int argc,char *argv[])
{

	if(argc < 2){
		printf("listfile <target path>\n");
		exit(1);
	}

	if(my_readir(argv[1]) < 0){
		exit(1);
	}
	
    return 0;
}
