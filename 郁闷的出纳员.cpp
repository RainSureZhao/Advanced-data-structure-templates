#include<iostream>
#include<cstring>
using namespace std;
const int maxn = 100010, inf = 1e9;
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
int n, m, delta;
int root, idx;
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
void splay(int x, int k)
{
    while(tr[x].p != k){
        int y = tr[x].p, z = tr[y].p;
        if(z != k){
            if((tr[y].s[1] == x) ^ (tr[z].s[1] == x)) rotate(x);
            else rotate(y);
        }
        rotate(x);
    }
    if(!k) root = x;
}
int insert(int v)
{
    int u = root, p = 0;
    while(u) p = u, u = tr[u].s[v > tr[u].v];
    u = ++ idx;
    if(p) tr[p].s[v > tr[p].v] = u;
    tr[u].init(v, p);
    splay(u, 0);
    return u;
}
int get_k(int k)
{
    int u = root;
    while(true)
    {
        if(tr[tr[u].s[0]].size >= k) u = tr[u].s[0];
        else if(tr[tr[u].s[0]].size + 1 == k) return tr[u].v;
        else k -= tr[tr[u].s[0]].size + 1, u = tr[u].s[1];
    }
    return -1;
}
int get(int v)  // 找到大于等于v的最小的节点
{
    int u = root, res;
    while(u){
        if(tr[u].v >= v) res = u, u = tr[u].s[0];
        else u = tr[u].s[1];
    }
    return res;
}
int main()
{
    scanf("%d %d", &n, &m);
    int L = insert(-inf), R = insert(inf);
    int tot = 0;
    while(n --)
    {
        char op[2];
        int k;
        scanf("%s %d", op, &k);
        if(*op == 'I'){
            if(k >= m) k -= delta, insert(k), tot ++;
        }else if(*op == 'A'){
            delta += k;
        }else if(*op == 'S'){
            delta -= k;
            R = get(m - delta);
            splay(R, 0), splay(L, R);
            tr[L].s[1] = 0;
            pushup(L), pushup(R);
        }else{
            if(tr[root].size - 2 < k) puts("-1");
            else printf("%d\n", get_k(tr[root].size - k) + delta);
        }
    }
    printf("%d\n", tot - (tr[root].size - 2));
    return 0;
}