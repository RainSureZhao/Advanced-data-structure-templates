#include<iostream>
#include<cstring>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;
const int maxn = 100010;
#define int long long
int n, m, len;
int w[maxn], cnt[maxn];
int ans[maxn];
typedef struct node
{
    int id, l, r;
}Query;
Query q[maxn];
vector<int> nums;
int get(int x)
{
    return x / len;
}
bool cmp(Query &a, Query &b)
{
    int i = get(a.l), j = get(b.l);
    if(i != j) return i < j;
    return a.r < b.r;
}
void add(int x, int &res)
{
    cnt[x] ++;
    res = max(res, cnt[x] * nums[x]);
}
signed main()
{
    scanf("%lld %lld", &n, &m);
    len = sqrt(n);
    for(int i = 1; i <= n; i ++) scanf("%lld", &w[i]), nums.push_back(w[i]);
    sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.begin());
    for(int i = 1; i <= n; i ++) w[i] = lower_bound(nums.begin(), nums.end(), w[i]) - nums.begin();
    for(int i = 0; i < m; i ++){
        int l, r; scanf("%lld %lld", &l, &r);
        q[i] = {i, l, r};
    }
    sort(q, q + m, cmp);
    for(int x = 0; x < m; ){
        int y = x;
        while(y < m && get(q[y].l) == get(q[x].l)) y ++;
        int right = get(q[x].l) * len + len - 1;

        // 暴力求块内的询问
        while(x < y && q[x].r <= right){
            int res = 0;
            int id = q[x].id, l = q[x].l, r = q[x].r;
            for(int k = l; k <= r; k ++) add(w[k], res);
            ans[id] = res;
            for(int k = l; k <= r; k ++) cnt[w[k]] -- ;
            x ++;
        }
        // 求块外的询问
        int res = 0;
        int i = right, j = right + 1;
        while(x < y){
            int id = q[x].id, l = q[x].l, r = q[x].r;
            while(i < r) add(w[ ++ i], res);
            int backup = res;
            while(j > l) add(w[ -- j], res);
            ans[id] = res;
            while(j < right + 1) cnt[w[j ++]] --;
            res = backup;
            x ++;
        }
        memset(cnt, 0, sizeof cnt);
    }
    for(int i = 0; i < m; i ++) printf("%lld\n", ans[i]);
    return 0;
}