#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define BUFFER_SIZE 1024


#define MaxValue 10000//权值最大值 
#define MaxBit 55//最大编码位数 
#define MaxN 55//最大结点个数 

//准备工作
int sockfd;//
char* IP = "127.0.0.1";//该地址请写测试服务器的IP，本机IP,回送地
short PORT = 10222;//端口号
typedef struct sockaddr SA;//类型转换
char message[100];
char demessge[100];






void init()
{
    printf("客户端开始启动\n");
    sockfd = socket(AF_INET,SOCK_STREAM,0);//启动socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if(connect(sockfd,(SA*)&addr,sizeof(addr))==-1){//向服务器请求建立链接
        perror("无法连接到服务器");
        printf("客户端启动失败\n");
        exit(-1);
    }
    printf("客户端启动成功\n");
}
//开始通信

void code(int * count)
{
    int t;
    printf("\nplease input message:");
    scanf("%s",message);
    send(sockfd,message,sizeof(message),0);// 把文件大小数据发送
    recv(sockfd,count,sizeof(int),0);//得到编码后的结果大小
    printf("\nthe result:\n");
    for(int i=0;i<*count;i++)
    {
        recv(sockfd,&t,sizeof(int),0);//一个一个接受，并输出
        if(t == 2) 
            printf(" ");
        else
            printf("%d",t);
    }  
    getchar();
    getchar();
}

void decode()
{
    char result[100];
    printf("\nplease input the message you want tanslate:");
    scanf("%s",demessge);//得到要译的码
    send(sockfd,demessge,sizeof(demessge),0);//发送到服务端
    recv(sockfd,result,sizeof(result),0);//从服务端接受到译码后的结果
    printf("\nthe result:\n");
    printf("%s",result);
    getchar();
    getchar();
}








void start()
{
    int count=0;
    char result[100];
    int choice=1;
    int judge;
   //菜单
    do{
        judge=1;
        printf("\n\t********************************************\n");
        printf("\t**            1.字符压缩                    **\n");
        printf("\t**            2.字符解压                    **\n");
        printf("\t**            0.退出                        **\n");
        printf("\t********************************************\n");
        printf("\t请选择:");
        scanf("%d",&choice);
        
        if(choice == 1||choice == 0||choice == 2)
            send(sockfd,&choice,sizeof(int),0);//发送用户选择信息
        else  
            continue;
        
        
        recv(sockfd,&judge,sizeof(int),0);//接受判断标准，判断其是否为错误
        


        if(judge==1) {//如果错误，提示先用1选项建树
            printf("there is no haffe tree! \nplease set up the tree first!\n");
            continue;
        }
        

        switch(choice)
        {
            case 1 :
                code(&count);//编码
                break;
            case 2 :
                decode();//译码
                break;
            default:
                break;
        }
    }while(choice != 0);



    close(sockfd);//关闭socket
}




void sig_close(int i)
{
    //关闭客户端的描述符
    close(sockfd);
    exit(0);
}


int main()
{
    int type;
    signal(SIGINT,sig_close);//关闭CTRL+C
    init();//启动并连接服务器
    start();//开始与服务器进行通信。
    return 0;
}
