#include "server.h"

/************************************
 * 跟据收到包的内容，找到登录者的信息
 * 做密码比对，并返回相应值
 * *********************************/

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
    else if( md5_judge(id,recv_pack->data.mes)){//the password is not crrect
        login_flag[0] = '1';
        //change user infor
        
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
    
    //包信息赋值
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    strcpy(recv_pack->data.mes,login_flag);
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;
    
    //发送包
    send_pack(recv_pack);
    //考虑到中间的数据传输耗时，停顿若干时间，再修改状态
    usleep(10);
    if(login_flag[0] =='1')
        m_infor_user[id].statu = ONLINE;
    free(recv_pack);
}




/************************************
 * 跟据收到包的内容，判断是否已经注册
 * 如果未注册，添加用户信息，并进行密码
 * 加密
 * *********************************/
void registe(PACK *recv_pack)
{
    char registe_flag[10];
 
    if(judge_usename_same(recv_pack->data.send_name)){//the password is not crrect
         
        registe_flag[0] = '1';
        //add user infor
        m_user_num++;
        strcpy(m_infor_user[m_user_num].username,recv_pack->data.send_name);
        md5(recv_pack->data.mes,m_infor_user[m_user_num].password);
 
        //服务端提示信息
        printf("\n\n\033[;44m&&&&function registe&&&&\033[0m \n");
        printf("\033[;44m*\033[0m registe success!\n");
        printf("\033[;44m*\033[0m username:%s\n", m_infor_user[m_user_num].username);
        //printf("\033[;44m*\033[0m password:%s\n", m_infor_user[m_user_num].password);
        printf("\033[;44m*\033[0m user_num:%d\n\n", m_user_num);
        
                                                      
        m_infor_user[m_user_num].statu = DOWNLINE;
    }
    else registe_flag[0] = '0';
    registe_flag[1] = '\0';
    //包信息赋值
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    strcpy(recv_pack->data.mes,registe_flag);
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;
    
    //发送包
    send_pack(recv_pack);
    free(recv_pack);
}






/**********************friend******************************/

/************************************
 * 跟据收到包的内容，利用字符串处理，
 * 把该客户端的朋友信息返回给客户端
 * *********************************/
