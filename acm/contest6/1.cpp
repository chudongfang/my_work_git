/*************************************************************************
	> File Name: 1.cpp
	> Author:chudongfang 
	> Mail:1149669942@qq.com 
	> Created Time: 2016年08月04日 星期四 12时05分09秒
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
ll s[1000006];
int main(int argc,char *argv[])
{
	int T,n,i,j;
	ll re;
	scanf("%d",&T);
	while(T--)
	{
        memset(s,0,sizeof(s));
		ll count=0,count1=0,count2=0;
		scanf("%d",&n);
		for(i=1;i<=n;i++)
		{
			scanf("%lld",&s[i]);
			if(s[i]>=3) 
        	{
        		if(s[i]%2)
        		{
        			count += s[i]/2;
        			count%=2;
        			count1+= s[i]%2;
        		}
        		else
        		{
        			count += (s[i]-2)/2;
        			count%=2;
        			count1 +=s[i]-2;
        			count1%=2;
        			count2++;
        		}
       		}    
        	else if(s[i] == 1)  count1++;
        	else if(s[i] == 2)  count2++;    
		}
		count%=2;
        count1%=2;
        count2%=2;
        printf("\n%lld %lld %lld\n",count,count1,count2); 
        if((((count1 | count2) != 0)+count)%2 )   
        	printf("First player wins.\n");
		else             
			printf("Second player wins.\n");
	}

    return 0;
}
