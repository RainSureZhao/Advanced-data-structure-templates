#include<iostream>
#include<cstring>
using namespace std;
const int maxn = 100010, M = maxn * 2;
#define int long long
int w[maxn], h[maxn], e[M], ne[M], idx;
int id[maxn], nw[maxn], cnt;
int dep[maxn], sz[maxn], top[maxn], fa[maxn], son[maxn];
int n;
typedef struct node
{
    int l, r;
    int sum, add;
}Node;
Node tr[maxn * 4];
void add(int a, int b)
{
    e[idx] = b, ne[idx] = h[a], h[a] = idx ++;
}
void pushup(Node &u, Node &l, Node &r)
{
    u.sum = l.sum + r.sum;
}
void pushup(int u)
{
    pushup(tr[u], tr[u << 1], tr[u << 1 | 1]);
}
void build(int u, int l, int r)
{
    if(l == r) tr[u] = {l, r, nw[l], 0};
    else{
        tr[u] = {l, r};
        int mid = l + r >> 1;
        build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
        pushup(u);
    }
}
void pushdown(Node &u, Node &l, Node &r)
{
    if(u.add){
        l.add += u.add, l.sum += (l.r - l.l + 1) * u.add;
        r.add += u.add, r.sum += (r.r - r.l + 1) * u.add;
        u.add = 0;
    }
}
void pushdown(int u)
{
    pushdown(tr[u], tr[u << 1], tr[u << 1 | 1]);
}
void modify(int u, int l, int r, int c)
{
    if(tr[u].l >= l && tr[u].r <= r){
        tr[u].add += c;
        tr[u].sum += (tr[u].r - tr[u].l + 1) * c;
        return;
    }
    pushdown(u);
    int mid = tr[u].l + tr[u].r >> 1;
    if(l <= mid) modify(u << 1, l, r, c);
    if(r > mid) modify(u << 1 | 1, l, r, c);
    pushup(u);
}
int query(int u, int l, int r)
{
    if(tr[u].l >= l && tr[u].r <= r) return tr[u].sum;
    else{
        pushdown(u);
        int mid = tr[u].l + tr[u].r >> 1;
        int res = 0;
        if(l <= mid) res += query(u << 1, l, r);
        if(r > mid) res += query(u << 1 | 1, l, r);
        return res;
    }
}
void dfs1(int u, int p, int depth)  // 找出重儿子和每个点的深度以及每棵子树的大小
{
    dep[u] = depth, fa[u] = p, sz[u] = 1;
    for(int i = h[u]; ~i; i = ne[i]){
        int j = e[i];
        if(j == p) continue;
        dfs1(j, u, depth + 1);
        sz[u] += sz[j];
        if(sz[son[u]] < sz[j]) son[u] = j;
    }
}
void dfs2(int u, int t)  // 构建dfs序列
{ 
    id[u] = ++ cnt, nw[cnt] = w[u], top[u] = t;
    if(!son[u]) return;
    dfs2(son[u], t);
    for(int i = h[u]; ~i; i = ne[i]){
        int j = e[i];
        if(j == fa[u] || j == son[u]) continue;
        dfs2(j, j);
    }
}
void modify_path(int u, int v, int k)
{
    while(top[u] != top[v]){
        if(dep[top[u]] < dep[top[v]]) swap(u, v);
        modify(1, id[top[u]], id[u], k);
        u = fa[top[u]];
    }
    if(dep[u] < dep[v]) swap(u, v);
    modify(1, id[v], id[u], k);
}
int query_path(int u, int v)
{   
    int res = 0;
    while(top[u] != top[v]){
        if(dep[top[u]] < dep[top[v]]) swap(u, v);
        res += query(1, id[top[u]], id[u]);
        u = fa[top[u]];
    }
    if(dep[u] < dep[v]) swap(u, v);
    res += query(1, id[v], id[u]);
    return res;
}
void modify_tree(int u, int k)
{
    modify(1, id[u], id[u] + sz[u] - 1, k);
}
int query_tree(int u)
{   
    return query(1, id[u], id[u] + sz[u] - 1);
}
signed main()
{
    cin >> n;
    for(int i = 1; i <= n; i ++) cin >> w[i];
    memset(h, -1, sizeof h);
    for(int i = 0; i < n - 1; i ++){
        int a, b; cin >> a >> b;
        add(a, b); add(b, a);
    }
    dfs1(1, -1, 1);
    dfs2(1, 1);
    build(1, 1, n);
    int m; cin >> m;
    while(m --)
    {
        int op; cin >> op;
        if(op == 1){
            int u, v, k; cin >> u >> v >> k;
            modify_path(u, v, k);
        }else if(op == 2){
            int u, k; cin >> u >> k;
            modify_tree(u, k);
        }else if(op == 3){
            int u, v; cin >> u >> v;
            cout << query_path(u, v) << endl;
        }else{
            int u; cin >> u;
            cout << query_tree(u) << endl;
        }
    }
    return 0;
}