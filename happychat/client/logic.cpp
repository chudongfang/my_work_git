#include "client.h"





/*
* 向服务端发送登陆信息
*/

int send_login(char username_t[],char password_t[])
{
    PACK recv_login_t;
    int login_judge_flag = 0;
    send_pack(LOGIN,username_t,"server",password_t);
     
    if(recv(sockfd,&recv_login_t,sizeof(PACK),0) < 0){
        my_err("recv",__LINE__);
    }
    login_judge_flag = recv_login_t.data.mes[0] - 48;
    return login_judge_flag;
}

/*
* 根据输入信息向客户端发送登陆请求
* 并根据返回内容，提示用户
*/
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
        printf("\033[;31mthe username is not exit.\033[0m\n");
        return 0;
    }   
    if(login_flag ==  3 ){
        printf("\033[;31mthe user has loged in .\033[0m\n");
        return 0;
    }  
    if(login_flag == 0) {
        printf("\033[;31mthe password is not crrect.\033[0m\n");
        return 0;
    }
    strcpy(m_my_infor.username,username_t);
    printf("\033[;32mload successfully!\033[0m\n");
    return 1;
}








/*
* 向服务端发送注册信息
*/
int send_registe(char username_t[],char password_t[])
{
    PACK recv_registe_t;
    int send_registe_flag;
    
    send_pack(REGISTER,username_t,"server",password_t);
    
    if(recv(sockfd,&recv_registe_t,sizeof(PACK),0) < 0){
        my_err("recv",__LINE__);
    }
    send_registe_flag = recv_registe_t.data.mes[0] - 48;
    return send_registe_flag;
}

/*
* 根据输入信息向客户端发送注册请求
* 并根据返回内容，提示用户
*/
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
        printf("\033[;32mregiste successfully!\033[0m\n");
    else 
        printf("\033[;31mthe name is used ,please input another one\033[0m\n");
}  



/*
 * 向服务端请求更新好友状态
*/
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

