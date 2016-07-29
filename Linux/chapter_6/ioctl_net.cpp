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
#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
typedef long long ll;
unsigned char g_eth_name[16];
unsigned char g_macaddr[6];
unsigned char g_subnetmask;
unsigned char g_ipaddr;
unsigned char g_broadcast_ipaddr;


void init_net(void)
{
	int i;
	int sock;
	struct sockddr_in sin;
	struct ifreq ifr;


	sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock == -1){
		perror("socket");
	}

	strcpy(g_eth_name,"eth0");
	strcpy(ifr.ifr_name,g_eth_name);
	printf("eth name\t%s\n", g_eth_name);

	if(ioctl(sock,SIOCGIFHWADDR,&ifr)<0){
		perror("ioctl");
	}

	memcpy(g_macaddr,ifr.ifr_hwaddr.sa_data,6);

	printf("local mac:\t");
	for(int i=0;i<5;i++){
		printf("%.2x:", g_macaddr[i]);
	}
	printf("%.2x\n", g_macaddr[i]);

	if(ioctl(sock,SIOCGIFADDR,&ifr)<0){
		perror("ioctl");
	}

	memcpy(&sin,&ifr.ifr_addr, sizeof(sin));
	g_ipaddr = sin.sin_addr.s_addr;
	printf("local eth0:\t%s\n", inet_ntoa(sin,sin_addr));

	if(ioctl(sock,SIOCGIFBRDADDR,&ifr)<0){
		perror("ioctl");
	}

	memcpy(&sin,&ifr.ifr_addr,sizeof(sin));
	g_subnetmask = sin.sin_addr.s_addr;
	printf("subnetmask:\t%s\n", inet_ntoa(sin.sin_addr));

	close(sock);

}



int main(int argc,char *argv[])
{
	init_net();
    return 0;
}
