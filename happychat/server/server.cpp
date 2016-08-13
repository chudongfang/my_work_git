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

#define LOGIN        1
#define REGISTER     2
#define FRIEND_SEE   3
#define FRIEND_ADD   4
#define FRIEND_DEL   5
#define GROUP_SEE    6  
#define GROUP_CREATE 7
#define GROUP_JOIN   8
#define GROUP_QIUT   9
#define GROUP_DEL    10
#define CHAT_ONE     11
#define CHAT_MANY    12
#define SEND_FILE    13
#define EXIT         -1

#define SIZE_PASS_NAME  30
#define MAX_CHAR        1024
#define EPOLL_MAX       200000
#define LISTENMAX       1000
#define USER_MAX        100  //the user on line
#define GROUP_MAX       50

#define DOWNLINE   0
#define ONLINE     1
#define BUZY       2









/********************user infor***********************************/
typedef struct infor_user 
{
    char username[MAX_CHAR];
    char password[MAX_CHAR];
    int  statu;//don't foget to change is to 0 when the server begin
    int  socket_id;
    char friends[USER_MAX][MAX_CHAR];//begin from 1
    int  friends_num;
    char group[GROUP_MAX][MAX_CHAR];  //begin from 1
    char group_num;
}INFO_USER;


INFO_USER  m_infor_user  [USER_MAX];
int        m_user_num; 
/**************************************************/








/*********************group infor***************************/
typedef struct infor_group
{
    char  group_name[MAX_CHAR];
    int   member_num;
    char  member_name[USER_MAX][MAX_CHAR];  //begin from 1
}INFO_GROUP;

INFO_GROUP   m_infor_group  [GROUP_MAX]; //begin from 1
int          m_group_num;









/*********************************************************/



typedef struct datas{
    char     send_name[MAX_CHAR];
    char     recv_name[MAX_CHAR];
    int      send_fd;
    int      recv_fd;
    time_t   time;
    char     mes[MAX_CHAR*2];
}DATA;

typedef struct package{
    int   type;
    DATA  data;
}PACK;



/*****************send*********************/
PACK m_pack_send   [MAX_CHAR*2];
int  m_send_num;


/**************************************/






/**********function***************/
void my_err(const char * err_string,int line);
void signal_close(int signal_num);
int find_userinfor(char username_t[]);
void login(PACK *recv_pack);
void registe(PACK *recv_pack);
void *deal(PACK *recv_pack);
void *serv_send_thread(void *arg);
void init_server_pthread();
int judge_usename_same(char username_t[]);





int listenfd,epollfd;//
char* IP = "127.0.0.1";//服务器IP
short PORT = 10222;//端口号
typedef struct sockaddr SA;//类型转换
pthread_mutex_t  mutex;











void my_err(const char * err_string,int line)
{
	fprintf(stderr, "line:%d  ", line);
	perror(err_string);
	exit(1);
}




void signal_close(int i)
{
//关闭服务器前 关闭服务器的socket
    //close(listenfd);
    printf("服务器已经关闭\n");
    exit(1);
}



void send_pack(PACK *pack_send_pack_t)
{
    pthread_mutex_lock(&mutex);  
    memcpy(&(m_pack_send[m_send_num++]),pack_send_pack_t,sizeof(PACK));
    pthread_mutex_unlock(&mutex);  
    free(pack_send_pack_t);
}




/***************************debug******************************/

void print_send_pack()
{
    for(int i=1;i<=m_send_num;i++)
    {
        printf("********%d*********\n", i);
        printf("type      :%d\n", m_pack_send[i].type);
        printf("send_name :%s\n", m_pack_send[i].data.send_name);
        printf("recv_name :%s\n", m_pack_send[i].data.recv_name);
        printf("send_fd   :%d\n", m_pack_send[i].data.send_fd);
        printf("recv_fd   :%s\n", m_pack_send[i].data.recv_fd);
        printf("mes       :%s\n", m_pack_send[i].data.mes);
        printf("***********************\n\n\n");
    }
}






/**********************login*******************************************/

/*void set_user_online(char username_t[])
{
    int i;
    for(i=1;i<=m_send_num;i++)
    {
        if(strcmp(m_infor_user[i].username,username_t) == 0)
        {
            m_infor_user[i].statu = ONLINE;
            return ;
        }
    }
}*/





int find_userinfor(char username_t[])
{

    int i;
    if(m_user_num == 0)  return 0;
    for(i=1;i<=m_user_num;i++)
    {
        if(strcmp(m_infor_user[i].username,username_t) == 0)
            return i;
    }
    if(i == m_user_num+1) 
        return 0;
}




