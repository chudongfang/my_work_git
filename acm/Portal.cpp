#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cstdio>
#include<stack>
#include<set>
using namespace std;
const int N= 10003;
const int M= 50002;
struct  Node
{
    int u,v;
    int len;
} edge[M];
bool cmp(Node a,Node b)
{
    return a.len<=b.len;
}
int fa[N];
int sum[N];
int n,m,q;
void init()
{
    for(int i=1; i<=n; i++)
        fa[i]=i,sum[i]=1;
}
int findx(int x)
{
    return fa[x]=fa[x]==x?x:findx(fa[x]);
}
int Union(int a,int b)
{
    int aa=findx(a);
    int bb=findx(b);
    if(aa==bb)
        return 0;
    else if(fa[aa]<fa[bb])
    {
        fa[bb]=aa;
        int tmp=sum[aa]*sum[bb];
        sum[aa]+=sum[bb];
        return tmp;
    }
    else
    {
        fa[aa]=bb;
        int tmp=sum[aa]*sum[bb];
        sum[bb]+=sum[aa];
        return tmp;
    }
}

int ans[N];
struct Q
{
    int L;
    int id;
    int ans;
} que[N];
bool cmp1(Q a,Q b)
{
    return a.L<=b.L;
}
bool cmp2(Q a,Q b)
{
    return a.id<b.id;
}
int main()
{
    while(scanf("%d%d%d",&n,&m,&q)!=EOF)
    {
        init();
        for(int i=0; i<m; i++)
            scanf("%d%d%d",&edge[i].u,&edge[i].v,&edge[i].len);
        for(int i=0; i<q; i++)
            {scanf("%d",&que[i].L);que[i].id=i;que[i].ans=0;}
        sort(edge,edge+m,cmp);
        sort(que,que+q,cmp1);
        int cnt=0;
        for(int i=0; i<q; i++)
        {
            while(edge[cnt].len<=que[i].L&&cnt<m)
            {
                //cout<<"FFF"<<endl;
                int aa=findx(edge[cnt].u);
                int bb=findx(edge[cnt].v);
                if(aa==bb)
                {
                    cnt++;
                    continue;
                }
                else
                {
                    que[i].ans+=Union(edge[cnt].u,edge[cnt].v);
                    cnt++;
                }
            }
            if(i>=1)
                que[i].ans+=que[i-1].ans;
        }
        sort(que,que+q,cmp2);
        for(int i=0; i<q; i++)
        {
           printf("%d\n",que[i].ans);
        }
    }
    return 0;
}
