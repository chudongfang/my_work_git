#include <mysql/mysql.h> 
//#include <gtk/gtk.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <math.h>

using namespace std;



#define LOGIN                    1
#define REGISTER                 2
#define FRIEND_SEE               3
#define FRIEND_ADD               4
#define FRIEND_DEL               5
#define GROUP_SEE                6  
#define GROUP_CREATE             7
#define GROUP_JOIN               8
#define GROUP_QIUT               9
#define GROUP_DEL                10
#define CHAT_ONE                 11
#define CHAT_MANY                12
#define FILE_SEND_BEGIN          13
#define FILE_SEND_BEGIN_RP       14
#define FILE_SEND_STOP_RP        15
#define FILE_RECV_RE             16
#define FILE_SEND                17
#define FILE_RECV_BEGIN          18 
#define FILE_RECV_BEGIN_RP       19
#define FILE_RECV_STOP_RP        20
#define FILE_RECV                21
#define FILE_FINI_RP             22
#define MES_RECORD               23
#define EXIT                     -1




#define FILE_STATU_RECV_ING       1
#define FILE_STATU_RECV_STOP      2
#define FILE_STATU_SEND_ING       3
#define FILE_STATU_SEND_FINI      4


#define SIZE_PASS_NAME  10
#define MAX_CHAR        300
#define EPOLL_MAX       200000
#define LISTENMAX       1000
#define USER_MAX        100  //the user on line
#define GROUP_MAX       50
#define FILE_MAX        50
#define NUM_MAX_DIGIT   10

#define DOWNLINE   0
#define ONLINE     1
#define BUZY       2


// #define LOG
 #define  MYSQL_OPEN

// 储存用户信息结构体
/********************user infor***********************************/
typedef struct infor_user 
{
    char username[MAX_CHAR];  //用户名
    unsigned int password[4]; //经加密后密码
    int  statu;               //用户状态
    int  socket_id;           //用户socketID
    char friends[USER_MAX][MAX_CHAR];//begin from 1  好友信息
    int  friends_num;                //  好友数量
    char group[GROUP_MAX][MAX_CHAR];  //begin from 1  群组信息
    char group_num;                   //群组数量
}INFO_USER;



//储存群组信息结构体
/*********************group infor***************************/
typedef struct infor_group
{
    char  group_name[MAX_CHAR];  //组名
    int   member_num;            //成名数量
    char  member_name[USER_MAX][MAX_CHAR];  //begin from 1  成员名
}INFO_GROUP;


//储存文件信息结构体
/******************file_infor************************************/

typedef struct file_infor
{
    char  file_name[MAX_CHAR];       //文件名
    char  file_send_name[MAX_CHAR];  //发送方
    char  file_recv_name[MAX_CHAR];  //接收方
    int   file_size;                 //文件大小
    int   file_size_now;             //文件当前大小
    int   flag ;                     //状态
}INFO_FILE;


//包结构体
/*****************************************************************/

typedef struct datas{
    char     send_name[MAX_CHAR];  //发送方
    char     recv_name[MAX_CHAR];  //接收方
    int      send_fd;              //发送方fd
    int      recv_fd;              //接收方fd
    //time_t   time;
    char     mes[MAX_CHAR*2];      //信息
}DATA;

typedef struct package{
    int   type;
    DATA  data;
}PACK;


//线程传参结构体
/*****************************************************************/

typedef struct pthread_parameter
{
    int a;
    char str1[SIZE_PASS_NAME];
    char str2[SIZE_PASS_NAME];
}PTHREAD_PAR;

typedef struct sockaddr SA;//类型转换










/*********************md5*****************************************

/***********************************
 * 非线性函数
 * (&是与,|是或,~是非,^是异或) 
 * 
 * 这些函数是这样设计的：
 *   如果X、Y和Z的对应位是独立和均匀的，
 *   那么结果的每一位也应是独立和均匀的。 
 * 
 * 函数F是按逐位方式操作：如果X，那么Y，否则Z。
 * 函数H是逐位奇偶操作符
 **********************************/
#define F(x,y,z) ((x & y) | (~x & z))  
#define G(x,y,z) ((x & z) | (y & ~z))  
#define H(x,y,z) (x^y^z)  
#define I(x,y,z) (y ^ (x | ~z))  


/**************************************
 *向右环移n个单位
 * ************************************/
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))  




