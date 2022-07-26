#include<iostream>
#include<cstring>
#include<algorithm>
#include<cmath>
using namespace std;
const int maxn = 50010, M = 200010, S = 1000010;
int n, m, len;
int w[maxn], ans[M];
int cnt[S];
typedef struct node
{
    int id, l, r;
}Node;
Node q[M];
int get(int x)
{
    return x / len;
}
bool cmp(Node &a, Node &b)
{
    int i = get(a.l), j = get(b.l);
    if(i != j) return i < j;
    return a.r < b.r;
}
void add(int x, int &res)
{
    if(!cnt[x]) res ++;
    cnt[x] ++;
}
void del(int x, int &res)
{
    cnt[x] --;
    if(!cnt[x]) res --;
}
int main()
{
    scanf("%d", &n);
    for(int i = 1; i <= n; i ++) scanf("%d", &w[i]);
    scanf("%d", &m);
    len = max(1, (int)sqrt((double)n * n / m));
    for(int i = 0; i < m; i ++){
        int l, r; scanf("%d %d", &l, &r);
        q[i] = {i, l, r};
    }
    sort(q, q + m, cmp);
    for(int k = 0, i = 0, j = 1, res = 0; k < m; k ++){
        int id = q[k].id, l = q[k].l, r = q[k].r;
        while(i < r) add(w[++ i], res);
        while(i > r) del(w[i --], res);
        while(j < l) del(w[j ++], res);
        while(j > l) add(w[-- j], res);
        ans[id] = res;
    }
    for(int i = 0; i < m; i ++) printf("%d\n", ans[i]);
    return 0;
}