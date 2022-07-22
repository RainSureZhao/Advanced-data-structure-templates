#include<iostream>
#include<cstring>
using namespace std;
const int maxn = 500010, inf = 1e9;
typedef struct node
{
    int s[2], p, v;
    int rev, same;
    int size, ms, ls, rs, sum;
    void init(int _p, int _v)
    {
        s[0] = s[1] = 0, p = _p, v = _v;
        rev = same = 0;
        size = 1, sum = ms = v;
        ls = rs = max(v, 0);
    }
}Node;
Node tr[maxn];
int root, nodes[maxn], tt;
int w[maxn];
void pushup(Node &u, Node &l, Node &r)
{
    u.size = l.size + r.size + 1;
    u.sum = l.sum + r.sum + u.v;
    u.ls = max(l.ls, l.sum + u.v + r.ls);
    u.rs = max(r.rs, r.sum + u.v + l.rs);
    u.ms = max(max(l.ms, r.ms), l.rs + u.v + r.ls);
}
void pushup(int u)
{
    pushup(tr[u], tr[tr[u].s[0]], tr[tr[u].s[1]]);
}
void pushdown(Node &u, Node &l, Node &r)
{
    if(u.same){
        u.same = u.rev = 0;
        if(u.s[0]) l.same = 1, l.v = u.v, l.sum = l.size * u.v;
        if(u.s[1]) r.same = 1, r.v = u.v, r.sum = r.size * u.v;
        if(u.v > 0){
            if(u.s[0]) l.ms = l.ls = l.rs = l.sum;
            if(u.s[1]) r.ms = r.ls = r.rs = r.sum;
        }else{
            if(u.s[0]) l.ms = l.v, l.ls = l.rs = 0;
            if(u.s[1]) r.ms = r.v, r.ls = r.rs = 0;
        }
    }else if(u.rev){
        u.rev = 0, l.rev ^= 1, r.rev ^= 1;
        swap(l.ls, l.rs), swap(r.ls, r.rs);
        swap(l.s[0], l.s[1]), swap(r.s[0], r.s[1]);
    }
}
void pushdown(int u)
{
    pushdown(tr[u], tr[tr[u].s[0]], tr[tr[u].s[1]]);
}
int build(int l, int r, int p)
{
    int mid = l + r >> 1;
    int u = nodes[tt --];
    tr[u].init(w[mid], p);
    if(l < mid) tr[u].s[0] = build(l, mid - 1, u);
    if(mid < r) tr[u].s[1] = build(mid + 1, r, u);
    pushup(u);
    return u;
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
void splay(int x, int k)
{
    while(tr[x].p != k){
        int y = tr[x].p, z = tr[y].p;
        if(z != k){
            if((tr[y].s[1] == x) ^ (tr[z].s[1] == y)) rotate(x);
            else rotate(y);
        }
        rotate(x);
    }
    if(!k) root = x;
}
int get_k(int k)
{
    int u = root;
    while(u){
        pushdown(u);
        if(tr[tr[u].s[0]].size >= k) u = tr[u].s[0];
        else if(tr[tr[u].s[0]].size + 1 == k) return u;
        else k -= tr[tr[u].s[0]].size + 1, u = tr[u].s[1];
    }
    return -1;
}
void dfs(int u)
{
    if(tr[u].s[0]) dfs(tr[u].s[0]);
    if(tr[u].s[1]) dfs(tr[u].s[1]);
    nodes[ ++ tt] = u;
}
int main()
{
    int n, m; scanf("%d %d", &n, &m);
    for(int i = 1; i < maxn; i ++) nodes[++ tt] = i;
    tr[0].ms = w[0] = w[n + 1] = -inf;
    for(int i = 1; i <= n; i ++) scanf("%d", &w[i]);
    root = build(0, n + 1, 0);
    char op[20];
    while(m --)
    {
        scanf("%s", op);
        if(!strcmp(op, "INSERT")) {
            int pos, len; scanf("%d %d", &pos, &len);
            for(int i = 0; i < len; i ++) scanf("%d", &w[i]);
            int l = get_k(pos + 1), r = get_k(pos + 2);
            splay(l, 0), splay(r, l);
            int u = build(0, len - 1, r);
            tr[r].s[0] = u;
            pushup(r), pushup(l);
        }else if(!strcmp(op, "DELETE")){
            int pos, len; scanf("%d %d", &pos, &len);
            int l = get_k(pos), r = get_k(l + len + 1);
            splay(l, 0), splay(r, l);
            dfs(tr[r].s[0]);
            tr[r].s[0] = 0;
            pushup(r), pushup(l);
        }else if(!strcmp(op, "MAKE_SAME")) {
            int pos, len, c; scanf("%d %d %d", &pos, &len, &c);
            int l = get_k(pos), r = get_k(pos + len + 1);
            splay(l, 0), splay(r, l);
            auto &son = tr[tr[r].s[0]];
            son.same = 1, son.v = c, son.sum = c * son.size;
            if(c > 0) son.ms = son.ls = son.rs = son.sum;
            else son.ms = c, son.ls = son.rs = 0;
            pushup(r), pushup(l);
        }else if(!strcmp(op, "REVERSE")){
            int pos, len; scanf("%d %d", &pos, &len);
            int l = get_k(pos), r = get_k(pos + len + 1);
            splay(l, 0), splay(r, l);
            auto &son = tr[tr[r].s[0]];
            son.rev ^= 1;
            swap(son.ls, son.rs);
            swap(son.s[0], son.s[1]);
            pushup(r), pushup(l);
        }else if(!strcmp(op, "GET-SUM")){
            int pos, len; scanf("%d %d", &pos, &len);
            int l = get_k(pos), r = get_k(pos + len + 1);
            splay(l, 0), splay(r, l);
            printf("%d\n", tr[tr[r].s[0]].sum);
        }else printf("%d\n", tr[root].ms);
    }
    return 0;
}