void login(PACK *recv_pack)
{
    int id=0;
    char login_flag[10];

    if((id=find_userinfor(recv_pack->data.send_name)) == 0){//not exit username
        login_flag[0] = '2';
    }
    else if (m_infor_user[id].statu == ONLINE)
    {
        login_flag[0] = '3';
    }
    else if(strcmp(recv_pack->data.mes,m_infor_user[id].password) == 0){//the password is not crrect
        login_flag[0] = '1';
        //change user infor
        m_infor_user[id].statu = ONLINE;
        m_infor_user[id].socket_id = recv_pack->data.send_fd;
        
        printf("\n\n\033[;45m********login**********\033[0m\n");
        printf("\033[;45m*\033[0m %s get online!\n", m_infor_user[id].username);
        printf("\033[;45m*\033[0m statu:    %d\n", m_infor_user[id].statu); 
        printf("\033[;45m*\033[0m socket_id:%d\n\n",m_infor_user[id].socket_id);
    }   
      
    else login_flag[0] = '0';
    
    login_flag[1] = '\0';
    /*printf("%s\n", recv_pack->data.send_name);
    printf("%s\n", recv_pack->data.mes);
    printf("%d\n", m_user_num); */    
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    strcpy(recv_pack->data.mes,login_flag);
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;
    send_pack(recv_pack);
    

}
/**********************************************************/




/********************registe********************************/
int judge_usename_same(char username_t[])
{
    int i;
    if(m_user_num == 0)  return 1;
    for(i=1;i<=m_user_num;i++)
    {
        if(strcmp(m_infor_user[i].username,username_t) == 0)
            return 0;
    }
    if(i == m_user_num+1) 
        return 1;
}




void registe(PACK *recv_pack)
{
    char registe_flag[10];
 
    
    if(judge_usename_same(recv_pack->data.send_name)){//the password is not crrect
        registe_flag[0] = '1';
        //add user infor
        m_user_num++;
        strcpy(m_infor_user[m_user_num].username,recv_pack->data.send_name);
        strcpy(m_infor_user[m_user_num].password,recv_pack->data.mes);
        printf("\n\n\033[;44m&&&&function registe&&&&\033[0m \n");
        printf("\033[;44m*\033[0m registe success!\n");
        printf("\033[;44m*\033[0m username:%s\n", m_infor_user[m_user_num].username);
        printf("\033[;44m*\033[0m password:%s\n", m_infor_user[m_user_num].password);
        printf("\033[;44m*\033[0m user_num:%d\n\n", m_user_num);
        m_infor_user[m_user_num].statu = DOWNLINE;
    }
    else registe_flag[0] = '0';
    registe_flag[1] = '\0';
   
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    strcpy(recv_pack->data.mes,registe_flag);
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;
    send_pack(recv_pack);

    
}
/**********************************************************/

void send_statu(PACK *recv_pack)
{
    char str[MAX_CHAR];
    char name_t[MAX_CHAR];
    char send_statu_mes[MAX_CHAR*2];
    int id;
    int count = 0;

    memset(send_statu_mes,0,sizeof(send_statu_mes));
    
    id = find_userinfor(recv_pack->data.send_name);
    /*for(int i = 1;i <= m_user_num ;i++)
    {
        if(strcmp(recv_pack->data.send_name,m_infor_user[i].username) == 0)
        {
            id = i;
            break;
        }
    }*/


    /*for(int i=1;i<=m_user_num;i++)
        printf("send_statu:%d,%s\n", i,m_infor_user[i].username);    
    printf("send_statu:id=%d\n", id);*/


    send_statu_mes[count++] = m_infor_user[id].friends_num;


    for(int i=1 ;i <= m_infor_user[id].friends_num ;i++)
    {
        strcpy(name_t,m_infor_user[id].friends[i]);
        printf("send_statufriend:%d %s\n",i,name_t);
        for(int j=1 ;j <= m_user_num ;j++)
        {
            if(strcmp(name_t,m_infor_user[j].username) == 0)
            {
                memset(str,0,sizeof(str));
                if(m_infor_user[j].statu == ONLINE)
                    sprintf(str,"%d%s\0",ONLINE,m_infor_user[j].username);
                else
                    sprintf(str,"%d%s\0",DOWNLINE,m_infor_user[j].username);
                printf("str = %s\n",str);
                for(int k = 0 ;k< SIZE_PASS_NAME;k++)
                {
                    send_statu_mes[k+count] = str[k];
                }
                count += SIZE_PASS_NAME;
            }
        }
    }

    send_statu_mes[count++] = m_infor_user[id].group_num;
    for(int i = 1;i <=  m_infor_user[id].group_num;i++)
    {
        memset(str,0,sizeof(str));
        strcpy(name_t,m_infor_user[id].group[i]);
        sprintf(str,"%s\0",name_t);
        for(int k = 0 ;k< SIZE_PASS_NAME;k++)
        {
            send_statu_mes[k+count] = str[k];
        }
        count += SIZE_PASS_NAME;
    }

    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    memcpy(recv_pack->data.mes,send_statu_mes,MAX_CHAR*2);
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;

    send_pack(recv_pack);
}



