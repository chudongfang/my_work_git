#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;
const double eps = 1e-6;
const double pi = acos(-1.0);
const int INF = 0x3f3f3f3f;
const int MOD = 1000000007;
#define ll long long
#define CL(a,b) memset(a,b,sizeof(a))
#define MAXN 200010

struct node
{
    int a,b,r;
}t[MAXN*4];
int h,w,n;

void build(int x, int y, int num)
{
    t[num].a = x;
    t[num].b = y;
    t[num].r = w;
    if(x == y) return ;
    int mid = (x+y)>>1;
    build(x, mid, num<<1);
    build(mid+1, y, num<<1|1);
}

int query(int x, int num)
{
    if(t[num].a == t[num].b)
    {
        t[num].r -= x;
        return t[num].a;
    }
    else
    {
        int sum1=0, sum2=0;
        if(x <= t[num<<1].r) sum1 = query(x, num<<1);
        else if(x <= t[num<<1|1].r) sum2 = query(x, num<<1|1);
        t[num].r = max(t[num<<1].r, t[num<<1|1].r);
        return sum1+sum2;
    }
}

int main()
{
    while(scanf("%d%d%d",&h,&w,&n)==3)
    {
        if(h > n) h = n;
        build(1, h, 1);
        int k;
        for(int i=1; i<=n; i++)
        {
            scanf("%d",&k);
            if(k <= t[1].r) printf("%d\n",query(k, 1));
            else printf("-1\n");
        }
    }
    return 0;
}