#include <gtk/gtk.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define LOGIN      1
#define REGISTER   2
#define FRIEND_SEE 3
#define FRIEND_ADD 4
#define FRIEND_DEL 5
#define CHAT_ONE   6
#define CHAT_MANY  7
#define SEND_FILE  8
#define EXIT      -1

#define MAX_CHAR  1024


/**************************************************/
typedef struct stat{
    int friend_see;
    int friend_add;
    int friend_del;
    int chat_one;
    int chat_many;
    int send_file;
}STAT;

/******************be sure same with server*******************************/
typedef struct datas{
    char send_name[MAX_CHAR];
    char recv_name[MAX_CHAR];
    int send_fd;
    int recv_fd;
    time_t time;
    char mes[MAX_CHAR*2];
}DATA;

typedef struct package{
    int type;
    DATA  data;
}PACK;



/***********************function***********************/
void my_err(const char * err_string,int line);
void init();

void sig_close();

int send_login(char username_t [],char password_t []);
int login();
int login_menu();
int send_registe(char username_t[],char password_t[]);
void registe();
int main_menu();


int sockfd;
char *IP = "127.0.0.1";
short PORT = 10222;
typedef struct sockaddr SA;


STAT m_status;


/************************客户端缓冲区**********************/
/*PACK m_pack_send_friend_see [MAX_CHAR];
PACK m_pack_send_chat_one   [MAX_CHAR];
PACK m_pack_send_chat_many  [MAX_CHAR];
PACK m_pack_send_send_file  [MAX_CHAR];


int m_send_num_friend_see;
int m_send_num_chat_one;
int m_send_num_chat_many;
int n_send_num_send_file;*/
/*****************send*********************/
PACK m_pack_send   [MAX_CHAR];
int  m_send_num;


/*****************recv*********************/
PACK m_pack_recv_login      [MAX_CHAR];
PACK m_pack_recv_friend_see [MAX_CHAR];
PACK m_pack_recv_chat_one   [MAX_CHAR];
PACK m_pack_recv_chat_many  [MAX_CHAR];
PACK m_pack_recv_send_file  [MAX_CHAR];

int m_recv_num_login;
int m_recv_num_friend_see;
int m_recv_num_chat_one;
int m_recv_num_chat_many;
int m_recv_num_send_file;





void my_err(const char * err_string,int line)
{
	fprintf(stderr, "line:%d  ", line);
	perror(err_string);
	exit(1);
}

void init()
{
	printf("客户端开始启动\n");
    sockfd = socket(AF_INET,SOCK_STREAM,0);//启动socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if(connect(sockfd,(SA*)&addr,sizeof(addr))==-1){
        perror("无法连接到服务器");
        printf("客户端启动失败\n");
        exit(-1);
    }
    printf("客户端启动成功\n");
}



void sig_close(int i)
{
    //关闭客户端的描述符
    close(sockfd);
    exit(0);
}