void friend_add(PACK *recv_pack)
{
    int id_own,id_friend;
    id_own = find_userinfor(recv_pack->data.send_name);
    
    strcpy(m_infor_user[id_own].friends[++(m_infor_user[id_own].friends_num)],recv_pack->data.mes);
    
    id_friend = find_userinfor(recv_pack->data.mes);
    strcpy(m_infor_user[id_friend].friends[++(m_infor_user[id_friend].friends_num)],recv_pack->data.send_name);

    //printf("friend add  m_infor_user[id].friends_num:%d\n", m_infor_user[id].friends_num);
    //printf("friend add  m_infor_user[id].friends_num:%s\n", m_infor_user[id].friends[m_infor_user[id].friends_num]);
    free(recv_pack);
}

void del_friend_infor(int id,char friend_name[]) 
{
    int id_1;
    for(int i = 1 ;i<=m_infor_user[id].friends_num;i++)
    {
        if(strcmp(m_infor_user[id].friends[i],friend_name) == 0)
        {   
            id_1 = i;
            break;
        }

    }
    for(int i = id_1;i<m_infor_user[id].friends_num;i++)
    {
        strcpy(m_infor_user[id].friends[i],m_infor_user[id].friends[i+1]);
    }
    m_infor_user[id].friends_num--;
}







void friend_del(PACK *recv_pack)
{
    int id_own,id_own_1;
    int id_friend,id_friend_1;
    id_own = find_userinfor(recv_pack->data.send_name);
    

    del_friend_infor(id_own,recv_pack->data.mes); 

    
    id_friend = find_userinfor(recv_pack->data.mes);

    del_friend_infor(id_friend,recv_pack->data.send_name); 
    //printf("friend add  m_infor_user[id].friends_num:%d\n", m_infor_user[id].friends_num);
    //printf("friend add  m_infor_user[id].friends_num:%s\n", m_infor_user[id].friends[m_infor_user[id].friends_num]);
    free(recv_pack);

}


void send_mes_to_one(PACK *recv_pack)
{
    send_pack(recv_pack);
}


void group_create(PACK *recv_pack)
{
    for(int i=1;i<= m_group_num;i++)
    {
        if(strcmp(m_infor_group[i].group_name,recv_pack->data.mes) == 0)
        {
            strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
            strcpy(recv_pack->data.send_name,"server");
            recv_pack->data.mes[0] = 1;
            send_pack(recv_pack);
            return ;
        }   
    }
    
    strcpy(m_infor_group[++m_group_num].group_name,recv_pack->data.mes);
    strcpy(m_infor_group[m_group_num].member_name[++m_infor_group[m_group_num].member_num],recv_pack->data.send_name);
    int id=find_userinfor(recv_pack->data.send_name);
    strcpy(m_infor_user[id].group[++m_infor_user[id].group_num],recv_pack->data.mes);


    printf("\n\n\033[;32mcreat group : %s  successfully!\033[0m \n\n", recv_pack->data.mes);
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");

    recv_pack->data.mes[0] = 2;
    send_pack(recv_pack);
}


void group_join(PACK *recv_pack)
{
    for(int i=1;i<= m_group_num;i++)
    {
        if(strcmp(m_infor_group[i].group_name,recv_pack->data.mes) == 0)
        {
            strcpy(m_infor_group[i].member_name[++m_infor_group[m_group_num].member_num],recv_pack->data.send_name);
            int id=find_userinfor(recv_pack->data.send_name);
            strcpy(m_infor_user[id].group[++m_infor_user[id].group_num],recv_pack->data.mes);

            strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
            strcpy(recv_pack->data.send_name,"server");
            recv_pack->data.mes[0] = 2; 
            printf("\n\n\033[;32m %s join group : %s  successfully!\033[0m \n\n",recv_pack->data.send_name, recv_pack->data.mes);

            send_pack(recv_pack);
            return ;
        }   
    }
    
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");

    recv_pack->data.mes[0] = 1;
    send_pack(recv_pack);
}



