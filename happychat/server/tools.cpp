#include "server.h"

//输出错误信息
void my_err(const char * err_string,int line)
{
    fprintf(stderr, "line:%d  ", line);
    perror(err_string);
    exit(1);
}

//根据姓名找到用户信息
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



//关闭服务器前 关闭服务器的socket
void signal_close(int i)
{
    #ifdef LOG
        close(log_file_fd);
    #else
    #endif
    
    write_infor(); 
    mysql_close();
    printf("服务器已经关闭\n");
    exit(1);
}


//发送包，把包的内容赋值到发送包的数组
void send_pack(PACK *pack_send_pack_t)
{
    pthread_mutex_lock(&mutex);  
    memcpy(&(m_pack_send[m_send_num++]),pack_send_pack_t,sizeof(PACK));
    pthread_mutex_unlock(&mutex);  
}


//根据参数信息发送包
void send_pack_memcpy_server(int type,char *send_name,char *recv_name,int sockfd1,char *mes)
{
    PACK pack_send_pack;
    time_t timep;
    pack_send_pack.type = type;
    strcpy(pack_send_pack.data.send_name,send_name);
    strcpy(pack_send_pack.data.recv_name,recv_name);
    memcpy(pack_send_pack.data.mes,mes,MAX_CHAR*2); 
    time(&timep);
    //pack_send_pack.data.time = timep;
    printf("sockfd1:%d\n", sockfd1);
    if(send(sockfd1,&pack_send_pack,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }
}


//判断客户端登录密码每是否正确
bool md5_judge(int id,char *log_pass)
{
    unsigned int md5_t[4];
    md5(log_pass,md5_t);
    for(int i=0;i<4;i++)
    {
        if(m_infor_user[id].password[i] != md5_t[i])
            return false;
    }
    return true;
}


//判断用户信息中是否有当前名字的信息
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


//给定用户ID，从用户信息删除好友
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


//给定用户信息，从用户信息中删除组
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


//给定组名，找到信息在数组中位置
int find_groupinfor(char groupname_t[])
{
    int i;
    if(m_group_num == 0)  return 0;
    for(i=1;i<=m_group_num;i++)
    {
        if(strcmp(m_infor_group[i].group_name,groupname_t) == 0)
            return i;
    }
    if(i == m_group_num+1) 
        return 0;
}


//给定文件名，返回信息在数组中位置
int find_fileinfor(char *filename)
{
    for(int i=1 ;i <= m_file_num ;i++)
    {
        if(strcmp(filename , m_infor_file[i].file_name) == 0)
        {
            return i;
        }
    }
    return 0;
}

