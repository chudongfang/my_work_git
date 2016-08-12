#include <mysql/mysql.h> 
#include <gtk/gtk.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
 
#define LOGIN      1
#define REGISTER   2
#define FRIEND_SEE 3
#define FRIEND_ADD 4
#define FRIEND_DEL 5
#define CHAT_ONE   6
#define CHAT_MANY  7
#define SEND_FILE  8
#define EXIT      -1


#define SIZE_PASS_NAME   30
#define MAX_PACK_CONTIAN 100
#define MAX_CHAR         1024

#define DOWNLINE   0
#define ONLINE     1
#define BUZY       2



/**************************************************/

typedef struct  friend_info
{
    int statu;
    int mes_num;
    char name[MAX_CHAR];
}FRIEND_INFO; 




typedef struct user_infor{
    char        username    [MAX_CHAR];
    FRIEND_INFO friends     [MAX_CHAR];
    int         friends_num;
    char        group       [MAX_CHAR][MAX_CHAR];
    int         group_num;
}USER_INFOR;

USER_INFOR m_my_infor;







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
pthread_mutex_t  mutex_local_user;


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

PACK m_pack_recv_friend_see   [MAX_PACK_CONTIAN];
PACK m_pack_recv_chat_one     [MAX_PACK_CONTIAN];
PACK m_pack_recv_chat_many    [MAX_PACK_CONTIAN];
PACK m_pack_recv_send_file    [MAX_PACK_CONTIAN];


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



/*************************login************************************/
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
    if(login_flag ==  3 ){
        printf("the user has loged in .\n");
        return 0;
    }  
    if(login_flag == 0) {
        printf("the password is not crrect.\n");
        return 0;
    }
    strcpy(m_my_infor.username,username_t);
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
                if(login() == 1)
                    return 1;
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
/*******************************************************/




/**************************registe******************************/

