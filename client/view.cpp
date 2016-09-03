
#include "client.h"



//登陆菜单
int login_menu()
{
    char choice_t[100];
    int chioce;
    do
    {
        printf("\n\t\t\033[;34m\33[1m*******************************\033[0m\n");
        printf("\t\t\033[;34m\33[1m*\033[0m        1.login in           \033[;34m\33[1m*\033[0m \n");
        printf("\t\t\033[;34m\33[1m*\033[0m        2.register           \033[;34m\33[1m*\033[0m \n");
        printf("\t\t\033[;34m\33[1m*\033[0m        0.exit               \033[;34m\33[1m*\033[0m \n");
        printf("\t\t\033[;34m\33[1m*******************************\033[0m\n");
        printf("\t\tchoice：");
        scanf("%s",choice_t);
        chioce = get_choice(choice_t);
        switch(chioce)
        {  
            case 1:
                if(login() == 1)
                    return 1;
                break;
            case 2:
                registe();
                break;
            default:
                break;
        }
    }while(chioce!=0);
    return 0;
}






//显示聊天信息
void show_mes_smart(char *name  ,char *mes)
{
    time_t timep;
    int number = 6;
    char time1[100];
    int len ; 
    time (&timep);
    strcpy(time1,ctime(&timep));
    len = strlen(time1);
    time1[len-5] = '\0'; 
    

    if(m_print_mes_num == number)  
    {
        for(int i=1;i<=5 ;i++)
            m_print_mes[i] = m_print_mes[i+1];
        strcpy(m_print_mes[number].name,name);
       // strcpy(m_print_mes[number].time,time);
        strcpy(m_print_mes[number].mes,mes);
    }
    else{
         strcpy(m_print_mes[++m_print_mes_num].name,name);
         //strcpy(m_print_mes[m_print_mes_num].time,time);
         strcpy(m_print_mes[m_print_mes_num].mes,mes);
    }


    printf("\33[s");
    fflush(stdout);

    printf("\33[25A\n");  
    
    for(int i=1;i<=20;i++)
        printf("\33[K\n");
    printf("\33[21A\n");

    for(int i=1;i<=m_print_mes_num;i++)
    {
        if(strcmp(m_print_mes[i].name,m_my_infor.username) == 0)
        {
            printf("\033[;34m\33[1m%s\033[0m\t\033[;34m%s\033[0m\n",m_print_mes[i].name,time1+10);
            printf("%s\n", m_print_mes[i].mes);
        }
        else
        {
            printf("\033[;33m\33[1m%s\033[0m\t\033[;33m%s\033[0m\n",m_print_mes[i].name,time1+10);
            printf("%s\n", m_print_mes[i].mes);
        }
        

    }
    for(int i=1;i<=6- m_print_mes_num ;i++)
    {
        printf("\n");
        printf("\n\n");

    }

    printf("\33[u");
    fflush(stdout);
}




//好友列表查看
void friends_see()
{
   
    pthread_mutex_lock(&mutex_local_user);
    printf("\33[2J\n");
    printf("\33[1;1H\n");
    printf("\033[;35m\33[1m\t  ***********the list of friends************* \033[0m \n");
    for(int i=1 ;i<=m_my_infor.friends_num ;i++)
    {
        switch(m_my_infor.friends[i].statu)
        {
           case ONLINE:
                printf("\033[;35m\33[1m\t  ID[%d]:  \033[0m     \033[;32m\33[1m%s\033[0m [ONLINE] ", i,m_my_infor.friends[i].name);
                if(m_my_infor.friends[i].mes_num)
                    printf("\033[;33m%d messages\033[0m\n", m_my_infor.friends[i].mes_num);
                else 
                    printf("\n");
                break;
           case DOWNLINE:
                printf("\033[;35m\33[1m\t  ID[%d]:  \033[0m     \033[;31m\33[1m%s\033[0m [DOWNLINE] ", i,m_my_infor.friends[i].name);
                if(m_my_infor.friends[i].mes_num)
                    printf("\033[;33m%d messages\033[0m\n", m_my_infor.friends[i].mes_num);
                else 
                    printf("\n");
                break;
        }
    }
    printf("\n\n");
    printf("\033[;35m\33[1m\t  *******************end********************* \033[0m \n");
    pthread_mutex_unlock(&mutex_local_user);  
}




//群组信息查看
void group_see()
{
    pthread_mutex_lock(&mutex_local_user); 
    printf("\33[2J\n");
    printf("\33[1;1H\n");
    printf("\033[;35m\33[1m\t  ***********the list of groups************* \033[0m \n");
    for(int i=1 ;i<=m_my_infor.group_num ;i++)
    {
        printf("\033[;35m\33[1m\t  ID[%d]:  \033[0m     \033[;32m\33[1m%s\033[0m", i,m_my_infor.group[i]);
    }
    printf("\n\n");
    printf("\033[;35m\33[1m\t  ********************end******************* \033[0m \n");
    pthread_mutex_unlock(&mutex_local_user);  
}




void print_main_menu()
{
    printf("\n\t\t\033[;34m\33[1m*********************************\033[0m \n");
    printf("\t\t\033[;34m\33[1m*\033[0m        1.show   friends       \033[;34m\33[1m*\033[0m\n");
    printf("\t\t*        2.add    friends       *\n");
    printf("\t\t\033[;34m\33[1m*\033[0m        3.delete friends       \033[;34m\33[1m*\033[0m\n");
    printf("\t\t\033[;34m\33[1m*\033[0m        4.show   group         \033[;34m\33[1m*\033[0m\n");
    printf("\t\t*        5.create group         *\n");
    printf("\t\t\033[;34m\33[1m*\033[0m        6.join   group         \033[;34m\33[1m*\033[0m\n");
    printf("\t\t\033[;34m\33[1m*\033[0m        7.quit   group         \033[;34m\33[1m*\033[0m\n");
    printf("\t\t*        8.delete group         *\n");
    printf("\t\t\033[;34m\33[1m*\033[0m        9.chat with one        \033[;34m\33[1m*\033[0m\n");
    printf("\t\t\033[;34m\33[1m*\033[0m        10.chat with many      \033[;34m\33[1m*\033[0m\n");
    printf("\t\t*        11.send  file          *\n");
    printf("\t\t\033[;34m\33[1m*\033[0m        12.file message box %d  \033[;34m\33[1m*\033[0m\n",m_recv_num_file_mes);
    printf("\t\t\033[;34m\33[1m*\033[0m        13.mes recording       \033[;34m\33[1m*\033[0m\n");
    printf("\t\t\033[;34m\33[1m*\033[0m        0.exit                 \033[;34m\33[1m*\033[0m\n");
    printf("\t\t\033[;34m\33[1m*********************************\033[0m\n");
    printf("\t\tchoice：");
}
