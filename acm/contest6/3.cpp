
/*************************************************************************
	> File Name: 2.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年08月04日 星期四 15时04分37秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include <algorithm>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
ll jiecheng(int n);
int main(int argc,char *argv[])
{

	int T;
	int i,j,k1,k2,k3,q;
	ll re;
	ll n,m;
	scanf("%d",&T);
	while(T--)
	{
		re = 0;
		scanf("%lld %lld",&n,&m);
		for(k1=0;k1<=n;k1++)
		{
			for(k2=k1;k2<=n;k2++)
			{
				for(k3=k2;k3<=n;k3++)
				{
                    printf("jiechneg =%lld %lld %lld\n",jiecheng(k3),jiecheng(k1));
					int t = 0;
					t = jiecheng(k3)/
					(jiecheng(k1)*jiecheng(k2-k1)*jiecheng(k3-k2));
					re +=t;
    			}
			}
		}
		printf("%lld\n",re);
	}

    return 0;
}



ll jiecheng(int n)
{
	ll re=1;
	for(int i=1;i<=n;i++)
		re*=i;
	return re;
}