/****************************************************
 * 每次操作对a，b，c和d中的其中三个作一次非线性函数运算
 *  F(b,c,d)   G(b,c,d)   H(b,c,d)   I(b,c,d)
 *
 * 然后将所得结果加上 第四个变量(a)，
 * F(b,c,d)+a
 *
 * 文本的一个子分组(x)
 * F(b,c,d)+a+x
 * 
 * 和一个常数(ac)。
 * F(b,c,d)+a+x+ac
 *
 * 再将所得结果向右环移一个不定的数(s)，
 * ROTATE_LEFT( F(b,c,d)+a+x+ac , s )
 * 
 * 并加上a，b，c或d中之一(b)。
 * ROTATE_LEFT( F(b,c,d)+a+x+ac , s )+b
 * 
 * 最后用该结果取代a，b，c或d中之一(a)。
 * a=ROTATE_LEFT( F(b,c,d)+a+x+ac , s )+b
 * 
 * ***************************************************/


#define FF(a,b,c,d,x,s,ac) { a += F(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }
#define GG(a,b,c,d,x,s,ac) { a += G(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }
#define HH(a,b,c,d,x,s,ac) { a += H(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }
#define II(a,b,c,d,x,s,ac) { a += I(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }





//储存一个MD5 text信息 
typedef struct  
{  
    unsigned int count[2];    
    //记录当前状态，其数据位数   
    
    unsigned int state[4];    
    //4个数，一共32位 记录用于保存对512bits信息加密的中间结果或者最终结果  
    
    unsigned char buffer[64];
    //一共64字节，512位      
}MD5_CTX;  
  







/**********main***************/
void *deal(void *recv_pack_t);
void *serv_send_thread(void *arg);
void *pthread_check_file(void *arg);
void init_server_pthread();


/************data_access*****************/
int read_infor();
int write_infor();
int conect_mysql_init();


/************debug*****************/
void print_send_pack();
void print_infor_group();
void print_infor_user();
void print_infor_file();




/************tools*****************/
void my_err(const char * err_string,int line);
int find_userinfor(char username_t[]);
void signal_close(int i);
void send_pack(PACK *pack_send_pack_t);
void send_pack_memcpy_server(int type,char *send_name,char *recv_name,int sockfd1,char *mes);
bool md5_judge(int id,char *log_pass);
int judge_usename_same(char username_t[]);
void del_friend_infor(int id,char friend_name[]) ;
void del_group_from_user(char *username,char *groupname);
int find_groupinfor(char groupname_t[]);
int find_fileinfor(char *filename);



/*****************server*********************/
void login(PACK *recv_pack);
void registe(PACK *recv_pack);
void send_statu(PACK *recv_pack);
void friend_add(PACK *recv_pack);
void friend_del(PACK *recv_pack);
void send_mes_to_one(PACK *recv_pack);
void group_create(PACK *recv_pack);
void group_join(PACK *recv_pack);
void group_qiut(PACK *recv_pack);
void group_del_one(int id);
void group_del(PACK *recv_pack);
void send_mes_to_group(PACK *recv_pack);
void file_recv_begin(PACK *recv_pack);
void file_recv_begin_rp(PACK *recv_pack);
void file_recv(PACK *recv_pack);
void *file_send_send(void *file_send_begin_t);
void file_send_begin(PACK *recv_pack);
void file_send_finish(PACK *recv_pack);
void send_record(PACK *recv_pack);



/**************md5*********************/
void MD5Init(MD5_CTX *context);  
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);  
void MD5Final(MD5_CTX *context,unsigned char digest[16]);  
void MD5Transform(unsigned int state[4],unsigned char block[64]);  
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);  
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);  
int md5(char encrypt_input[10000],unsigned int decrypt[4]);









INFO_USER  m_infor_user  [USER_MAX];
int        m_user_num; 
/**************************************************/


INFO_GROUP   m_infor_group  [GROUP_MAX]; //begin from 1
int          m_group_num;

/******************file_infor************************************/

INFO_FILE    m_infor_file  [FILE_MAX]; //begin from 1
int          m_file_num;


PACK m_pack_send   [MAX_CHAR*2];
int  m_send_num;


unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  
  

int listenfd,epollfd;//
short PORT = 11112;//端口号
int log_file_fd;
int user_infor_fd;
int group_infor_fd;