void send_statu(PACK *recv_pack)
{
    char str[MAX_CHAR];
    char name_t[MAX_CHAR];
    char send_statu_mes[MAX_CHAR*2];
    int id;
    int count = 0;

    memset(send_statu_mes,0,sizeof(send_statu_mes));
    
    id = find_userinfor(recv_pack->data.send_name);

    send_statu_mes[count++] = m_infor_user[id].friends_num;

    //字符串处理
    for(int i=1 ;i <= m_infor_user[id].friends_num ;i++)
    {
        strcpy(name_t,m_infor_user[id].friends[i]);
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
    //包信息赋值
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    memcpy(recv_pack->data.mes,send_statu_mes,MAX_CHAR*2);
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;

    send_pack(recv_pack);
    free(recv_pack);
}

/************************************
 * 跟据收到包的内容，添加朋友信息
 * *********************************/

void friend_add(PACK *recv_pack)
{
    int id_own,id_friend;
    id_own = find_userinfor(recv_pack->data.send_name);
    strcpy(m_infor_user[id_own].friends[++(m_infor_user[id_own].friends_num)],recv_pack->data.mes);
    id_friend = find_userinfor(recv_pack->data.mes);
    strcpy(m_infor_user[id_friend].friends[++(m_infor_user[id_friend].friends_num)],recv_pack->data.send_name);
    free(recv_pack);
}


/************************************
 * 跟据收到包的内容，删除朋友信息
 * *********************************/
void friend_del(PACK *recv_pack)
{
    int id_own,id_own_1;
    int id_friend,id_friend_1;
    id_own = find_userinfor(recv_pack->data.send_name);
    del_friend_infor(id_own,recv_pack->data.mes); 
    id_friend = find_userinfor(recv_pack->data.mes);
    del_friend_infor(id_friend,recv_pack->data.send_name); 
    free(recv_pack);
}


/************************************
 * 直接发送信息，并存储到mysql数据库
 * *********************************/
void send_mes_to_one(PACK *recv_pack)
{
    //存储到数据库
    mysql_save_message(recv_pack);
    send_pack(recv_pack);
    free(recv_pack);
}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/










/****************group*************************/
/************************************
 * 跟据收到包的内容，首先判断该群组是否已经创建
 * 如果已经创建，则返回出错
 * 否则，创建群，并添加客户端为群主
 * *********************************/
void group_create(PACK *recv_pack)
{
    //判断是否已经建立
    for(int i=1;i<= m_group_num;i++)
    {
        if(strcmp(m_infor_group[i].group_name,recv_pack->data.mes) == 0)
        {
            strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
            strcpy(recv_pack->data.send_name,"server");
            recv_pack->data.mes[0] = 1;
            send_pack(recv_pack);
            free(recv_pack);
            return ;
        }   
    }
    //新建群信息
    strcpy(m_infor_group[++m_group_num].group_name,recv_pack->data.mes);
    strcpy(m_infor_group[m_group_num].member_name[++m_infor_group[m_group_num].member_num],recv_pack->data.send_name);
    int id=find_userinfor(recv_pack->data.send_name);
    strcpy(m_infor_user[id].group[++m_infor_user[id].group_num],recv_pack->data.mes);


    printf("\n\n\033[;32mcreat group : %s  successfully!\033[0m \n\n", recv_pack->data.mes);
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");

    recv_pack->data.mes[0] = 2;
    send_pack(recv_pack);
    free(recv_pack);
}


/************************************
 * 跟据收到包的内容，把客户端信息添加到
 * 群成员中，若没有该群，返回错误
 * *********************************/
void group_join(PACK *recv_pack)
{
    //找到该群，并加入
    for(int i=1;i<= m_group_num;i++)
    {
        if(strcmp(m_infor_group[i].group_name,recv_pack->data.mes) == 0)
        {
            strcpy(m_infor_group[i].member_name[++m_infor_group[i].member_num],recv_pack->data.send_name);
            int id=find_userinfor(recv_pack->data.send_name);
            strcpy(m_infor_user[id].group[++m_infor_user[id].group_num],recv_pack->data.mes);

            strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
            strcpy(recv_pack->data.send_name,"server");
            recv_pack->data.mes[0] = 2; 
            printf("\n\n\033[;32m %s join group : %s  successfully!\033[0m \n\n",recv_pack->data.recv_name, recv_pack->data.mes);
            print_infor_group();
            print_infor_user();
            send_pack(recv_pack);
            free(recv_pack);
            return ;
        }   
    }
    //未找到该群   
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");

    recv_pack->data.mes[0] = 1;
    send_pack(recv_pack);
    free(recv_pack);
}


/************************************
 * 退出群组，从群组中删除该客户端信息
 * 并从客户端信息中删除掉群组
 * *********************************/
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
                    print_infor_group();
                }
            }
        }
    }
}

//删除一个指定ID的群组，并从其群成员的信息中删除该组的信息
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

/************************************
 * 跟据收到包的内容，判断删除者是否为群主（这里令第一个人为群主）
 * 如果是，删除该群
 * 否则  返回错误
 * *********************************/
void group_del(PACK *recv_pack)
{
    for(int i=1;i<=m_group_num;i++)
    {
        if(strcmp(m_infor_group[i].group_name,recv_pack->data.mes) == 0)
        {
            //判断是否为群主
            if(strcmp(m_infor_group[i].member_name[1],recv_pack->data.send_name) == 0)
            {
                group_del_one(i);
                recv_pack->data.mes[0] = 2;
                printf("\n\n\033[;32m delete group : %s  successfully!\033[0m \n\n",recv_pack->data.mes);
            }
            else
                recv_pack->data.mes[0] = 1;
        }
    }
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    send_pack(recv_pack);
    free(recv_pack);

}

/************************************
 * 跟据收到包的内容，利用循环给群成员每一个
 * 人都发一遍，并存储到mysql数据库
 * *********************************/
void send_mes_to_group(PACK *recv_pack)
{

    int id = find_groupinfor(recv_pack->data.recv_name);
    int len_mes = strlen(recv_pack->data.mes);
    char send_name[SIZE_PASS_NAME];

    for(int i=len_mes;i>=0;i--){
        recv_pack->data.mes[i+SIZE_PASS_NAME] = recv_pack->data.mes[i];
    }
    
    strcpy(send_name ,recv_pack->data.send_name);
    for(int i=0;i<SIZE_PASS_NAME;i++){
        recv_pack->data.mes[i] = recv_pack->data.send_name[i];
    }
    strcpy(recv_pack->data.send_name,recv_pack->data.recv_name);

    for(int i=1; i<=m_infor_group[id].member_num;i++)
    {
        strcpy(recv_pack->data.recv_name,m_infor_group[id].member_name[i]);
        if(strcmp(send_name , m_infor_group[id].member_name[i]) != 0)
        {
            mysql_save_message(recv_pack);
            //存储到数据库
          
        
            send_pack(recv_pack);
        }
    }
    free(recv_pack);
}


