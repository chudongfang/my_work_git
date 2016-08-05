#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int H,W,n,Q;
const int MAXN=200000+100;
int maxv[MAXN*4];
int id[MAXN*4];
int cnt;
#define lson i*2,l,m
#define rson i*2+1,m+1,r
void PushUp(int i)
{
    maxv[i]=max(maxv[i*2] , maxv[i*2+1]);
}
void build(int i,int l,int r)
{
    if(l==r)
    {
        maxv[i]=W;
        id[i]=++cnt;
        return ;
    }
    int m=(l+r)/2;
    build(lson);
    build(rson);
    PushUp(i);
}
int update(int w,int i,int l,int r)
{
    if(w>maxv[i])
        return -1;
    if(l==r)
    {
        maxv[i]-=w;
        return id[i];
    }
    int res=-1;
    int m=(l+r)/2;
    if(maxv[i*2]>=w)res = update(w,lson);
    else if(maxv[i*2+1]>=w)res= update(w,rson);
    PushUp(i);
    return res;
}
int main()
{
    while(scanf("%d%d%d",&H,&W,&Q)==3)
    {
        cnt=0;
        n = min(Q,H);//线段树叶节点的最大数目
        build(1,1,n);
        for(int i=1;i<=Q;i++)
        {
            int w;
            scanf("%d",&w);
            printf("%d\n",update(w,1,1,n));
        }
    }
    return 0;
}