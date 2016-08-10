#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

void dispaly(const char *string)
{
	int seconds;
	seconds = time((time_t *)NULL);
	printf("%s,%d\n", string,seconds);
}

int main(int argc, char const *argv[])
{
	fd_set readfds;
	struct timeval timeout ;
	int ret;

	FD_ZERO(&readfds);
	FD_SET (0,&readfds);

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	while(1){
		dispaly("before select");

        
        dispaly("after select");
		switch(ret)
		{
			case 0:
				printf("No data in ten seconds\n");
				//exit(0);
				break;
			case -1:
				printf("select\n");
				exit(0);
				break;
			default :
			getchar();
			printf("Data is available now.\n");
		}
 
	}
	return 0;
}