/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/










/*****************file*******************/

/************************************
 * 跟据收到包的内容，客户端请求发送文件
 * 首先判断是否已经有该文件信息
 * 如果有，返回该文件的大小
 * 否则，建立文件信息，并返回文件大小为0
 * *********************************/
void file_recv_begin(PACK *recv_pack)
{
    int flag = 0;
    int i;
    int file_size_now_t;

pthread_mutex_lock(&mutex_check_file);  
    for(i=1 ;i<= m_file_num ;i++)
    {
        //文件存在
        if(strcmp(m_infor_file[i].file_name,recv_pack->data.mes+NUM_MAX_DIGIT) == 0)
        {
            file_size_now_t = m_infor_file[i].file_size_now;
            flag = 1;
            break;
        } 
    }
    //不存在，则建立文件信息
    if(!flag)
    {
        file_size_now_t = 0;
        strcpy(m_infor_file[++m_file_num].file_name,recv_pack->data.mes+NUM_MAX_DIGIT);
        strcpy(m_infor_file[m_file_num].file_send_name,recv_pack->data.send_name);
        strcpy(m_infor_file[m_file_num].file_recv_name,recv_pack->data.recv_name);
        
        //解析文件大小
        int t=0;
        for(int k=0;k<NUM_MAX_DIGIT;k++)
        {
            if(recv_pack->data.mes[k] == -1)
                break;
            int t1 = 1;
            for(int l=0 ;l<k;l++)
                t1*=10;
            t += (int)(recv_pack->data.mes[k])*t1;
        }
       //建立文件信息 
        m_infor_file[m_file_num].file_size = t;
        m_infor_file[m_file_num].file_size_now  = 0;
        m_infor_file[m_file_num].flag = FILE_STATU_RECV_ING;
    }

pthread_mutex_unlock(&mutex_check_file);  
    
    recv_pack->type = FILE_SEND_BEGIN_RP;
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    
    int digit = 0;
    while(file_size_now_t != 0)
    {   
        recv_pack->data.mes[digit++] = file_size_now_t%10;
        file_size_now_t /= 10;
    }
    recv_pack->data.mes[digit]  = -1;
    
    send_pack(recv_pack);
    free(recv_pack);
}


/************************************
 * 跟据收到包的内容,接收到文件内容
 * 就把文件内容写入到文件中
 * *********************************/
void file_recv(PACK *recv_pack)
{
//pthread_mutex_lock(&mutex_recv_file);
    int fd;
    char file_name[MAX_CHAR];
    char file_path_t[SIZE_PASS_NAME];
    

    int  len = 0;
    for(int i=0 ;i<NUM_MAX_DIGIT ;i++)
    {
        if(recv_pack->data.mes[i] == -1)  
            break;
        int t1 = 1;
        for(int l=0;l<i;l++)
            t1*=10;
        len += (int)recv_pack->data.mes[i]*t1;

    }


    
    strcpy(file_name,recv_pack->data.recv_name);
    if((fd = open(file_name,O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)) < 0)
    {
        my_err("open",__LINE__);
        return ;
    }

    if(write(fd,recv_pack->data.mes + NUM_MAX_DIGIT,len) < 0)
        my_err("write",__LINE__);
    // 关闭文件 
    close(fd);
    
    int id  =  find_fileinfor(file_name);
    m_infor_file[id].file_size_now += len;
    free(recv_pack);
    //pthread_mutex_unlock(&mutex_recv_file);  
    //输出文件信息
    print_infor_file();
}




/************************************
 * 根据文件的起始位置，开始发送，
 * 在发送过程中，不断监测接收端的状态
 * 若接收端下线，则向接收端发送提醒消息
 * *********************************/
