#include "server.h"

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

void print_infor_group()
{
    for(int i=1;i<=m_group_num;i++)
    {
        printf("\n\n********%d*********\n", i);
        printf("group_name  :%s\n", m_infor_group[i].group_name);
        printf("group_num   :%d\n", m_infor_group[i].member_num);
        for(int j=1 ;j<=m_infor_group[i].member_num;j++)
        printf("*%s\n", m_infor_group[i].member_name[j]);
        printf("***********************\n\n\n");
    }
}


void print_infor_user()
{
    for(int i=1;i<=m_user_num;i++)
    {
        printf("\n**\033[;33m *****user***%d*********\033[0m \n", i);
        printf("user_name  :%s\n", m_infor_user[i].username);
        printf("int  statu :%d\n", m_infor_user[i].statu);
        printf("friends_num:%d\n", m_infor_user[i].friends_num);
        for(int j=1 ;j<=m_infor_user[i].friends_num;j++)
        printf("*%s\n", m_infor_user[i].friends[j]);

        printf("user_num   :%d\n", m_infor_user[i].group_num);
        for(int j=1 ;j<=m_infor_user[i].group_num;j++)
        printf("*%s\n", m_infor_user[i].group[j]);
        printf("***********************\n\n\n");

    }
}

void print_infor_file()
{
    pthread_mutex_lock(&mutex_check_file);  
    for(int i=1;i<=m_file_num;i++)
    {  
        printf("\n\n****file***%d*********\n", i);
        printf("file_name       :%s\n", m_infor_file[i].file_name);
        printf("send_name       :%s\n", m_infor_file[i].file_send_name);
        printf("recv_name       :%s\n", m_infor_file[i].file_recv_name);
        printf("file_size       :%d\n", m_infor_file[i].file_size);
        printf("file_size_now   :%d\n", m_infor_file[i].file_size_now);
        printf("flag            :%d\n", m_infor_file[i].flag);
        printf("***********************\n\n\n");

    }
    pthread_mutex_unlock(&mutex_check_file);  
}
