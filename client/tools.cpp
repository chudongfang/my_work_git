#include "client.h"





void my_err(const char * err_string,int line)
{
    fprintf(stderr, "line:%d  ", line);
    perror(err_string);
    exit(1);
}

void sig_close(int i)
{
    //关闭客户端的描述符
    close(sockfd);
    exit(0);
}

/*you need creat the file before!*/
//得到文件大小
int get_file_size(char *file_name)
{
    int fd;
    int len;
    if((fd = open(file_name,O_RDONLY)) == -1)
    {
        my_err("open",__LINE__);
        return 0;
    }
    len = lseek(fd, 0, SEEK_END);
    close(fd);
    return len;
}

//从文件数组中删除文件信息
void file_infor_delete(int id)
{
    pthread_mutex_lock(&mutex_local_user); 
    for(int j = id ;j <=m_recv_num_file_mes ;j++)
    {
        m_pack_recv_file_mes[j]  = m_pack_recv_file_mes[j+1];
    }
    m_recv_num_file_mes--;
    pthread_mutex_unlock(&mutex_local_user); 
}



//判断是否有重复的组
int judge_same_group(char *group_name)
{
    int i;
    for(i=1;i<=m_my_infor.group_num;i++)
    {
        if(strcmp(m_my_infor.group[i],group_name) == 0)
            return 1;
    }
    return 0;
}



//判断是否有重复的好友
int judge_same_friend(char add_friend_t[])
{
    int i;
    for(i=1;i<=m_my_infor.friends_num;i++)
    {
        if(strcmp(m_my_infor.friends[i].name,add_friend_t) == 0)
            return i;
    }
    return 0;
}

//为避免输入时出现的意外错误，进行字符串解析
int get_choice(char *choice_t)
{
    int choice =0;
    for(int i=0;i<strlen(choice_t) ;i++)
        if(choice_t[i]<'0' || choice_t[i]>'9')
            return -1;
    for(int i=0;i<strlen(choice_t);i++)
    {
        int t=1;
        for(int j=1;j<strlen(choice_t)-i;j++)
        {
            t *=10;
        }
        choice += t*(int)(choice_t[i] - 48);
    }
    return choice;
}

//根据参数信息发送包
void send_pack(int type,char *send_name,char *recv_name,char *mes)
{
    PACK pack_send_pack;
    time_t timep;
    pack_send_pack.type = type;
    strcpy(pack_send_pack.data.send_name,send_name);
    strcpy(pack_send_pack.data.recv_name,recv_name);
    strcpy(pack_send_pack.data.mes,mes); 
    time(&timep);
    //pack_send_pack.data.time = timep;
    if(send(sockfd,&pack_send_pack,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }
}


//根据参数信息发送包
void send_pack_memcpy(int type,char *send_name,char *recv_name,char *mes)
{
    PACK pack_send_pack;
    time_t timep;
    pack_send_pack.type = type;
    strcpy(pack_send_pack.data.send_name,send_name);
    strcpy(pack_send_pack.data.recv_name,recv_name);
    memcpy(pack_send_pack.data.mes,mes,MAX_CHAR*2); 
    time(&timep);
   // pack_send_pack.data.time = timep;
    if(send(sockfd,&pack_send_pack,sizeof(PACK),0) < 0){
        my_err("send",__LINE__);
    }
}

