
#include <bits/stdc++.h>
#define int long long
#define de(x) cout << #x << '=' << x << ", "
#define dd cout << '\n';

/*
本題給n個長方形 0 <= x1, y1, x2, y2 <= 10^9
求被奇數個長方形覆蓋的面積

想法：掃描線+離散化線段樹
區間xor加值 + 區間sum查詢
*/

/*
先將輸入點離散化 [a1, a2, ... , an] => [1, 2, ... , n]
將每個相鄰離散點區間重新編號 [1, 2] => 編號1  [2, 3] => 編號 2 , ... , [n - 1, n] 編號 n - 1
將每個區間的長度都記下 pre[1] = a2 - a1 , pre[2] = a3 - a2 , ... , pre[n - 1] = an - an-1
若要加值區間[a, b]，則加線段樹上的區間[a, b - 1]
和一般線段樹唯一差別在 void apply()，區間長度從(r - l + 1)變成(pre[r] - pre[l - 1])
*/

/*

testcase 1 :
2
0 0 4 4
1 1 3 3

answer : 12

testcase 2 :
4
0 0 10 10
1 1 11 11
2 2 12 12
3 3 13 13

answer : 72

*/

using namespace std;
int tt = 1;
 
int a[600009];
int seg[4 * 600009];
int lazy[4 * 600009];
int mp[600009];
int pre[600009];
 
map<int, int> mp2;
 
void pull(int id){
    seg[id] = seg[id * 2] + seg[id * 2 + 1];
}
void apply(int id, int l, int r, int v){
    seg[id] = (pre[r] - pre[l - 1]) - seg[id];
    lazy[id] ^= 1;
}
 
void push(int id, int l, int r){
    if(!lazy[id] || l == r) return;
    int mid = (l + r) / 2;
    apply(id * 2, l, mid, lazy[id]);
    apply(id * 2 + 1, mid + 1, r, lazy[id]);
    lazy[id] = 0;
}
 
int query(int id, int l, int r, int ql, int qr){
    if(r < ql || qr < l) return 0;
    if(ql <= l && r <= qr) return seg[id];
    push(id, l, r);
    int mid = (l + r) / 2;
    return query(id * 2, l, mid, ql, qr) + query(id * 2 + 1, mid + 1, r, ql, qr);
}
 
void range_add(int id, int l, int r, int ql, int qr, int x){
    if(r < ql || qr < l) return ;
    if(ql <= l && r <= qr) {
        apply(id, l, r, x);
        return;
    }
    push(id, l, r);
    int mid = (l + r) / 2;
    range_add(id * 2, l, mid, ql, qr, x);
    range_add(id * 2 + 1, mid + 1, r , ql, qr, x);
    pull(id);
}
 
void _pre(){
    cout.tie(nullptr);
    //cin >> tt;
}
 
struct rec{
    int loc, down, top;
};
 
bool comp(rec a, rec b){
    return a.loc < b.loc;
}
 
void swap(int &a, int &b){
    int tmp = a;
    a = b;
    b = tmp;
}
 
void solve(){
    int n, a, b, c, d, i, j, k;
    vector<rec> v;
    cin >> n;
    set<int> st;
    for(i = 1; i <= n; i ++){
        cin >> a >> b >> c >> d;
        int high = max(b, d);
        int low = min(b, d);
        v.push_back({a, low, high});
        v.push_back({c, low, high});
        st.insert(low);
        st.insert(high);
    }
    int N = 0;
    pre[0] = 0;
    mp[0] = 0;
    vector<int> vv;
 
    for(auto it = st.begin(); it != st.end(); it ++){
        mp[++ N] = *it;
        mp2[*it]= N;
        vv.push_back(*it);
    }
    
    for(i = 0; i < (int)vv.size() - 1; i ++){
    	pre[i + 1] = pre[i] + (vv[i + 1] - vv[i]);
    }
 
    for(i = 0; i < (int)v.size(); i ++){
        v[i] = {v[i].loc, mp2[v[i].down], mp2[v[i].top] - 1};
    }
    sort(v.begin(), v.end(), comp);
    int ans = 0;
 
    int lastloc = v[0].loc;
    range_add(1, 1, N - 1, v[0].down, v[0].top, 1);
 
    for(i = 1; i < (int)v.size(); i ++){
        int down = v[i].down;
        int top = v[i].top;
        int loc = v[i].loc;
 
        int q = query(1, 1, N - 1, 1, N - 1);
 
        ans += (loc - lastloc) * q;
        range_add(1, 1, N - 1, down, top, 1);
        lastloc = loc;
    }
 
    cout << ans << '\n';
}
signed main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    _pre();
    while(tt--){
        solve();
    }
}
