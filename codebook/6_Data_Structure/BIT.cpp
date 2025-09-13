#define lowbit(x) x & -x

void modify(vector<int> &bit, int idx, int val) {
	for(int i = idx; i <= bit.size(); i+= lowbit(i)) bit[i] += val;
}
 
int query(vector<int> &bit, int idx) {
	int ans = 0;
	for(int i = idx; i > 0; i-= lowbit(i)) ans += bit[i];
	return ans;
}
 
// the first i s.t. a[1]+...+a[i] >= k
int findK(vector<int> &bit, int k) {
	int idx = 0, res = 0;
	int mx = __lg(bit.size()) + 1;
	for(int i = mx; i >= 0; i--) {
		if((idx | (1<<i)) > bit.size()) continue;
		if(res + bit[idx | (1<<i)] < k) {
			idx = (idx | (1<<i)); 
			res += bit[idx];
		}
	}
	return idx + 1;
}
//O(n)建bit
for (int i = 1; i <= n; ++i) {
    bit[i] += a[i];
    int j = i + lowbit(i);
    if (j <= n) bit[j] += bit[i];
}