void del_group_from_user(char *username,char *groupname)
{
    int id = find_userinfor(username);
    for(int i = 1;i<=m_infor_user[id].group_num;i++)
    {
        if(strcmp(m_infor_user[id].group[i],groupname) == 0)
        {
            for(int j = i ;j < m_infor_user[id].group_num ;j++)
            {
                strcpy(m_infor_user[id].group[j],m_infor_user[id].group[j+1]);
            }
            m_infor_user[id].group_num--;
        }
    }
}




void group_qiut(PACK *recv_pack)
{
    del_group_from_user(recv_pack->data.send_name,recv_pack->data.mes);

    for(int i=1 ;i<=m_group_num;i++)
    {
        if (strcmp(recv_pack->data.mes,m_infor_group[i].group_name) == 0)
        {
            for(int j=1;j<=m_infor_group[i].member_num;j++)
            {
                if(strcmp(recv_pack->data.send_name,m_infor_group[i].member_name[j]) == 0)
                {
                    for(int k=j;k <m_infor_group[i].member_num;k++)
                    {
                        strcpy(m_infor_group[i].member_name[k],m_infor_group[i].member_name[k+1]);
                    }
                    m_infor_group[i].member_num--;
                }
            }
        }
    }
}






void group_del_one(int id)
{
    for(int i=1;i <= m_infor_group[id].member_num;i++)
    {
        del_group_from_user(m_infor_group[id].member_name[i],m_infor_group[id].group_name);
    }

    for(int i = id ;i < m_group_num;i++)
    {
        m_infor_group[i] = m_infor_group[i+1];
    }
    m_group_num--;
}









void group_del(PACK *recv_pack)
{
    for(int i=1;i<=m_group_num;i++)
    {
        if(strcmp(m_infor_group[i].group_name,recv_pack->data.mes) == 0)
        {
            if(strcmp(m_infor_group[i].member_name[1],recv_pack->data.send_name) == 0)
            {
                group_del_one(i);
                recv_pack->data.mes[0] = 2;
            }
            else
                recv_pack->data.mes[0] = 1;
        }
    }
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    send_pack(recv_pack);

}













void *deal(void *recv_pack_t)
{
    int i;
    PACK *recv_pack = (PACK *)recv_pack_t;
    printf("\n\n\ndeal function = %d\n", recv_pack->type);
  
    switch(recv_pack->type)
    {
        case LOGIN:
            login(recv_pack);
            break;
        case REGISTER:
            registe(recv_pack);
            break;
        case FRIEND_SEE:
            send_statu(recv_pack);
            break;
        case FRIEND_ADD:
            friend_add(recv_pack);
            break;
        case FRIEND_DEL:
            friend_del(recv_pack);
            break;
        case GROUP_CREATE:
            group_create(recv_pack);
            break;
        case GROUP_JOIN:
            group_join(recv_pack);
            break;
        case GROUP_QIUT:
            group_qiut(recv_pack);
            break;
        case GROUP_DEL:
            group_del(recv_pack);
            break;
        case CHAT_ONE:
            send_mes_to_one(recv_pack);
            break;
        case CHAT_MANY:

            break;
        case SEND_FILE:

            break;
        case EXIT:

            break;       
    }

}





