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
void mysql_save_message(PACK *recv_pack);
void mysql_close();

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












extern INFO_USER     m_infor_user  [USER_MAX] ;
extern int           m_user_num; 
/**************************************************/


extern INFO_GROUP    m_infor_group [GROUP_MAX] ; //begin from 1
extern int           m_group_num;

/******************file_infor************************************/

extern INFO_FILE     m_infor_file  [FILE_MAX]; //begin from 1
extern int           m_file_num;


extern PACK          m_pack_send  [MAX_CHAR*2];
extern int           m_send_num;


extern unsigned char PADDING[];
  

extern int listenfd,epollfd;//
extern short PORT;
extern int log_file_fd;
extern int user_infor_fd;
extern int group_infor_fd;
extern pthread_mutex_t  mutex;
extern pthread_mutex_t  mutex_recv_file;
extern pthread_mutex_t  mutex_check_file;

