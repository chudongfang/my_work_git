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
#define MOD 1000000007
using namespace std;
typedef long long ll;
ll mod(ll a,ll b,ll m);
int main(int argc,char *argv[])
{

	int T;
	ll re;
	ll  n,m;
	scanf("%d",&T);
	while(T--)
	{
		re = 0;
		scanf("%lld %lld",&n,&m);
		if(n==0)
		{
			printf("%d\n",0);
			continue;
		}  
			
        re = (((mod(m,n+1,MOD)-1))/(m-1))%MOD;
	    printf("%lld\n",re);
    }

    return 0;
}


ll mod(ll a,ll b,ll m)
{  
    ll result = 1;  
    ll base = a;  
    while(b>0){  
         if(b & 1==1){  
            result = (result*base) % m;  
         }  
         base = (base*base) %m;  
         b >>= 1;  
    }  
    return result;  
}  

