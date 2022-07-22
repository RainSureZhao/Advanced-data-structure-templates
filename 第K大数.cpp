#include<iostream>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;
#define int long long
const int maxn = 50010;
typedef struct node
{
    int l, r;
    int sum, add;
}WeightTree;
typedef struct node1
{
    int l, r;
    int root;  // 存内层区间线段树的根节点
}ValTree;
typedef struct node2
{
    int op, a, b, c;
}Query;
int n, m;
Query q[maxn];
namespace WeightSegTree
{
    // 内层线段树采用动态开点
    int idx = 0;
    WeightTree trI[maxn * 17 * 17];

    void pushup(WeightTree &u, WeightTree &l, WeightTree &r)
    {
        u.sum = l.sum + r.sum;
    }
    void pushup(int u)
    {
        pushup(trI[u], trI[trI[u].l], trI[trI[u].r]);
    }
    void pushdown(int u, int l, int r)  // 当前层一定有节点
    {
        if(trI[u].add){
            if(!trI[u].l) trI[u].l = ++ idx;
            if(!trI[u].r) trI[u].r = ++ idx;
            int mid = l + r >> 1;
            auto &root = trI[u], &ls = trI[trI[u].l], &rs = trI[trI[u].r];
            ls.add += root.add, ls.sum += (mid - l + 1) * root.add;
            rs.add += root.add, rs.sum += (r - mid) * root.add;
            root.add = 0;
        }
    }
    void modifyI(int u, int l, int r, int pl, int pr)
    {
        if(l >= pl && r <= pr){
            trI[u].add ++;
            trI[u].sum += r - l + 1;
        }else{
            pushdown(u, l, r);
            int mid = l + r >> 1;
            if(pl <= mid){
                if(!trI[u].l) trI[u].l = ++ idx;
                modifyI(trI[u].l, l, mid, pl, pr);
            }
            if(pr > mid){
                if(!trI[u].r) trI[u].r = ++ idx;
                modifyI(trI[u].r, mid + 1, r, pl, pr);
            }
            pushup(u);
        }
    }
    int queryI(int u, int l, int r, int pl, int pr)
    {
        if(l >= pl && r <= pr) return trI[u].sum;
        pushdown(u, l, r);
        int sum = 0;
        int mid = l + r >> 1;
        if(pl <= mid && trI[u].l) sum += queryI(trI[u].l, l, mid, pl, pr);
        if(pr > mid && trI[u].r) sum += queryI(trI[u].r, mid + 1, r, pl, pr);
        return sum;
    }
}
using namespace WeightSegTree;
namespace ValSegTree
{
    vector<int> alls;
    ValTree tr[maxn * 4];
    int get(int x)
    {
        return lower_bound(alls.begin(), alls.end(), x) - alls.begin();
    }
    void build(int u, int l, int r)
    {
        tr[u] = {l, r, ++ idx};
        if(l == r) return;
        int mid = l + r >> 1;
        build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
    }
    void modifyO(int u, int pl, int pr, int x)
    {
        modifyI(tr[u].root, 1, n, pl, pr);
        if(tr[u].l == tr[u].r) return;
        int mid = tr[u].l + tr[u].r >> 1;
        if(x <= mid) modifyO(u << 1, pl, pr, x);
        else modifyO(u << 1 | 1, pl, pr, x);
    }
    int queryO(int u, int pl, int pr, int k)
    {
        if(tr[u].l == tr[u].r) return tr[u].l;
        int cnt = queryI(tr[u << 1 | 1].root, 1, n, pl, pr);
        int mid = tr[u].l + tr[u].r >> 1;
        if(cnt >= k) return queryO(u << 1 | 1, pl, pr, k);
        else return queryO(u << 1, pl, pr, k - cnt);
    }
}
using namespace ValSegTree;
signed main()
{
    scanf("%lld %lld", &n, &m);
    for(int i = 0; i < m; i ++){
        scanf("%lld %lld %lld %lld", &q[i].op, &q[i].a, &q[i].b, &q[i].c);
        if(q[i].op == 1) alls.push_back(q[i].c);
    }
    sort(alls.begin(), alls.end());
    alls.erase(unique(alls.begin(), alls.end()), alls.end());
    build(1, 0, alls.size() - 1);
    for(int i = 0; i < m; i ++){
        if(q[i].op == 1) modifyO(1, q[i].a, q[i].b, get(q[i].c));
        else printf("%lld\n", alls[queryO(1, q[i].a, q[i].b, q[i].c)]);
    }
    return 0;
}