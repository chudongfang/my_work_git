

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MaxValue 10000//权值最大值 
#define MaxBit 55//最大编码位数 
#define MaxN 55//最大结点个数 

char message[100];
//准备工作

int sockfd;//
char* IP = "127.0.0.1";//服务器IP
short PORT = 10222;//端口号
typedef struct sockaddr SA;//类型转换


/*====================================
=1.构造哈夫曼树 
=2.构造哈夫曼编码  
=3.构造哈夫曼解码 
====================================*/

//哈夫曼树的结点结构 
typedef struct
{
	char wei;
	int weight;//权值 
	int flag;//标记 
	int parent;// 双亲结点下一标 
	int leftChild;//左孩子下标 
	int rightChild;//右孩子下标 
 } HaffNode;
 
 //哈夫曼编码结构
typedef struct
{
 	int bit[MaxN];//存放哈夫曼编码的数组 
 	int start;//编码的起始下标 
 	int weight;//字符权值 
 }Code;
typedef struct
{
	char ch;
	int  weight;
}STR;

int sear_weight(char ch[],int weight[],char ch2[])
{
	 STR str[27]={{' ',183},{'a',64},{'b',13},{'c',22},{'d',32},{'e',103},
	             {'f',21},{'g',15},{'h',47},{'i',57},{'j',1},{'k',5},
				 {'l',32},{'m',20},{'n',57},{'o',63},{'p',15},{'q',1},
				 {'r',48},{'s',51},{'t',80},{'u',23},{'v',8},{'w',18},
				 {'x',1},{'y',16},{'z',1}};
	int i,k=0;
	
	for(i=0;ch[i]!='\0';i++)
	{
		if(ch[i]==' ')
		{
		    ch2[i]=' ';
		    weight[i]=183;
		}
		else if(ch[i]>=65&&ch[i]<=90)
		{
			k=ch[i];
			ch2[i]=ch[i];
			weight[i]=str[k-64].weight;
		}
		else if(ch[i]>=97&&ch[i]<=122)
		{
			k=ch[i];
			ch2[i]=ch[i];
			weight[i]=str[k-96].weight;
		}
		else 
		{
			return 1; 
		}
	}
}

  // 建立叶节点个数为n,权值数组为weight的哈夫曼树
void HaffTree(int weight[],int n,HaffNode haffTree[],char ch2[])
{
  	int i,j,m1,m2,x1,x2;
  	
  	//初始化哈夫曼树，n个叶结点的二叉树共有2n-1个结点
	for(i=0; i<2*n-1; i++)
	{
	  	if(i<n) 
		{
		  haffTree[i].wei=ch2[i];
		  haffTree[i].weight = weight[i];//叶节点 
		} 
	  	else haffTree[i].weight = 0;//非叶结点
	  	
	  	haffTree[i].parent      = -1;
	  	haffTree[i].flag        = 0;//是否加入二叉树
		haffTree[i].leftChild   = -1;
		haffTree[i].rightChild  = -1;
	}
	   
	//构造哈夫曼树haffTree的n-1个非叶结点
	for(i=0; i<n-1; i++)
	{
		m1=m2=MaxValue;
		x1=x2=0;
		for(j=0; j<n+i; j++)
		{
			if(haffTree[j].weight<=m1 && haffTree[j].flag==0)
			{
			  	m2=m1;
			  	x2=x1;
			  	m1=haffTree[j].weight;
			  	x1=j;
			}
			else if(haffTree[j].weight<=m2 && haffTree[j].flag==0)
			{
				m2=haffTree[i].weight;
				x2=j; 
			}
	    }	  
		//将找出的两棵权值最小的子树合并为一棵子树
		haffTree[x1].parent=n+i;
		haffTree[x2].parent=n+i;
		haffTree[x1].flag=1;
		haffTree[x2].flag=1;
		haffTree[n+i].weight=haffTree[x1].weight+haffTree[x2].weight;
		haffTree[n+i].leftChild=x1;
		haffTree[n+i].rightChild=x2; 
    } 
}  


//由n个结点的哈夫曼树->构造哈夫曼编码 
void HaffCode(HaffNode haffTree[],int n,Code haffCode[])
{
	Code *cd = (Code *)malloc(sizeof(Code));
	int i,j,child,parent;
			
	//求n个叶结点的哈夫曼编码
	for(i=0; i<n; i++)
	{
	    cd->start = n-1;//权值越小，编码越长
		cd->weight=haffTree[i].weight;
		child=i;//从小到大依次编码 
		parent=haffTree[child].parent;
		//刨根问底->从叶结点沿路直到根结点
		while(parent !=-1)//-1根结点
		{
			if(haffTree[parent].leftChild==child)
				cd->bit[cd->start]=0;
			else 
				cd->bit[cd->start]=1;
				   
			cd->start--;//从后往前依次填码
			child=parent;
			parent=haffTree[child].parent; 
		} 
		//保存每个叶结点的信息
		for(j=cd->start+1; j<n; j++)
			haffCode[i].bit[j]=cd->bit[j];//编码
		haffCode[i].start=cd->start+1;//编码起始位 
		haffCode[i].weight=cd->weight;//权值 
		
		
	} 		  
}