//根据服务端发送来的包，利用字符串解析，更新当前好友状态
void change_statu(PACK pack_deal_statu_t)
{
    int count = 0;
    m_my_infor.friends_num=pack_deal_statu_t.data.mes[count++];
    //更新好友信息
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
    //更新群组信息
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


/**************************************/





//添加好友
void add_friend()
{
    char add_friend_t[MAX_CHAR];
    
    printf("please input the name of friend you want to add:\n");
    scanf("%s",add_friend_t);
    if(strcmp(add_friend_t,m_my_infor.username) == 0)
    {
        printf("you can't add youself to be your friend!\n");
        return;
    }
    //判断是否已经添加过该好友
    if(judge_same_friend(add_friend_t))
    {
        printf("you already have same friends!\n");
        return ;
    }
    printf("m_my_infor.username:%s\n", m_my_infor.username);
    send_pack(FRIEND_ADD,m_my_infor.username,"server",add_friend_t);
    get_status_mes();
}



//删除好友
void del_friend()
{
    char del_friend_t[MAX_CHAR];
    printf("please input the name of friend you want to delete:\n");
    scanf("%s",del_friend_t);
    //判断是否添加过该好友
    if(!judge_same_friend(del_friend_t))
    {
        printf("you don't have this friends on list!\n");
        return ;
    }
    printf("m_my_infor.username:%s\n", m_my_infor.username);
    //发送包
    send_pack(FRIEND_DEL,m_my_infor.username,"server",del_friend_t);
    //得到状态信息
    get_status_mes();
}








//创建群组
void group_create()
{
    char group_name[MAX_CHAR];
    printf("please input the group name you want to create:\n");
    scanf("%s",group_name);
    send_pack(GROUP_CREATE,m_my_infor.username,"server",group_name);
    while(!m_flag_group_create);
    printf("m_flag_group_create=%d\n", m_flag_group_create);
    if(m_flag_group_create == 2) 
        printf("create group successfully!\n");
    else if(m_flag_group_create == 1)
        printf("this group has been created!\n");
    m_flag_group_create = 0;
}


//加入群组
void group_join()
{

    char group_name[MAX_CHAR];
    printf("please input the group name you want to join:\n");
    scanf("%s",group_name);
    //判断是否已经加入该群
    for(int i=1;i <= m_my_infor.group_num ;i++)
    {
        if(strcmp(m_my_infor.group[i],group_name) == 0)
        {
            printf("you have join this group!\n");
            return ;
        }
    }

    send_pack(GROUP_JOIN,m_my_infor.username,"server",group_name);
    while(!m_flag_group_join);
    if(m_flag_group_join == 2) 
        printf("join group successfully!\n");
    else if(m_flag_group_join == 1)
        printf("there is no group named %s\n",group_name);
    m_flag_group_join = 0;
}

//退出群组
void group_qiut()
{
    char group_name[MAX_CHAR];
    printf("please input the group name you want to qiut:\n");
    scanf("%s",group_name);
    //判断是否添加过该群组
    for(int i=1;i <= m_my_infor.group_num ;i++)
    {
        if(strcmp(m_my_infor.group[i],group_name) == 0)
        {
            send_pack(GROUP_QIUT,m_my_infor.username,"server",group_name);
            printf("quit group %s successfully!\n",group_name);
            return ;
        }
    }
    
    printf("you did't join this group!\n");
}

//解散群
void group_del()
{
    char group_name[MAX_CHAR];
    printf("please input the group name you want to delete:\n");
    scanf("%s",group_name);
    for(int i=1;i <= m_my_infor.group_num ;i++)
    {
        if(strcmp(m_my_infor.group[i],group_name) == 0)
        {
            send_pack(GROUP_DEL,m_my_infor.username,"server",group_name);
            while(!m_flag_group_del);
            printf("m_flag_group_del=%d\n", m_flag_group_del);
            if(m_flag_group_del == 2) 
                printf("delete group successfully!\n");
            else if(m_flag_group_del == 1)
                printf("you isn't the owner of group %s\n",group_name);
            return ;
        }
    }
    
    printf("you did't join this group!\n");
}





















/*************************************************/

//私聊
void send_mes_to_one()
{
    pthread_t pid;
    int id;
    char mes_recv_name[MAX_CHAR];
    friends_see();//print friend list !

    printf("please input the name you want to chat\n");
    scanf("%s",mes_recv_name);
    if (!(id=judge_same_friend(mes_recv_name)))
    {
        printf("sorry,you don't have the friend named !%s\n",mes_recv_name);
        return ;
    }
    printf("\33[2J \33[1;1H\033[;33m\33[1m-^-^-^-^-^-^-^-^-^-^-message-^-^-^-^-^-^-^-^-^-^-\033[0m");
    printf("\33[26B\n");
    m_flag_print_mes = 1;
    m_my_infor.friends[id].mes_num = 0;
    //开启线程显示信息
    pthread_create(&pid,NULL,show_mes,(void *)mes_recv_name);
    send_mes(mes_recv_name,CHAT_ONE);
}

//群聊
void send_mes_to_group()
{
    pthread_t pid;
    char mes_recv_group_name[MAX_CHAR];
    group_see();
    printf("please input the group you want to chat\n");
    scanf("%s",mes_recv_group_name);
    if (!judge_same_group(mes_recv_group_name))
    {
        printf("sorry,you don't have the group named !%s\n",mes_recv_group_name);
        return ;
    }

    m_flag_print_mes = 1;
    printf("\33[2J \33[1;1H\033[;33m\33[1m-^-^-^-^-^-^-^-^-^-^-message-^-^-^-^-^-^-^-^-^-^-\033[0m");
    printf("\33[26B\n");
    //开启线程显示信息
    pthread_create(&pid,NULL,show_mes,(void *)mes_recv_group_name);
    send_mes(mes_recv_group_name,CHAT_MANY);
}


//接收用户输入并发送信息到客户端
void send_mes(char mes_recv_name[],int type)
{
    PACK pack_send_mes;
    char mes[MAX_CHAR];
    time_t timep;
    getchar();
    printf("\033[;34m\33[1m-^-^-^-^-^-^-^-^-^-^-please input-^-^-^-^-^-^-^-^-^-^-\033[0m\n");
    while(1)
    {   
        time(&timep);
        memset(mes,0,sizeof(mes));

        //if(type == CHAT_ONE)
            // printf("%s->",m_my_infor.username);
        fgets(mes,MAX_CHAR,stdin);
        while(mes[0] == 10)
        {
            printf("\33[1A");
            fflush(stdout);
            fgets(mes,MAX_CHAR,stdin);
        }

        //当用户输入quit时退出
        if(strcmp(mes,"quit\n") == 0)
            break;
        printf("\33[29;0H");
        fflush(stdout);
        printf("\33[K\n\33[K\n\33[K\n");
        fflush(stdout);
        printf("\33[29;0H");
        fflush(stdout);

        //输入的的同时，输出信息
        show_mes_smart(m_my_infor.username ,mes);
        
        //printf("\t%s\n%s\n", m_my_infor.username,ctime(&timep),mes);
        //发送给客户端
        send_pack(type,m_my_infor.username,mes_recv_name,mes);
    }
    m_flag_print_mes = EXIT;
}

/*
 * 在聊天的同时启动,启动线程读取，存储区域的
 * 消息，并显示出来
*/
void *show_mes(void *username)
{
    int id;
    char *user_name = (char *)username;
    while(1)
    {
        //如果退出聊天，就不再显示
        if (m_flag_print_mes == EXIT)
            break;
        pthread_mutex_lock(&mutex_local_user); 
        id = 0;

        //检索信息
        for(int i = 1 ;i <= m_recv_num_chat; i++)
        {
            if(strcmp(m_pack_recv_chat[i].data.send_name,user_name) == 0)
            {
                id = i;

                //输出信息
                print_mes(id);
                m_recv_num_chat--;
                for(int j = id; j <= m_recv_num_chat&&m_recv_num_chat ;j++)
                {
                    m_pack_recv_chat[j]  =  m_pack_recv_chat[j+1];
                }
                break;
            }
        }
        
        pthread_mutex_unlock(&mutex_local_user); 
        usleep(1);    
    }
}

//根据寻找到的包把信息输出
void print_mes(int id)
{
    char group_print_name[MAX_CHAR];
    memset(group_print_name,0,sizeof(group_print_name));
    if(m_pack_recv_chat[id].type == CHAT_ONE)
    {
        show_mes_smart(m_pack_recv_chat[id].data.send_name,m_pack_recv_chat[id].data.mes);
    }
    else
    {
        for(int i=0;i<SIZE_PASS_NAME;i++)
        {
            group_print_name[i] = m_pack_recv_chat[id].data.mes[i];
        }
        show_mes_smart( group_print_name,m_pack_recv_chat[id].data.mes+SIZE_PASS_NAME);
    }
}



/*******************************************/







/*
 * 向服务端请求发送文件，并将文件的大小
 * 发送给服务端
*/

void send_file()
{
    char  recv_name[MAX_CHAR];
    char  file_path[MAX_CHAR];
    int   file_size_t;
    char  mes_t[MAX_CHAR];
    friends_see();
    printf("please input the friend name:\n");
    scanf("%s",recv_name);

    int id = judge_same_friend(recv_name);
    if(id == 0)
    {
        printf("you don't hava this friend!\n");
        return ;
    }
    printf("please input the path of file you want to send :\n");
    scanf("%s",file_path);
    
    //得到文件的大小
    file_size_t = get_file_size(file_path);
    printf("file_size :%d\n", file_size_t);

    if(file_size_t == 0)
    {
        printf("please input creact file path\n");
        return ;
    }
    //字符串分析
    int digit = 0;
    while(file_size_t != 0)
    {   
        mes_t[digit++] = file_size_t%10;
        file_size_t /= 10;
    }
    mes_t[digit]  = -1;
   
    for(int i=0 ;i< SIZE_PASS_NAME ;i++)
    {
        mes_t[NUM_MAX_DIGIT+i] = file_path[i];
    }
    //发送请求
    send_pack_memcpy(FILE_SEND_BEGIN,m_my_infor.username,recv_name,mes_t);
}



//当接收到允许发送的消息时，开启线程发送文件
void *pthread_send_file(void *mes_t)
{
    char *mes = (char *)mes_t;
    int begin_location = 0;
    char file_name[MAX_CHAR];
    printf("\033[;31m \nfunction:pthread_send_file\033[0m \n");
    //解析到服务端已接收文件大小
    for(int i=0 ;i<NUM_MAX_DIGIT ;i++)
    {
        if(mes[i] == -1)  
            break;
        int t1 = 1;
        for(int l=0;l<i;l++)
            t1*=10;
        begin_location += (int)mes[i]*t1;

    }
    strcpy(file_name,mes+NUM_MAX_DIGIT);
    send_file_send(begin_location,file_name);
}


//从起始位置向服务端发送文件
void send_file_send(int begin_location,char *file_path)
{
    int fd;
    int length;
    int file_size;
    int sum = begin_location;
    char mes[MAX_CHAR*2];
    printf("\33[2J\33[1;1H\n");
    printf("\033[;34m\33[1m^-^-^-^-^-^-^-^-^-^happychat^-^-^-^-^-^-^-^-^-^\033[0m\n");
    printf("\n\nsending the file.........\n");
    
    //打开文件
    if((fd = open(file_path,O_RDONLY)) == -1)
    {
        my_err("open",__LINE__);
        return ;
    }
    file_size=lseek(fd, 0, SEEK_END);
    
    printf("file_size=%d",file_size);
    //文件内部指针移动
    lseek(fd ,begin_location ,SEEK_SET);

    bzero(mes, MAX_CHAR*2); 

    // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止 
    while((length = read(fd  ,mes+NUM_MAX_DIGIT ,MAX_CHAR*2 - NUM_MAX_DIGIT)) > 0) 
    {
        sum += length;
      //  printf("length = %d\n", length);
        int digit = 0;
        while(length != 0)
        {   
            mes[digit++] = length%10;
            length /= 10;
        }
        mes[digit]  = -1;
        int color;
        int jindu=(int)((double)sum/file_size*100);
        printf("have sended :");
        for(int i = 0 ;i<jindu/10;i++)
        {
            color = i%6;
            printf("\033[;%dm\33[1m=\033[0m",color+32);
            fflush(stdout);
        }
        printf("\033[;31m\33[1m>\033[0m");
        fflush(stdout);
        for(int j = 0;j<10-jindu/10;j++)
        {
            printf(" ");
            fflush(stdout);
        }
        printf("\033[;31m\33[1m|\033[0m %d%%    %d  \n",jindu,sum);
        
        send_pack_memcpy(FILE_SEND,m_my_infor.username,file_path,mes);
        
        if(sum == file_size)  
            break;
        bzero(mes, MAX_CHAR*2); 
        usleep(100000);
        printf("\33[1A\33[K");
        fflush(stdout);
    } 
    // 关闭文件 
    close(fd);
    printf("send finished!!\n");
    print_main_menu();
}









/*
 * 显示文件处理消息，并根据提供选择
*/

int file_mes_box()
{
    char choice_t[100];
    int chioce;
    do
    {
        get_status_mes();
        printf("\033[;34m\33[1m^-^-^-^-^-^-^-^-^-^file mes box^-^-^-^-^-^-^-^-^-^\033[0m\n");
        for(int i = 1; i <= m_recv_num_file_mes;i++)
        {
            if(m_pack_recv_file_mes[i].type == FILE_SEND_STOP_RP)
                printf("\t\033[;34m\33[1m*\033[0m   \033[;31m\33[1msend file %s filed\033[0m       \n",m_pack_recv_file_mes[i].data.send_name);
            if(m_pack_recv_file_mes[i].type == FILE_RECV_BEGIN)
                printf("\t\033[;34m\33[1m*\033[0m   \033[;33m\33[1m%s send file %s to you\033[0m   \n", m_pack_recv_file_mes[i].data.send_name,m_pack_recv_file_mes[i].data.mes+SIZE_PASS_NAME);
            if(m_pack_recv_file_mes[i].type == FILE_RECV_STOP_RP)
                printf("\t\033[;34m\33[1m*\033[0m   \033[;31m\33[1mrecv file %s filed\033[0m       \n", m_pack_recv_file_mes[i].data.mes+NUM_MAX_DIGIT);
        }
                printf("\t\033[;34m\33[1m*\033[0m             0.exit                               \n");
        printf("\033[;34m\33[1m^-^-^-^-^-^-^-^-^-^end^-^-^-^-^-^-^-^-^-^-^-^-^-^-\033[0m\n");
        printf("\t\tchoice：");
        scanf("%s",choice_t);
        chioce = get_choice(choice_t);
        //进行处理
        if(chioce != -1)   
            deal_file_mes(chioce);

    }while(chioce!=0);
    return 0;
}




//对文件信息进行分类处理
void deal_file_mes(int id)
{
    if(m_pack_recv_file_mes[id].type == FILE_SEND_STOP_RP)
    {
        mes_sendfile_fail(id);
    }
    else if(m_pack_recv_file_mes[id].type == FILE_RECV_BEGIN)
    {
        mes_recv_requir(id);
    }else if(m_pack_recv_file_mes[id].type == FILE_RECV_STOP_RP)
    {
        mes_recvfile_fail(id);
    }
}


//处理文件上传失败，并询问是否重发
//进行断点续传
void mes_sendfile_fail(int id)
{
    char chioce[10];
    //解析已经上传的字节数
    int begin_location = 0;
    for(int i=0 ;i<NUM_MAX_DIGIT ;i++)
    {
        if( m_pack_recv_file_mes[id].data.mes[i] == -1)  
            break;
        printf("%d\n\n",m_pack_recv_file_mes[id].data.mes[i]);
        int t1 = 1;
        for(int l=0;l<i;l++)
            t1*=10;
        begin_location += (int)m_pack_recv_file_mes[id].data.mes[i] * t1;

    }


    //询问是否重传
    int file_size_t = get_file_size(m_pack_recv_file_mes[id].data.send_name);
    printf("\033[;31m\33[1mthe file %s send failed ,have sended \033[;32m%d%%\033[;31m,do you want send again?\033[0m\n", m_pack_recv_file_mes[id].data.send_name,(int)((double)begin_location/file_size_t*100));
    printf("y/n :");
    scanf("%s",chioce);
    

    if(chioce[0] != 'Y' && chioce[0] != 'y')
    {
        file_infor_delete(id);
        return ;
    }

    send_pack_memcpy(FILE_RECV_BEGIN_RP,m_my_infor.username ,m_pack_recv_file_mes[id].data.send_name ," ");
    //进行重传
    send_file_send(begin_location,m_pack_recv_file_mes[id].data.send_name);
    file_infor_delete(id);
}

//处理下载文件请求，询问是否接收文件
void mes_recv_requir(int id)
{
    pthread_t  pid_recv_file;
    char choice[10];
    int len ;
    int fd;
    char mes_t[MAX_CHAR];
    int file_size = 0;
    char file_name[SIZE_PASS_NAME];
    
    PTHREAD_PAR * par_t = (PTHREAD_PAR *)malloc(sizeof(PTHREAD_PAR));
    //解析文件大小
    for(int i=0 ;i<NUM_MAX_DIGIT ;i++)
    {
        if(m_pack_recv_file_mes[id].data.mes[i] == -1)  
            break;
        int t1 = 1;
        for(int l=0;l<i;l++)
            t1*=10;
        file_size += (int)m_pack_recv_file_mes[id].data.mes[i]*t1;

    }   
    // for(int i=0 ;i<=50;i++)
        // printf("%d\n", m_pack_recv_file_mes[id].data.mes[i]);
    // printf("%s\n", m_pack_recv_file_mes[id].data.mes+NUM_MAX_DIGIT);
    strcpy(file_name,m_pack_recv_file_mes[id].data.mes+NUM_MAX_DIGIT);

    
    printf("\033[;33m\33[1m %s send file %s size(%db)to you \033[0m\n", m_pack_recv_file_mes[id].data.send_name,file_name,file_size);
    printf(" do you want receive it? \n");
    printf("(y/n) :");
    scanf("%s", choice);
    if(choice[0] != 'Y' && choice[0] != 'y')
    {
        file_infor_delete(id);
        return ;
    }


    //建立文件
    if((fd = open(file_name,O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR)) < 0)
    {
        my_err("open",__LINE__);
        return ;
    }
    len = lseek(fd, 0, SEEK_END);
    close(fd);

    par_t->a = file_size;
    par_t->b = len;
    
    int digit = 0;
    while(len != 0)
    {   
        mes_t[digit++] = len%10;
        len /= 10;
    }
    mes_t[digit]  = -1;
    //发送同意同意
    send_pack_memcpy(FILE_SEND_BEGIN_RP ,m_my_infor.username ,file_name ,mes_t);
    //开启线程接收文件
    pthread_create(&pid_recv_file,NULL,pthread_recv_file,(void *)par_t);

    file_infor_delete(id);
}

//处理接收文件中断信息，
//并询问是否继续接收
void mes_recvfile_fail(int id)
{
    pthread_t  pid_recv_file;
    char chioce[10];
    int begin_location_server;
    int file_size;
    char file_name[SIZE_PASS_NAME];
    char mes_t[MAX_CHAR];
    PTHREAD_PAR * par_t = (PTHREAD_PAR *)malloc(sizeof(PTHREAD_PAR));
    //解析已经接收的字节数
    for(int i=0 ;i<NUM_MAX_DIGIT ;i++)
    {
        if(m_pack_recv_file_mes[id].data.mes[i] == -1)  
            break;
        int t1 = 1;
        for(int l=0;l<i;l++)
            t1*=10;
        file_size += (int)m_pack_recv_file_mes[id].data.mes[i]*t1;

    }   

    strcpy(file_name,m_pack_recv_file_mes[id].data.mes+NUM_MAX_DIGIT);
    
    begin_location_server= get_file_size(file_name);
    

    par_t->a = file_size;
    par_t->b = begin_location_server;
    printf("\033[;31m\33[1mthe file %s recv failed ,have recved \033[;32m%d%%\033[;31m,do you want recv continue?\033[0m\n", file_name,(int)((double)begin_location_server/file_size*100));
    printf("y/n :");
    scanf("%s",chioce);
    
    

    if(chioce[0] != 'Y' && chioce[0] != 'y')
    {
        file_infor_delete(id); 
        return ;
    }


    int len = begin_location_server;
    int digit = 0;
    while(len != 0)
    {   
        mes_t[digit++] = len%10;
        len /= 10;
    }
    mes_t[digit]  = -1;

    //返回同意信息
    send_pack_memcpy(FILE_SEND_BEGIN_RP ,m_my_infor.username ,file_name ,mes_t);
    //开启线程接收文件
    pthread_create(&pid_recv_file,NULL,pthread_recv_file,(void *)par_t);

    file_infor_delete(id);
}

//接收文件线程，
//从存储接收包的地方检索到信息
//并写入文件
//当文件写入完成，关闭线程
void *pthread_recv_file(void *par_t)
{
    PTHREAD_PAR * pthread_par  = (PTHREAD_PAR * )par_t;
    int file_size              = pthread_par->a ;
    int begin_location_server  = pthread_par->b;
    int sum                    = begin_location_server; 
    while(1)
    {
        pthread_mutex_lock(&mutex_recv_file); 
        int  fd;
        char file_name[MAX_CHAR];
        for(int i=1;i<=m_recv_num_file ;i++)
        {
            
            int  len = 0;
            for(int j=0 ;j<NUM_MAX_DIGIT ;j++)
            {
                if(m_pack_recv_file[i].data.mes[j] == -1)  
                    break;
                int t1 = 1;
                for(int l=0;l<j;l++)
                    t1*=10;
                len += (int)m_pack_recv_file[i].data.mes[j]*t1;

            }

            strcpy(file_name,m_pack_recv_file[i].data.send_name);
            //you can creat this file when you get the file_send_begin
            if((fd = open(file_name,O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)) < 0)
            {
                my_err("open",__LINE__);
                return NULL;
            }

            if(write(fd,m_pack_recv_file[i].data.mes + NUM_MAX_DIGIT,len) < 0)
                my_err("write",__LINE__);
            // 关闭文件 
            close(fd);
            sum += len;
            int color;
            int jindu=(int)((double)sum/file_size*100);
            printf("have recved :");
            for(int i = 0 ;i<jindu/10;i++)
            {
                color = i%6;
                printf("\033[;%dm\33[1m=\033[0m",color+32);
                fflush(stdout);
            }
            printf("\033[;31m\33[1m>\033[0m");
            fflush(stdout);
            for(int j = 0;j<10-jindu/10;j++)
            {
                printf(" ");
                fflush(stdout);
            }
            printf("\033[;31m\33[1m|\033[0m %d%%    %d  \n",jindu,sum);
            
            m_recv_num_file = 0;
            //文件接收完成，输出提示信息，跳出线程
            if(sum >= file_size)  
            {
                send_pack(FILE_FINI_RP,m_my_infor.username,"server",file_name);
                printf("send finished!!\n");
                print_main_menu();
                return NULL;  
            }
        }

        pthread_mutex_unlock(&mutex_recv_file);
        usleep(10);
    }   
}







//显示历史信息
//向服务端发送请求消息
int mes_record()
{
    char username[MAX_CHAR];
    printf("please input the username that you want see:\n");
    scanf("%s",username);
    printf("the recording :\n");
    send_pack(MES_RECORD,m_my_infor.username,"server",username);
    printf("\33[2J\33[20A\033[;33m\33[1m^-^-^-^-^-^-^-^-^message recordes^-^-^-^-^-^-^-^\033[0m\n");
    while(!m_flag_print_recorde) ;
    printf("printf recordes finished!!\n");
    m_flag_print_recorde = 0;
    return 0;
}



//输出历史消息
void print_mes_record(PACK* pack_t)
{
    if(strcmp(pack_t->data.send_name , "server") == 0)
    {
        if(strcmp(pack_t->data.mes,m_my_infor.username) == 0)
        {    
            printf("\033[;34m\33[1m%s\033[0m\t\033[;34m\033[0m\n",pack_t->data.mes);
            printf("%s\n", pack_t->data.mes+SIZE_PASS_NAME);
        }
        else
        {
            printf("\033[;33m\33[1m%s\033[0m\t\033[;33m\033[0m\n",pack_t->data.mes);
            printf("%s\n", pack_t->data.mes+SIZE_PASS_NAME);
        }
        
    }
    else 
        m_flag_print_recorde = 1;    
    free(pack_t);
}

/*****************************************/




/*void print_file_mes()
{
    for(int i=1 ;i<=5;i++)
    {
        printf("hahahahahaha*****\n");
        printf("%s\n", m_pack_recv_file_mes[i].data.send_name);
        printf("%s",m_pack_recv_file_mes[i].data.recv_name);
        for(int j=0;j<=5;j++)
            printf("%d\n\n",m_pack_recv_file_mes[i].data.mes[j]);

    }
}*/

