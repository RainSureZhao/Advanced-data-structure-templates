#include<iostream>
#include<cstring>
using namespace std;
const int maxn = 2000010, inf = 2147483647;
typedef struct node
{
    int s[2], p, v;
    int size;
    void init(int _v, int _p){
        v = _v, p = _p;
        size = 1;
    }
}Node;
Node tr[maxn];
int n, m;
int L[maxn], R[maxn], T[maxn], idx;
int w[maxn];
void pushup(Node &u, Node &l, Node &r)
{
    u.size = l.size + r.size + 1;
}
void pushup(int u)
{
    pushup(tr[u], tr[tr[u].s[0]], tr[tr[u].s[1]]);
}
void rotate(int x)
{
    int y = tr[x].p, z = tr[y].p;
    int k = tr[y].s[1] == x;
    tr[z].s[tr[z].s[1] == y] = x, tr[x].p = z;
    tr[y].s[k] = tr[x].s[k ^ 1], tr[tr[x].s[k ^ 1]].p = y;
    tr[x].s[k ^ 1] = y, tr[y].p = x;
    pushup(y), pushup(x);
}
void splay(int &root, int x, int k)
{
    while(tr[x].p != k){
        int y = tr[x].p, z = tr[y].p;
        if(z != k){
            if((tr[y].s[1] == x) ^ (tr[z].s[1] == y)) rotate(x);
            rotate(y);
        }
        rotate(x);
    }
    if(!k) root = x;
}
void insert(int &root, int v)
{
    int u = root, p = 0;
    while(u) p = u, u = tr[u].s[v > tr[u].v];
    u = ++ idx;
    if(p) tr[p].s[v > tr[p].v] = u;
    tr[u].init(v, p);
    splay(root, u, 0);
}
int get_k(int root, int v) // 查询有多少个小于v的数
{
    int u = root, res = 0;
    while(u)
    {
        if(tr[u].v < v) res += tr[tr[u].s[0]].size + 1, u = tr[u].s[1];
        else u = tr[u].s[0];
    }
    return res;
}
void update(int &root, int x, int y)
{
    int u = root;
    while(u)
    {
        if(tr[u].v == x) break;
        if(tr[u].v < x) u = tr[u].s[1];
        else u = tr[u].s[0];
    }
    splay(root, u, 0);
    int l = tr[u].s[0], r = tr[u].s[1];
    while(tr[l].s[1]) l = tr[l].s[1];
    while(tr[r].s[0]) r = tr[r].s[0];
    splay(root, l, 0), splay(root, r, l);
    tr[r].s[0] = 0;
    pushup(r), pushup(l);
    insert(root, y);
}
void build(int u, int l, int r)
{
    L[u] = l, R[u] = r;
    insert(T[u], -inf), insert(T[u], inf);
    for(int i = l; i <= r; i ++) insert(T[u], w[i]);
    if(l == r) return;
    int mid = l + r >> 1;
    build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
}
void modify(int u, int x, int c)
{
    update(T[u], w[x], c);
    if(L[u] == R[u]) return;
    int mid = L[u] + R[u] >> 1;
    if(x <= mid) modify(u << 1, x, c);
    else modify(u << 1 | 1, x, c);
}
int query(int u, int l, int r, int x)
{
    if(L[u] >= l && R[u] <= r) return get_k(T[u], x) - 1;
    int mid = L[u] + R[u] >> 1, res = 0;
    if(l <= mid) res += query(u << 1, l, r, x);
    if(r > mid) res += query(u << 1 | 1, l, r, x);
    return res;
}
int get_pre(int root, int v)
{
    int u = root, res = -inf;
    while(u)
    {
        if(tr[u].v < v) res = max(res, tr[u].v), u = tr[u].s[1];
        else u = tr[u].s[0];
    }
    return res;
}
int get_suc(int root, int v)
{
    int u = root, res = inf;
    while(u)
    {
        if(tr[u].v > v) res = min(res, tr[u].v), u = tr[u].s[0];
        else u = tr[u].s[1];
    }
    return res;
}
int query_pre(int u, int a, int b, int x)
{
    if(L[u] >= a && R[u] <= b) return get_pre(T[u], x);
    int mid = L[u] + R[u] >> 1, res = -inf;
    if(a <= mid) res = max(res, query_pre(u << 1, a, b, x));
    if(b > mid) res = max(res, query_pre(u << 1 | 1, a, b, x));
    return res;
}
int query_suc(int u, int a, int b, int x)
{
    if(L[u] >= a && R[u] <= b) return get_suc(T[u], x);
    int mid = L[u] + R[u] >> 1, res = inf;
    if(a <= mid) res = min(res, query_suc(u << 1, a, b, x));
    if(b > mid) res = min(res, query_suc(u << 1 | 1, a, b, x));
    return res;
}
int main()
{
    scanf("%d %d", &n, &m);
    for(int i = 1; i <= n; i ++) scanf("%d", &w[i]);
    build(1, 1, n);
    while(m --)
    {
        int op; scanf("%d", &op);
        if(op == 1){
            int l, r, x; scanf("%d %d %d", &l, &r, &x);
            printf("%d\n", query(1, l, r, x) + 1);
        }else if(op == 2){
            int a, b, x; scanf("%d %d %d", &a, &b, &x);
            int l = 0, r = 1e8;
            while(l < r){
                int mid = l + r + 1 >> 1;
                if(query(1, a, b, mid) + 1 <= x) l = mid;
                else r = mid - 1;
            }
            printf("%d\n", r);
        }else if(op == 3){
            int pos, x; scanf("%d %d", &pos, &x);
            modify(1, pos, x);
            w[pos] = x;
        }else if(op == 4){
            int a, b, x; scanf("%d %d %d", &a, &b, &x);
            printf("%d\n", query_pre(1, a, b, x));
        }else{
            int a, b, x; scanf("%d %d %d", &a, &b, &x);
            printf("%d\n", query_suc(1, a, b, x));
        }
    }
    return 0;
}