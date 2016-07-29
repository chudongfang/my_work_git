/*************************************************************************
	> File Name: set_file.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年07月18日 星期一 19时54分28秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
typedef long long ll;
int main(int argc,char *argv[])
{
	umask(0);
	if(creat("test3.cpp",S_IRWXU|S_IRWXG|S_IRWXO) < 0){
		perror("creat");
		exit(1);
	}
	getchar();
	getchar();
	umask(S_IRWXO);//屏蔽其他用户权限
	if(creat("test4.cpp",S_IRWXU|S_IRWXG|S_IRWXO) < 0){
		perror("creat");
		exit(1);
	}
    
    truncate("test4.cpp",100);//修改大小

    if(chown("test3.cpp",123,123) < 0){//更改组
        perror("chown");
    }
    return 0;
}
