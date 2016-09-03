#include <mysql/mysql.h> 
#include <gtk/gtk.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <termios.h>  

/*****************same with server**********************/
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


#define SIZE_PASS_NAME   10
#define MAX_PACK_CONTIAN 100
#define MAX_CHAR         300
#define NUM_MAX_DIGIT    10

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








/******************be sure same with server*******************************/
typedef struct datas{
    char    send_name[MAX_CHAR];
    char    recv_name[MAX_CHAR];
    int     send_fd;
    int     recv_fd;
    //time_t  time;
    char    mes[MAX_CHAR*2];
}DATA;

typedef struct package{
    int type;
    DATA  data;
}PACK;

typedef struct pthread_parameter
{
    int a;
    int b;
}PTHREAD_PAR;



typedef struct prinit_mes
{
    char name[MAX_CHAR];
    char time[MAX_CHAR];
    char mes [MAX_CHAR];
    
}PRINT_MES;

typedef struct sockaddr SA;



void init();
void *deal_statu(void *arg);
void *clien_recv_thread(void *arg);
void init_clien_pthread();
int main_menu();




int send_login(char username_t[],char password_t[]);
int login();
int send_registe(char username_t[],char password_t[]);
void registe();
void get_status_mes();
void change_statu(PACK pack_deal_statu_t);
void add_friend();
void del_friend();
void group_create();
void group_join();
void group_qiut();
void group_del();
void send_mes_to_one();
void send_mes_to_group();
void send_mes(char mes_recv_name[],int type);
void *show_mes(void *username);
void print_mes(int id);
void send_file();
void *pthread_send_file(void *mes_t);
void send_file_send(int begin_location,char *file_path);
int file_mes_box();
void deal_file_mes(int id);
void mes_sendfile_fail(int id);
void mes_recv_requir(int id);
void mes_recvfile_fail(int id);
void *pthread_recv_file(void *par_t);
int mes_record();
void print_mes_record(PACK pack_t);



int login_menu();
void show_mes_smart(char *name  ,char *mes);
void friends_see();
void group_see();
void print_main_menu();


void my_err(const char * err_string,int line);
void sig_close(int i);
int get_file_size(char *file_name);
void file_infor_delete(int id);
int judge_same_group(char *group_name);
int judge_same_friend(char add_friend_t[]);
int get_choice(char *choice_t);
void send_pack(int type,char *send_name,char *recv_name,char *mes);
void send_pack_memcpy(int type,char *send_name,char *recv_name,char *mes);




extern USER_INFOR m_my_infor;

extern PACK m_pack_send   [MAX_CHAR];
extern int  m_send_num;


/*****************recv*********************/
extern PACK m_pack_recv_friend_see   [MAX_PACK_CONTIAN];
extern PACK m_pack_recv_chat         [MAX_PACK_CONTIAN];
extern PACK m_pack_recv_send_file    [MAX_PACK_CONTIAN];
extern PACK m_pack_recv_file_mes     [MAX_PACK_CONTIAN];
extern PACK m_pack_recv_file         [MAX_PACK_CONTIAN];

extern int m_recv_num_friend_see;
extern int m_recv_num_chat;
extern int m_recv_num_send_file;
extern int m_recv_num_file_mes;
extern int m_recv_num_file;
extern int m_print_mes_flag;

/****************************************************/

extern int m_flag_group_create;
extern int m_flag_group_join ;
extern int m_flag_group_del  ;
extern int m_flag_print_mes;
extern int m_flag_print_recorde;
// int window_col;
// int window_row;
/****************************************************/

extern PRINT_MES m_print_mes[10];
extern int m_print_mes_num;
/***********************function***********************/
extern int sockfd;
extern char *IP ;
extern short PORT ;

extern pthread_mutex_t  mutex_local_user;
extern pthread_mutex_t  mutex_recv_file;
