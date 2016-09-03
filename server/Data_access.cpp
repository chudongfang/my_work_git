#include "server.h"


//数据库全局变量
/********************mysql*************************/
MYSQL           mysql;
MYSQL_RES       *res = NULL;
MYSQL_ROW       row;
char            query_str[MAX_CHAR*4] ;
int             rc, fields;
int             rows;



//从文件中读取用户信息
int read_infor()
{
    INFO_USER read_file_t;
    if((user_infor_fd = open("user_infor.txt",O_RDONLY | O_CREAT , S_IRUSR | S_IWUSR)) < 0)
    {
        my_err("open",__LINE__);
        return -1;
    } 
    int length ;
    while((length = read(user_infor_fd, &read_file_t, sizeof(INFO_USER))) > 0) 
    {
        read_file_t.statu = DOWNLINE;
        m_infor_user[++m_user_num]  = read_file_t;
    } 
    close(user_infor_fd);
    printf("have read the user_infor.txt.\n");


    INFO_GROUP read_file_group;

    if((group_infor_fd = open("group_infor.txt",O_RDONLY | O_CREAT , S_IRUSR | S_IWUSR)) < 0)
    {
        my_err("open",__LINE__);
        return -1;
    } 

    while((length = read(group_infor_fd, &read_file_group, sizeof(INFO_GROUP))) > 0) 
    {
        m_infor_group[++m_group_num]  = read_file_group;
    } 
    close(group_infor_fd);
    printf("have read the group_infor.txt.\n");
}

//服务端关闭时把信息写入到文件中
int write_infor()
{
    INFO_USER read_file_t;
    if((user_infor_fd = open("user_infor.txt",O_WRONLY | O_CREAT |O_TRUNC , S_IRUSR | S_IWUSR)) < 0)
    {
        my_err("open",__LINE__);
        return -1;
    } 
    int length ;
    for(int i= 1 ; i<=m_user_num ;i++)
    {
        if(write(user_infor_fd, m_infor_user+i, sizeof(INFO_USER)) < 0)
        {
            my_err("close",__LINE__);
            return -1;
        }
    }
    close(user_infor_fd);
    printf("have save the user_infor.txt\n");

    


    if((group_infor_fd = open("group_infor.txt",O_WRONLY | O_CREAT |O_TRUNC , S_IRUSR | S_IWUSR)) < 0)
    {
        my_err("open",__LINE__);
        return -1;
    } 
    for(int i= 1 ; i<=m_group_num ;i++)
    {
        if(write(group_infor_fd, m_infor_group+i, sizeof(INFO_GROUP)) < 0)
        {
            my_err("close",__LINE__);
            return -1;
        }
    }

    close(group_infor_fd);
    printf("have save the group_infor.txt.\n");
}


//链接mysql数据库
/***********************mysql**********************************/
int conect_mysql_init()
{
    if (NULL == mysql_init(&mysql)) {               //初始化mysql变量
        printf("mysql_init(): %s\n", mysql_error(&mysql));
        return -1;
    }

    if (NULL == mysql_real_connect(&mysql,         //链接mysql数据库
                "localhost",                       //链接的当地名字
                "root",                            //用户名字
                "root",                            //用户密码
                "happychat",                          //所要链接的数据库
                0,
                NULL,
                0)) {
        printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
        return -1;
    }
    printf("1. Connected MySQL successful! \n");
}

void mysql_save_message(PACK *recv_pack)
{
    #ifdef MYSQL_OPEN
    sprintf(query_str, "insert into message_tbl values ('%s','%s' ,'%s')",recv_pack->data.send_name,recv_pack->data.recv_name,recv_pack->data.mes);
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));     //对数据库执行 SQL 语句
    if (0 != rc) {
        printf("mysql_real_query(): %s\n", mysql_error(&mysql));
        return ;
    }
    printf("the message get into the mysql!!!\n");
    #else
    #endif
}


void mysql_close()
{
    mysql_free_result(res);
    mysql_close(&mysql);
}

/************************************
 * 客户端请求历史记录，通过查询，把mysql数据
 * 库中的信息导出，发送给客户端
 * *********************************/
void send_record(PACK *recv_pack)
{
    char send_name[MAX_CHAR];
    char recv_name[MAX_CHAR];
    char mes[MAX_CHAR*2];
    PACK *pack_send_record_t = (PACK *)malloc(sizeof(PACK));
    strcpy(send_name,recv_pack->data.send_name);
    strcpy(recv_name,recv_pack->data.mes);
    sprintf(query_str,"select send_name ,recv_name,mes from message_tbl where send_name='%s' or send_name = '%s'",send_name,recv_name);            //显示 表中数据
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    if (0 != rc) {
        printf("mysql_real_query(): %s\n", mysql_error(&mysql));
        return ;
    }

    res = mysql_store_result(&mysql);                             //获取查询结果
    if (NULL == res) {
         printf("mysql_restore_result(): %s\n", mysql_error(&mysql));
         return ;
    }
    int rows = mysql_num_rows(res);                                  //获取查询结果的行数  
        printf("The total rows is: %d\n", rows);                        
      
    int fields = mysql_num_fields(res);                              //获取查询结果的列数  
        printf("The total fields is: %d\n", fields);   
    //发送查询到的数据
    while ((row = mysql_fetch_row(res))) 
    {
        
        pack_send_record_t->type = MES_RECORD;
        strcpy(pack_send_record_t->data.send_name,"server");
        strcpy(pack_send_record_t->data.recv_name,send_name);
        strcpy(pack_send_record_t->data.mes,row[0]);
        //strcpy(pack_send_record_t->data.mes+SIZE_PASS_NAME,row[3]);
        strcpy(pack_send_record_t->data.mes+SIZE_PASS_NAME,row[2]);
        printf("%s\n",pack_send_record_t->data.mes+SIZE_PASS_NAME);
        send_pack(pack_send_record_t);
        usleep(100000);
        
    }
    usleep(100000);
    printf("haha\n");
   //发送包，表示已经传输完毕
    pack_send_record_t->type = MES_RECORD;
    strcpy(pack_send_record_t->data.send_name,"server_end");
    strcpy(pack_send_record_t->data.recv_name,send_name);
    send_pack(pack_send_record_t);

    usleep(10000);
    free(pack_send_record_t);
}






