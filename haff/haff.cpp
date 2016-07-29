#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxValue 10000//权值最大值 
#define MaxBit 55//最大编码位数 
#define MaxN 55//最大结点个数 

 
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
			printf("字符错误！");
			return 0; 
		}
	}
}

void HaffTree(int weight[],int n,HaffNode haffTree[],char ch2[]);
void HaffCode(HaffNode haffTree[],int n,Code haffCode[]);
void haffyima(HaffNode haffTree[],int n,Code haffCode[],int weight[],char ch[]);

int main(void)
{
    int i,j,n;
    int weight[100]={0};
    char ch[100];
    char ch2[100];
    
    printf("     输入待编码字符串："); 
    gets(ch);
    n=strlen(ch);//输入编码长度
    sear_weight(ch,weight,ch2);//计算权重

	HaffNode *myHaffTree=(HaffNode *)malloc(sizeof(HaffNode)*(2*n-1));
	Code     *myHaffCode=(Code *)malloc(sizeof(Code)*n);
	
	if(n>MaxN)
	{
		printf("n越界!!!\n");
		getchar();
		return 0;
	}
	 
	HaffTree(weight,n,myHaffTree,ch2);
	HaffCode(myHaffTree,n,myHaffCode);
	 
	 //输出
	printf("                 哈夫曼编码               \n");
	printf("==========================================\n");
	for(i=0; i<n; i++)
	{
	    for(j = myHaffCode[i].start; j<n; j++)
	 	    printf("%d",myHaffCode[i].bit[j]);
	 	printf(" ");
	} 
	  
	printf("\n==========================================\n");
	printf("                 哈夫曼译码               \n");
	printf("==========================================\n"); 
	haffyima(myHaffTree,n,myHaffCode,weight,ch2);
	printf("==========================================\n");
	printf("按任意键结束·······\n");
	getchar();
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


void haffyima(HaffNode haffTree[],int n,Code haffCode[],int weight[],char ch2[])
{
	int i,j;
	HaffNode haffTree1;
	
	for(i=0;i<n;i++)
	{
		haffTree1=haffTree[2*n-2];

		for(j = haffCode[i].start; j<n; j++)
		{   
			if(haffCode[i].bit[j]==1)
			{
				haffTree1=haffTree[haffTree1.rightChild];
			}
			else 
				haffTree1=haffTree[haffTree1.leftChild];
		}
		for(j=0;j<n;j++)
		{
		    if(haffTree1.weight==weight[j])
		    {
				printf("%c",haffTree1.wei);
		        break;
            }
		}
	}
	printf("\n");
}   

 