int send_login(char username_t[],char password_t[])
{
    PACK send_login_t;
    PACK recv_login_t;
    int login_judge_flag = 0;
    send_login_t.type = LOGIN;
    strcpy(send_login_t.data.send_name,username_t);
    strcpy(send_login_t.data.recv_name,"server");
    strcpy(send_login_t.data.mes,password_t); 

    if(send(sockfd,&send_login_t,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }
    if(recv(sockfd,&recv_login_t,sizeof(PACK),0) < 0){
        my_err("recv",__LINE__);
    }
    login_judge_flag = recv_login_t.data.mes[0] - 48;
    return login_judge_flag;
}


int login()
{
    int flag = 0;
    
    int login_flag = 0;
    char username_t [MAX_CHAR];
    char password_t [MAX_CHAR];
    printf("please input the username:\n");
    scanf("%s",username_t);
    printf("please input the password\n");
    scanf("%s",password_t);

    login_flag = send_login(username_t,password_t);
    if(login_flag ==  2){
        printf("the username is not exit.\n");
        return 0;
    }   
    if(login_flag == 0) {
        printf("the password is not crrect.\n");
        return 0;
    }
    printf("load successfully!\n");
    return 1;
}


int login_menu()
{
    int chioce;
    do
    {
        printf("\n\t\t*******************************\n");
        printf("\t\t*        1.login in           *\n");
        printf("\t\t*        2.register           *\n");
        printf("\t\t*        0.exit              *\n");
        printf("\t\t*******************************\n");
        printf("\t\tchoice：");
        scanf("%d",&chioce);
        switch(chioce)
        {  
            case 1:
                login();
                break;
            case 2:
                registe();
                break;
            default:
                break;
        }
    }while(chioce!=0);
    return 0;
}



int send_registe(char username_t[],char password_t[])
{
    PACK send_registe_t;
    
    send_registe_t.type = REGISTER;
    strcpy(send_registe_t.data.send_name,username_t);
    strcpy(send_registe_t.data.recv_name,"server");
    strcpy(send_registe_t.data.mes,password_t); 
    if(send(sockfd,&send_registe_t,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }
    return 1;
}

void registe()
{
    int flag = 0;
    flag = REGISTER;
    char username_t[MAX_CHAR];
    char password_t[MAX_CHAR];

   
    
    printf("please input the username you want register:\n");
    scanf("%s",username_t);
    printf("please input the password you want register:\n");
    scanf("%s",password_t);
    if(send_registe(username_t,password_t))
        printf("registe successfully!\n");
    else 
        printf("registe failed!\n");
}



























int main_menu()
{
    int chioce;
    do
    {
        printf("\n\t\t*******************************\n");
        printf("\t\t*        1.show   friends       *\n");
        printf("\t\t*        2.add    friends       *\n");
        printf("\t\t*        3.delete friends       *\n");
        printf("\t\t*        4.chat with one        *\n");
        printf("\t\t*        5.chat with many       *\n");
        printf("\t\t*        6.send   file          *\n");
        printf("\t\t*        0.exit                 *\n");
        printf("\t\t*******************************\n");
        printf("\t\tchoice：");
        scanf("%d",&chioce);
        switch(chioce)
        {  
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            default:
                break;
        }
    }while(chioce!=0);
    return 0;
}





void *clien_send_thread(void *arg)
{
    int i;
    while(1)
    {
        for(i=0;i<m_send_num;i++)
        {
            if(send(sockfd,m_pack_send+i,sizeof(PACK),0) < 0){
               my_err("send",__LINE__);
            }
        }
        sleep(1);
    }
}



void *clien_recv_thread(void *arg)
{
    int i;
    PACK pack_t;
    while(1)
    {
        if(recv(sockfd,&pack_t,sizeof(PACK),0) < 0){
            my_err("recv",__LINE__);
        }
        switch(pack_t.type)
        {
            case LOGIN:
                m_pack_recv_login[m_recv_num_login++] = pack_t;
                break;
            case FRIEND_SEE:
                m_pack_recv_friend_see[m_recv_num_friend_see++] = pack_t;
                break;
            case CHAT_ONE:
                m_pack_recv_chat_one[m_recv_num_chat_one++] = pack_t;
                break;
            case CHAT_MANY:
                m_pack_recv_chat_many[m_recv_num_chat_many++] = pack_t;
                break;
            case SEND_FILE:
                m_pack_recv_send_file[m_recv_num_send_file++] = pack_t;
                break;
        }
        sleep(1);
    }
}






void init_clien_pthread()
{
    pthread_t pid_send,pid_recv;
    pthread_create(&pid_send,NULL,clien_send_thread,NULL);
    pthread_create(&pid_recv,NULL,clien_recv_thread,NULL);
} 









int main(int argc, char const *argv[])
{
	int flag =0;

	signal(SIGINT,sig_close);//关闭CTRL+C
    init();//启动并连接服务器
    //init_clien_pthread();

    login_menu();
    /*while(1){
        if(send(sockfd,&flag,sizeof(int),0) < 0){
            my_err("send",__LINE__);
        }
        if(login())  break;
    }

    while(1)
    {
        if(send(sockfd,&flag,sizeof(int),0) < 0){
            my_err("send",__LINE__);
        }
        switch(flag)
        {
            case LOGIN:
                break;
            case REGISTER:
                break;
            case FRIEND_SEE:
                break;
            case FRIEND_ADD:
                break;
            case FRIEND_DEL:
                break;
            case CHAT_ONE:
                break;
            case CHAT_MANY:
                break;
            case SEND_FILE:
                break;
            case EXIT:
                break;       
        }
    }*/
    

	return 0;
}