int send_registe(char username_t[],char password_t[])
{
    PACK send_registe_t,recv_registe_t;
    int send_registe_flag;
    send_registe_t.type = REGISTER;
    strcpy(send_registe_t.data.send_name,username_t);
    strcpy(send_registe_t.data.recv_name,"server");
    strcpy(send_registe_t.data.mes,password_t); 
    if(send(sockfd,&send_registe_t,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }

    if(recv(sockfd,&recv_registe_t,sizeof(PACK),0) < 0){
        my_err("recv",__LINE__);
    }
    send_registe_flag = recv_registe_t.data.mes[0] - 48;
    return send_registe_flag;
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
        printf("the name is used ,please input another one\n");
}


/************************************************************/





/*void *clien_send_thread(void *arg)
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
}*/










void change_statu(PACK pack_deal_statu_t)
{
    int count = 0;
    m_my_infor.friends_num=pack_deal_statu_t.data.mes[count++];

    for(int i=1; i <= m_my_infor.friends_num ;i++)
    {
        for(int j=0;j<SIZE_PASS_NAME;j++)
        {
            if(j == 0)   
                m_my_infor.friends[i].statu = pack_deal_statu_t.data.mes[count+j] - 48;
            else
                m_my_infor.friends[i].name[j-1] = pack_deal_statu_t.data.mes[count+j];
        }
        count += SIZE_PASS_NAME;
    }

    m_my_infor.group_num=pack_deal_statu_t.data.mes[count++];
    for(int i=1 ;i <= m_my_infor.group_num ;i++)
    {
        for(int j=0;j<SIZE_PASS_NAME;j++)
        {
            m_my_infor.group[i][j] = pack_deal_statu_t.data.mes[count+j];
        }
        count += SIZE_PASS_NAME;
    }
}




void *deal_statu(void *arg)
{
    int i;
    printf("function deal_statu:\n");
    while(1)
    {
        pthread_mutex_lock(&mutex_local_user); 
        for(i=1;i<=m_recv_num_friend_see;i++)
        {
            //printf("flag1\n");
            change_statu(m_pack_recv_friend_see[i]);
        }
        m_recv_num_friend_see = 0;
        pthread_mutex_unlock(&mutex_local_user);  
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
        pthread_mutex_lock(&mutex_local_user); 
        switch(pack_t.type)
        {
            printf("clien_recv_thread:%d\n", pack_t.type);
            case FRIEND_DEL:
                m_pack_recv_friend_del[++m_recv_num_friend_del] = pack_t;
                break;
            case FRIEND_ADD:
                m_pack_recv_friend_add[++m_recv_num_friend_add] = pack_t;
                break;
            case FRIEND_SEE:
                m_pack_recv_friend_see[++m_recv_num_friend_see] = pack_t;
                break;
            case CHAT_ONE:
                m_pack_recv_chat_one[++m_recv_num_chat_one]     = pack_t;
                break;
            case CHAT_MANY:
                m_pack_recv_chat_many[++m_recv_num_chat_many]   = pack_t;
                break;
            case SEND_FILE:
                m_pack_recv_send_file[++m_recv_num_send_file]   = pack_t;
                break;
        }
        pthread_mutex_unlock(&mutex_local_user);  
    }
}


void init_clien_pthread()
{
    pthread_t pid_deal_statu,pid_recv;
    pthread_create(&pid_deal_statu,NULL,deal_statu,NULL);
    pthread_create(&pid_recv,NULL,clien_recv_thread,NULL);
} 



/********************friend***************************************/


void get_status_mes()
{
    PACK pack_friend_see;
    pack_friend_see.type = FRIEND_SEE;
    strcpy(pack_friend_see.data.send_name,m_my_infor.username);
    strcpy(pack_friend_see.data.recv_name,"server");
    memset(pack_friend_see.data.mes,0,sizeof(pack_friend_see.data.mes));
    if(send(sockfd,&pack_friend_see,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }
}





void friends_see()
{
   
    pthread_mutex_lock(&mutex_local_user);
    printf("friends num:%d\n", m_my_infor.friends_num); 
    printf("\n\nthe list of friends:\n");
    for(int i=1 ;i<=m_my_infor.friends_num ;i++)
    {
        //printf("m_my_infor.friends[i].statu:%d\n", m_my_infor.friends[i].statu);
        switch(m_my_infor.friends[i].statu)
        {
            case ONLINE:
                printf("ID:%d \033[;32m%s\033[0m[ONLINE]\n", i,m_my_infor.friends[i].name);
                break;
            case DOWNLINE:
                printf("ID:%d \033[;31m%s\033[0m[DOWNLINE]\n", i,m_my_infor.friends[i].name);
                break;

        }


    }
    pthread_mutex_unlock(&mutex_local_user);  

}










int judge_same_friend(char add_friend_t[])
{
    int i;
    for(i=1;i<=m_my_infor.friends_num;i++)
    {
        if(strcmp(m_my_infor.friends[i].name,add_friend_t) == 0)
            return 1;
    }
    return 0;
}





void add_friend()
{
    PACK pack_add_friend_t;
    char add_friend_t[MAX_CHAR];
    pack_add_friend_t.type = FRIEND_ADD;
    printf("please input the name of friend you want to add:\n");
    scanf("%s",add_friend_t);

    if(judge_same_friend(add_friend_t))
    {
        printf("you already have same friends!\n");
        return ;
    }
    printf("m_my_infor.username:%s\n", m_my_infor.username);
    strcpy(pack_add_friend_t.data.send_name,m_my_infor.username);
    strcpy(pack_add_friend_t.data.recv_name,"server");
    strcpy(pack_add_friend_t.data.mes,add_friend_t); 
    
    if(send(sockfd,&pack_add_friend_t,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }
    get_status_mes();
}


void del_friend()
{
    PACK pack_del_friend_t;
    char del_friend_t[MAX_CHAR];
    pack_del_friend_t.type = FRIEND_DEL;
    printf("please input the name of friend you want to delete:\n");
    scanf("%s",del_friend_t);

    if(!judge_same_friend(del_friend_t))
    {
        printf("you don't have this friends on list!\n");
        return ;
    }
    printf("m_my_infor.username:%s\n", m_my_infor.username);
    strcpy(pack_del_friend_t.data.send_name,m_my_infor.username);
    strcpy(pack_del_friend_t.data.recv_name,"server");
    strcpy(pack_del_friend_t.data.mes,del_friend_t); 
    
    if(send(sockfd,&pack_del_friend_t,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }
    get_status_mes();
}

















/***********************group***********************************/




void group_see()
{
    pthread_mutex_lock(&mutex_local_user); 
    printf("\n\nthe list of group:\n");
    for(int i=1 ;i<=m_my_infor.group_num ;i++)
    {
        printf("ID:%d %s\n", i,m_my_infor.group[i]);
    }
    pthread_mutex_unlock(&mutex_local_user);  
}


























void massege()
{

}


int main_menu()
{
    int chioce;
    do
    {
        get_status_mes();
        printf("\n\t\t*******************************\n");
        printf("\t\t*        1.show   friends       *\n");
        printf("\t\t*        2.add    friends       *\n");
        printf("\t\t*        3.delete friends       *\n");
        printf("\t\t*        4.show   group         *\n");
        printf("\t\t*        5.add    group         *\n");
        printf("\t\t*        6.quit   group         *\n");
        printf("\t\t*        7.delete group         *\n");
        printf("\t\t*        8.chat with one        *\n");
        printf("\t\t*        9.chat with many       *\n");
        printf("\t\t*        10.send  file          *\n");
        printf("\t\t*        11.message             *\n");
        printf("\t\t*        0.exit                 *\n");
        printf("\t\t*******************************\n");
        printf("\t\tchoice：");
        scanf("%d",&chioce);
        switch(chioce)
        {  
            case 1:
                friends_see();
                break;
            case 2:
                add_friend();
                break;
            case 3:
                del_friend();
                break;
            case 4:
                group_see();
                break;
            case 5:

                break;
            case 6:

                break;
            case 7:

                break;
            case 8:

                break;
            case 9:

                break;
            case 10:

                break;
            case 11:
                massege();
            default:
                break;
        }
    }while(chioce!=0);
    return 0;
}




















int main(int argc, char const *argv[])
{
	int flag =0;

	signal(SIGINT,sig_close);//关闭CTRL+C
    init();//启动并连接服务器

    login_menu();   
    init_clien_pthread();
    main_menu();
  
    

	return 0;
}