/************************send_thread*****************************/
void *serv_send_thread(void *arg)
{
    int user_statu = DOWNLINE;
    int id_stop;
    int i,recv_fd_t,recv_fd_online;
    while(1)
    {
        pthread_mutex_lock(&mutex); 
        //printf("serv_send_thread:%d\n", m_send_num);
        user_statu = DOWNLINE;
        for(i = m_send_num-1;i>=0;i--)
        {

            for(int j =1; j <= m_user_num ;j++)
            {

                if(strcmp(m_pack_send[i].data.recv_name,m_infor_user[j].username) == 0)
                {
                    user_statu = m_infor_user[j].statu;
                    if(user_statu == ONLINE)
                        recv_fd_online = m_infor_user[j].socket_id;
                    break;
                }
            }

            if(user_statu == ONLINE || m_pack_send[i].type == LOGIN || m_pack_send[i].type == REGISTER)
            {
                if(user_statu == ONLINE)
                    recv_fd_t = recv_fd_online;
                else
                    recv_fd_t = m_pack_send[i].data.recv_fd;
                if(send(recv_fd_t,&m_pack_send[i],sizeof(PACK),0) < 0){
                    my_err("send",__LINE__);
                }
                printf("\n\n\033[;42m*****send pack****\033[0m\n");
                printf("\033[;42m*\033[0m from: %s\n",m_pack_send[i].data.recv_name);
                printf("\033[;42m*\033[0m to  : %s\n",m_pack_send[i].data.send_name);
                printf("\033[;42m*\033[0m mes : %s\n",m_pack_send[i].data.mes);
                m_send_num-- ;
                printf("\033[;42m*\033[0m pack left Now is:%d\n\n", m_send_num);
                for(int j = i;j<=m_send_num&&m_send_num;j++)
                {
                    m_pack_send[j] = m_pack_send[j+1];
                }
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        usleep(1);  
    }
}


void init_server_pthread()
{
    printf("\ninit_server_pthread\n");
    pthread_t pid_send;
    pthread_create(&pid_send,NULL,serv_send_thread,NULL);
} 

/****************************************************/





int main()
{
    int n;
    pthread_t pid;
    PACK recv_t;
    PACK *recv_pack;
    int err,socketfd,fd_num;
    struct sockaddr_in fromaddr;
    socklen_t len = sizeof(fromaddr);
    struct epoll_event ev, events[LISTENMAX];


    signal(SIGINT,signal_close);//退出CTRL+C
    pthread_mutex_init(&mutex, NULL);  


    printf("服务器开始启动..\n");
    init_server_pthread();
    epollfd = epoll_create(EPOLL_MAX);//生成epoll句柄
    
    listenfd = socket(AF_INET,SOCK_STREAM,0);//启动socket
    if(listenfd == -1){
        perror("创建socket失败");
        printf("服务器启动失败\n");
        exit(-1);
    }

    err = 1;

    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&err,sizeof(int)); 

    ev.data.fd = listenfd;//设置与要处理事件相关的文件描述符
    ev.events = EPOLLIN;//设置要处理的事件类型
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);//注册epoll事件

    //准备网络通信地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if(bind(listenfd,(SA*)&addr,sizeof(addr))==-1){//绑定服务器
        perror("绑定失败");
        printf("服务器启动失败\n");
        exit(-1);
    }
    printf("成功绑定\n");

    //设置监听
    if(listen(listenfd,10)==-1){
        perror("设置监听失败");
        printf("服务器启动失败\n");
        exit(-1);
    }
    printf("设置监听成功\n");
    printf("初始化服务器成功\n");
    printf("服务器开始服务\n");
    while(1)
    {

        fd_num = epoll_wait(epollfd, events, EPOLL_MAX, 1000);
        for(int i=0; i<fd_num; i++)
        {
            if(events[i].data.fd == listenfd)
            {   

                socketfd = accept(listenfd,(SA*)&fromaddr,&len);
                printf("%d get conect!\n",socketfd);
                ev.data.fd = socketfd;
                ev.events = EPOLLIN;//设置监听事件可写
                //新增套接字
                epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
            }
            else if(events[i].events & EPOLLIN)//fd can read
            {

                n = recv(events[i].data.fd,&recv_t,sizeof(PACK),0);//读取数据
                recv_t.data.send_fd = events[i].data.fd;
                
                printf("\n\n\n\033[;31m ***PACK***\033[0m\n");
                printf("\033[;31m*\033[0m send_name: %s\n", recv_t.data.send_name);
                printf("\033[;31m*\033[0m recv_name: %s\n",recv_t.data.recv_name);
                printf("\033[;31m*\033[0m mes      : %s\n", recv_t.data.mes);
                printf("\033[;31m*\033[0m send_fd   : %d\n", recv_t.data.send_fd);
                printf("\033[;31m*\033[0m recv_fd   : %d\n", recv_t.data.recv_fd);
                printf("\033[;31m*\033[0m send_pack_num:%d\n", m_send_num);
                printf("\033[;31m*************\033[0m\n\n\n");
                

                if(n < 0)//recv错误
                {     
                    close(events[i].data.fd);
                    perror("recv");
                    continue;
                }
                else if(n == 0)
                {
                 
                    for(int j=1;j<=m_user_num;j++)
                    {
                        if(events[i].data.fd == m_infor_user[j].socket_id)
                        {
                            printf("%s down line!\n",m_infor_user[j].username);
                            m_infor_user[j].statu = DOWNLINE;
                            break;
                        }
                    }   
                    ev.data.fd = events[i].data.fd;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);//删除套接字
                    close(events[i].data.fd);
                    continue;   
                }
                pthread_mutex_lock(&mutex); 
                //print_send_pack();
                
                pthread_mutex_unlock(&mutex);  
                
                recv_pack = (PACK*)malloc(sizeof(PACK));
                memcpy(recv_pack, &recv_t, sizeof(PACK));
                pthread_create(&pid,NULL,deal,(void *)recv_pack);
            }

        }
    }
    return 0;
}
