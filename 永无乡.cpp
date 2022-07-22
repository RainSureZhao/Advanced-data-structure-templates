#include<iostream>
#include<cstring>
using namespace std;
const int maxn = 1000010;
typedef struct node
{
    int s[2], p, v, id;
    int size;
    void init(int _v, int _p, int _id){
        p = _p, v = _v, _id = id;
        size = 1;
    }
}Node;
Node tr[maxn];
int root[maxn], p[maxn], idx;
int n, m;
int find(int x)
{
    if(x != p[x]) p[x] = find(p[x]);
    return p[x];
}
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
void splay(int x, int k, int b)
{
    while(tr[x].p != k){
        int y = tr[x].p, z = tr[y].p;
        if(z != k){
            if((tr[y].s[1] == x) ^ (tr[z].s[1] == y)) rotate(x);
            else rotate(y);
        }
        rotate(x);
    }
    if(!k) root[b] = x;
}
void insert(int v, int id, int b)
{
    int u = root[b], p = 0;
    while(u) p = u, u = tr[u].s[v > tr[u].v];
    u = ++ idx;
    if(p) tr[p].s[v > tr[p].v] = u;
    tr[u].init(v, p, id);
    splay(u, 0, b);
}
int get_k(int k, int b)
{
    int u = root[b];
    while(true)
    {
        if(tr[tr[u].s[0]].size >= k) u = tr[u].s[0];
        else if(tr[tr[u].s[0]].size + 1 == k) return tr[u].id;
        else k -= tr[tr[u].s[0]].size + 1, u = tr[u].s[1];
    }
    return -1;
}
void dfs(int u, int b)
{
    if(tr[u].s[0]) dfs(tr[u].s[0], b);
    if(tr[u].s[1]) dfs(tr[u].s[1], 1);
    insert(tr[u].v, tr[u].id, b);
}

int main()
{
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    cin >> n >> m;
    for(int i = 1; i <= n; i ++) {
        p[i] = root[i] = i;
        int v; cin >> v;
        tr[i].init(v, 0, i);
    }
    idx = n;
    while(m --)
    {
        int a, b; cin >> a >> b;
        a = find(a), b = find(b);
        if(a != b){
            if(tr[root[a]].size > tr[root[b]].size) swap(a, b);
            dfs(root[a], b);
            p[a] = b;
        }
    }
    cin >> m;
    while(m --)
    {
        char op;
        int a, b; cin >> op >> a >> b;
        if(op == 'B'){
            a = find(a), b = find(b);
            if(a != b){
                if(tr[root[a]].size > tr[root[b]].size) swap(a, b);
                dfs(root[a], b);
                p[a] = b;
            }
        }else{
            a = find(a);
            if(tr[root[a]].size < b) puts("-1");
            else cout << get_k(b, a) << endl;
        }
    }
    return 0;
}