void *file_send_send(void *file_send_begin_t)
{

    PTHREAD_PAR *file_send_begin = (PTHREAD_PAR *)file_send_begin_t;
    int fd;
    int length;
    int statu;
    int id;
    int sockfd;
    int file_size;
    int begin_location = file_send_begin->a;
    int sum = begin_location;
    char file_name[SIZE_PASS_NAME];
    char recv_name[SIZE_PASS_NAME];
    char mes[MAX_CHAR*2];
    printf("\n\nsending the file.........\n");
    
    strcpy(file_name,file_send_begin->str1);
    strcpy(recv_name,file_send_begin->str2);

    if((fd = open(file_name,O_RDONLY)) == -1)
    {
        my_err("open",__LINE__);
        return NULL;
    }
    
    file_size=lseek(fd, 0, SEEK_END);
    //定位到开始位置
    lseek(fd ,begin_location ,SEEK_SET);
    printf("file_size %d\n", file_size);
    printf("begin_location%d\n", begin_location);
    bzero(mes, MAX_CHAR*2); 

    // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止 
    while((length = read(fd  ,mes+NUM_MAX_DIGIT ,MAX_CHAR*2 - NUM_MAX_DIGIT)) > 0) 
    {
        printf("send_::%d\n", sum);
        printf("file_size%d\n",file_size);
        if(sum >= file_size)  
            break;

        id = find_userinfor(recv_name);
        //当文件传输过程中，接收端下线，则向接收端发送提醒消息
        if(m_infor_user[id].statu == DOWNLINE)
        {
            int  file_id   = find_fileinfor(file_name);
            int  file_size = m_infor_file[file_id].file_size;
            char mes_t[MAX_CHAR];
            PACK file_send_stop_t;

            memset(&file_send_stop_t, 0,sizeof(PACK));
            
            file_send_stop_t.type = FILE_RECV_STOP_RP;
            strcpy(file_send_stop_t.data.send_name ,m_infor_file[file_id].file_send_name);
            strcpy(file_send_stop_t.data.recv_name,m_infor_file[file_id].file_recv_name);
            
            memset(mes_t,0,sizeof(mes_t));
            //发送服务端已经发送的字节数
            int digit = 0;
            while(file_size != 0)
            {   
                mes_t[digit++] = file_size%10;
                file_size /= 10;
            }
            mes_t[digit]  = -1;
            for(int j=0 ;j< SIZE_PASS_NAME ;j++)
            {
                mes_t[NUM_MAX_DIGIT+j] = m_infor_file[file_id].file_name[j];
            }  
            memcpy(file_send_stop_t.data.mes,mes_t,sizeof(mes_t));  
            
            send_pack(&file_send_stop_t);
            free(file_send_begin);
            return NULL ;
        }

        sockfd = m_infor_user[id].socket_id;
        
        //在发送数据的前面，为文件已发送的字节数 
        int t=length;
        int digit = 0;
        while(t != 0)
        {   
            mes[digit++] = t%10;
            t /= 10;
        }
        mes[digit]  = -1;
        
        send_pack_memcpy_server(FILE_RECV,file_name,recv_name,sockfd,mes);
        
        
        sum += length; 
        bzero(mes, MAX_CHAR*2); 
        //为防止发送顺序被打乱，发送时有一定时间间隔
        usleep(200000);
    } 
    // 关闭文件 
    close(fd);
    free(file_send_begin);
}

/************************************
 * 客户端请求接收文件，解析出客户端已经
 * 接收的文件字节数，然后从该字节数开始发送
 * *********************************/

void file_send_begin(PACK *recv_pack)
{
    PTHREAD_PAR *file_send_begin_t;
    file_send_begin_t = (PTHREAD_PAR *)malloc(sizeof(PTHREAD_PAR));
    char recv_name[SIZE_PASS_NAME];
    char file_name[SIZE_PASS_NAME];
    int  begin_location=0;
    strcpy(recv_name,recv_pack->data.send_name);
    strcpy(file_name,recv_pack->data.recv_name);
   
    //解析出客户端已经接收文件字节数
    for(int i=0 ;i<NUM_MAX_DIGIT ;i++)
    {
        if(recv_pack->data.mes[i] == -1)  
            break;
        int t1 = 1;
        for(int l=0;l<i;l++)
            t1*=10;
        begin_location += (int)recv_pack->data.mes[i]*t1;
    }
    int file_id = find_fileinfor(file_name);

    if(begin_location >= m_infor_file[file_id].file_size)
    {
        m_infor_file[file_id].flag  = FILE_STATU_SEND_FINI;
    }

    file_send_begin_t->a = begin_location;
    strcpy(file_send_begin_t->str1,file_name);
    strcpy(file_send_begin_t->str2,recv_name);
    //断点续传
    file_send_send((void *)file_send_begin_t);
    free(recv_pack);
}

/************************************
 * 客户端接收完，会发送确认信息，
 * 然后把文件状态改为:已经发送完毕
 * 其他函数对该文件进行相应处理
 * *********************************/
void file_send_finish(PACK *recv_pack)
{   
    int id = find_fileinfor(recv_pack->data.mes);
    m_infor_file[id].flag = FILE_STATU_SEND_FINI;
    free(recv_pack);
}



/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/