//线程锁
pthread_mutex_t  mutex;
pthread_mutex_t  mutex_recv_file;
pthread_mutex_t  mutex_check_file;






//处理包的函数，每接受到一个包，开新线程，根据类型进行处理
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
            send_mes_to_group(recv_pack);
            break;
        case FILE_SEND_BEGIN:
            file_recv_begin(recv_pack);
            break;
        case FILE_SEND:
            file_recv(recv_pack);
            break;
        case FILE_SEND_BEGIN_RP:
            file_send_begin(recv_pack);
            break;
        case FILE_RECV_BEGIN_RP:
            file_recv_begin_rp(recv_pack);
            break;
        case FILE_FINI_RP:
            file_send_finish(recv_pack);
            break;
        case MES_RECORD:
            send_record(recv_pack);
        case EXIT:

            break;       
    }
}



//包发送函数，不断遍历存储包的数组，如果要发送的客户端在线则发送，否则一直等到其上线，
//实现离线功能
/************************send_thread*****************************/
void *serv_send_thread(void *arg)
{
    int user_statu = DOWNLINE;
    int id_stop;
    int i,recv_fd_t,recv_fd_online;
    while(1)
    {
        pthread_mutex_lock(&mutex);//线程锁，保护存包数组 
        // printf("serv_send_thread:%d\n", m_send_num);
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
            //上线，则发送包
            if(user_statu == ONLINE || m_pack_send[i].type == LOGIN || m_pack_send[i].type == REGISTER)
            {
                if(user_statu == ONLINE&&m_pack_send[i].type!= LOGIN)
                    recv_fd_t = recv_fd_online;
                else
                    recv_fd_t = m_pack_send[i].data.recv_fd;
                

                if(send(recv_fd_t,&m_pack_send[i],sizeof(PACK),0) < 0){
                    my_err("send",__LINE__);
                }
                printf("\n\n\033[;42m*****send pack****\033[0m\n");
                printf("\033[;42m*\033[0m type    :%d\n",m_pack_send[i].type);
                printf("\033[;42m*\033[0m from    : %s\n",m_pack_send[i].data.send_name);
                printf("\033[;42m*\033[0m to      : %s\n",m_pack_send[i].data.recv_name);
                printf("\033[;42m*\033[0m mes     : %s\n",m_pack_send[i].data.mes);
                printf("\033[;42m*\033[0m recv_fd : %d\n",m_pack_send[i].data.recv_fd);
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


//不断检测文件状态，当文件传输失败，发送提醒给客户端
void *pthread_check_file(void *arg)
{
    while(1)
    {
       
        pthread_mutex_lock(&mutex_check_file);  
        for(int i=1 ;i<=m_file_num ;i++)
        {
            //文件上传完毕，发送下载提醒给客户端
            if(m_infor_file[i].file_size <= m_infor_file[i].file_size_now&&(m_infor_file[i].flag == FILE_STATU_RECV_ING ||m_infor_file[i].flag == FILE_STATU_RECV_STOP))
            {
                char mes_t[MAX_CHAR];
                printf("\033[; 35m**********jijmei********?\033[0m \n");
                int id = find_userinfor(m_infor_file[i].file_recv_name);

                PACK *pthread_check_file_t = (PACK*)malloc(sizeof(PACK));
                memset(pthread_check_file_t, 0,sizeof(PACK));
                pthread_check_file_t->type = FILE_RECV_BEGIN;
                strcpy(pthread_check_file_t->data.send_name ,m_infor_file[i].file_send_name);
                strcpy(pthread_check_file_t->data.recv_name,m_infor_file[i].file_recv_name);
                
                int len = m_infor_file[i].file_size;

                memset(mes_t,0,sizeof(mes_t));
                int digit = 0;
                while(len != 0)
                {   
                    mes_t[digit++] = len%10;
                    len /= 10;
                }
                mes_t[digit]  = -1;
                for(int j=0 ;j< SIZE_PASS_NAME ;j++)
                {
                    mes_t[NUM_MAX_DIGIT+j] = m_infor_file[i].file_name[j];
                }  


                memcpy(pthread_check_file_t->data.mes,mes_t,sizeof(mes_t));  
                send_pack(pthread_check_file_t);
                free(pthread_check_file_t);
                m_infor_file[i].flag = FILE_STATU_SEND_ING;

                
                // m_file_num --;
                // for(int j = i;j<=(m_file_num+1)&&m_file_num;j++)
                // {
                    // m_infor_file[j] = m_infor_file[j+1];
                // }     
                break;
            }

            //用户突然下线，发送提醒给客户端
            if(m_infor_file[i].file_size > m_infor_file[i].file_size_now )
            {
                int id = find_userinfor(m_infor_file[i].file_send_name);
                if(m_infor_user[id].statu == DOWNLINE && m_infor_file[i].flag == FILE_STATU_RECV_ING)
                {
                    printf("\033[;35m file_name %s d\033[0m\n", m_infor_file[i].file_name);
                    
                    char mes[MAX_CHAR];
                    memset(mes,0,sizeof(mes));
                    int num = m_infor_file[i].file_size_now;
                    
                    printf("\033[;35m file_sizejj :%d\033[0m\n",num );

                    int digit = 0;
                    while(num != 0)
                    {   
                        mes[digit++] = num%10;
                        num /= 10;
                    }
                    mes[digit] = -1;
                    for(int i=0;i<10;i++)
                        printf("%d\n\n",mes[i]);

                    printf("\033[;35m file_name %s jjj\033[0m\n", m_infor_file[i].file_name);
                   

                    PACK pthread_check_file_t;
                    memset(&pthread_check_file_t, 0,sizeof(PACK));
                    pthread_check_file_t.type = FILE_SEND_STOP_RP;
                    strcpy(pthread_check_file_t.data.send_name ,m_infor_file[i].file_name);
                    strcpy(pthread_check_file_t.data.recv_name,m_infor_file[i].file_send_name);
                    memcpy(pthread_check_file_t.data.mes,mes,sizeof(mes));
                    
                    send_pack(&pthread_check_file_t);
                    printf("jiji$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
                    m_infor_file[i].flag = FILE_STATU_RECV_STOP;
                    break;
                }
            }
            //删除服务端缓存文件，并从文件数组中清除该文件信息
            if(m_infor_file[i].flag == FILE_STATU_SEND_FINI)
            {
                unlink(m_infor_file[i].file_name);
                m_file_num --;
                for(int j = i;j<=(m_file_num+1)&&m_file_num;j++)
                {
                    m_infor_file[j] = m_infor_file[j+1];
                } 

            }     
        }
        pthread_mutex_unlock(&mutex_check_file); 
        usleep(10);
    }    
}

//开启线程
void init_server_pthread()
{
    printf("\ninit_server_pthread\n");
    pthread_t pid_send,pid_file_check;
    pthread_create(&pid_send,NULL,serv_send_thread,NULL);
    pthread_create(&pid_file_check,NULL,pthread_check_file,NULL);
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


    read_infor();
    //日志开关，重定向到文件中
    #ifdef LOG
        if((log_file_fd = open("server_log.txt",O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)) < 0)
        {
            my_err("open",__LINE__);
            return 0;
        }
        dup2(log_file_fd , 1);
    #else
    #endif
    

    //mysql开关，链接mysql
    #ifdef MYSQL_OPEN
       conect_mysql_init();
    #else
    #endif



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
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
                
                
                //输出收到的包信息
                printf("\n\n\n\033[;31m ***PACK***\033[0m\n");
                printf("\033[;31m*\033[0m type     : %d\n", recv_t.type);
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
                    //客户端下线后，把客户端状态设置为下线
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
                    print_infor_user();
                    continue;   
                }
                
                
                //pthread_mutex_lock(&mutex); 
                //print_send_pack();
                //pthread_mutex_unlock(&mutex);                 
                //printf("m_file_num1:%d\n", m_file_num);
                //print_infor_file();
                // printf("m_file_num2:%d\n", m_file_num);
                //printf("hahah ****\n"); 

                int type =  0;
                type = recv_t.type;
                recv_pack = (PACK*)malloc(sizeof(PACK));
                memcpy(recv_pack, &recv_t, sizeof(PACK));
                

                //开启线程处理接受到的包
                if(pthread_create(&pid,NULL,deal,(void *)recv_pack) != 0)
                    my_err("pthread_create",__LINE__);
                if(type == FILE_SEND)
                {
                    printf("wati...\n");
                    usleep(100000);

                }
            }

        }
    }
    return 0;
}
