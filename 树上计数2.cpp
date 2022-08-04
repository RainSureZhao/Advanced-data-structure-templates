#include<iostream>
#include<cstring>
#include<vector>
#include<queue>
#include<algorithm>
#include<cmath>
using namespace std;
const int maxn = 100010;
int n, m, len;
int w[maxn];
int h[maxn], e[maxn], ne[maxn], idx;
int depth[maxn], f[maxn][20];
int seq[maxn], top, first[maxn], last[maxn];
int cnt[maxn], st[maxn], ans[maxn];
struct Query
{
    int id, l, r, p;
}q[maxn];
vector<int> nums;
void add(int a, int b)
{
    e[idx] = b, ne[idx] = h[a], h[a] = idx ++;
}
void dfs(int u, int fa)
{
    seq[ ++ top] = u;
    first[u] = top;
    for(int i = h[u]; ~i; i = ne[i]){
        int j = e[i];
        if(j == fa) continue;
        dfs(j, u);
    }
    seq[ ++ top] = u;
    last[u] = top;
}
void bfs()
{
    memset(depth, 0x3f, sizeof depth);
    depth[0] = 0, depth[1] = 1;
    queue<int> q;
    q.push(1);
    while(q.size())
    {
        auto t = q.front();
        q.pop();
        for(int i = h[t]; ~i; i = ne[i]) {
            int j = e[i];
            if(depth[j] > depth[t] + 1){
                depth[j] = depth[t] + 1;
                f[j][0] = t;
                for(int k = 1; k <= 15; k ++){
                    f[j][k] = f[f[j][k - 1]][k - 1];
                }
                q.push(j);
            }
        }
    }
}
int lca(int a, int b)
{
    if(!a) return b;
    if(!b) return a;
    if(depth[a] < depth[b]) swap(a, b);
    for(int k = 15; k >= 0; k --){
        if(depth[f[a][k]] >= depth[b]) {
            a = f[a][k];
        }
    }
    if(a == b) return a;
    for(int k = 15; k >= 0; k --){
        if(f[a][k] != f[b][k]) {
            a = f[a][k];
            b = f[b][k];
        }
    }
    return f[a][0];
}
int get(int x)
{
    return x / len;
}
void Add(int x, int &res)
{
    st[x] ^= 1;
    if(st[x] == 0){
        cnt[w[x]] --;
        if(!cnt[w[x]]) res --;
    }else{
        if(!cnt[w[x]]) res ++;
        cnt[w[x]] ++;
    }
}
bool cmp(Query &a, Query &b)
{
    int i = get(a.l), j = get(b.l);
    if(i != j) return i < j;
    return a.r < b.r;
}
int main()
{
    scanf("%d %d", &n, &m);
    for(int i = 1; i <= n; i ++) scanf("%d", &w[i]), nums.push_back(w[i]);
    sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.end());
    for(int i = 1; i <= n; i ++){
        w[i] = lower_bound(nums.begin(), nums.end(), w[i]) - nums.begin();
    }
    memset(h, -1, sizeof h);
    for(int i = 0; i < n - 1; i ++){
        int a, b; scanf("%d %d", &a, &b);
        add(a, b); add(b, a);
    }
    dfs(1, -1);
    bfs();
    for(int i = 0; i < m; i ++){
        int a, b; scanf("%d %d", &a, &b);
        if(first[a] > first[b]) swap(a, b);
        int p = lca(a, b);
        if(a == p) q[i] = {i, first[a], first[b]};
        else q[i] = {i, last[a], first[b], p};
    }
    len = sqrt(top);
    sort(q, q + m, cmp);
    for(int k = 0, i = 1, j = 0, res = 0; k < m; k ++){
        int id = q[k].id, l = q[k].l, r = q[k].r, p = q[k].p;
        while(j < r) Add(seq[ ++ j], res);
        while(j > r) Add(seq[j -- ], res);
        while(i < l) Add(seq[i ++ ], res);
        while(i > l) Add(seq[-- i ], res);
        if(p) Add(p, res);
        ans[id] = res;  
        if(p) Add(p, res);
    }
    for(int i = 0; i < m; i ++) printf("%d\n", ans[i]);
    return 0;
}