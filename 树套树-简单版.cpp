#include<iostream>
#include<cstring>
#include<set>
using namespace std;
const int maxn = 50010, inf = 1e9;
typedef struct node
{
    int l, r;
    multiset<int> s;
}Node;
Node tr[maxn * 4];
int w[maxn];
int n, m;
void build(int u, int l, int r)
{
    tr[u] = {l, r};
    tr[u].s.insert(-inf), tr[u].s.insert(inf);
    for(int i = l; i <= r; i ++) tr[u].s.insert(w[i]);
    if(l == r) return;
    int mid = l + r >> 1;
    build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
}
void modify(int u, int x, int c)
{
    tr[u].s.erase(tr[u].s.find(w[x]));
    tr[u].s.insert(c);
    if(tr[u].l == tr[u].r) return;
    int mid = tr[u].l + tr[u].r >> 1;
    if(x <= mid) modify(u << 1, x, c);
    else modify(u << 1 | 1, x, c);
}
int query(int u, int a, int b, int x)
{
    if(tr[u].l >= a && tr[u].r <= b){
        auto it = tr[u].s.lower_bound(x);
        it --;
        return *it;
    }
    int mid = tr[u].l + tr[u].r >> 1;
    int res = -inf;
    if(a <= mid) res = max(res, query(u << 1, a, b, x));
    if(b > mid) res = max(res, query(u << 1 | 1, a, b, x));
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
            int a, b; scanf("%d %d", &a, &b);
            modify(1, a, b);
            w[a] = b;
        }else{
            int a, b, x; scanf("%d %d %d", &a, &b, &x);
            printf("%d\n", query(1, a, b, x));
        }
    }
    return 0;
}