void haffyima(HaffNode haffTree[],int n,char demessage[],int weight[],char message[])
{
	int i,j,k=0;
	HaffNode haffTree1;
    int len=strlen(demessage);
	for(i=0;i<len;i++)
	{
		haffTree1=haffTree[2*n-2];

		for(;i<len;i++)
		{   
			if(demessage[i]=='1')
			{
				haffTree1=haffTree[haffTree1.rightChild];
			}
			else 
				haffTree1=haffTree[haffTree1.leftChild];
            if(haffTree1.rightChild == -1 && haffTree1.leftChild == -1)
                break;
		}
		for(j=0;j<n;j++)
		{
		    if(haffTree1.weight==weight[j])
		    {
                message[k++]=haffTree1.wei;
		        break;
            }
		}
	}
}   

 




//初始化服务器的网络,创建socket
void init()
{
    printf("服务器开始启动..\n");
    sockfd = socket(AF_INET,SOCK_STREAM,0);//启动socket
    if(sockfd == -1){
        perror("创建socket失败");
        printf("服务器启动失败\n");
        exit(-1);
    }
    //准备网络通信地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if(bind(sockfd,(SA*)&addr,sizeof(addr))==-1){//绑定服务器
        perror("绑定失败");
        printf("服务器启动失败\n");
        exit(-1);
    }
    printf("成功绑定\n");

    //设置监听
    if(listen(sockfd,10)==-1){
        perror("设置监听失败");
        printf("服务器启动失败\n");
        exit(-1);
    }
    printf("设置监听成功\n");
    printf("初始化服务器成功\n");
    //等待客户端链接，放到另一个函数中
}



void* service_thread(void* p)
{
	
	int fd = *(int*)p;          //拿到标记客户端的sockfd  
	int count=0;
    int i,j,n;
    int weight[100]={0};
    char ch[100];
    char ch2[100];
    int cht;
    char demessage[100];
    int choice;
    int flag=0;
    int judge;

	HaffNode *myHaffTree=NULL;
	Code     *myHaffCode=NULL;
    while(1)// 死循环，等待客户端信息
    {
        recv(fd,&choice,sizeof(int),0);//得到客户端选择信息

        if(flag == 0 && choice == 2){//如果其还没建立haff树，则返回错误，提醒其建立haff树
        	judge=1;
        	send(fd,&judge,sizeof(int),0);//返回错误标志
            continue;
        }else{
        	judge=0;
        	send(fd,&judge,sizeof(int),0);
        }

        if(choice == 1)//如果是编码，建立haff树，并编码
        {
            count = 0 ;
        	flag=1;
            recv(fd,ch,sizeof(ch),0);
            n=strlen(ch);//输入编码长度
            sear_weight(ch,weight,ch2);//计算权重

	
	        if(n>MaxN)
	    	{
		   		printf("n越界!!!\n");
	    		return 0;
    		}
	 
            //建树
	  		myHaffTree=(HaffNode *)malloc(sizeof(HaffNode)*(2*n-1));
	        myHaffCode=(Code *)malloc(sizeof(Code)*n);
	    	
	    	HaffTree(weight,n,myHaffTree,ch2);
	    	HaffCode(myHaffTree,n,myHaffCode);
	 

	    	for(i=0; i<n; i++)
	    	{
	        	for(j = myHaffCode[i].start; j<n; j++)
                	count++;
            	count++;
	    	} 
	    	send(fd,&count,sizeof(int),0);//返回其编码后的长度  

	    	for(i=0; i<n; i++)//一个字符一个字符发送
	    	{
	        	for(j = myHaffCode[i].start; j<n; j++)
            	{
	    	    	send(fd,&(myHaffCode[i].bit[j]),sizeof(int),0);       
            	}
            	cht=2;
            	send(fd,&cht,sizeof(int),0);
	    	}  
    	}

    	else if(choice == 2)//如果为译码
    	{
        	recv(fd,demessage,sizeof(demessage),0);//得到要译码的信息

        	haffyima(myHaffTree,n,demessage,weight,message);//进行译码    

        	send(fd,message,sizeof(message),0);//把译码信息返回
   	 	}
   	 	else{
   	 		break;
   	 	}	
   	}
    close(fd);//关闭socket
}
//等待客户端的连接，启动服务器的服务






void service()
{
    printf("服务器开始服务\n");
    while(1)//一直循环，等待客户端链接
    {
        struct sockaddr_in fromaddr;
        socklen_t len = sizeof(fromaddr);
        int fd = accept(sockfd,(SA*)&fromaddr,&len);
        if(fd == -1)
        {
            printf("客户端链接出错\n");
            continue;//继续循环，处理连接
        }
        //如果客户端成功连接上
        pthread_t pid;
        pthread_create(&pid,0,service_thread,&fd);
    }
}

void signal_close(int signal_num){
//关闭服务器前 关闭服务器的socket
    close(sockfd);
    printf("服务器已经关闭\n");
    exit(1);
}

int main()
{
    signal(SIGINT,signal_close);//退出CTRL+C
    init();//初始化服务器
    service();//服务器开始运行
    return 0;
}













