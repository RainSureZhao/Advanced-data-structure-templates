#include<iostream>
#include<cstring>
#include<cmath>
using namespace std;
const int maxn = 100010, M = 350;
#define int long long
int n, m, len;
int w[maxn];
int add[M], sum[M];
int get(int x)
{
    return x / len;
}
void modify(int l, int r, int d)
{
    if(get(l) == get(r)){
        for(int i = l; i <= r; i ++) w[i] += d, sum[get(i)] += d;
    }else{
        int i = l, j = r;
        while(get(i) == get(l)) w[i] += d, sum[get(i)] += d, i ++;
        while(get(j) == get(r)) w[j] += d, sum[get(j)] += d, j --;
        for(int k = get(i); k <= get(j); k ++) sum[k] += len * d, add[k] += d;
    }
}
int query(int l, int r)
{
    int res = 0;
    if(get(l) == get(r)) {
        for(int i = l; i <= r; i ++) res += w[i] + add[get(i)];
    }else{
        int i = l, j = r;
        while(get(i) == get(l)) res += w[i] + add[get(i)], i ++;
        while(get(j) == get(r)) res += w[j] + add[get(j)], j --;
        for(int k = get(i); k <= get(j); k ++) res += sum[k];
    }
    return res;
}
signed main()
{
    cin >> n >> m;
    len = sqrt(n);
    for(int i = 1; i <= n; i ++){
        cin >> w[i];
        sum[get(i)] += w[i];
    }
    while(m --)
    {
        char op; cin >> op;
        if(op == 'C'){
            int l, r, d; cin >> l >> r >> d;
            modify(l, r, d);
        }else{
            int l, r; cin >> l >> r;
            cout << query(l, r) << endl;
        }
    }
    return 0;
}