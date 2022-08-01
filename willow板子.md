# 数据结构

## Segment_Tree Beats

```c++
// nlog^2 对于取max的jiry线段树
// a the min b the second min
const int N = 1.6e6 + 5;
const int Segment_Size = N * 4;
#define int long long
int n, m, num2[N], cnt2, num[N];
map <int, int> Map, C;
int S[Segment_Size], a[Segment_Size], b[Segment_Size], L[Segment_Size], R[Segment_Size], ans[Segment_Size], laz[Segment_Size];
int cnt[Segment_Size], ll[Segment_Size], rr[Segment_Size];
#define getmid int mid = (L[no] + R[no]) >> 1
#define lc (no << 1)
#define rc (no << 1 | 1)
bool one(int no) {
    return (b[no] == 1e9 + 1);
}
void up(int no) {
    if(L[no] == R[no]) return;
    ans[no] = ans[lc] + ans[rc];
    S[no] = S[lc] + S[rc];
    b[no] = 1e9 + 1;
    if(!one(lc)) b[no] = b[lc];
    if(!one(rc)) b[no] = min(b[no], b[rc]);
    if(a[lc] <= a[rc]) {
        a[no] = a[lc];
        cnt[no] = cnt[lc] + (a[lc] == a[rc]) * cnt[rc];
    }
    else a[no] = a[rc], cnt[no] = cnt[rc];
    if(a[no] != a[lc]) b[no] = min(b[no], a[lc]);
    if(a[no] != a[rc]) b[no] = min(b[no], a[rc]);
    ll[no] = ll[lc];
    rr[no] = rr[rc];
    ans[no] += abs(rr[lc] - ll[rc]);
    if(rr[lc] != ll[rc]) {
        if(rr[lc] == a[no] || ll[rc] == a[no]) {
            ++ cnt[no];
        }
    }
}

void pt(int no, int x) {
    if(x <= a[no]) return;
    ans[no] -= 1LL * cnt[no] * (x - a[no]);
    a[no] = x;
    ll[no] = max(ll[no], x);
    rr[no] = max(rr[no], x);
    laz[no] = max(laz[no], x);
}

void pd(int no) {
    if(!laz[no]) return;
    pt(lc, laz[no]);
    pt(rc, laz[no]);
    laz[no] = 0;
    up(no);
}

void build(int no, int l, int r) {
    if(l > r) return;
    L[no] = l, R[no] = r;
    laz[no] = 0;
    getmid;
    if(l == r) {
        S[no] = num2[l + 1] - num2[l] - 1;
        ans[no] = 0;
        a[no] = 0;
        b[no] = 1e9 + 1;
        cnt[no] = 0;
        ll[no] = rr[no] = 0;
        return;
    }
    build(lc, l, mid);
    build(rc, mid + 1, r);
    up(no);
}

void modify(int no, int l, int r, int x) {
    if(a[no] > x) return;
    if(L[no] == l && R[no] == r) {
        if(b[no] > x) {
            pt(no, x);
            return;
        } 
    }
    pd(no);
    getmid;
    if(l > mid) modify(rc, l, r, x);
    else if(r <= mid) modify(lc, l, r, x);
    else modify(lc, l, mid, x), modify(rc, mid + 1, r, x);
    up(no);
}

long long cur = 0;

struct Qu {
    int l, r, h;
    void input() {
        scanf("%lld%lld%lld", &l, &r, &h);
    }
}Q[N];

set < pair <int, int> > S2;

bool inter(pair <int, int> a, pair <int, int> b) {
    if(a.first > b.first) swap(a, b);
    if(a.second < b.first) return 0;
    return 1;
}

void song(int l, int r) {
    while(S2.size()) {
        set < pair <int, int> > :: iterator it = S2.upper_bound(make_pair(l, 1e9 + 1));
        bool flg = 0;
        if(inter(*it, make_pair(l, r))) {
            l = min((*it).first, l);
            r = max((*it).second, r);
            flg = 1;
            cur -= (*it).second - (*it).first;
            S2.erase(*it);
            continue;
        }
        if(it != S2.begin()) {
            -- it;
            if(inter(*it, make_pair(l, r))) {
                l = min((*it).first, l);
                r = max((*it).second, r);
                flg = 1;
                cur -= (*it).second - (*it).first;
                S2.erase(*it);
                continue;
            }
        }
        if(!flg) break;
    }
    cur += r - l;
    S2.insert(make_pair(l, r));
}

void solve() {
    cin >> n;
    for(int i = 1; i <= n; ++ i) Q[i].input();
    Map.clear();
    for(int i = 1; i <= n; ++ i) ++ Map[Q[i].l], ++ Map[Q[i].r];
    cnt2 = 0; C.clear();
    for(auto x : Map) C[x.first] = ++ cnt2, num2[cnt2] = x.first;
    S2.clear(); cur = 0;
    build(1, 1, cnt2);
    for(int i = 1; i <= n; ++ i){
        song(Q[i].l, Q[i].r);
        Q[i].l = C[Q[i].l];
        Q[i].r = C[Q[i].r] - 1;
        modify(1, Q[i].l, Q[i].r, Q[i].h);
        printf("%lld\n", ans[1] + cur * 2 + ll[1] + rr[1]);
    }
}
```

## 动态区间第k小

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn = 2e6 + 5, maxm = 1e5 + 5;
int n, m, ty[maxm], opl[maxm], opr[maxm], opk[maxm], rt[maxm], val[maxm], all;
vector<int>vec, L, R;
int Id(int x) {
    return lower_bound(vec.begin(), vec.end(), x) - vec.begin() + 1;
}
namespace Seg {
    int cnt = 0;
    struct Node {
        int ch[2], sz;
        int& operator [] (const int &x) {
            return ch[x];
        }
    }tr[maxn];
    void Add(int &o, int old, int l, int r, int p, int d) {
        o = ++ cnt;
        tr[o] = tr[old];
        ++ tr[o].sz;
        if(l == r)
            return;
        int mid = (l + r) >> 1;
        if(p <= mid)
            Add(tr[o][0], tr[old][0], l, mid, p, d);
        else
            Add(tr[o][1], tr[old][1], mid + 1, r, p, d);
    }
    int Ask(int l, int r, int k) {
        if(l == r)
            return l;
        int mid = (l + r) >> 1, suml = 0, sumr = 0;
        for(auto &t : L)
            suml += tr[tr[t][0]].sz;
        for(auto &t : R)
            sumr += tr[tr[t][0]].sz;
        if(sumr - suml >= k) {
            for(auto &t : L)
                t = tr[t][0];
            for(auto &t : R)
                t = tr[t][0];
            Ask(l, mid, k);
        }
        else {
            for(auto &t : L)
                t = tr[t][1];
            for(auto &t : R)
                t = tr[t][1];
            Ask(mid + 1, r, k - (sumr - suml));
        }
    }
}
namespace BIT {
    void Add(int x, int d) {
        int v = Id(val[x]);
        for(; x <= n; x += x & -x)
            Seg :: Add(rt[x], rt[x], 1, all, v, d);
    }
    void Ask(int x, vector<int> &v) {
        for(; x <= n; x += x & -x)
            v.push_back(rt[x]);
    }
}
int main() {
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; ++ i)
        scanf("%d", &val[i]), vec.push_back(val[i]);
    for(int i = 1; i <= m; ++ i) {
        scanf("%d%d%d", &ty[i], &opl[i], &opr[i]);
        if(ty[i] == 1)
            scanf("%d", &opk[i]);
        else
            vec.push_back(opr[i]);
    }
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
    all = vec.size();
    for(int i = 1; i <= n; ++ i)
        BIT :: Add(i, 1);
    for(int i = 1; i <= m; ++ i)
        if(rt[i] == 1) {
            L.clear(), R.clear();
            BIT :: Ask(opl[i] - 1, L), BIT :: Ask(opr[i], R);
            printf("%d\n", vec[Seg :: Ask(1, all, opk[i]) - 1]);
        }
        else
            BIT :: Add(opl[i], -1), val[opl[i]] = opr[i], BIT :: Add(opr[i], 1);
}
```



## 维护历史最大值

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn = 1e5 + 5;
int n, val[maxn], pre[maxn << 1], q;
long long ans[maxn];
vector<pair<int, int> > que[maxn];
namespace Seg {
	long long tag[maxn << 2], his_tag[maxn << 2], mx[maxn << 2], his_mx[maxn << 2];
	inline void Pushdown(int o) {
		if(tag[o] || his_tag[o]) {
			his_mx[o << 1] = max(his_mx[o << 1], mx[o << 1] + his_tag[o]);
			his_tag[o << 1] = max(his_tag[o << 1], tag[o << 1] + his_tag[o]);
			mx[o << 1] += tag[o], tag[o << 1] += tag[o];
			
			his_mx[o << 1 | 1] = max(his_mx[o << 1 | 1], mx[o << 1 | 1] + his_tag[o]);
			his_tag[o << 1 | 1] = max(his_tag[o << 1 | 1], tag[o << 1 | 1] + his_tag[o]);
			mx[o << 1 | 1] += tag[o], tag[o << 1 | 1] += tag[o];

			tag[o] = his_tag[o] = 0;
		}
	}
	inline void Pushup(int o) {
		his_mx[o] = max(his_mx[o << 1], his_mx[o << 1 | 1]), mx[o] = max(mx[o << 1], mx[o << 1 | 1]);
	}
	inline void Update(int o, int l, int r, int ql, int qr, long long d) {
		if(ql <= l && r <= qr) {
			mx[o] += d, tag[o] += d;
			his_mx[o] = max(his_mx[o], mx[o]), his_tag[o] = max(his_tag[o], tag[o]);
			return;
		}
		Pushdown(o);
		int mid = (l + r) >> 1;
		if(ql <= mid)
			Update(o << 1, l, mid, ql, qr, d);
		if(qr > mid)
			Update(o << 1 | 1, mid + 1, r, ql, qr, d);
		Pushup(o);
	}
	inline long long Ask(int o, int l, int r, int ql, int qr) {
		if(ql <= l && r <= qr)
			return his_mx[o];
		Pushdown(o);
		int mid = (l + r) >> 1;
		long long ans = 0;
		if(ql <= mid)
			ans = max(ans, Ask(o << 1, l, mid, ql, qr));
		if(qr > mid)
			ans = max(ans, Ask(o << 1 | 1, mid + 1, r, ql, qr));
		return ans;
	}
}
int main() {
	scanf("%d", &n);
	for(int i = 1; i <= n; ++ i)
		scanf("%d", &val[i]);
	scanf("%d", &q);
	for(int i = 1, l, r; i <= q; ++ i)
		scanf("%d%d", &l, &r), que[r].push_back(make_pair(l, i));
	for(int i = 1; i <= n; ++ i) {
		Seg :: Update(1, 1, n, pre[val[i] + maxn] + 1, i, val[i]), pre[val[i] + maxn] = i;
		for(int j = 0; j < (int)que[i].size(); ++ j)
			ans[que[i][j].second] = Seg :: Ask(1, 1, n, que[i][j].first, i);
	}
	for(int i = 1; i <= q; ++ i)
		printf("%lld\n", ans[i]);
}
```

## Trie 全局异或合并

```cpp
namespace Trie {
	const int Maxn = maxn << 5;
	int son[Maxn][2], tag[Maxn], flag[Maxn], tot;
	inline void Pd(int rt, int dep) {
		if(tag[rt]) {
			tag[son[rt][0]] ^= tag[rt], tag[son[rt][1]] ^= tag[rt];
			if(tag[rt] >> (dep - 1) & 1)
				swap(son[rt][0], son[rt][1]);
			tag[rt] = 0;
		}
	}
	inline void Pu(int rt) {
		flag[rt] = flag[son[rt][0]] && flag[son[rt][1]];
	}
	inline void Insert(int &rt, int k, int dep = 20) {
		if(!rt)
			rt = ++ tot;
		if(!dep) {
			flag[rt] = 1;
			return;
		}
		Pd(rt, dep);
		if(flag[rt])
			return;
		Insert(son[rt][k >> (dep - 1) & 1], k, dep - 1);
		Pu(rt);
	}
	inline int Mex(int rt, int dep = 20) {
		if(!rt)
			return 0;
// cerr << rt << " " << dep << " " << flag[rt] << " " << flag[son[rt][0]] << " " << tag[rt] << endl;
		Pd(rt, dep);
		if(!flag[son[rt][0]])
			return Mex(son[rt][0], dep - 1);
		return 1 << (dep - 1) | Mex(son[rt][1], dep - 1);
	}
	inline int Merge(int x, int y, int dep = 20) {
		if(!x || !y)
			return x + y;
		if(flag[x])
			return x;
		if(flag[y])
			return y;
		int now = ++ tot;
		if(dep) {
			Pd(x, dep), Pd(y, dep);
			son[now][0] = Merge(son[x][0], son[y][0], dep - 1), son[now][1] = Merge(son[x][1], son[y][1], dep - 1);
			Pu(now);
		}
		else
			flag[now] = 1;
		return now;
	}
}
```



## 高维kd树

``` c++
// fastest able to extend to more dimensions 
// n ^ (1 +(d - 1) / d)
const int N = 2e5 + 5;
struct kd {
    int sm[2], sn[2], v[2], Sum, S, ls, rs;
    kd (int a, int b, int c)  {
		ls = rs = 0;
		sm[0] = sn[0] = v[0] = a;
		sm[1] = sn[1] = v[1] = b; 
		s = sum = c;
	}
    kd () {}
};
kd t[N], p[N];
int n, m, A, B, C, D, root, ans;
bool cmp(kd a, kd b) {
    return (a.v[D] == b.v[D]) ? (a.v[D ^ 1] < b.v[D ^ 1]) : (a.v[D] < b.v[D]);
}
void up(int x, int y) {
    for(int i = 0; i < 2; ++ i)
	t[x].sm[i] = max(t[x].sm[i], t[y].sm[i]), t[x].sn[i] = min(t[x].sn[i], t[y].sn[i]);
    t[x].sum += t[y].sum;
}

int build(int l, int r, int d) {
    if(l > r)  return 0;
    int mid = l + r >> 1;
    D = d;
    nth_element(p + l, p + mid, p + r + 1, cmp);
    t[mid] = p[mid];
    t[mid].ls = build(l, mid-1, d ^ 1), t[mid].rs = build(mid + 1, r, d ^ 1);
    if(t[mid].ls)  up(mid, t[mid].ls);
    if(t[mid].rs)  up(mid, t[mid].rs);
    return mid;
}
void ins(int y) {
    int x = root, d = 0;
    while(x != y) {
        pushup(x, y);
        if(t[y].v[0] == t[x].v[0] && t[y].v[1] == t[x].v[1]) {
            t[x].s += t[y].s;
			-- n;
            return;
        }
        if(t[y].v[d] < t[x].v[d]) 
		t[x].ls = (!t[x].ls) ? y: t[x].ls, x = t[x].ls;
        else  
		t[x].rs = (!t[x].rs) ? y : t[x].rs , x = t[x].rs;
        d ^= 1;
    }
}
int query(int x) {
    if(!x || t[x].sm[0] < A || t[x].sn[0] > C ||  t[x].sm[1] < B || t[x].sn[1] > D)
	  return 0;
    if(t[x].sm[0] <= C && t[x].sn[0] >= A && t[x].sm[1] <= D && t[x].sn[1] >= B)
	  return t[x].sum;
    int ret=0;
    if(t[x].v[0] >= A && t[x].v[0] <= C && t[x].v[1] >= B && t[x].v[1] <= D)
	  ret += t[x].s;
    ret += query(t[x].ls) + query(t[x].rs);
    return ret;
}
```



## LCT Simple

```c++
#define lc ch[x][0]
#define rc ch[x][1]
//no lian cao zuo,just liantongxing 
// n log n 
struct NODE {
	int LMAX, RMAX, S, MAX;
	friend NODE operator + (NODE A, NODE B) {
		C.S = A.S + B.S;
		C.LMAX = max(A.S + B.LMAX, A.LMAX);
		C.RMAX = max(B.RMAX, B.S + A.RMAX);
		C.MAX = max(A.MAX, max(B.MAX, A.RMAX + B.LMAX));
		return C;
	}
}C, T[N]; // info, to be changed
multiset <int> Cl[N], Cm[N];
multiset <int> ::reverse_iterator IT;
bool rev[N];int n, m, x, y, rt, ch[N][2], fa[N], val[N]
void up(int x) {
	NODE &D = T[x]; 
	D.S = D.MAX = D.LMAX = D.RMAX = 0;
	D.S = val[x];
	D.MAX = D.LMAX = D.RMAX = max(max(val[x] , 0) , get(x) + val[x]);
	D.MAX = max(get2(x) + val[x], max(D.MAX, (Cm[x].size()) ? (*Cm[x].rbegin()) : 0));
	if(lc) D = T[lc] + D;
	if(rc) D = D + T[rc];
}

void pt(int x) {
	rev[x] ^= 1;
	swap(lc, rc); 
	swap(T[x].LMAX, T[x].RMAX);
}

void pd(int x) {
	if(rev[x]) {
		if(lc)pt(lc); if(rc)pt(rc);
		rev[x] = 0;
	}
}

bool dir(int x) {
	return ch[fa[x]][1] == x;
}

bool isrt(int x) {
	return (ch[fa[x]][1] != x && ch[fa[x]][0] != x);
}

void dn(int x) {
	if(!isroot(x))
	dn(fa[x]);
	pd(x);
}

void rotate(int x) {
	int Dx = dir(x), f = fa[x], Df = dir(fa[x]), GF = fa[f];
	if(!isrt(f)) ch[GF][Df] = x; 
	fa[x] = GF; fa[f] = x; 
	if(ch[x][!Dx]) fa[ch[x][!Dx]] = f;
	ch[f][Dx] = ch[x][!Dx]; ch[x][!Dx] = f; 
	up(f); up(x);
}

void splay(int x) {
	dn(x);
	for(; !isrt(x); rotate(x))
	if(!isrt(fa[x])) rotate(dir(fa[x]) == dir(x) ? fa[x] : x);
}

void access(int x) {
	int t = 0;
	for(; x; t = x, x = fa[x]) {
		splay(x);
		rc = t; up(x);
	}
}

void make_root(int x) {
	access(x); 
	splay(x); 
	pt(x); 
	rt = x;
}

void link(int x , int y) {
	make_root(x);
	make_root(y);
	fa[y] = x;
	A(x, y, 1); 
	up(x);
	rt = x;
}

void cut(int x , int y) {
	make_root(x); 
	access(y); 
	splay(y); 
	ch[y][0] = fa[x] = 0; 
	up(y);
}
```



## LCT链维护

```c++
#define lc ch[x][0]
#define rc ch[x][1]
// LCT complexed version ji suan zuidaquanzhilian 
// n log^2
struct NODE {
	int LMAX, RMAX, S, MAX;

	friend NODE operator + (NODE A, NODE B) {
		C.S = A.S + B.S;
		C.LMAX = max(A.S + B.LMAX, A.LMAX);
		C.RMAX = max(B.RMAX, B.S + A.RMAX);
		C.MAX = max(A.MAX, max(B.MAX, A.RMAX + B.LMAX));
		return C;
	}
}C, T[N]; // info, to be changed
multiset <int> Cl[N], Cm[N];
multiset <int> ::reverse_iterator IT;
bool rev[N];
int n, m, x, y, rt, ch[N][2], fa[N], val[N];
int get(int x) {
	if(!Cl[x].size())
	return 0;
	return *Cl[x].rbegin();
}// choose one 
int get2(int x) {
	if(Cl[x].size() < 2) return get(x);
	IT = Cl[x].rbegin();
	return *IT + *(++ IT);
}//choose two
void up(int x) {
	NODE &D = T[x]; 
	D.S = D.MAX = D.LMAX = D.RMAX = 0;
	D.S = val[x];
	D.MAX = D.LMAX = D.RMAX = max(max(val[x] , 0) , get(x) + val[x]);
	D.MAX = max(get2(x) + val[x], max(D.MAX, (Cm[x].size()) ? (*Cm[x].rbegin()) : 0));
	if(lc) D = T[lc] + D;
	if(rc) D = D + T[rc];
}

void pt(int x) {
	rev[x] ^= 1;
	swap(lc, rc); 
	swap(T[x].LMAX, T[x].RMAX);
}

void pd(int x) {
	if(rev[x]) {
		if(lc)pt(lc); if(rc)pt(rc);
		rev[x] = 0;
	}
}

bool dir(int x) {
	return ch[fa[x]][1] == x;
}

#define isroot(x) (ch[fa[x]][1] != x && ch[fa[x]][0] != x)

void dn(int x) {
	if(!isroot(x))
	dn(fa[x]);
	pd(x);
}

int Dx , f , Df , GF;

void rotate(int x) {
	Dx = dir(x);
	f = fa[x];
	Df = dir(fa[x]);
	GF = fa[f];
	if(!isroot(f)) ch[GF][Df] = x; 
	fa[x] = GF; 
	fa[f] = x; 
	if(ch[x][!Dx]) fa[ch[x][!Dx]] = f;
	ch[f][Dx] = ch[x][!Dx]; 
	ch[x][!Dx] = f; 
	up(f); 
	up(x);
}

void splay(int x) {
	dn(x);
	while(!isroot(x)) {
		if(isroot(fa[x])) {
			rotate(x); 
			return;
		}
		if(dir(x) == dir(fa[x])) 
		rotate(fa[x]), rotate(x);
		else rotate(x), rotate(x);
	}
}

#define del(K, P) K.erase(K.find(P))

void A(int x, int y, int oh) {
	if(oh == 1) {
		Cl[x].insert(T[y].LMAX);
		Cm[x].insert(T[y].MAX);
	}
	else {
		del(Cl[x], T[y].LMAX);
		del(Cm[x], T[y].MAX);
	}
}

void access(int x) {
	int t = 0;
	for(; x; t = x, x = fa[x]){
		splay(x);
		if(rc) A(x, rc, 1);
		if(t) A(x, t, -1);
		rc = t; up(x);
	}
}

void make_root(int x) {
	access(x); 
	splay(x); 
	pt(x); 
	rt = x;
}

void link(int x , int y) {
	make_root(x);
	make_root(y);
	fa[y] = x;
	A(x, y, 1); // of importance 
	up(x);
	rt = x;
}

void cut(int x , int y) {
	make_root(x); 
	access(y); 
	splay(y); 
	ch[y][0] = fa[x] = 0; 
	up(y);
}
```



## Treap

```c++
// cheating version
// nlog n
#pragma GCC optimize(2)
#include <bits/stdc++.h>
using namespace std;

const int N = 1e6 + 5;

int S[N], ch[N][2], n, m, l, r, x, rt, q, D[N], tot = 0;
bool rev[N]; char s[N];

#define pii pair <int, int>
#define mp make_pair
#define lc ch[x][0]
#define rc ch[x][1]

void up(int x) {
	if(x)
	S[x] = S[lc] + S[rc] + 1;
}

void pt(int x) {
	if(!x) return;
	rev[x] ^= 1;
	swap(lc, rc);
}

void dn(int x) {
	if(rev[x]) {
		rev[x] = 0;
		pt(lc); pt(rc);
	}
}

int Rnd() {
	++ tot;
	if(tot > n) tot = 1;
	return D[tot];
	// attention 
	// windows
	return 32768 * rand() + rand();
	// Linux
	return rand();
}

int merge(int x, int y) { // Here x is smaller because Xulie
	if(!x || !y) return x + y;
	dn(x); dn(y);
	if(Rnd() % (S[x] + S[y]) >= S[x]) {
		ch[y][0] = merge(x, ch[y][0]);
		up(y);
		return y;
	}
	else {
		ch[x][1] = merge(ch[x][1], y);
		up(x);
		return x;
	}
}

pii split(int x, int len) {
	dn(x);
	if(!len) return mp(0, x);
	if(S[lc] + 1 == len) {
		int t = ch[x][1];
		ch[x][1] = 0;
		up(x);
		return mp(x, t);
	}
	if(len <= S[lc]) {
		pii now = split(lc, len);
		ch[x][0] = now.second;
		up(x);
		return mp(now.first, x);
	}
	else {
		pii now = split(rc, len - S[lc] - 1);
		ch[x][1] = now.first;
		up(x);
		return mp(x, now.second);
	}
}

int All = 0;

void dfs(int x) {
	dn(x);
	if(ch[x][0]) dfs(ch[x][0]);
	++ All; 
	printf("%d%c", x, (All == n) ? '\n' : ' ');
	if(ch[x][1]) dfs(ch[x][1]);
}

pii A, B, C;

int build(int l, int r) {
	if(l > r) return 0;
	int x = (l + r) >> 1;
	lc = build(l, x - 1);
	rc = build(x + 1, r);
	up(x);
	return x;
}

int main() {
	while(1) {
		scanf("%d%d", &n, &q);
		if(n == -1) return 0;
		for(int i = 1; i <= n; ++ i) D[i] = i;
		random_shuffle(D + 1, D + n + 1);
		tot = 0;
		rt = build(1, n);
		for(int i = 1; i <= q; ++ i) {
			scanf("%s%d%d", s, &l, &r);
			if(s[0] == 'C') {
				scanf("%d", &x);
				A = split(rt, l - 1);
				B = split(A.second, r - l + 1);
				int w = merge(A.first, B.second);
				C = split(w, x);
				rt = merge(C.first, merge(B.first, C.second));
			}
			else {
				A = split(rt, l - 1);
				B = split(A.second, r - l + 1);
				pt(B.first);
				rt = merge(A.first, merge(B.first, B.second));
			}
		}
		All = 0;
		dfs(rt);
	}
}
```

## 可持久化Treap

```c++
// nlog n nlog n
#include <bits/stdc++.h> 
using namespace std;

const int N = 2e5 + 5;
const int MAX = N * 128;

#define LL long long

#define lc ch[x][0]
#define rc ch[x][1] 

namespace Treap{  
	int rt[N], pos[MAX], S[MAX];
	bool rev[MAX];
    int val[MAX];;
	LL Sum[MAX]; 
	int now, tot, ch[MAX][2];
    inline int Rand() {static int seed = 703; return seed = int(seed * 48271LL % (~0u>>1));}
    int newnode(LL x) {
		val[++ tot] =x;
		Sum[tot] = x;
		pos[tot] = Rand();
		S[tot] = 1;
		return tot;
	}
	
    int copy(int x){ 
		int t = newnode(0);
        ch[t][0] = lc;
		ch[t][1] = rc;
		S[t] = S[x];
        val[t] = val[x];
		Sum[t] = Sum[x];
		rev[t] = rev[x];
		return t;
    }
    
    int pt(int x) {
    	x = copy(x);
    	rev[x] ^= 1;
    	return x;
	}
    
    void up(int x){
        S[x] = S[lc] + S[rc] + 1;
        Sum[x] = Sum[lc] + Sum[rc] + val[x];
    }
    
    void dn(int x){ 
		if(!rev[x]) return;
        if(lc) lc = pt(lc);
        if(rc) rc = pt(rc);
        swap(lc, rc);
        rev[x] = 0;
    }
    
    int merge(int x,int y){ 
		if(!x || !y) return x + y;
        if(pos[x] < pos[y]) {
			dn(x);
			rc = merge(rc ,y);
			up(x);
			return x;
		}
        dn(y);
		ch[y][0] = merge(x, ch[y][0]);
		up(y);
		return y;
    }
    
    void split(int rt, int k, int &x, int &y) {
        if(!rt) return x = y = 0, void();
		dn(rt);
        if(S[ch[rt][0]] >= k) 
		y = copy(rt), split(ch[y][0], k, x, ch[y][0]),  up(y);
        else x = copy(rt), split(ch[x][1], k - S[ch[x][0]] - 1, ch[x][1], y), up(x);
    }
} 

using namespace Treap;

int main() {
	int n, v = 0, op, l, r, a, b, c;
	LL ans = 0;
    for(cin >> n; n --;) {
    	scanf("%d%d", &v, &op);
        if(op == 1) {
        	scanf("%d%d", &l, &r); l ^= ans; r ^= ans;
            split(rt[v], l, a, b);
            rt[++ now] = merge(a, merge(newnode(r), b));
        } else if(op == 2) {
            scanf("%d", &l); l ^= ans;
			split(rt[v], l - 1, a, b);
            split(b, 1, b, c);
			rt[++ now] = merge(a, c);
        } else if(op == 3) { 
        	scanf("%d%d", &l, &r);
        	l ^= ans, r ^= ans;
            split(rt[v], r, a, c);
			split(a, l - 1, a, b);
            rev[b]^=1;
			rt[++ now] = merge(a, merge(b, c));
        } else if(op == 4) { 
        	scanf("%d%d", &l, &r);
        	l ^= ans, r ^= ans;
            split(rt[v], r, a, c);
			split(a, l - 1, a, b);
            printf("%lld\n", ans = Sum[b]);
			rt[++ now] = merge(a, merge(b, c));
        }
    } 
	return 0;
}
```



# 字符串

## AC-automaton

```c++
using namespace std;
#include<cstdio>
#define N 204
char s1[N*2],s2[N*2];
int fail[N*2], en[N*2], ch[N*2][26], sz,root;
bool vis[N*2];

void init() {
    sz = root = 1;
}
void build() {
    queue <int> q;
    q.push(1);
    fail[1]=0;
    while(!q.empty()) {
        int ind = q.front(); q.pop();
        for(int i = 0;i < 26; ++ i)
        if(ch[ind][i]) {
            int it = fail[ind];
            while(!ch[it][i] && it) it = fail[it];
            if(!it) it = 1; else it = ch[it][i];
            fail[ch[ind][i]] = it;
            if(!vis[ch[ind][i]]) 
			q.push(ch[ind][i]);
        }
    }
    for(int i = 1; i <= sz; ++ i) {
        for(int j = 0; j < 26; ++ j) {
            if(!ch[i][j]) {
                int it = fail[i];
                while(it && !ch[it][j]) it = fail[it];
                if(!it) ++ it;
				else it = ch[it][j];
                ch[i][j]=it;
            }
        }
    }
}
```

## ex_KMP

```c++
// O (n + m)
const int N = 1e6 + 5; 
int next[N], ex[N];
void getnxt(char *s) {
    int i = 0, j, po, len = strlen(s);
    next[0] = len;
    while(str[i] == str[i + 1] && i + 1 < len) ++ i;
    next[1] = i;
    po = 1;
    for(i = 2; i < len; ++ i) {
        if(next[i - po] + i < next[po] + po)
            next[i] = next[i - po];
        else {
            j = next[po] + po - i;
            if(j < 0) j = 0;
            while(i + j < len && str[j] == str[j + i])
                j ++;
            next[i] = j;
            po = i;
        }
    }
}

void exkmp(char *s1,char *s2) {
    int i = 0, j, po, len = strlen(s1), l2 = strlen(s2);
    getnxt(s2);
    while(s1[i] == s2[i] && i < l2 && i < len)
        ++ i;
    ex[0] = i;
    po = 0;
    for(i = 1; i < len; ++ i) {
        if(next[i - po] + i < ex[po] + po)
            ex[i] = next[i - po];
        else {
            j = ex[po] + po - i;
            if(j < 0) 
			j = 0;
            while(i + j < len && j < l2 && s1[j + i] == s2[j]) ++ j;
            ex[i] = j;
            po = i;
        }
    }
}
```

## ex_SAM

```c++
// O(|S|) 
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
  
const int N = 6e5 + 5;
  
long long ans;
  
int S[N] , MAX[N] , ch[N][26] , n , sz , fa[N] , stk[N] , tag[N] , m , last , p , np , q , nq , top , taged[N];
  
int cur;
  
char s[N] , t[N];
  
void extend(int c) {
    if(ch[last][c] && MAX[ch[last][c]] == MAX[last] + 1) {
        last = ch[last][c]; stk[++ top] = last; taged[last] = cur; ++ tag[last]; return;
    }
    int fl = !!ch[last][c];
    np = last; p = last = ++ sz; MAX[p] = MAX[np] + 1;
    for(;!ch[np][c] && np;np = fa[np]) ch[np][c] = p;
    if(!np) {
        stk[++ top] = last; taged[last] = cur; ++ tag[last]; fa[p] = 1; return;
    }
    else {
        q = ch[np][c];
        if(MAX[q] == MAX[np] + 1) {
            fa[p] = q; stk[++ top] = last; taged[last] = cur; ++ tag[last]; return;
        }
        nq = (!fl) ? ++ sz : p;
        for(register int i = 0;i < 26;++ i) ch[nq][i] = ch[q][i];
        fa[nq] = fa[q]; MAX[nq] = MAX[np] + 1; if(!fl)fa[p] = nq; fa[q] = nq; S[nq] = S[q];
        stk[++ top] = last; taged[last] = cur; ++ tag[last];
        for(;ch[np][c] == q && np;np = fa[np]) ch[np][c] = nq;
        return;
    }
}
  
long long sqr(int x) {
    return x * 1ll * x;
}
  
void chg(int x) {
    ans -= sqr(S[x]) * 1ll * (MAX[x] - MAX[fa[x]]); ans += sqr((S[x] += tag[x])) * 1ll * (MAX[x] - MAX[fa[x]]);
    int now = tag[x];
    tag[x] = 0;
    x = fa[x];
    while(taged[x] != cur) {
        ans += -sqr(S[x]) * 1ll * (MAX[x] - MAX[fa[x]]);
        S[x] += now;
        ans += sqr(S[x]) * 1ll * (MAX[x] - MAX[fa[x]]);
        x = fa[x];
    }
    tag[x] += now;
}
  
main(void) {
    scanf("%d" , &n); sz = 1;
    for(int i = 1;i <= n;++ i) {
        scanf("%s" , s + 1);
        m = strlen(s + 1);
        last = 1; top = 0; cur = i;
        for(register int j = 1;j <= m;++ j) extend(s[j] - 'a'); taged[1] = cur;
        for(register int j = top;j >= 1;-- j) {
            chg(stk[j]);
        }
        printf("%lld\n" , ans);
    }
}
```

## KMP

```c++
// O(n) 
for(int i = 1;i <= n;++ i) {
    int now = nxt[i - 1];
    while(now != -1 && s[now + 1] != s[i]) now = nxt[now];
    nxt[i] = now + 1;
}
```



## manacher

```c++
//O(n)
int len;
char s[N], str[N];
 
void getstr(){
    int k = 0;
    str[k ++] = '$';
    for(int i = 0; i < len; ++ i)
        str[k ++] = '#',
        str[k ++] = s[i];
    str[k ++] = '#';
    len = k;
}

void Manacher() {
    getstr();
    int mx = 0, id;
    for(int i = 1; i < len; ++ i)
    {
        if(mx > i) 
		Len[i] = min(Len[2 * id - i], mx - i);
        else Len[i] = 1;
        while(str[i + Len[i]] == str[i - Len[i]]) 
            ++ Len[i];
        if(Len[i] + i>mx)
            mx = Len[i] + i, id = i;
    }
}
```

## onewordforjcvb

```c++
if usable
border's length between [2^(i - 1), 2 ^ i]
should be a 等差数列
consider a border u.
|u| >2 ^ (i - 1)
then every other border would be a border of it
and let two others are s1 and s2 (|s1| is the longest) 
then 
|u| - |s1| and |u| - |s2| is a 周期 of u
and both longer than the half length
so |u| - |s1| + |u| - |s2| < |u|
so |u| - |s1| = gcd(|u| - |s1|, |u| - |s2|)
proved
```

## PAM

```c++
//O(n)
/// the fastest one PAM APIO2014
#include <cstdio>
#include <cstring>
#include <algorithm>
 
using namespace std;
typedef long long LL;
const int N = 300005;
char S[N];
int son[N][26], fail[N];
int cnt[N], len[N];
int n, st, i, x, cur, nw, last;
LL ans;
 
int newnode(int x) {
  len[st] = x;
  cnt[st] = 0;
  return st++;
}
 
void init() {
  gets(S + 1);
  n = strlen(S + 1);
  S[0] = '#';
  newnode(0);
  newnode(-1);
  fail[0] = 1;
  last = 0;
}
 
int get_fail(int x, int n) {
  while (S[n - len[x] - 1] != S[n]) x = fail[x];
  return x;
}
 
int main() {
  init();
  for(int i = 1; i <= n; ++ i) {
    x = S[i] - 'a';
    cur = get_fail(last, i);
    if (!son[cur][x]) {
      nw = newnode(len[cur] + 2);
      fail[nw] = son[get_fail(fail[cur], i)][x];
      son[cur][x] = nw;
    }
    last = son[cur][x];
    cnt[last]++;
  }
  for (int i = st - 1; i >= 0; -- i)
    cnt[fail[i]] += cnt[i];
  for (int i = 2; i < st; ++ i)
    ans = max(ans, (LL)len[i] * cnt[i]);
  printf("%lld\n", ans);
  return 0;
}
```



## SA

```c++
// nlog n
// SA can be fully replaced by SAM
//so we get here a O(nlogn) approach For its thoughts.(Can be used in jcvb) 
#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

#define REP(i, a, b) for(int i = (a); i <= (b); ++ i)
#define PER(i, a, b) for(int i = (a); i >= (b); -- i)

int n;
char s[N];

namespace SA {
  int Mem[N * 10], sa[N], H[N], rk[N];
  
  void build(int m) {
    int *x = Mem, *y = Mem + (N << 1), *z = y + (N << 1);
    REP(i, 1, m) z[i] = 0;
    REP(i, 1, n) ++ z[x[i] = s[i]];
    REP(i, 1, m) z[i] += z[i - 1];
    REP(i, 1, n) sa[z[x[i]] --] = i;
    for(int k = 1; k <= n; k <<= 1) {
      int p = 0;
      REP(i, n - k + 1, n) y[++ p] = i;
      REP(i, 1, n) if(sa[i] > k) y[++ p] = sa[i] - k;
      REP(i, 1, m) z[i] = 0;
      REP(i, 1, n) ++ z[x[i]];
      REP(i, 1, m) z[i] += z[i - 1];
      PER(i, n, 1) sa[z[x[y[i]]] --] = y[i];
      swap(x, y); p = 0;
      REP(i, 1, n) x[sa[i]] = (y[sa[i]] == y[sa[i - 1]] && y[sa[i] + k] == y[sa[i - 1] + k]) ? p : ++ p;
      if(p == n) break;
      m = p;
    }
  }

  void getheight() {
    int Now = 0;
    REP(i, 1, n) rk[sa[i]] = i;
    REP(i, 1, n) {
      if(Now) -- Now;
      if(rk[i] == n) continue;
      while(s[i + Now] == s[sa[rk[i] + 1] + Now]) ++ Now;
      H[rk[i]] = Now;
    }
  }
}

int main() {
  scanf("%s", s + 1); n = strlen(s + 1);
  SA :: build('z');
  SA :: getheight();
  REP(i, 1, n) printf("%d ", SA :: sa[i]); puts("");
  REP(i, 1, n - 1) printf("%d ", SA :: H[i]); puts("");
}
```

## SAM

```c++
// O(n)
void extend(int c) {
    if(ch[last][c] && MAX[ch[last][c]] == MAX[last] + 1) {
        last = ch[last][c]; stk[++ top] = last; taged[last] = cur; ++ tag[last]; return;
    }
    int fl = !!ch[last][c];
    np = last; p = last = ++ sz; MAX[p] = MAX[np] + 1;
    for(;!ch[np][c] && np;np = fa[np]) ch[np][c] = p;
    if(!np) {
        stk[++ top] = last; taged[last] = cur; ++ tag[last]; fa[p] = 1; return;
    }
    else {
        q = ch[np][c];
        if(MAX[q] == MAX[np] + 1) {
            fa[p] = q; stk[++ top] = last; taged[last] = cur; ++ tag[last]; return;
        }
        nq = (!fl) ? ++ sz : p;
        for(register int i = 0;i < 26;++ i) ch[nq][i] = ch[q][i];
        fa[nq] = fa[q]; MAX[nq] = MAX[np] + 1; if(!fl)fa[p] = nq; fa[q] = nq; S[nq] = S[q];
        stk[++ top] = last; taged[last] = cur; ++ tag[last];
        for(;ch[np][c] == q && np;np = fa[np]) ch[np][c] = nq;
        return;
    }
}
  
```

## 最小表示法

```c++
// O(n)
int k = 0, i = 0, j = 1;
while (k < n && i < n && j < n) {
  if (sec[(i + k) % n] == sec[(j + k) % n]) {
    k ++;
  } else {
    sec[(i + k) % n] > sec[(j + k) % n] ? i = i + k + 1 : j = j + k + 1;
    if (i == j) i ++;
    k = 0;
  }
}
i = min(i, j);
return i;
```

## 最小回文串拆分

```c++
// O(n log n )
// O(n * sigma) to build PAM 
#include <bits/stdc++.h>
using namespace std;

const int N = 1000000 + 5;
 
namespace pam {
	
  int sz, tot, last;
  int ch[N][26], len[N], fail[N];
  int cnt[N], dep[N], dif[N], slink[N];
  char s[N];
  
  int node(int l) {
    ++ sz; 
    memset(ch[sz], 0, sizeof(ch[sz]));
    len[sz] = l; 
    fail[sz] = fail[sz] = dep[sz] = 0;
    return sz;
  }
  
  void clear() {
    sz = -1; last = 0;
    s[tot = 0] = '$';
    node(0); node(-1);
    fail[0] = 1;
  }
  
  int getfail(int x) {
    while (s[tot - len[x] - 1] != s[tot]) x = fail[x];
    return x;
  }
  
  void insert(char c) {
    s[++ tot] = c;
    int now = getfail(last);
    if (!ch[now][c - 'a']) {
      int x = node(len[now] + 2);
      fail[x] = ch[getfail(fail[now])][c - 'a'];
      dep[x] = dep[fail[x]] + 1;
      ch[now][c - 'a'] = x;
 
      dif[x] = len[x] - len[fail[x]];
      if (dif[x] == dif[fail[x]]) slink[x] = slink[fail[x]];
      else slink[x] = fail[x];
    }
    last = ch[now][c - 'a'];
    cnt[last]++;
  }
  
}
using pam :: len;
using pam :: fail;
using pam :: slink;
using pam :: dif;
 
int n, dp[N], g[N], pre[N], pre2[N]; 
char s[N], t[N], c[N];
 
void print(int x) {
	if(!x) return;
	if(pre[x] + 2 == x); 
	else printf("%d %d\n", pre[x] / 2 + 1, x / 2);
	print(pre[x]);
	return;
}
 
int main() {
  pam :: clear();
  scanf("%s", s + 1);
  n = strlen(s + 1); 
  dp[0] = 0;
  for(int i = 1; i <= n; ++ i) dp[i] = 1e9;
  for(int i = 1; i <= n; ++ i) {
    pam :: insert(s[i]);
    for(int x = pam :: last; x > 1; x = slink[x]) {
      g[x] = dp[i - len[slink[x]] - dif[x]] + 1, 
	  pre2[x] = i - len[slink[x]] - dif[x];
      if (dif[x] == dif[fail[x]]) {
      	if(g[fail[x]] < g[x])
      	g[x] = g[fail[x]], pre2[x] = pre2[fail[x]];
	  }
	  if(g[x] < dp[i]) {
	  	dp[i] = g[x];
	  	pre[i] = pre2[x];
	  }
    }
  }
  cout << dp[n] << endl;
  return 0;
}
```



# 图论

## 一般图最大匹配

```c++
// 挑战npc 
#include <bits/stdc++.h>
using namespace std;

const int N = 1e3 + 5;

int n, m, x, y, mat[N], pre[N], fa[N], ty[N], e, n2;
bool way[N][N], inq[N];

void add(int x, int y) {
  way[x][y] = way[y][x] = 1;
}

int getf(int x) {
  return (fa[x] == x) ? x : fa[x] = getf(fa[x]);
}

void init(void) {
  for(int i = 1;i <= n2;++ i) 
  fa[i] = i, pre[i] = ty[i] = 0;
}

bool vis[N];

int LCA(int x , int y) {
  memset(vis , 0 , sizeof(vis));
  for(int i = x; i; i = pre[mat[i]]) i = getf(i), vis[i] = 1;
  for(int i = y; i; i = pre[mat[i]]) {
  		i = getf(i); 
  		if(vis[i]) return i;
  }
}

queue <int> q;

void shrink(int x , int y , int lca) {
  while(getf(x) != lca) {
    pre[x] = y; y = mat[x];
    if(ty[y] == 2) q.push(y); ty[y] = 1;
    fa[x] = fa[y] = lca;
    x = pre[y];
  }
}

int aug(int x) {
  memset(inq, 0, sizeof(inq)); init();
  while(!q.empty()) q.pop();
  q.push(x); inq[x] = 1; ty[x] = 1;
  while(!q.empty()) {
    int u = q.front(); q.pop();
    for(int i = 1; i <= n2; ++ i) if(way[u][i]) {
	if(ty[i] == 2 || getf(i) == getf(u)) continue;
	if(!ty[i]) {
	  pre[i] = u; ty[i] = 2;
	  if(!mat[i]) {
	    for(int a = i, la ,b; a; a = la) {
	      la = pre[a];
		  b = mat[la]; 
		  mat[a] = la; 
		  mat[la] = a;
		  la = b;
	    }
	    return 1;
	  }
	  ty[mat[i]] = 1; q.push(mat[i]); inq[mat[i]] = 1;
	}
	else {
	  int lca = LCA(u, i);
	  shrink(u, i, lca); shrink(i, u, lca);
	}
      }
  }
  return 0;
}

void onedance(void) {
  memset(mat, 0, sizeof(mat)); memset(way, 0, sizeof(way));
  int ans = 0;
  scanf("%d%d%d" , &n, &m, &e);
  n2 = n + 3 * m;
  for(int i = 1; i <= e; ++ i) {
    scanf("%d%d" , &x, &y);
    for(int j = 1; j <= 3; ++ j) add(x, n + 3 * y - 3 + j);
  }
  for(int i = 1; i <= m; ++ i) {
    for(int j = 1; j <= 3; ++ j) {
      for(int k = j + 1; k <= 3; ++ k) add(n + 3 * i - 3 + j, n + 3 * i - 3 + k);
    }
  }
  for(int i = 1; i <= n2; ++ i) if(!mat[i]) ans += aug(i);
  cout << ans - n << endl;
  for(int i = 1; i <= n;++ i) {
    printf("%d ", (mat[i] - n + 3 - 1) / 3);
  }
  puts("");
}

int main() {
  int t;
  for(scanf("%d", &t);t --;) {
    onedance();
  }
}
```

## 支配树

$有一个起点r，能到所有点x。u在到x路径上的必经之路。u支配x。$

```c++
int n, m, timer, root, dfn[MAXN], p[MAXN], father[MAXN];
int idom[MAXN], sdom[MAXN], f[MAXN], home[MAXN];
long long ans[MAXN]; vector <int> a[MAXN], b[MAXN], c[MAXN];
void dfs(int pos) {
	dfn[pos] = ++timer, p[timer] = pos;
	for (unsigned i = 0; i < a[pos].size(); i++)
		if (dfn[a[pos][i]] == 0) {
			father[a[pos][i]] = pos;
			dfs(a[pos][i]);
		}
}
int F(int x) {
	if (f[x] == x) return x;
	int tmp = f[x];
	f[x] = F(f[x]);
	if (sdom[home[tmp]] < sdom[home[x]]) home[x] = home[tmp];
	return f[x];
}
int gethome(int x) {
	F(x);
	return home[x];
}
void work(int pos, long long sum) {
	ans[pos] = sum;
	for (unsigned i = 0; i < a[pos].size(); i++)
		work(a[pos][i], sum + a[pos][i]);
}
int main() {
	while (scanf("%d%d", &n, &m) != EOF) {
		for (int i = 1; i <= n; i++) {
			a[i].clear();
			b[i].clear();
			c[i].clear();
		}
		for (int i = 1; i <= m; i++) {
			int x, y; read(x), read(y);
			a[x].push_back(y);
			b[y].push_back(x);
		}
		memset(dfn, 0, sizeof(dfn));
		timer = 0; dfs(root = n);
		for (int i = 1; i <= timer; i++) {
			sdom[p[i]] = i;
			idom[p[i]] = 0;
			f[p[i]] = home[p[i]] = p[i];
		}
		for (int i = timer; i >= 2; i--) {
			int tmp = p[i];
			for (unsigned j = 0; j < b[tmp].size(); j++)
				if (dfn[b[tmp][j]]) chkmin(sdom[tmp], sdom[gethome(b[tmp][j])]);
			c[sdom[tmp]].push_back(tmp);
			f[tmp] = father[tmp];
			tmp = dfn[father[tmp]];
			for (unsigned j = 0; j < c[tmp].size(); j++) {
				int tnp = gethome(c[tmp][j]);
				if (sdom[tnp] == tmp) idom[c[tmp][j]] = tmp;
				else idom[c[tmp][j]] = dfn[tnp];
			}
			c[tmp].clear();
		}
		for (int i = 1; i <= n; i++)
			a[i].clear();
		for (int i = 2; i <= timer; i++) {
			int tmp = p[i];
			if (sdom[tmp] == idom[tmp]) idom[tmp] = p[idom[tmp]];
			else idom[tmp] = idom[p[idom[tmp]]];
			sdom[tmp] = p[sdom[tmp]];
			a[idom[tmp]].push_back(tmp);
		}
		memset(ans, 0, sizeof(ans));
		work(root, root);
		for (int i = 1; i <= n; i++) {
			write(ans[i]);
			if (i == n) putchar('\n');
			else putchar(' ');
		}
	}
	return 0;
}

```

## k短路

简单的贪心

```c++
#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;
const int M = N * 10;

int n, m, x, y, fir[N], ne[M],  to[M], cnt = 1;
double C[M], e, z;

vector < int > G[N];
vector < double > Co[N];

void add(int x, int y, double z) {
	ne[++ cnt] = fir[x];
	fir[x] = cnt;
	to[cnt] = y;
	C[cnt] = z;
	G[y].push_back(x);
	Co[y].push_back(z);
}

struct NODE {
	double dis;
	int lst;
	
	friend bool operator < (NODE a, NODE b) {
		return a.dis > b.dis;
	}
}now;

priority_queue < NODE > q;
double dis[N]; int fa[N];
vector <int> Son[N];

namespace dij {
	bool vis[N];
	void build(int s, int t) {
		for(int i = 1; i <= n; ++ i) dis[i] = 2e9, vis[i] = 0;
		dis[t] = 0;
		q.push((NODE){0, t});
		while(!q.empty()) {
			now = q.top();
			q.pop();
			int it = now.lst;
			if(vis[it]) continue;
			vis[it] = 1;
			for(int i = 0; i < (int) G[it].size(); ++ i) {
				int V = G[it][i];
				if(vis[V]) continue;
				double Dis = dis[it] + Co[it][i];
				if(Dis >= dis[V]) continue;
				dis[V] = Dis; fa[V] = it;
				q.push((NODE){Dis, V});
			}
		}
		for(int i = 1; i <= n; ++ i) Son[fa[i]].push_back(i);
	}
	
}

struct info {
	double val;
	int dep, lc, rc, v;
}T[M * 30];
int sz;

int newnode(int x) {
	if(T[x].val == -1) return 0;
	++ sz;
	T[sz] = T[x];
	return sz;
}

int merge(int x, int y) {
	if(!x || !y) {
		return newnode(x + y);
	}
	if(T[x].val > T[y].val) swap(x, y);
	// T[x].val < T[y].val
	int p = newnode(x);
	T[p].rc = merge(T[x].rc, y);
	if(T[T[p].rc].dep > T[T[p].lc].dep)
	swap(T[p].lc, T[p].rc);
	T[p].dep = T[T[p].rc].dep + 1;
	return p;
}

int rt[N];

bool fcmp(double x) {
	if(fabs(x) <= 1e-7) return 0;
	return 1;
}

#define Foreachson(i, x) for(int i = fir[x]; i; i = ne[i]) 

void dfs(int x) {
	T[0].val = T[0].lc = T[0].rc = T[0].dep = 0;
	T[0].val = -1;
	rt[x] = merge(rt[x], rt[fa[x]]);
	
	bool cnt = 0;
	
	if(x != n)
	Foreachson(i, x) {
		int V = to[i];
		if(!cnt) {
			if(V == fa[x] && !fcmp(dis[x] - dis[V] - C[i])) {
				cnt = 1; 
				continue;
			}
		}
		int now = newnode(sz + 1);
		T[now].val = C[i] + dis[V] - dis[x]; T[now].v = V;
		rt[x] = merge(rt[x], now);
	}
	
	if(x != n) {
		assert(cnt);
	}
	
	for(int i = 0; i < (int) Son[x].size(); ++ i) {
		int V = Son[x][i];
		dfs(V);
	}
}

void getStack() {
	dfs(n);
}

void getans() {
	int ans = 1;
	e -= dis[1];
	q.push((NODE){dis[1] + T[rt[1]].val, rt[1]});
	while(!q.empty()) {
		NODE now = q.top(); q.pop();
		
		if(now.dis > e) {
			cout << ans << endl;
			return;
		}
		
		e -= now.dis; ++ ans;
		int it = now.lst;
		if(now.lst) {
			if(T[it].lc) {
				q.push((NODE){now.dis - T[it].val + T[T[it].lc].val, T[it].lc});
			}
			if(T[it].rc) {
				q.push((NODE){now.dis - T[it].val + T[T[it].rc].val, T[it].rc});

			}
		}
		if(rt[T[it].v]) {
			q.push((NODE){now.dis + T[rt[T[it].v]].val, rt[T[it].v]});
			
		}
	}
	cout << ans << endl;
}

int main() {
//	freopen("P2483_1.in", "r", stdin);
//	freopen("out1.txt", "w", stdout);
	cin >> n >> m >> e;
	for(int i = 1; i <= m; ++ i) {
		scanf("%d%d%lf", &x, &y, &z);
		add(x, y, z);
	}
	
	dij :: build(1, n);
	getStack();
	getans();
}
```

## 最小树形图

O(nlogm)

```c++
#include<iostream>
#include<cstring>
using namespace std;
#define int long long
const int N=1e6+10,INF=0x3f3f3f3f;
struct ltt_node{
	int lson,rson;
	int val,tag;
	int from,to;
	int dis;
};
struct leftist_tree{
	ltt_node ltt[N];
	int tot;
	inline int newnode(int val,int from,int to){
		tot++;
		ltt[tot].val=val;
		ltt[tot].from=from;
		ltt[tot].to=to;
		return tot;
	}
	inline void pushdown(int now){
		int ls=ltt[now].lson,rs=ltt[now].rson;
		ltt[ls].val+=ltt[now].tag;
		ltt[rs].val+=ltt[now].tag;
		ltt[ls].tag+=ltt[now].tag;
		ltt[rs].tag+=ltt[now].tag;
		ltt[now].tag=0;
	}
	int merge(int x,int y){
		if(!x||!y) return x+y;
		pushdown(x),pushdown(y);
		if(ltt[x].val>ltt[y].val) swap(x,y);
		ltt[x].rson=merge(ltt[x].rson,y);
		if(ltt[ltt[x].rson].dis>ltt[ltt[x].lson].dis)
			swap(ltt[x].lson,ltt[x].rson);
		ltt[x].dis=ltt[ltt[x].rson].dis+1;
		return x;
	}
	int del(int rt){
		pushdown(rt);
		int ls=ltt[rt].lson;
		int rs=ltt[rt].rson;
		return merge(ls,rs);
	}
};//左偏树基本操作
leftist_tree ltt;
int root[N],fa[N];
int sta[N],top;
bool vis[N];
inline int find(int x){
	return x==fa[x]?x:fa[x]=find(fa[x]);
}//并查集，用于查找一个点被收缩多次后的新点
signed main(){
	int n,m,r;
	cin>>n>>m>>r;
	for(int i=1;i<=m;i++){
		int x,y,z;
		cin>>x>>y>>z;
		int lp=ltt.newnode(z,x,y);//新建一个左偏树节点，边从x到y，边权为z
		root[y]=ltt.merge(root[y],lp);//插入到y的左偏树中
	}
	for(int i=1;i<=n;i++){
		int x=i,y=i%n+1;
		int p=ltt.newnode(INF,x,y);
		root[y]=ltt.merge(root[y],p);
	}//加入n条边强行使其强连通
	for(int i=1;i<=2*n;i++) fa[i]=i;//算上收缩的点共有2n个点
	sta[++top]=r,vis[r]=true;
	int ans=0,cnt=n;
	while(root[sta[top]]){//还有边
		int lp=root[sta[top]];
		ltt_node tmp=ltt.ltt[lp];
		int u=find(tmp.from);
		if(u==sta[top]){
			root[sta[top]]=ltt.del(root[sta[top]]);
			continue;
		}//自环
		if(!vis[u]){
			sta[++top]=u;
			vis[u]=true;
			continue;
		}//不构成环，加入即可
		int p=++cnt;//把环缩为p
		while(vis[u]){//u还没被弹出
			int v=sta[top--];//环上的节点
			vis[v]=false,fa[v]=p;//这个点缩成了p
			int val=ltt.ltt[root[v]].val;//最短弧的边权
			ltt.ltt[root[v]].tag-=val;//懒标记
			int x=find(ltt.ltt[root[v]].to);
			ans+=(x!=find(r))*val;//如果x等于r，说明这条边通向r，不能选
			root[v]=ltt.del(root[v]);//删掉最短弧
			root[p]=ltt.merge(root[p],root[v]);//合并到p的左偏树上
		}//把整个环找出来
		sta[++top]=p;
		vis[p]=true;//把p加入
	}
	cout<<(ans>=INF?-1:ans)<<'\n';
}
```





## 克鲁斯卡尔重构树

NOI2018归程

```cpp
#pragma GCC optimize(3)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 800005;
struct Edge {
    int u, v, l, a;
    Edge(int u = 0, int v = 0, int l = 0, int a = 0): u(u), v(v), l(l), a(a) {}
    inline bool operator < (const Edge &rhs) const {
        return a > rhs.a;
    }
} E[maxn];
vector<Edge>edges;
vector<int>G[maxn];
int n, m, T, fa[maxn], anc[maxn], val[maxn], dis[maxn], hei[maxn], g[maxn][20], q, k, s;
bool vis[maxn];
struct Node {
    int u;
    ll d;
    Node(int u = 0, ll d = 0): u(u), d(d) {}
    inline bool operator < (const Node &rhs) const {
        return d > rhs.d;
    }
};
priority_queue<Node>Q;
inline void Dijkstra() {
    while (!Q.empty())
        Q.pop();
    memset(dis, 0x3f, sizeof dis);
    memset(vis, 0, sizeof vis);
    dis[1] = 0;
    Q.push(Node(1, 0));
    while (!Q.empty()) {
        Node now = Q.top();
        Q.pop();
        int u = now.u;
        ll d = now.d;
        if(vis[u])
            continue;
        vis[u] = 1;
        for(int i = 0; i < (int)G[u].size(); ++i) {
            Edge &e = edges[G[u][i]];
            int v = e.v;
            if(dis[v] > d + e.l)
                dis[v] = d + e.l, Q.push(Node(v, dis[v]));
        }
    }
}
inline int gf(int x) {
    return x == fa[x] ? x : fa[x] = gf(fa[x]);
}
int main() {
    for(scanf("%d", &T); T--; ) {
		scanf("%d%d", &n, &m);
        edges.clear();
        for(int i = 1; i <= n; ++i)
            G[i].clear();
        for(int i = 1; i <= m; ++i)
            scanf("%d%d%d%d", &E[i].u, &E[i].v, &E[i].l, &E[i].a);
		scanf("%d%d%d", &q, &k, &s);
        sort(E + 1, E + m + 1);
        for(int i = 1; i <= m; ++i) {
            edges.push_back(Edge(E[i].u, E[i].v, E[i].l));
            G[E[i].u].push_back(edges.size() - 1);
            edges.push_back(Edge(E[i].v, E[i].u, E[i].l));
            G[E[i].v].push_back(edges.size() - 1);
        }
        Dijkstra();
        for(int i = 1; i <= n; ++i)
            fa[i] = anc[i] = i;
        int cnt = n;
        for(int i = 1; i <= m; ++i) {
            int u = E[i].u, v = E[i].v;
            cnt ++, fa[cnt] = cnt, fa[u] = gf(u), fa[v] = gf(v);
            dis[cnt] = min(dis[fa[u]], dis[fa[v]]);
            anc[cnt] = anc[fa[u]] = anc[fa[v]] = cnt;
            fa[fa[u]] = fa[fa[v]] = cnt;
            hei[cnt] = E[i].a;
        }
        for(int i = 1; i <= cnt; ++i)
            g[i][0] = anc[i];
        for(int i = 1; i < 20; ++i)
            for(int j = 1; j <= cnt; ++j)
                g[j][i] = g[g[j][i - 1]][i - 1];
        for(int i = 1; i <= n; ++i)
            hei[i] = s + 1;
        int lstans = 0;
        for(int x, y; q --; ) {
			scanf("%d%d", &x, &y);
            x = (x + k * lstans - 1) % n + 1;
            y = (y + k * lstans) % (s + 1);
            for(int i = 19; ~i; --i)
                if(hei[g[x][i]] > y)
                    x = g[x][i];
            printf("%d\n", lstans = dis[x]);
        }
    }
}
```



## 长链剖分(O(nlogn)-O(1)求k次祖先)

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn = 5e5 + 5;
int n, m, son[maxn], dep[maxn], maxd[maxn], len[maxn], anc[maxn][20], top[maxn], highbit[maxn];
vector<int>G[maxn];
void dfs1(int u, int fa) {
	maxd[u] = dep[u] = dep[fa] + 1;
	anc[u][0] = fa;
	for(int i = 1; i < 20; ++ i)
		anc[u][i] = anc[anc[u][i - 1]][i - 1];
	for(auto v : G[u]) {
		if(v == fa)
			continue;
		dfs1(v, u);
		if(maxd[v] > maxd[son[u]])
			son[u] = v, maxd[u] = maxd[v];
	}
}
void dfs2(int u, int tp) {
	top[u] = tp;
	len[u] = maxd[u] - dep[top[u]] + 1;
	if(son[u])
		dfs2(son[u], tp);
	for(auto v : G[u])
		if(v != anc[u][0] && v != son[u])
			dfs2(v, v);
}
vector<int> U[maxn], D[maxn];
int Query(int u,int k) {
	if(k > dep[u])
		return 0;
	if(!k)
		return u;
	u = anc[u][highbit[k]];
	k ^= 1 << highbit[k];
	if(!k)
		return u;
	if(dep[u] - dep[top[u]] == k)
		return top[u];
	if(dep[u] - dep[top[u]] > k)
		return D[top[u]][dep[u] - dep[top[u]] - k - 1];
	return U[top[u]][k - dep[u] + dep[top[u]] - 1];
}
int main() {
	scanf("%d", &n);
	for(int i = 1, u, v; i < n; ++ i) {
		scanf("%d%d", &u, &v);
		G[u].push_back(v), G[v].push_back(u);
	}
	dfs1(1, 0);
	dfs2(1, 1);
	for(int i = 1; i <= n; ++ i)
		if(i==top[i]) {
			int l = 0, x = i;
			while(l < len[i] && x)
				x = anc[x][0], ++ l, U[i].push_back(x);
			l = 0, x = i;
			while(l < len[i])
				x = son[x], ++ l, D[i].push_back(x);
		}
	int mx = 1;
	for(int i = 1; i <= n; ++ i) {
		if((i >> mx) & 1)
			++ mx;
		highbit[i] = mx - 1;
	}
	scanf("%d", &m);
	for(int u, v, ans = 0; m --;) {
		scanf("%d", &u), u ^= ans;
		scanf("%d", &v), v ^= ans;
		printf("%d\n",ans = Query(u, v));
	}
}
```



## 树分治

#### 点分治、点分树

例：与 $u$ 距离 $\leq k$ 的点的权值和

```c++
struct DS {
    void update(int x, int d, int v) //在 x 的子树中距离 d 处插入权值 v
    int query(int x, int d) //查询 x 的子树中距离 <= d 的权值和
}

struct TREE {
    struct Edge {
        int v, w, nxt;
    } e[MAXN << 1];
    int first[MAXN], eCnt;
 
    inline void AddEdge(int u, int v, int w) {
        e[++eCnt].v = v;
        e[eCnt].w = w;
        e[eCnt].nxt = first[u];
        first[u] = eCnt;
    }
 
    int vis[MAXN], siz[MAXN], son[MAXN], tsiz, root;
    int Fa[MAXN], dep[MAXN];
    vector<int> dis[MAXN]; //每个点与每一层的祖先的距离
 
    void GetRoot(int u, int fa) {
        siz[u] = 1;
        son[u] = 0;
        for (int i = first[u]; i; i = e[i].nxt) {
            int v = e[i].v;
            if (v == fa || vis[v]) continue;
            GetRoot(v, u);
            son[u] = max(son[u], siz[v]);
            siz[u] += siz[v];
        }
        son[u] = max(son[u], tsiz - siz[u]);
        if (son[u] < son[root]) {
            root = u;
        }
    }
 
    void GetDis(int u, int fa, int d) {
        siz[u] = 1;
        dis[u].push_back(d);
        for (int i = first[u]; i; i = e[i].nxt) {
            int v = e[i].v;
            if (v == fa || vis[v]) continue;
            GetDis(v, u, d + e[i].w);
            siz[u] += siz[v];
        }
    }
 
    void PointDC(int u) {
        vis[u] = 1;
        GetDis(u, 0, 0);
        for (int i = first[u]; i; i = e[i].nxt) {
            int v = e[i].v;
            if (vis[v]) continue;
            tsiz = siz[v];
            root = 0;
            GetRoot(v, 0);
            Fa[root] = u;
            dep[root] = dep[u] + 1;
            PointDC(root);
        }
    }
    
    void Build() {
        for (int i = 1, u, v, w; i < n; ++i) {
            read(u); read(v); read(w);
            AddEdge(u, v, w);
            AddEdge(v, u, w);
        }
        son[root = 0] = n + 1;
        tsiz = n;
        GetRoot(1, 0);
        PointDC(root);
    }
    
    void Update(int u) {
        DS1.update(u, 0, v);
        for (int x = u; Fa[x]; x = Fa[x]) {
           	int d = dis[u][dep[Fa[x]]];
            DS1.update(Fa[x], d, v);
            DS2.update(x, d, v);
        }
    }
    
    int Query(int u, int k) {
        int ret = DS1.query(u, k);
        for (int x = u; Fa[x]; x = Fa[x]) {
            int d = dis[u][dep[Fa[x]]];
            if (d > k) continue;
            ret += DS1.query(Fa[x], k - d);
            ret -= DS2.query(x, k - d);
        }
        return ret;
    }
} T2;
```



#### 边分治、边分树

记得 MAXN 开两倍

```
struct Edge {
    int v, w, nxt;
} e[MAXN << 3];
int t1[MAXN], t2[MAXN], t3[MAXN], eCnt;

inline void AddEdge(int *t, int u, int v, int w) {
    e[++eCnt].v = v;
    e[eCnt].w = w;
    e[eCnt].nxt = t[u];
    t[u] = eCnt;
    e[++eCnt].v = u;
    e[eCnt].w = w;
    e[eCnt].nxt = t[v];
    t[v] = eCnt;
}

struct EdgeDivide {
    void Rebuild(int u, int fa) {
        int lst = 0;
        for (int i = t1[u]; i; i = e[i].nxt) {
            int v = e[i].v;
            if (v == fa) continue;
            Rebuild(v, u);
            if (!lst) {
                AddEdge(t3, u, v, 1);
                lst = u;
            } else {
                AddEdge(t3, lst, ++n2, 0);
                AddEdge(t3, n2, v, 1);
                lst = n2;
            }
        }
    }

    int vis[MAXN << 2], siz[MAXN], mx, root;
    int dir[MAXN][21], mxd[MAXN], dis[MAXN][21];

    void GetRoot(int u, int fa, int tsiz) {
        siz[u] = 1;
        for (int i = t3[u]; i; i = e[i].nxt) {
            int v = e[i].v;
            if (v == fa || vis[i >> 1]) continue;
            GetRoot(v, u, tsiz);
            siz[u] += siz[v];
            int cur = max(siz[v], tsiz - siz[v]);
            if (cur < mx) {
                mx = cur; root = i;
            }
        }
    }

    void GetDis(int u, int fa, int d, int dep, int t) {
        if (u <= n) {
            dir[u][dep] = t;
            dis[u][dep] = d + t * e[root].w;
        }
        for (int i = t3[u]; i; i = e[i].nxt) {
            int v = e[i].v;
            if (v == fa || vis[i >> 1]) continue;
            GetDis(v, u, d + e[i].w, dep, t);
        }
    }

    void EdgeDC(int u, int tsiz, int dep) {
        if (tsiz == 1) {
            if (u <= n) mxd[u] = dep;
            return;
        }
        mx = n2 + 1;
        GetRoot(u, 0, tsiz);
        vis[root >> 1] = 1;
        int ru = e[root].v, rv = e[root ^ 1].v, tmp = siz[ru];
        GetDis(ru, 0, 0, dep, 0);
        GetDis(rv, 0, 0, dep, 1);
        EdgeDC(ru, tmp, dep + 1);
        EdgeDC(rv, tsiz - tmp, dep + 1);
    }

    int tot, Root[MAXN], lc[MAXM], rc[MAXM];
    int sum[MAXM].....

    void Insert(int &rt, int u, int dep, /*别的参数*/) {
        rt = ++tot;
        //更新维护的信息
        if (dep == mxd[u]) return;
        if (dir[u][dep] == 0) {
            Insert(lc[rt], u, dep + 1, );
        } else {
            Insert(rc[rt], u, dep + 1, );
        }
    }

    int Merge(int x, int y, /*别的参数*/) {
        if (!x || !y) return x + y;
        //统计答案
        //把 y 的信息加给 x
        lc[x] = Merge(lc[x], lc[y], );
        rc[x] = Merge(rc[x], rc[y], );
        return x;
    }

    void DFS(int u, int fa) {
        Insert(Root[u], u, 0);
        for (int i = t2[u]; i; i = e[i].nxt) {
            int v = e[i].v;
            if (v == fa) continue;
            DFS(v, u);
            Root[u] = Merge(Root[u], Root[v]);
        }
    }

    void Calc() {
        n2 = n;
        Rebuild(1, 0);
        EdgeDC(1, n2, 0);
        DFS(1, 0, 0);
    }
};
```





## 点双连通分量

```c++
	// 所有都清一遍！！！ 尤其stk
	int vis[N], clk, dfn[N], low[N], dlc;
	int stk[N], top;
	
	void divide(int x, int f) {
		dfn[x] = low[x] = ++ dlc;
		stk[++ top] = x;
		
		int py = 0;
		
		for(int i = 0; i < (int) G[x].size(); ++ i) {
			int V = G[x][i];
			if(id[x][i] == f) continue;
			if(!dfn[V]) {
                ++ py;
				divide(V, id[x][i]);
				low[x] = min(low[x], low[V]);
				
				if(low[V] >= dfn[x]) {  // x is a cut point
					vector <int> pbc;
					for(; stk[top + 1] != V; pbc.push_back(stk[top]), -- top);
					pbc.push_back(x);
				}
				
			} else if(dfn[V] < dfn[x]) {
				low[x] = min(low[x], dfn[V]);
			}
		}
        if(py > 1 && x is root) x is a cut point. Don't  forget to push vertex
	}	
```

## 边双连通分量

```c++
	// 所有都清一遍！！！ 尤其stk
	int dfn[N], low[N], dlc, S[N];
	
	int stk[N], top;
	
	void dfs(int x, int f) {
		dfn[x] = low[x] = ++ dlc;
		stk[++ top] = x;
		
		S[x] = 1;
		
		for(int i = 0; i < (int) G[x].size(); ++ i) {
			int V = G[x][i];
			if(id[x][i] == f) continue;
			if(!dfn[V]) {
				dfs(V, id[x][i]);
				S[x] += S[V];
				low[x] = min(low[x], low[V]);
			} else if(dfn[V] < dfn[x]) {
				low[x] = min(low[x], dfn[V]);
			}
		}
		
		if(dfn[x] == low[x]) {
			
			vector <int> po; po.clear();
			
			while(stk[top + 1] != x) {
				po.push_back(stk[top --]);		
			}
		}
	}
```



## 强连通分量



```c++
void tarjan(int x) {
	dfn[x] = low[x] = ++ dlc;
	stk[++ top] = x;
	Foreachedge(i, x) {
		int V = to[i];
		if(!dfn[V]) {
			tarjan(V);
			low[x] = min(low[x], low[V]);
		} else if(ins[V])
		low[x] = min(low[x], dfn[V]);
		
		if(dfn[x] == low[x]) {
			++ col;
			for(; stk[top + 1] != x; -- top) {
				ins[stk[top]] = 0;
				who[col].push_back(stk[top]);
			}
		} // strongly-connected
	}
}
```



## Hall 定理

```c++
此定理使用于组合问题中，二部图G中的两部分顶点组成的集合分别为X, Y, X={X1, X2, X3,X4,.........,Xm}, Y={y1, y2, y3, y4 ,.........,yn},G中有一组无公共点的边，一端恰好为组成X的点的充分必要条件是：
X中的任意k个点至少与Y中的k个点相邻。（1≤k≤m)
本论还有一个重要推论：
二部图G中的两部分顶点组成的集合分别为X,Y, 若∣X∣=∣Y∣,且G中有一组无公共端点的边，一端恰好组成X中的点，一端恰好组成Y中的点，则称二部图G中存在完美匹配。若图G的每个点度数为t，则称二部图G为t---正则的二部图存在完美匹配.
```

# 计算几何

```cpp
#include<bits/stdc++.h>
using namespace std;
#define Vector Point
const double eps = 1e-10, pi = acos(-1.0);
double Sqr(const double &x) {
	return x * x;
}
int Dcmp(const double &x) { // warning: nan will return -1 and inf will return 1
	return x < -eps ? -1 : x > eps ? 1 : 0;
}
struct Point {
	double x, y;
	Point(double x = 0, double y = 0) : x(x), y(y) {}
	int operator == (const Point &rhs) const {
		return Dcmp(x - rhs.x) == 0 && Dcmp(y - rhs.y) == 0;
	}
	int operator < (const Point &rhs) const {
		return Dcmp(x - rhs.x) == 0 ? Dcmp(y - rhs.y) < 0 : Dcmp(x - rhs.x) < 0;
	}
	Point operator + (const Point &rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator - (const Point &rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
	Point operator * (const double &k) const {
		return Point(x * k, y * k);
	}
	Point operator / (const double &k) const {
		return Point(x / k, y / k);
	}
	double operator ^ (const Point &rhs) const {
		return x * rhs.y - y * rhs.x;
	}
	double operator * (const Point &rhs) const {
		return x * rhs.x + y * rhs.y;
	}
	double len2() {
		return x * x + y * y;
	}
	double len() {
		return sqrt(len2());
	}
	Point Rotate90() {
		return Point(-y, x);
	}
	Point Rotate(const double &rad) { // counterclockwise
		return Point(x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad));
	}
	Point Unit() {
		double l = len();
		return (*this) / l;
	}
};
double Area(const Point &a, const Point &b, const Point &c) {
	return fabs((b - a) ^ (c - a));
}
int Left(const Point &a, const Point &b, const Point &c) {
	return Dcmp((b - a) ^ (c - a)) >= 0;
}
Point base;
vector<Point> Convex(vector<Point>a) {
	int n = a.size(), cnt = 0;
	if(n < 3)
		return a;
	base = a[0];
	for(auto p : a)
		if(make_pair(p.x, p.y) < make_pair(base.x, base.y))
			base = p;
	sort(a.begin(), a.end(), [](const Point &a, const Point &b) {
		int d = Dcmp((a - base) ^ (b - base));
		if(d)
			return d > 0;
		return (a - base).len() < (b - base).len();
	});
	vector<Point>res;
	for(int i = 0; i < n; ++ i) {
		while(cnt > 1 && Left(res[cnt - 2], a[i], res[cnt - 1]))
			-- cnt, res.pop_back();
		res.push_back(a[i]), ++ cnt;
	}
	int fixed = cnt;
	for(int i = n - 2; ~i; -- i) {
		while(cnt > fixed && Left(res[cnt - 2], a[i], res[cnt - 1]))
			-- cnt, res.pop_back();
		res.push_back(a[i]), ++ cnt;
	}
	res.pop_back();
	return res;
}
vector<Point> MinkSum(vector<Point> a, vector<Point> b) {
	if(!a.size())
		return b;
	if(!b.size())
		return a;
	vector<Point>p[2] = {a, b};
	for(int i = 0; i < 2; ++ i)
		p[i].push_back(p[i][0]);
	int now[2] = {0, 0}, len[2] = {p[0].size() - 1, p[1].size() - 1};
	vector<Point>res;
	res.push_back(p[0][0] + p[1][0]);
	do {
		int d = Dcmp((p[1][now[1] + 1] - p[1][now[1]]) ^ (p[0][now[0] + 1] - p[0][now[0]])) >= 0;
		res.push_back(p[d][now[d] + 1] - p[d][now[d]] + res.back());
		now[d] = (now[d] + 1) % len[d];
	}while(now[0] || now[1]);
	return res;
}
int Lower(Point x, const vector<Point> &v, int l, int r, int sg) {
	if(l > r)
		l = r;
	while(l != r) {
		int mid = (l + r) >> 1;
		if(Dcmp((v[mid] - x) ^ (v[mid + 1] - v[mid])) == sg)
			l = mid + 1;
		else
			r = mid;
	}
	return l;
}
// 点到凸包切线，v[0]为顺时针上凸壳，v[1]为顺时针下凸壳，均允许起始两个点横坐标相同
// 顺时针旋转为d1先碰到凸包
int GetPolyTangent(Point x, const vector<vector<Point> > &v, int &d1, int &d2) { // return 1: Point is out of Polygon
	if(Dcmp(x.x - v[0][0].x) < 0) { // Tangent Point is v[0][d1] and v[1][d2]
		d1 = Lower(x, v[0], 0, (int)v[0].size() - 1, 1);
		d2 = Lower(x, v[1], 0, (int)v[1].size() - 1, - 1);
		return 1;
	}
	else if(Dcmp(x.x - v[0].back().x) > 0) { // Tangent Point is v[1][d1] and v[0][d2]
		d1 = Lower(x, v[1], 0, (int)v[1].size() - 1, 1);
		d2 = Lower(x, v[0], 0, (int)v[0].size() - 1, -1);
		return 1;
	}
	else {
		for(int d = 0; d < 2; ++ d) {
			int id = lower_bound(v[d].begin(), v[d].end(), x, [&](const Point &a, const Point &b) {
				return d == 0 ? a < b : b < a;
			}) - v[d].begin();
			if(id && (id == (int)v[d].size() || Dcmp((v[d][id - 1] - x) ^ (v[d][id] - x)) > 0)) { // Tangent Point is v[d][d1] and v[d][d2]
				d1 = Lower(x, v[d], id, (int)v[d].size() - 1, 1);
				d2 = Lower(x, v[d], 0, id, -1);
			}
		}
	}
}
int PointInside(Point p, vector<Point> poly) { // O(logn), poly is convex
	int n = poly.size();
	if(((poly[0] - poly[n - 1]) ^ (p - poly[n - 1])) < 0 || ((poly[1] - poly[0]) ^ (p - poly[0])) < 0)
		return 0;
	int l = 1, r = n - 2, ans = -1;
	while(l <= r) {
		int mid = (l + r) >> 1;
		if(((poly[mid] - poly[0]) ^ (p - poly[0])) >= 0)
			l = mid + 1, ans = mid;
		else
			r = mid - 1;
	}
	return ((poly[ans + 1] - poly[ans]) ^ (p - poly[ans])) >= 0;
}
Point InCenter(Point a, Point b, Point c) {
	double dab = (a - b).len(), dbc = (b - c).len(), dca = (c - a).len(), p = dab + dbc + dca;
	return (a * dbc + b * dca + c * dab) / p;
}
Point CircumCenter(Point a, Point b, Point c) {
	Point p = b - a, q = c - a, s = Point(p * p / 2, q * q / 2);
	double d = p ^ q;
	return (a + Point((s ^ Point(p.y, q.y)), (Point(p.x, q.x) ^ s))) / d;
}
Point OrthoCenter(Point a, Point b, Point c) { // Vertical-Center
	return (a + b + c - CircumCenter(a, b, c) * 2);
}

struct Seg {
	Point s, t;
	Seg(){}
	Seg(Point s, Point t) : s(s), t(t) {}
};
struct Line {
	Point s, t;
	Line(){}
	Line(Point s, Point t) : s(s), t(t) {}
};
Line Trans(Line l, double d) {
	Vector v = l.t - l.s;
	v = v.Unit().Rotate90();
	return Line(l.s + v * d, l.t + v * d);
}
int Onseg(Point p, Seg seg) {
	if(Dcmp((p - seg.s) ^ (seg.t - seg.s)) == 0 && Dcmp((p - seg.s) * (p - seg.t)) <= 0)
		return 1;
	return 0;
}
int PointInPoly(Point p, vector<Point> poly) { // O(n)
	int n = poly.size(), flg = 0;
	for(int i = 0; i < n; ++ i) {
		if(Onseg(p, Seg(poly[i], poly[(i + 1) % n])))
			return -1;
		int k = Dcmp((poly[(i + 1) % n] - p) ^ (poly[i] - p)), d1 = Dcmp(poly[i].y - p.y), d2 = Dcmp(poly[(i + 1) % n].y - p.y);
		if(k > 0 && d1 <= 0 && d2 > 0)
			return 1;
		if(k < 0 && d2 <= 0 && d1 > 0)
			return 1;
	}
	return 0;
}
int SegInterSeg(Seg seg1, Seg seg2) {
	if(Onseg(seg1.s, seg2) || Onseg(seg1.t, seg2) || Onseg(seg2.s, seg1) || Onseg(seg2.t, seg1))
		return 1;
	if(Dcmp(((seg1.t - seg1.s) ^ (seg2.s - seg1.s)) * ((seg1.t - seg1.s) ^ (seg2.t - seg1.s))) < 0 && Dcmp(((seg2.t - seg2.s) ^ (seg1.s - seg2.s)) * ((seg2.t - seg2.s) ^ (seg1.t - seg2.s))) < 0)
		return 1;
	return 0;
}
int LineInterSeg(Line l1, Seg s1) {
	return Dcmp(((l1.t - l1.s) ^ (s1.t - l1.s)) * ((l1.t - l1.s) ^ (s1.s - l1.s))) < 0;
}
int PointOnLine(Line l1, Point p) {
	return Dcmp((l1.t - l1.s) ^ (p - l1.s)) == 0;
}
// 0: same 1: par 2: inter
int LineRelation(Line l1, Line l2) {
	if(Dcmp((l1.t - l1.s) ^ (l2.t - l2.s)) == 0)
		if(Dcmp((l1.t - l1.s) ^ (l2.s - l1.s)) == 0)
			return 0;
		else
			return 1;
	else
		return 2;
}
Point LineInter(Line l1, Line l2) {
	double a1 = (l2.t - l2.s) ^ (l1.s - l2.s), a2 = (l2.t - l2.s) ^ (l1.t - l2.s);
	return (l1.s * a2 - l1.t * a1) / (a2 - a1);
}
double DisToLine(Line l, Point p) {
	Vector v1 = l.t - l.s, v2 = p - l.s;
	return fabs(v1 ^ v2) / v1.len();
}
double DisToSeg(Seg s, Point p) {
	if(s.t == s.s)
		return (p - s.s).len();
	Vector v1 = s.t - s.s, v2 = p - s.s, v3 = p - s.t;
	if(Dcmp(v1 * v2) < 0)
		return v2.len();
	if(Dcmp(v1 * v3) > 0)
		return v3.len();
	return fabs(v1 ^ v2) / v1.len();
}
Point LineVer(Line l, Point p) {
	Vector v = l.t - l.s;
	v = v.Rotate90();
	return LineInter(l, Line(p, p + v));
}
Point FermatPoint(Point a, Point b, Point c) { // minimize |A-X|+|B-X|+|C-X|
	if(a == b)
		return a;
	if(b == c)
		return b;
	if(c == a)
		return c;
	double ab = (b - a).len(), bc = (c - b).len(), ca = (a - c).len();
	double cosa = (b - a) * (c - a) / ab / ca;
	double cosb = (a - b) * (c - b) / ab / bc;
	double cosc = (b - c) * (a - c) / ca / bc;
	double sq3 = pi / 3.0;
	Point mid;
	if(Dcmp(cosa + 0.5) < 0)
		mid = a;
	else if(Dcmp(cosb + 0.5) < 0)
		mid = b;
	else if(Dcmp(cosc + 0.5) < 0)
		mid = c;
	else if(Dcmp((b - a) ^ (c - a)) < 0)
		mid = LineInter(Line(b, a + (c - a).Rotate(-sq3)), Line(c, a + (b - a).Rotate(sq3)));
	else
		swap(b, c), mid = LineInter(Line(b, a + (c - a).Rotate(-sq3)), Line(c, a + (b - a).Rotate(sq3)));
	return mid;
}
bool OnLeft(Line l, Point p) {
	return Left(l.s, l.t, p);
}
vector<Point> HalfPlaneInterSect(vector<Line> h) {
	typedef pair<double, Line> polar;
	vector<polar> g; // warning: using atan2
	for(auto l : h) {
		Point v = l.t - l.s;
		g.push_back(make_pair(atan2(v.y, v.x), l));
	}
	sort(g.begin(), g.end(), [] (polar a, polar b) {
		if(Dcmp(a.first - b.first) == 0)
			return Dcmp((a.second.t - a.second.s) ^ (b.second.t - a.second.s)) < 0;
		return Dcmp(a.first - b.first) < 0;
	});
	h.resize(unique(g.begin(), g.end(), [] (polar a, polar b) {
		return Dcmp(a.first - b.first) == 0;
	}) - g.begin());
	for(int i = 0; i < (int)h.size(); ++ i)
		h[i] = g[i].second;
	int fore = 0, rear = -1;
	vector<Line>ret;
	ret.clear();
	for(int i = 0; i < (int)h.size(); ++ i) {
		while(fore < rear && !OnLeft(h[i], LineInter(ret[rear - 1], ret[rear])))
			-- rear, ret.pop_back();
		while(fore < rear && !OnLeft(h[i], LineInter(ret[fore], ret[fore + 1])))
			++ fore;
		++ rear, ret.push_back(h[i]);
	}
	while(rear - fore > 1 && !OnLeft(ret[fore], LineInter(ret[rear - 1], ret[rear])))
		-- rear, ret.pop_back();
	while(rear - fore > 1 && !OnLeft(ret[rear], LineInter(ret[fore], ret[fore + 1])))
		++ fore;
	if(rear - fore < 2)
		return vector<Point>();
	vector<Point>ans;
	ans.resize(rear - fore + 1);
	for(int i = 0; i < (int)ans.size(); ++ i)
		ans[i] = LineInter(ret[fore + i], ret[fore + (i + 1) % ans.size()]);
	return ans;
}

struct Circle {
	Point c;
	double r;
	Circle(){}
	Circle(Point c, double r) : c(c), r(r) {}
	bool operator == (const Circle &rhs) const {
		return c == rhs.c && Dcmp(r - rhs.r) == 0;
	}
};
bool Inner(Circle a, Circle b) {
	return Dcmp((a.c - b.c).len() - abs(a.r - b.r)) < 0 && Dcmp(a.r - b.r) <= 0;
}
bool PointInCircle(Circle c, Point p) {
	return Dcmp(c.r - (p - c.c).len()) >= 0;
}
Circle MakeCircle(Point a, Point b) {
	Point c = (a + b) / 2;
	double r = (a - c).len();
	return Circle(c, r);
}
Circle MakeCircle(Point a, Point b, Point c) {
	Point p = b - a, q = c - a, s = Point(p * p / 2, q * q / 2);
	double d = p ^ q;
	p = Point(s ^ Point(p.y, q.y), Point(p.x, q.x) ^ s) / d;
	return Circle(a + p, p.len());
}
Circle MakeInCircle(Point a, Point b, Point c) {
	double ab = (a - b).len(), bc = (b - c).len(), ca = (c - a).len();
	Point p = (c * ab + a * bc + b * ca) / (ab + bc + ca);
	return Circle(p, DisToLine(Line(a, b), p));
}
vector<Point> LineInterCircle(Line a, Circle b) {
	if(Dcmp(DisToLine(a, b.c) - b.r) > 0)
		return vector<Point>();
	if(Dcmp(DisToLine(a, b.c) - b.r) == 0)
		return {LineVer(a, b.c)};
	double x = sqrt(Sqr(b.r) - Sqr(DisToLine(a, b.c)));
	Point o = LineVer(a, b.c);
	return {o + (a.t - a.s).Unit() * x, o - (a.t - a.s).Unit() * x};
}
double CircleInterArea(Circle a, Circle b) {
	double d = (a.c - b.c).len();
	if(Dcmp(d - (a.r + b.r)) >= 0)
		return 0;
	if(Dcmp(d - fabs(a.r - b.r)) <= 0) {
		double r = min(a.r, b.r);
		return r * r * pi;
	}
	double x = (d * d + a.r * a.r - b.r * b.r) / (2 * d), t1 = acos(min(1., max(-1., x / a.r))), t2 = acos(min(1., max(-1., (d - x) / b.r)));
	return Sqr(a.r) * t1 + Sqr(b.r) * t2 - d * a.r * sin(t1);
}
vector<Point> CircleInterCircle(Circle a, Circle b) {
	if(a.c == b.c || Dcmp((a.c - b.c).len() - a.r - b.r) > 0 || Dcmp((a.c - b.c).len() - abs(a.r - b.r)) < 0)
		return vector<Point>();
	Point r = (b.c - a.c).Unit();
	double d = (a.c - b.c).len();
	double x = ((Sqr(a.r) - Sqr(b.r)) / d + d) / 2;
	double h = sqrt(Sqr(a.r) - Sqr(x));
	if(Dcmp(h) == 0)
		return {a.c + r * x};
	return {a.c + r * x + r.Rotate90() * h, a.c + r * x - r.Rotate90() * h};
}
vector<Point> Tangent(Point p, Circle b) {
	return CircleInterCircle(MakeCircle(p, b.c), b);
}
vector<Line> ExTangent(Circle a, Circle b) {
	vector<Line> res;
	res.clear();
	if(Dcmp((a.c - b.c).len() - abs(a.r - b.r)) <= 0)
		return res;
	if(Dcmp(a.r - b.r) == 0) {
		Vector dir = b.c - a.c;
		dir = dir.Unit().Rotate90() * a.r;
		res.push_back(Line(a.c + dir, b.c + dir));
		res.push_back(Line(a.c - dir, b.c - dir));
	}
	else {
		Point p = (b.c * a.r - a.c * b.r) / (a.r - b.r);
		vector<Point> pp = Tangent(p, a), qq = Tangent(p, b);
		if(pp.size() == 2u && qq.size() == 2u) {
			if(Dcmp(a.r - b.r) < 0)
				swap(pp[0], pp[1]), swap(qq[0], qq[1]);
			res.push_back(Line(pp[0], qq[0]));
			res.push_back(Line(pp[1], qq[1]));
		}
	}
	return res;
}
vector<Line> InTangent(Circle a, Circle b) {
	vector<Line> res;
	res.clear();
	Point p = (b.c * a.r + a.c * b.r) / (a.r + b.r);
	vector<Point> pp = Tangent(p, a), qq = Tangent(p, b);
	if(pp.size() == 2u && qq.size() == 2u) {
		res.push_back(Line(pp[0], qq[0]));
		res.push_back(Line(pp[1], qq[1]));
	}
	return res;
}
Circle MinCircle(vector<Point> p) { // don't forget srand()
	Circle ret;
	random_shuffle(p.begin(), p.end());
	for(int i = 0; i < (int)p.size(); ++ i)
		if(!PointInCircle(ret, p[i])) {
			ret = Circle(p[i], 0);
			for(int j = 0; j < i; ++ j)
				if(!PointInCircle(ret, p[j])) {
					ret = MakeCircle(p[i], p[j]);
					for(int k = 0; k < j; ++ k)
						if(!PointInCircle(ret, p[k]))
							ret = MakeCircle(p[i], p[j], p[k]);
				}
		}
	return ret;
}
namespace CircleArea {
	const int Maxn = 1e3 + 5;
	Circle c[Maxn];
	double ans[Maxn]; // ans[i] : The area that is Covered by Circles for i times
	double pre[Maxn];
	int n;
	void Add(Circle now) {
		c[n ++] = now;
	}
	void Init_Or() { // 求圆面积并需要InitOr，但求被覆盖k次面积不能用
		bool mark[Maxn] = {0};
		for(int i = 0; i < n; ++ i)
			for(int j = 0; j < n; ++ j)
				if(i != j && !mark[j])
					if(c[i] == c[j] || Inner(c[i], c[j])) {
						mark[i] = 1;
						break;
					}
		int k = 0;
		for(int i = 0; i < n; ++ i)
			if(!mark[i])
				c[k ++] = c[i];
		n = k;
	}
	void Init_Add() {
		bool mark[Maxn] = {0};
		for(int i = 0; i < n; ++ i)
			for(int j = 0; j < n; ++ j)
				if(i != j && !mark[j])
					if(c[i] == c[j] || Inner(c[j], c[i])) {
						mark[i] = 1;
						break;
					}
		int k = 0;
		for(int i = 0; i < n; ++ i)
			if(!mark[i])
				c[k ++] = c[i];
		n = k;
	}
	double Arcarea(double th, double r) {
		return r * r * (th - sin(th)) * 0.5;
	}
	void Getarea() {
		memset(ans, 0, sizeof ans);
		vector<pair<double, int> > v;
		for(int i = 0; i < n; ++ i) {
			v.clear();
			v.push_back(make_pair(-pi, 1));
			v.push_back(make_pair(pi, -1));
			for(int j = 0; j < n; ++ j) {
				if(i != j) {
					Point q = (c[j].c - c[i].c);
					double ab = q.len(), ac = c[i].r, bc = c[j].r;
					if(Dcmp(ab + ac - bc) <= 0) {
						v.push_back(make_pair(-pi, 1));
						v.push_back(make_pair(pi, -1));
						continue;
					}
					if(Dcmp(ab + bc - ac) <= 0)
						continue;
					if(Dcmp(ab - ac - bc) > 0)
						continue;
					double th = atan2(q.y, q.x), fai = acos((ac * ac + ab * ab - bc * bc) / (2 * ac * ab)); // Warning: use atan2()
					double a0 = th - fai;
					if(Dcmp(a0 + pi) < 0)
						a0 += 2 * pi;
					double a1 = th + fai;
					if(Dcmp(a1 - pi) > 0)
						a1 -= 2 * pi;
					if(Dcmp(a0 - a1) > 0) {
						v.push_back(make_pair(a0, 1));
						v.push_back(make_pair(pi, -1));
						v.push_back(make_pair(-pi, 1));
						v.push_back(make_pair(a1, -1));
					}
					else {
						v.push_back(make_pair(a0, 1));
						v.push_back(make_pair(a1, -1));
					}
				}
			}
			sort(v.begin(), v.end());
			int cur = 0;
			for(int j = 0; j < (int)v.size(); ++ j) {
				if(cur && Dcmp(v[j].first - pre[cur])) {
					ans[cur] += Arcarea(v[j].first - pre[cur], c[i].r);
					ans[cur] += 0.5 * (Point(c[i].c.x + c[i].r * cos(pre[cur]), c[i].c.y + c[i].r * sin(pre[cur])) ^ Point(c[i].c.x + c[i].r * cos(v[j].first), c[i].c.y + c[i].r * sin(v[j].first)));
				}
				cur += v[j].second;
				pre[cur] = v[j].first;
			}
		}
		for(int i = 1; i < n; ++ i) // 加上后ans[i]为覆盖恰好i次的面积
			ans[i] -= ans[i + 1];
	}
	inline void Solve() {
		scanf("%d", &n);
		for(int i = 0; i < n; ++ i)
			scanf("%lf%lf%lf", &c[i].c.x, &c[i].c.y, &c[i].r);
		Init_Or();
		Getarea();
		printf("%.5f\n", ans[n]);
	}
}

namespace Planar { // Given the Point set p and the Edge from function Add(u, v)
	struct Edge { // sort in CCW.
		int u, v, id;
		double a;
		Edge(){}
		Edge(int u, int v, int id) : u(u), v(v), id(id) {
			// atan2 warning.
			a = atan2(p[v].y - p[u].y, p[v].x - p[u].x); // Lines' Angle, p[u] -> p[v];
		}
		inline bool operator == (const Edge &rhs) const {
			return Dcmp(a - rhs.a) == 0;
		}
		inline bool operator < (const Edge &rhs) const {
			return Dcmp(a - rhs.a) < 0;
		}
	};
	vector<pair<int, int> > all; // all_point_edges;
	vector<Edge>edges[maxn]; // edges between points, edges[i]: out edges from i;
	vector<int>vis, col;
	int n; // number of regions.
	double area[maxn]; // the area of regions.
	vector<int>G[maxn]; // edges between regions.
	inline void Add(int x, int y) {
		all.push_back(make_pair(x, y));
		edges[x].push_back(Edge(x, y, (int)all.size() - 1));
		all.push_back(make_pair(y, x));
		edges[y].push_back(Edge(y, x, (int)all.size() - 1));
	}
	inline int Next(int u, double a) { // Find Next Edge out from u, and in the CW of angle -a
		a += pi;
		while(Dcmp(a + pi) <= 0)
			a += 2 * pi;
		while(Dcmp(a - pi) > 0)
			a -= 2 * pi;
		int m = edges[u].size(), l = 0, r = m - 1, ans = -1;
		while(l <= r) {
			int mid = (l + r) >> 1;
			if(Dcmp(edges[u][mid].a - a) < 0)
				ans = mid, l = mid + 1;
			else
				r = mid - 1;
		}
		return ans == -1 ? m - 1 : ans;
	}
	int rt, clk[maxn];
	double ans = 0;
	inline void Dfs(int u, int c) { // black and white color, the neighbour region have different colors, calculate the area.
		clk[u] = 1;
		if(c)
			assert(Dcmp(area[u]) >= 0), ans += area[u] / 2;
		for(auto v : G[u])
			if(!clk[v])
				Dfs(v, c ^ 1);
	}
	inline void Build() {
		n = 0;
		vis.resize(all.size());
		col.resize(all.size());
		int m = p.size();
		for(int i = 0; i < m; ++ i)
			sort(edges[i].begin(), edges[i].end());
		for(int i = 0; i < m; ++ i) {
			for(int j = 0; j < (int)edges[i].size(); ++ j) {
				int id = edges[i][j].id;
				if(vis[id])
					continue;
				Point st = p[edges[i][j].u];
				++ n, col[id] = n, vis[id] = 1;
				int fr = i, pos = j;
				while(1) {
					int to = edges[fr][pos].v;
					int w = Next(to, edges[fr][pos].a);
					assert(!vis[edges[to][w].id] && !col[edges[to][w].id]);
					vis[edges[to][w].id] = 1, col[edges[to][w].id] = n;
					area[n] += ((p[edges[to][w].u] - st) ^ (p[edges[to][w].v] - st));
					if(edges[to][w].v == edges[i][j].u)
						break;
					fr = to, pos = w;
				}
				if(Dcmp(area[n]) < 0) // external region.
					rt = n;
			}
		}
		for(int i = 0; i < (int)all.size(); ++ i)
			G[col[i]].push_back(col[i ^ 1]);
		for(int i = 1; i <= n; ++ i)
			sort(G[i].begin(), G[i].end()), G[i].erase(unique(G[i].begin(), G[i].end()), G[i].end());
	}
}


struct Point3 {
	double x, y, z;
	Point3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
	bool operator == (const Point3 &rhs) const {
		return Dcmp(x - rhs.x) == 0 && Dcmp(y - rhs.y) == 0 && Dcmp(z - rhs.z) == 0;
	}
	Point3 operator + (const Point3 &rhs) const {
		return Point3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	Point3 operator - (const Point3 &rhs) const {
		return Point3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	Point3 operator * (const double &k) const {
		return Point3(x * k, y * k, z * k);
	}
	Point3 operator / (const double &k) const {
		return Point3(x / k, y / k, z / k);
	}
	double operator * (const Point3 &rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	Point3 operator ^ (const Point3 &rhs) const {
		return Point3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
	}
	double len2() {
		return x * x + y * y + z * z;
	}
	double len() {
		return sqrt(len2());
	}
	Point3 Unit() {
		double l = len();
		return (*this) / l;
	}
};

struct Line3 {
	Point3 s, t;
	Line3(){}
	Line3(Point3 s, Point3 t) : s(s), t(t) {}
	bool operator == (const Line3 &rhs) const {
		return s == rhs.s && t == rhs.t;
	}
	double len2() {
		return (t - s).len2();
	}
	double len() {
		return (t - s).len();
	}
};
double DisToLine(Point3 p, Line3 l) {
	return ((l.t - l.s) ^ (p - l.s)).len() / l.len();
}
double DisToSeg(Point3 p, Line3 s) {
	if(Dcmp((p - s.s) * (s.t - s.s)) < 0 || Dcmp((p - s.t) * (s.s - s.t)) < 0)
		return min((p - s.s).len(), (p - s.t).len());
	return DisToLine(p, s);
}
Point3 LinePro(Point3 p, Line3 l) {
	return l.s + (((l.t - l.s) * ((l.t - l.s) * (p - l.s))) / (l.len2()));
}
Point3 Rotate(Point3 p, Line3 l, double ang) {
	if(Dcmp(((l.s - p) ^ (l.t - p)).len()) == 0)
		return p;
	Point3 f1 = (l.t - l.s) ^ (p - l.s), f2 = (l.t - l.s) ^ f1;
	double len = ((l.s - p) ^ (l.t - p)).len() / l.len();
	f1 = f1.Unit() * len, f2 = f2.Unit() * len;
	Point3 h = p + f2, pp = h + f1;
	return h + ((p - h) * cos(ang)) + ((pp - h) * sin(ang));
}
bool PointOnSeg(Point3 p, Line3 l) {
	return Dcmp(((l.s - p) ^ (l.t - p)).len()) == 0 && Dcmp((l.s - p) * (l.t - p)) <= 0;
}
double Area(Point3 a, Point3 b, Point3 c) {
	return ((b - a) ^ (c - a)).len();
}
double Volume(Point3 a, Point3 b, Point3 c, Point3 d) {
	return ((b - a) ^ (c - a)) * (d - a);
}
bool InTri(Point3 a, Point3 b, Point3 c, Point3 p) {
	return Dcmp(Area(a, b, c) - Area(a, b, p) - Area(a, c, p) - Area(b, c, p)) == 0;
}
double SegToSeg(Line3 x, Line3 y) {
	double a = (x.t - x.s) * (x.t - x.s);
	double b = (x.t - x.s) * (y.t - y.s);
	double e = (y.t - y.s) * (y.t - y.s);
	double d = a * e - b * b;
	Point3 r = x.s - y.s;
	double c = (x.t - x.s) * r, f = (y.t - y.s) * r;
	if(Dcmp(d) == 0)
		return min(min(DisToSeg(x.s, y), DisToSeg(x.t, y)), min(DisToSeg(y.s, x), DisToSeg(y.t, x)));
	double s = (b * f - c * e) / d, t = (a * f - c * b) / d;
	Point3 g = x.s + (x.t - x.s) * s, h = y.s + (y.t - y.s) * t;
	if(PointOnSeg(g, x) && PointOnSeg(h, y))
		return (g - h).len();
	else
		return min(min(DisToSeg(x.s, y), DisToSeg(x.t, y)), min(DisToSeg(y.s, x), DisToSeg(y.t, x)));
}

struct Plane {
	Point3 a, b, c, o;
	Plane(){}
	Plane(Point3 a, Point3 b, Point3 c) : a(a), b(b), c(c) {
		o = pvec();
	}
	Point3 pvec() {
		return (b - a) ^ (c - a);
	}
	// ax + by + cz + d = 0
	Plane(double _a, double _b, double _c, double _d) {
		o = Point3(_a, _b, _c);
		if(Dcmp(_a) != 0)
			a = Point3((-_d - _c - _b) / _a, 1, 1);
		else if(Dcmp(_b) != 0)
			a = Point3(1, (-_d - _c - _a) / _b, 1);
		else if(Dcmp(_c) != 0)
			a = Point3(1, 1, (-_d - _a - _b) / _c);
	}
};
bool PointOnPlane(Point3 p, Plane pl) {
	return Dcmp((p - pl.a) * pl.o) == 0;
}
double PlaneAngle(Plane pl1, Plane pl2) {
	return acos(pl1.o * pl2.o) / (pl1.o.len() * pl2.o.len());
}
double LineCrossPlane(Line3 l, Plane pl, Point3 &p) {
	double x = pl.o * (l.t - pl.a);
	double y = pl.o * (l.s - pl.a);
	double d = x - y;
	if(Dcmp(d) == 0)
		return 0;
	p = ((l.s * x) - l.t * y) / d;
	return 1;
}
Point3 PointToPlane(Point3 p, Plane pl) {
	Line3 l = Line3(p, p + pl.o);
	LineCrossPlane(l, pl, p);
	return p;
}
int PlaneCrossPlane(Plane pl1, Plane pl2, Line3 &l) {
	Point3 oo = pl1.o ^ pl2.o, v = pl1.o ^ oo;
	double d = fabs(pl2.o * v);
	if(Dcmp(d) == 0)
		return 0;
	Point3 q = pl1.a + (v * (pl2.o * (pl2.a - pl1.a)) / d);
	l = Line3(q, q + oo);
	return 1;
}
double DisToTri(Point3 p, Point3 a, Point3 b, Point3 c) {
	double dis = 1e18;
	dis = min(dis, DisToSeg(p, Line3(a, b)));
	dis = min(dis, DisToSeg(p, Line3(a, c)));
	dis = min(dis, DisToSeg(p, Line3(c, b)));
	Point3 proj = PointToPlane(p, Plane(a, b, c));
	if(InTri(a, b, c, proj))
		dis = min(dis, (p - proj).len());
	return dis;
}

namespace Hull3 {
	const int Maxn = 1e3 + 5; // if Maxn > 5e3 remember change g to map, Maxn = 1e5 (O2 0.7s)
	struct Face {
		int a, b, c, ok;
	};
	int n;
	Point3 P[Maxn];
	int num;
	Face F[8 * Maxn];
	int g[Maxn][Maxn];
	// map<int, int>g[Maxn];
	double Dblcmp(Point3 p, Face f) { // 点在面同向返回正
		Point3 p1 = P[f.b] - P[f.a], p2 = P[f.c] - P[f.a], p3 = p - P[f.a];
		return (p1 ^ p2) * p3;
	}
	void Dfs(int p, int now);
	void Deal(int p, int a, int b) {
		int f = g[a][b];
		Face add;
		if(F[f].ok)
			if(Dblcmp(P[p], F[f]) > eps)
				Dfs(p, f);
			else
				add.a = b, add.b = a, add.c = p, add.ok = 1, g[p][b] = g[a][p] = g[b][a] = num, F[num ++] = add;
	}
	void Dfs(int p, int now) {
		F[now].ok = false;
		Deal(p, F[now].b, F[now].a);
		Deal(p, F[now].c, F[now].b);
		Deal(p, F[now].a, F[now].c);
	}
	bool Same(int s, int t) {
		Point3 &a = P[F[s].a], &b = P[F[s].b], &c = P[F[s].c];
		return Dcmp(Volume(a, b, c, P[F[t].a])) == 0 && Dcmp(Volume(a, b, c, P[F[t].b])) == 0 && Dcmp(Volume(a, b, c, P[F[t].c])) == 0;
	}
	void Create() {
		num = 0;
		Face add;

		int flag = 1;
		for(int i = 1; i < n; ++ i) {
			if(!(P[0] == P[i])) {
				swap(P[1], P[i]);
				flag = 0;
				break;
			}
		}
		if(flag)
			return;
		flag = 1;
		for(int i = 2; i < n; ++ i) {
			if(Dcmp(((P[1] - P[0]) ^ (P[i] - P[0])).len()) == 1) {
				swap(P[2], P[i]);
				flag = 0;
				break;
			}
		}
		if(flag)
			return;
		flag = 1;
		for(int i = 3; i < n; ++ i) {
			if(Dcmp(((P[1] - P[0]) ^ (P[2] - P[0])) * (P[i] - P[0]))) {
				swap(P[3], P[i]);
				flag = 0;
				break;
			}
		}
		if(flag)
			return;
		
		for(int i = 0; i < 4; ++ i) {
			add.a = (i + 1) % 4;
			add.b = (i + 2) % 4;
			add.c = (i + 3) % 4;
			add.ok = 1;
			if(Dblcmp(P[i], add) > 0)
				swap(add.b, add.c);
			g[add.a][add.b] = g[add.b][add.c] = g[add.c][add.a] = num;
			F[num ++] = add;
		}
		for(int i = 4; i < n; ++ i)
			for(int j = 0; j < num; ++ j)
				if(F[j].ok && Dblcmp(P[i], F[j]) > eps) {
					Dfs(i, j);
					break;
				}
		int tmp = num;
		num = 0;
		for(int i = 0; i < tmp; ++ i)
			if(F[i].ok)
				F[num ++] = F[i];
	}
	double HullArea() {
		double res = 0;
		if(n == 3)
			return Area(P[0], P[1], P[2]) / 2;
		for(int i = 0; i < num; ++ i)
			res += Area(P[F[i].a], P[F[i].b], P[F[i].c]);
		return res / 2;
	}
	double HullVolume() {
		double res = 0;
		Point3 tmp = Point3(0, 0, 0);
		for(int i = 0; i < num; ++ i)
			res += Volume(tmp, P[F[i].a], P[F[i].b], P[F[i].c]);
		return fabs(res / 6);
	}
	int PolyNum() {
		int res = 0;
		for(int i = 0; i < num; ++ i) {
			int flag = 1;
			for(int j = 0; j < i; ++ j)
				if(Same(i, j)) {
					flag = 0;
					break;
				}
			res += flag;
		}
		return res;
	}
	Point3 Gcenter() {
		Point3 ans = Point3(0, 0, 0);
		Point3 o = Point3(0, 0, 0);
		double all = 0;
		for(int i = 0; i < num; ++ i) {
			double vol = Volume(o, P[F[i].a], P[F[i].b], P[F[i].c]);
			ans = ans + (((o + P[F[i].a] + P[F[i].b] + P[F[i].c]) / 4) * vol);
			all += vol;
		}
		ans = ans / all;
		return ans;
	}
	double PointToFace(Point3 p, int i) {
		double tmp1 = fabs(Volume(P[F[i].a], P[F[i].b], P[F[i].c], p));
		double tmp2 = ((P[F[i].b] - P[F[i].a]) ^ (P[F[i].c] - P[F[i].a])).len();
		return tmp1 / tmp2;
	}
}
struct Ball {
	Point3 c;
	double r;
	Ball(){}
	Ball(Point3 c, double r) : c(c), r(r) {}
};
// MinBall is same as MinCircle
```



## 最大空凸包

```c++
// given n points, find a empty convex polygon of k vertexes, <= O(n^3k).
#include<bits/stdc++.h>
using namespace std;
const int maxn = 205, maxk = 55;
const double eps = 1e-8;
inline int Dcmp(double x) {
	return x < -eps ? -1 : x > eps ? 1 : 0;
}
struct Point {
	int x, y;
	Point(int x = 0, int y = 0) : x(x), y(y) {}
	inline int operator < (const Point &rhs) const {
		return y == rhs.y ? x < rhs.x : y < rhs.y;
	}
	inline Point operator + (const Point &rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	inline Point operator - (const Point &rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
	inline Point operator * (const double &k) const {
		return Point(x * k, y * k);
	}
	inline Point operator / (const double &k) const {
		return Point(x / k, y / k);
	}
	inline long long operator * (const Point &rhs) const {
		return 1ll * x * rhs.x + 1ll * y * rhs.y;
	}
	inline long long operator ^ (const Point &rhs) const {
		return 1ll * x * rhs.y - 1ll * y * rhs.x;
	}
	inline long long len2() {
		return 1ll * x * x + 1ll * y * y;
	}
}po[maxn], p[maxn];
int n, k, cnt, idx[maxn];
double ang[maxn];
long long f[maxn][maxn][maxk], mx[maxk];
vector<int>to[maxn], fr[maxn];
inline int Cmp(int a, int b) {
	return Dcmp(ang[a] - ang[b]) < 0;
}
inline long long Solve(int id) {
	p[0] = Point(0, 0), cnt = 1;
	for(int i = 0; i < n; ++ i)
		if(po[id] < po[i]) {
			p[cnt] = po[i] - po[id];
			ang[cnt] = atan2(p[cnt].y, p[cnt].x);
			idx[cnt] = cnt;
			to[cnt].clear();
			fr[cnt].clear();
			cnt ++;
		}
	sort(idx + 1, idx + cnt, Cmp);
	for(int i = 0; i < cnt; ++ i)
		for(int j = 0; j < cnt; ++ j)
			for(int l = 1; l <= k; ++ l)
				f[i][j][l] = -1e18;
	for(int i = 1; i < cnt; ++ i) {
		int x = idx[i];
		if(i < cnt - 1) {
			int lst = idx[i + 1];
			to[x].push_back(lst);
			for(int j = i + 2; j < cnt; ++ j) {
				int y = idx[j];
				if(((p[lst] - p[x]) ^ (p[y] - p[x])) > 0)
					lst = y, to[x].push_back(y);
			}
		}
		if(i > 1) {
			int pre = idx[i - 1];
			fr[x].push_back(pre);
			for(int j = i - 2; j > 0; -- j) {
				int y = idx[j];
				if(((p[pre] - p[x]) ^ (p[y] - p[x])) < 0)
					pre = y, fr[x].push_back(y);
			}
		}
	}
	for(int i = 1; i < cnt; ++ i) {
		int x = idx[i], now = fr[x].size() - 1;
		for(int j = 1; j <= k; ++ j)
			mx[j] = -1e18;
		for(auto y : to[x]) {
			f[x][y][3] = p[x] ^ p[y];
			while(~now && ((p[y] - p[x]) ^ (p[fr[x][now]] - p[x])) > 0) {
				for(int l = 3; l < k; ++ l)
					mx[l] = max(mx[l], f[fr[x][now]][x][l]);
				-- now;
			}
			for(int l = 3; l < k; ++ l)
				f[x][y][l + 1] = max(f[x][y][l + 1], mx[l] + (p[x] ^ p[y]));
		}
	}
	long long res = 0;
	for(int i = 1; i < cnt; ++ i)
		for(auto j : to[idx[i]])
			res = max(res, f[idx[i]][j][k]);
	return res;
}
int main() {
	scanf("%d%d", &n, &k);
	for(int i = 0; i < n; ++ i)
		scanf("%d%d", &po[i].x, &po[i].y);
	long long ans = 0;
	for(int i = 0; i < n; ++ i)
		ans = max(ans, Solve(i));
	printf("%lld.%d0\n", ans >> 1, ans & 1 ? 5 : 0);
}
```



## Hash

```c++
#include<bits/stdc++.h>
using namespace std;
const int base1 = 29, base2 = 31, mod1 = 998244353, mod2 = 1e9 + 7, maxn = 1e5 + 5;
const pair<int, int> base = make_pair(base1, base2), mod = make_pair(mod1, mod2);
inline int Add(int x, int y, int mod) {
    return (x += y) >= mod ? x - mod : x;
}
inline int Sub(int x, int y, int mod) {
    return (x -= y) < 0 ? x + mod : x;
}
inline int Mul(int x, int y, int mod) {
    return 1ll * x * y % mod;
}
inline int Pow(int x, int y, int mod) {
    int res = 1;
    for(; y; x = Mul(x, x, mod), y >>= 1)
        if(y & 1)
            res = Mul(res, x, mod);
    return res;
}
inline pair<int, int> operator + (const pair<int, int> &a, const pair<int, int> &b) {
    return make_pair(Add(a.first, b.first, mod.first), Add(a.second, b.second, mod.second));
}
inline pair<int, int> operator - (const pair<int, int> &a, const pair<int, int> &b) {
    return make_pair(Sub(a.first, b.first, mod.first), Sub(a.second, b.second, mod.second));
}
inline pair<int, int> operator * (const pair<int, int> &a, const pair<int, int> &b) {
    return make_pair(Mul(a.first, b.first, mod.first), Mul(a.second, b.second, mod.second));
}
pair<int, int> fac[maxn], ifac[maxn], preh[maxn], sufh[maxn];
char s[maxn], t[maxn];
int n, T;
inline void Init() {
    n = 1e5;
    fac[0] = make_pair(1, 1);
    for(int i = 1; i <= n; ++ i)
        fac[i] = fac[i - 1] * base;
    ifac[n].first = Pow(fac[n].first, mod1 - 2, mod1), ifac[n].second = Pow(fac[n].second, mod2 - 2, mod2);
    for(int i = n; i; -- i)
        ifac[i - 1] = ifac[i] * base;
}
inline int Get(int l, int r, int pos) {
    if(pos < l || pos > r)
        return pos;
    return l + r - pos;
}
inline pair<int, int> Getpre(int l, int r, int pos) {
   if(pos < l)
        return preh[pos];
    else if(pos <= r) {
        int rpos = l + r - pos;
        pair<int, int> ans = preh[l - 1], mid = (sufh[rpos] - sufh[r + 1]) * ifac[n - r];
        return ans + mid * fac[l - 1];
    }
    else {
        pair<int, int> ans = preh[l - 1], mid = (sufh[l] - sufh[r + 1]) * ifac[n - r], rig = (preh[pos] - preh[r]);;
        return ans + mid * fac[l - 1] + rig;
    }
}
inline int Check(int l, int r1, int r2) {
    int le = 1, ri = n, ans = 0;
    while(le <= ri) {
        int mid = (le + ri) >> 1;
        if(Getpre(l, r1, mid) == Getpre(l, r2, mid))
            ans = mid, le = mid + 1;
        else
            ri = mid - 1;
    }
    return s[Get(l, r2, ans + 1)] < s[Get(l, r1, ans + 1)];
}
int main() {
    srand(unsigned(time(NULL)));
    Init();
    for(scanf("%d", &T); T --; ) {
        scanf("%s", s + 1);
        n = strlen(s + 1);
        for(int i = 1; i <= n; ++ i)
            t[i] = s[i];
        sort(t + 1, t + n + 1);
        int le = 0;
        for(int i = 1; i <= n; ++ i)
            if(s[i] != t[i]) {
                le = i;
                break;
            }
        preh[0] = sufh[n + 1] = make_pair(0, 0);
        for(int i = 1; i <= n; ++ i)
            preh[i] = preh[i - 1] + fac[i - 1] * make_pair(s[i] - 'a', s[i] - 'a');
        for(int i = n; i; -- i)
            sufh[i] = sufh[i + 1] + fac[n - i] * make_pair(s[i] - 'a', s[i] - 'a');
        if(!le) {
            for(int i = 1; i <= n; ++ i)
                putchar(s[i]);
            puts("");
            continue;
        }
        int ansr = 0;
        for(int i = le + 1; i <= n; ++ i) {
            if(s[i] != t[le])
                continue;
            if(!ansr) {
                ansr = i;
                continue;
            }
            if(Check(le, ansr, i))
                ansr = i;
        }
        for(int i = 1; i <= n; ++ i)
            putchar(s[Get(le, ansr, i)]);
        puts("");
    }
}
```





## Voronoi 图

```cpp
/*
平面上一些点，求每个点“控制”的多边形，即该多边形内部任意一点到该点的距离小于到其他点的距离。
通俗地说：对于每个点，求该点与其他点的中垂线，并做半平面交。
*/
const int N = 1100;
const double eps = 1e-10;
double x[N], y[N], xx[N], yy[N];
vector<Point> bd = {Point(-100, -100), Point(100, -100), Point(100, 100), Point(-100, 100)}; // Border
vector<vector<Point>> ret; // 每个点所“控制”的多边形
inline void Cut(int &cnt, double aa, double bb, double cc) {
    x[cnt] = x[0], y[cnt] = y[0];
    int ncnt = 0;
    for(int i = 0; i < cnt; ++ i) {
        double z = aa * x[i] + bb * y[i] + cc;
        if(z < eps)
			xx[ncnt] = x[i], yy[ncnt] = y[i], ++ ncnt;
        double zn = aa * x[i+1] + bb * y[i+1] + cc;
        if((z < -eps && zn > eps) || (z > eps && zn < -eps)) {
            double a = y[i+1] - y[i], b = x[i] - x[i + 1], c = -a * x[i] - b * y[i];
            double d = a * bb - b * aa;
            xx[ncnt] = (b * cc - c * bb) / d;
            yy[ncnt] = (c * aa - a * cc) / d;
            ncnt++;
        }
    }
    cnt = ncnt;
    for(int i = 0; i < cnt; ++ i)
		x[i] = xx[i], y[i] = yy[i];
}
inline void Voronoi(vector<Point> &p) {
    random_shuffle(p.begin(), p.end());
    int n = p.size();
    for(int st = 0; st < n; ++ st) {
        int cnt = bd.size();
		double _x = p[st].x, _y = p[st].y;
        for(int i = 0; i < cnt; ++ i)
			x[i] = bd[i].x, y[i] = bd[i].y;
        for(int i = 0; i < n; ++ i) {
            double aa = p[i].x - p[st].x, bb = p[i].y - p[st].y;
            double cc = -aa * (p[i].x + _x) / 2 - bb * (p[i].y + _y) / 2;
            Cut(cnt, aa, bb, cc);
        }
        vector<Point> pol;
        for(int i = 0; i < cnt; ++ i)
			pol.emplace_back(x[i], y[i]);
        ret.push_back(pol);
    }
}

```

#### simpson积分

```c++
// Care for epsilon && long double.
double F(double x) {
	// Write here.
}
double Integrate(double l, double r, double fl, double fr, double fm){
    return (r - l) / 6 * (fl + fr + 4 * fm);
}
double Simpson(double l, double r, double eps, double fl, double fr, double fm, double ans) { // Initially eps = 1e-3
    double mid = (l + r) / 2;
    double flm = F((l + mid) / 2), frm = F((mid + r) / 2);
    double ansl = Integrate(l, mid, fl, fm, flm), ansr = Integrate(mid, r, fm, fr, frm);
    if(fabs(ansl + ansr - ans) <= 15 * eps)
		return ansl + ansr + (ansl + ansr - ans) / 15;
    return Simpson(l, mid, eps / 2, fl, fm, flm, ansl) + Simpson(mid, r, eps / 2, fm, fr, frm, ansr);
}
```



# 数学

## 同余相关

#### 扩展欧几里得

对于一组 $(a, b)$，求一组 $(x,y)$ 满足 $ax + by = \gcd(a,b)$

应用：求解同余方程 $ax \equiv 1 \pmod b$，其中 $b$ 为素数，转化为方程 $ax + by = 1$

```c++
void exgcd(LL a, LL b, LL &x, LL &y) {
    if (b == 0) {
        x = 1; y = 0; return;
    }
    exgcd(b, a % b, y, x);
    y -= (a / b) * x;
}
```

#### 类欧几里得

基本形态：求
$$
f(a,b,c,n)=\sum_{i=0}^{n}\lfloor\frac{ai+b}{c}\rfloor
$$
当 $a \geq c 或 b \geq c$ 时，
$$
f(a,b,c,n) = \lfloor\ \frac{a}{c} \rfloor*\frac{n(n+1)}{2}+\lfloor\ \frac{b}{c} \rfloor * (n+1) + f(a\%c, b\%c, c, n)
$$
当 $a\leq c 且 b\leq c$ 时，

设 $m = \lfloor\ \frac{an+b}{c} \rfloor$，把求和看作是数二维坐标系上的整点
$$
f(a,b,c,n) = \sum_{i=0}^{n}\sum_{j=1}^{m}[\lfloor\ \frac{ai+b}{c} \rfloor \geq j]
$$

$$
=\sum_{i=0}^{n}\sum_{j=0}^{m-1}[\lfloor\ \frac{ai+b}{c} \rfloor \geq j+1]
$$

恒等变换一下，使不等号左边只有 i
$$
=\sum_{i=0}^{n}\sum_{j=1}^{m}[i>\frac{jc+c-b-1}{a}]
$$

$$
=\sum_{j=0}^{m-1}(n-\lfloor \frac{jc+c-b-1}{a} \rfloor)
$$

发现后半部分也是一个 f 的形式，直接递归求下去即可。递归终点为 a=0
$$
=nm - f(c,c-b-1,a,m-1)
$$
由于第一和第三项系数的变化有点像欧几里得算法，并有相同的时间复杂度，故得此名。

#### 中国剩余定理

用途：解n个形如 $ x\equiv a\mod{b} $ 的同余方程组，且保证所有b互质

解法：令 $lcm = \prod_{i=1}^{n}b_{i}$ ，对于每一个方程 $x\equiv a_{i}\mod{b_{i}}$，解同余方程 $(lcm/b_{i})*t\equiv 1\mod{b_{i}}$ 

得到其对答案的贡献 $x_{i}=(lcm/b_{i})*t$，则原方程组解为 $\sum_{i=1}^{n}x_{i}*a_{i} \pmod {lcm}$ 

正确性证明：$\forall i \ne k$ ，$lcm/b_i \equiv 0 \pmod {b_k}$，所以 $x_i*a_i \equiv 0 \pmod {b_k}$

而 $a_k*(lcm/b_k)*t \equiv a_k*1 \pmod {b_k}$，所以 $\forall k$，都有 $\sum_{i=1}^{n}x_i*a_i  \equiv a_k \pmod {b_k}$

拓展版：b 不保证互质

解法：对于第一个方程，显然 $ans = a_i$，设前 $k-1$ 个方程组的解为 $x$， $lcm = lcm(b_1, b_2,..., b_{k-1})$

那么显然 $x+t*lcm,t\in N$ 也是前 k-1 个方程的一个解

扩展CRT的核心就是找到一个 t，使 $x + t*lcm$ 也是第 k 个方程的解
$$
x + t*lcm \equiv a_k \pmod {b_k}
$$

$$
t*lcm \equiv a_k-x \pmod {b_k}
$$

这个同余方程如果有解，那么 $x+t*lcm$ 就是前 k 个方程的解

```c++
void EXCRT() {
    LL rem, mod, lcm, ans, x, y;
    read(n);
    read(lcm); read(ans);
    for (int i = 2; i <= n; ++i) {
        read(mod); read(rem);
        rem = (rem - ans % mod + mod) % mod;
        extend_gcd(lcm, mod, x, y);
        x = Mul(x, rem / d, mod);
        ans += x * lcm;
        lcm *= mod / d;
        ans = (ans % lcm + lcm) % lcm;
    }
    printf("%lld\n", ans);
}
```

#### BSGS

拓展BSGS

```C++
struct HashTable{
    int first[MOD];
    LL key1[MAXN];
    int key2[MAXN], nxt[MAXN], tot;

    inline void clean() {
        memset(first, 0, sizeof(first));
        tot = 0;
    }

    void Insert(LL k1, int k2) {
        int hsh = k1 % MOD;
        key1[++tot] = k1;
        key2[tot] = k2;
        nxt[tot] = first[hsh];
        first[hsh] = tot;
    }

    int Find(LL k1) {
        int hsh = (int)k1 % MOD;
        for(int i = first[hsh]; i; i = nxt[i]){
            if(key1[i] == k1) return key2[i];
        }
        return -1;
    }
} ht;

LL ExBSGS(LL A, LL B, LL mod) {
    if (B == 1) return 0;
    LL k = 0, div = 1, d;
    while (1) {
        d = gcd(A, mod);
        if (d == 1) break;
        if (B % d) return -1;
        B /= d; mod /= d; ++k;
        div = div*(A / d) % mod;
        if (div == B) return k;
    }
    ht.clean();
    LL m = (LL)ceil(sqrt((double)mod));
    LL tmp = B % mod;
    ht.Insert(tmp, 0);
    for(int j = 1; j <= m; ++j){
        tmp = tmp * A % mod;
        ht.Insert(tmp, j);
    }
    LL tmp2 = ksm(A, m, mod);
    tmp = div;
    for(int i = 1; i < m; ++i){
        tmp = tmp * tmp2 % mod;
        int j = ht.Find(tmp);
        if(~j) return m * i - j + k;
    }
    return -1;
}
```

#### 拓展欧拉定理

若 $\gcd(a,m) = 1$，则 $a^{\varphi(m)} \equiv 1 \pmod m$；一般情况：
$$
a^b \equiv \begin{cases}
a^b \pmod m, b < \varphi(m)\\
a^{b\% \varphi(p) + \varphi(p)} \pmod p, b \geq \varphi(m)
\end{cases}
$$
迭代 $2\log m$ 轮后 $\varphi$ 一定会减小到 1。

```c++
inline LL Reduce(LL x, LL mod) {
    if (x < mod) return x;
    return x % mod + mod;
}

inline LL Ksm(LL x, LL y, LL mod) {
    LL ret = 1;
    for(; y; y >>= 1) {
        if (y & 1) ret = Reduce(ret * x, mod);
        x = Reduce(x * x, mod);
    }
    return ret;
}

inline LL Phi(LL x) {
    if (phi.count(x)) return phi[x];
    LL ret = x;
    for (LL i = 2; i * i <= x; ++i) {
        if (x % i) continue;
        ret = ret / i * (i - 1);
        while (x % i == 0) {
            x /= i;
        }
    }
    if (x > 1) {
        ret = ret / x * (x - 1);
    }
    return phi[x] = ret;
}

LL Calc(int l, int r, LL mod) {
    if (l == r || mod == 1) return Reduce(a[l], mod);
    return Ksm(a[l], Calc(l + 1, r, Phi(mod)), mod);
}
```

#### 原根、阶与指标

满足 $a^n\equiv 1 \pmod m$  的最小正整数 $n$ 称为 $a$ 模 $m$ 的阶，记作 $ord_m(a)$

若 $m$ 为质数则由费马小定理，$a^{m-1} \equiv 1 \pmod m$，所以所有阶都是 $p-1$ 的约数

求阶：求关于 $m-1$ 的每个质因子的幂次，以 $a^{\frac{m-1}{p^k}}$ 为基础暴力枚举。可以进一步分治优化求基础的过程。

```c++
LL ord;
void Divide(int l, int r, LL oth) {
    if (l == r) {
        for (; oth != 1; oth = Ksm(oth, pfac[l])) {
            ord = ord  * pfac[l];
        }
        return;
    }
    int mid = (l + r) >> 1;
    LL tmp = 1;
    for (int i = l; i <= mid; ++i) {
        tmp *= ppow[i];
    }
    Divide(mid + 1, r, Ksm(oth, tmp));
    tmp = 1;
    for (int i = mid + 1; i <= r; ++i) {
        tmp *= ppow[i];
    }
    Divide(l, mid, Ksm(oth, tmp));
}
```

原根：$\gcd(a,m)=1$ 且 $ord_m(a) = \varphi(m)$ 的 $a$ 称为模 $m$ 的原根。

```c++
int solve(int p) {
    int tmp = p - 1;
    for (int i = 2; i * i <= tmp; ++i) {
        if (tmp % i) continue;
        pr[++pcnt] = i;
        if (i * i != tmp) {
            pr[++pcnt] = tmp / i;
        }
    }
    for (int i = 2; i < p; ++i) {
        bool flag = 1;
        for (int j = 1; j <= pcnt; ++j) {
            if (Ksm(i, pr[j]) == 1) {
                flag = 0; break;
            }
        }
        if (flag) return i;
    }
}
```

原根表：

$998244353：3；1e9+7：5；1e9+9：13$

## 整除与素数

#### GCD

```c++
LL gcd(LL a, LL b) {
    return (b == 0)? a : gcd(b, a % b);
}
```

#### Miller Rabin

由费马小定理，若 $p$ 为素数且 $(a,p)=1$，则 $a^{p-1}\equiv 1\pmod p$

二次探测定理：若 $p$ 为素数，$x \in (0,p)$，则 $x^2\equiv 1 \pmod p$ 的解为 $x=1$ 或 $x=p-1$

Miller Rabin：验证 $p$ 是否为素数。随机选一个 $a$，先用费马小定理检验，若通过则进入二次探测，设 $p-1 = 2^k*m$，由二次探测定理若 $p$ 为素数则 $a^{2^{k-1}m} \equiv 1\ or \ p-1 \pmod p$，如果是前者且 $k > 0$ 则继续迭代下去。$a$ 一般选用 $\{2,7,61\}$ 在 int 范围内一定正确。

```c++
LL tester[3] = {2, 7, 61}; //2,3,5,7,11,13,17,37,24251

inline LL Gsm(LL x, LL y, LL mod) {
    LL ret = x * y - (LL)((long double)x / mod * y + .5) * mod;
    return (ret < 0)? (ret + mod) : ret; //这里省去一个取模可以快1.5倍
}

inline LL Ksm(LL x, LL y, LL mod) {
    LL ret = 1;
    for (; y; y >>= 1) {
        if (y & 1) ret = Gsm(ret, x);
        x = Gsm(x, x);
    }
    return ret;
}

bool MillerRabin(LL x) {
    if (x == 2 || x == 3 || x == 5 || x == 7 || x == 61) return 1;
    if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 61 == 0) return 0;
    for (int i = 0; i < 3; ++i) {
        LL a = tester[i];
        LL tmp = Ksm(a, x - 1, x), k = x - 1;
        if (tmp != 1) return 0;
        while (tmp == 1 && !(k & 1)) {
            k >>= 1;
            tmp = Ksm(a, k, x);
            if (tmp != 1 && tmp != x - 1) return 0;
        }
    }
    return 1;
}
```

#### Pollard Rho

一次 $Pollard\ Rho$ 可以找到 $n$ 的一个非平凡因子 $p$。

```c++
LL Pollard_Rho(LL n, int c) {
    LL i = 0, k = 2, x = rand() % (n - 1) + 1, y = x;
    while (1) {
        x = (Gsm(x, x, n) + c) % n;
        LL d = gcd(n, (y - x + n) % n);
        if (d != 1 && d != n) return d;
        if (x == y) return n;
        if (++i == k) {
            y = x;
            k <<= 1;
        }
    }
}

void Factor(LL n, int c) {
       if (n == 1) return;
    if (MillerRabin::Judge(n)) {
        pr[++pcnt] = n;
        return;
    }
    LL p = n;
    while (p >= n) {
        p = Pollard_Rho(n, c++);
    }
    while (n % p == 0) n /= p;
    Factor(p, c);
    Factor(n, c);
}
```

## 数论函数

#### 埃氏筛

主要用于卡空间的情况

小优化：跳过所有偶数

```c++
bitset<MAXN> notp;

void sieve() {
    for (int i = 3; i * i <= r; i += 2) {
        if (notp[i]) continue;
        for (int j = i * i; j <= r; j += i + i) {
            notp[j] = 1;
        }
    }
}
```

#### 线性筛

一般数论函数的线筛：只需要能快速算出 $f(p^k)$ （p 为素数）即可。

```c++
int vis[MAXN], pr[MAXN], pcnt, low[MAXN];
LL f[MAXN];

void sieve() {
    f[1] = ???;
    for (int i = 2; i <= n; ++i) {
        if (!vis[i]) {
            pr[++pcnt] = i;
            f[i] = ???;
            low[i] = i;
        }
        for (int j = 1; j <= pcnt && i * pr[j] <= n; ++j) {
            vis[i * pr[j]] = 1;
            if (i % pr[j]) {
                f[i * pr[j]] = f[i] * f[pr[j]];
                low[i * pr[j]] = pr[j];
            } else {
                if (low[i] == i) {
                    f[i * pr[j]] = ???;
                } else {
                    f[i * pr[j]] = f[i / low[i]] * f[low[i] * pr[j]];
                }
                low[i * pr[j]] = low[i] * pr[j];
                break;
            }
        }
    }
}
```

#### 狄利克雷卷积

定义：考虑数论函数 $f(n), g(n)$，则 $(f*g)(n) = \sum_{d|n}f(d)g(\dfrac{n}{d})$

常见数论函数：$1(n) = 1, id(n)=n, e(n)=[n=1], \sigma(n) = 约数和,\tau(n)=约数个数$

常见狄利克雷卷积：$1*\mu=e$、$1*id=\sigma$、$\mu*id = \varphi$、$\varphi *1 = id$、$\varphi * \tau = \sigma$

#### 杜教筛

想求 $S(n)=\sum_{i=1}^{n}f(i)$，$f(i)$ 为积性函数

找一个积性函数 $g$，$\sum_{i=1}^{n}(g*f) = \sum_{d=1}^{n}g(d)S(\dfrac{n}{d})$，于是 $g(1)S(n) = \sum_{i=1}^{n}(g*f)-\sum_{d=2}^{n}g(d)S(\dfrac{n}{d})$

只要让 $\sum_{i=1}^{n}(g*f)$ 可以快速求即可。

例：$f(n) = \mu(n)$，取 $g(n) = 1(n)$，则 $\sum_{i=1}^{n}(g*f) = \sum_{i=1}^{n}e(n) =1$

例：$f(n) = \varphi(n)$，取 $g(n) = 1(n)$，则 $\sum_{i=1}^{n}(g*f) = \sum_{i=1}^{n}id(i) = \dfrac{n(n+1)}{2}$

```c++
int pr[MAXN], pcnt, vis[MAXN];
LL mu[MAXN], phi[MAXN];
map<int, LL> Mu, Phi;

void prework() {
    phi[1] = mu[1] = 1;
    for(int i = 2; i< MAXN; ++i){
        if (!vis[i]) {
            pr[++cnt] = i;
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for(int j = 1; j <= cnt && i * pr[j] < MAXN; ++j){
            int k = i * pr[j];
            vis[k] = 1;
            if (i % pr[j]) {
                phi[k] = phi[i] * (pr[j] - 1);
                mu[k] = -mu[i];
            } else {
                phi[k] = phi[i] * pr[j];
                mu[k] = 0;
                break;
            }
        }
    }
    for(int i = 1; i < MAXN; ++i){
        phi[i] += phi[i - 1];
        mu[i] += mu[i - 1];
    }
}

LL Sum_Phi(int n) {
    if(n < MAXN) return phi[n];
    if(Phi.count(n)) return Phi[n];
    LL ret = (LL)n * (n + 1) / 2;
    for(int i = 2, j; i <= n; i = j + 1){
        j = n / (n / i);
        ret -= Sum_Phi(n / i) * (j - i + 1);
    }
    return Phi[n] = ret;
}

LL Sum_Mu(int n) {
    if(n < MAXN) return mu[n];
    if(Mu.count(n)) return Mu[n];
    LL ret = 1;
    for(int i = 2, j; i <= n; i = j + 1){
        j = n / (n / i);
        ret -= Sum_Mu(n / i) * (j - i + 1);
    }
    return Mu[n] = ret;
}
```

#### min25筛

先对 $\lfloor \frac{n}{i}\rfloor$ 的 $O(\sqrt n)$ 个取值，求 $\sum_{i=2}^{x}[i为质数]f(i)$，其中 $f(i)$ 为原函数在质数处的表达式，如果不满足完全积性则要拆成若干个完全积性函数之和。

设 $A(x, j) = $$\sum_{i=2}^{x}[i为质数或i的所有质因子>p_j]f(i)$，那么我们要求的就是 $A(x, |P|)$ 
$$
A(x,j)=A(x,j-1)-f(p_j)*[A(\dfrac{x}{p_j},j-1)-A(p_j-1,j-1)]
$$
然后再把合数和 1 算进来，设 $S(x,j)=\sum_{i=2}^{x}[i的所有质因子\geq p_j]f(i)$，答案即 $S(n,1)+1$
$$
S(x,j)=A(x,j-1)+\sum_{k=j}^{|P|}\sum_{p_{k}^{t+1}\leq x}[f(p_k^t)S(\dfrac{x}{p_k^t},k+1)+f(p_k^{t+1})]
$$
例：luogu模板题

```C++
int INV2, INV6;
LL n, rt, pr[MAXN], w[MAXN];
int pcnt, vis[MAXN], sum1[MAXN], sum2[MAXN];
int id1[MAXN], id2[MAXN], m, g1[MAXN], g2[MAXN];

inline int ID(LL x) {
    return (x <= rt)? id1[x] : id2[n / x];
}

inline int f(LL x) {
    x %= MOD;
    return Mul(x, x - 1);
}

inline int S1(LL x) {
    x %= MOD;
    return x * (x + 1) / 2 % MOD;
}

inline int S2(LL x) {
    x %= MOD;
    return Mul(Mul(x, Mul(x + 1, x * 2 + 1)), INV6);
}

void sieve(int lim) {
    for (int i = 2; i <= lim; ++i) {
        if (!vis[i]) {
            pr[++pcnt] = i;
            sum1[pcnt] = Add(sum1[pcnt - 1], i);
            sum2[pcnt] = Add(sum2[pcnt - 1], Mul(i, i));
        }
        for (int j = 1; j <= pcnt && i * pr[j] <= lim; ++j) {
            vis[i * pr[j]] = 1;
            if (i % pr[j] == 0) break;
        }
    }
}

int S(LL x, int j) {
    if (x <= 1 || x < pr[j]) return 0;
    int ret = Sub(g2[ID(x)], g1[ID(x)]);
    Dec(ret, Sub(sum2[j - 1], sum1[j - 1]));
    for (int k = j; k <= pcnt; ++k) {
        LL pt = pr[k], pt2 = pr[k] * pr[k];
        if (pt2 > x) break;
        for (int t = 1; pt2 <= x; ++t, pt = pt2, pt2 *= pr[k]) {
            Inc(ret, Mul(f(pt), S(x / pt, k + 1)));
            Inc(ret, f(pt2));
        }
    }
    return ret;
}

void solve() {
    read(n);
    sieve(rt = sqrt(n));
    m = 0;
    for (LL i = 1, j; i <= n; i = j + 1) {
        w[++m] = n / i;
        j = n / w[m];
        if (w[m] <= rt) id1[w[m]] = m;
        else id2[j] = m;
        g1[m] = Sub(S1(w[m]), 1);
        g2[m] = Sub(S2(w[m]), 1);
    }
    for (int j = 1; j <= pcnt; ++j) {
        for (int i = 1; i <= m && pr[j] * pr[j] <= w[i]; ++i) {
            int k = ID(w[i] / pr[j]);
            Dec(g1[i], Mul(pr[j], Sub(g1[k], sum1[j - 1])));
            Dec(g2[i], Mul(Mul(pr[j], pr[j]), Sub(g2[k], sum2[j - 1])));
        }
    }
    int ans = Add(S(n, 1), 1);
    printf("%d\n", ans);
}
```

#### Powerful Number

定义：所有质因数幂次大于 1 的数。都可以表示为 $a^2b^3$ 的形式，积分可证数量为 $O(\sqrt n)$。

筛法：对于一个不太好搞的积性函数 $f(n)$，造一个容易求前缀和的积性函数 $g(n)$，满足 $n$ 为素数时两者相等。由此构造 $h(n)$ 满足 $f=g*h$，则 $h$ 仅在 PN 处有非零值。于是
$$
F(n) = \sum_{d=1,d为PN}^{n} h(d)G(\dfrac{n}{d})
$$
只要搞出 $h(p^c)$ 的公式即可。

#### 莫比乌斯反演

$$
f(n)=\sum_{d|n}g(d) \Leftrightarrow g(n)=\sum_{d|n}\mu(d)f(n/d)
$$

常见应用：把 $[\gcd(x,y)=1]$ 转化为 $\sum_{d|x,d|y}\mu(d)$

#### 欧拉函数

$$
\sum_{d|x,d|y}\varphi(d) = \gcd(x,y)
$$

$$
\sum_{i=1}^{n}i[\gcd(i,n)=1] = \dfrac{n\varphi(n) + [n=1]}{2}
$$

#### 其它

约数个数的一个公式
$$
\tau(ij)=\sum_{x|i}\sum_{y|j}[gcd(x,y)=1]
$$
约数和
$$
\sigma(ij) = \sum_{x|i}\sum_{y|j}x*\dfrac{j}{y}[\gcd(x,y)=1]
$$


## 组合计数

#### 组合数

```c++
inline int C(int x, int y) {
    if (x < y || y < 0) return 0;
    return Mul(fac[x], Mul(finv[y], finv[x - y]));
}

void prework() {
    fac[0] = 1;
    for (int i = 1; i <= n; ++i) {
        fac[i] = Mul(fac[i - 1], i);
    }
    finv[n] = Inv(fac[n]);
    for (int i = n; i >= 1; --i) {
        finv[i - 1] = Mul(finv[i], i);
    }
}
```

#### 卢卡斯定理

推论：$n\&k=k$ 时，$n\choose k$ 为奇数，否则为偶数。

```c++
LL Lucas(int n, int m, int p) {
    if (n < m) return 0;
    if (n < p) return (fac[n] * finv[m] * finv[n-m]) % p;
    return Lucas(n / p, m / p, p) * Lucas(n % p, m % p, p) % p;
}
```

扩展卢卡斯：转成模质数的情况再CRT合并。需要用一堆优化

```C++
LL ksm(LL x, LL y, LL mod) {
    LL ret = 1;
    for (; y; y >>= 1) {
        if (y & 1) ret = ret * x % mod;
        x = x * x % mod;
    }
    return ret;
}

inline void extend_gcd(LL a, LL b, LL &x, LL &y) {
    if(b == 0){
        x = 1; y = 0; return;
    }
    extend_gcd(b, a % b, y, x);
    y -= a / b * x;
}

inline LL Inv(LL a, LL mod) {
    LL x, y;
    extend_gcd(a, mod, x, y);
    return (x += mod) > mod? x - mod : x;
}

inline void CRT(LL &ans, LL rem, LL mod) {
    LL other = p / mod;
    ans = (ans + rem * Inv(other, mod) % p * other % p) % p;
}

LL Fac(LL n, LL pi, LL pk) {
    if (!n) return 1;
    LL ret = 1;
    for(LL i = 2; i <= pk; ++i) {
        if(i % pi){
            ret = ret * i % pk;
        }
    }
    ret = ksm(ret, n / pk, pk);
    for (LL i = n % pk; i >= 2; --i) {
        if(i % pi){
            ret = ret * i % pk;
        }
    }
    return ret * Fac(n / pi, pi, pk) % pk;
}

inline LL Count(LL n, LL pi) {
    LL ret = 0;
    for(LL i = n; i; i /= pi){
        ret += i / pi;
    }
    return ret;
}

inline LL C(LL n, LL m, LL pi, LL pk) {
    LL facn = Fac(n, pi, pk);
    LL facm = Fac(m, pi, pk);
    LL facnm = Fac(n - m, pi, pk);
    LL cnt = Count(n, pi) - Count(m, pi) - Count(n - m, pi);
    return facn * Inv(facm, pk) % pk * Inv(facnm, pk) % pk * ksm(pi, cnt, pk) % pk;
}

LL ExLucas(LL n, LL m) {
    LL ret = 0, tmp = p, pk;
    for(LL i = 2; i * i <= tmp; ++i){
        if(tmp % i == 0){
            pk = 1;
            while(tmp % i == 0){
                pk *= i;
                tmp /= i;
            }
            CRT(ret, C(n, m, i, pk), pk);
        }
    }
    if(tmp > 1){
        CRT(ret, C(n, m, tmp, tmp), tmp);
    }
    return ret;
}
```

#### 卡特兰数

卡特兰数有四种求法，你知道么？
$$
h(n)=\sum_{i=0}^{n-1}h(i)*h(n-i-1)\\
h(n)=\frac{h(n-1)*(4n-2)}{n+1}\\
h(n)=\frac{C(2n, n)}{n+1}\\
h(n)=C(2n, n)-C(2n, n-1)
$$

#### 斯特林数

第一类：n 个元素划分为 k 个非空环排列
$$
\begin{bmatrix}n\\k\end{bmatrix} = \begin{bmatrix}n - 1\\k - 1\end{bmatrix} + (n-1)\begin{bmatrix}n-1\\k\end{bmatrix}
$$
第二类：n 个元素划分为 k 个非空子集
$$
\begin{Bmatrix}n\\k\end{Bmatrix} = \begin{Bmatrix}n-1\\k-1\end{Bmatrix} + k\begin{Bmatrix}n-1\\k\end{Bmatrix}
$$
通项：
$$
\begin{Bmatrix}n\\m\end{Bmatrix} \sum_{i=0}^{m}\dfrac{(-1)^{m-i}i^n}{i!(m-i)!}
$$
斯特林反演：
$$
f(n) = \sum_{i=1}^{n}\begin{Bmatrix}n\\i\end{Bmatrix}g(i) \Leftrightarrow g(n) = \sum_{i=1}^{n}(-1)^{n-i}\begin{bmatrix}n\\i\end{bmatrix}f(i)
$$

$$
f(k) = \sum_{i=k}^{n}\begin{Bmatrix}i\\k\end{Bmatrix}g(i) \Leftrightarrow g(k) = \sum_{i=k}^{n}(-1)^{i-k}\begin{bmatrix}i\\k\end{bmatrix}f(i)
$$

下降幂与斯特林数：

$x^n = \sum_{i=0}^{n}S(n,i)x^{\underline{i}}$普通幂转下降幂

$x^{\underline{n}} = \sum_{i=0}^{n}(-1)^{n-i}s(n,i)x^i$ 下降幂转普通幂

例：ICPC2021济南

题意：给出普通多项式 $f(x)$，计算 $\sum_{i=0}^{\infty}\dfrac{f(i)}{i!} = p * e$，求 $p$

解1：考虑 $f(x) = x^n$ 的答案的 EGF
$$
\sum_{n}\dfrac{x^n}{n!}\sum_{i}\dfrac{i^n}{i!} = \sum_{i}\dfrac{1}{i!}\sum_{n}\dfrac{(ix)^n}{n!} = \sum_{i}\dfrac{(e^x)^i}{i!} = e^{e^x} = e^{e^x-1}e
$$
解2：把 $x^n$ 用下降幂展开
$$
\sum_{i}\dfrac{i^n}{i!} = \sum_{i}\dfrac{1}{i!}\sum_{j=0}^{n}S(n,j)i^{\underline{j}} = \sum_{i}\sum_{j=0}^{n}S(n,j)\dfrac{1}{(i-j)!} = \sum_{j=0}^{n}S(n,j)\sum_{i\geq j}\dfrac{1}{(i-j)!} = \sum S(n,j)e
$$
于是答案就是 $\sum a_n * \sum S(n,j)$，一行的斯特林数求和是贝尔数，其 EGF 为 $e^{e^x-1}$

下降幂与点值多项式：

考虑一个点值与这个点代入下降幂多项式
$$
f(k) = \sum_{i=0}^{k}a_i\dfrac{k!}{(k-i)!}\\
\dfrac{f(k)}{k!} = \sum_{i=0}^{k}a_i\dfrac{1}{(k-i)!}\\
$$
设 $F(x)$ 表示点值多项式的 EGF，$A(x)$ 表示下降幂多项式的 OGF，则
$$
F(x) = A(x)e^x\\
A(x) = F(x)e^{-x}
$$

下降幂与普通幂：

普通幂转下降幂：发现点值可以转下降幂，那普通幂多点求值即可。

下降幂转普通幂：先转点值，发现点值恰好是连续的，快速插值回去即可。



斯特林数的行、列

1）第二类斯特林数求一行：考虑基本公式 $x^n = \sum_{i=0}^{n}S(n,i)x^{\underline{i}}$，那么 $S(n,i)$ 就是 $f(x)=x^n$ 对应的下降幂多项式的系数，求出点值后即可转换

2）第二类斯特林数求一列：先假设集合是有序的，一个集合的 EGF 是 $e^x-1$，划分为 k 个集合就是 $(e^x-1)^k$，最后再除掉 $k!$

3）第一类斯特林数求一行：简单的公式是 $\sum S(n,i)x^i = x^{\overline{n}}$，可以分治 NTT 优化，也可以考虑倍增：

$x^{\overline{2n}} = x^{\overline{n}} * (x+n)^{\overline{n}}$，只需要求多项式平移，这个可以 $O(n\log n)$ 解决，根据主定理总复杂度 $O(n\log n)$

4）第一类斯特林数求一列：一个环排列的 EGF 是 $S(x) = \sum \dfrac{x^i}{i} = \ln\dfrac{1}{1-x}$，划分为 k 个就是 $S(x)^k$



#### 伯努利数

定义：$B_0=1, \sum_{i=0}^{n}B_i{n+1\choose i}=0$

生成函数：$B(x) = \frac{x}{e^x-1}$

应用：$S_k(n) = \sum_{i=0}^{n-1}i^k = \frac{1}{k+1}\sum_{i=0}^{k}{k+1\choose i}B_in^{k+1-i}$ 

#### 容斥反演

$$
f(k) = \sum_{i=0}^{n}{n\choose i}g(i) \Leftrightarrow g(k) = \sum_{i=0}^{n}(-1)^{n-i}{n\choose i}f(i)\\
f(k) = \sum_{i=k}^{n}{i\choose k}g(i) \Leftrightarrow g(k) = \sum_{i=k}^{n}(-1)^{i-k}{i\choose k}f(i)
$$

#### 常见生成函数

$$
{2n\choose n}x^n = \dfrac{1}{\sqrt{1-4x}}\\
\ln(\frac{1}{1-x^i}) = \sum_{j=1}^{\infty}\frac{x^{ij}}{j}\\
\dfrac{e^x-e^{-x}}{2} = x+\dfrac{x^3}{3!}+\dfrac{x^5}{5!}+...\\
\dfrac{e^x+e^{-x}}{2} = 1+\dfrac{x^2}{2!} + \dfrac{x^4}{4!} + ...\\
\dfrac{x}{(1-x)^2} = \sum_{i=0}^{\infty}ix^i\\
\dfrac{x(x+1)}{(1-x)^3} = \sum_{i=0}^{\infty}i^2x^i\\
\dfrac{2x^2}{(1-x)^3} = \sum_{i=0}^{\infty}i(i-1)x^i\\
\dfrac{1}{(1-x)^n}=\sum_{i=0}^{\infty}{n+i-1\choose i}x^i
$$

#### 群论

Burnside 引理

设 $G$ 是 $1~n$ 的一个置换群，$G$ 中不同的等价类个数为：
$$
\frac{1}{|G|}\sum_{g\in G}c_1(g)
$$
其中 $c_1(g)$ 表示 g 中不变元的个数

Polya 定理

Polya 定理用于计算形如 “用m种颜色给n个对象着色，在某些条件下两个着色方案视为一种，求不同的着色方案数” 这样的问题。方案数为：
$$
\frac{1}{|G|}\sum_{g\in G}m^{c(a_g)}
$$
其中 $c(a_g)$ 表示 g 的循环节数。

## 卷积与多项式

#### 拉格朗日插值

普通版

已知 $k$ 个 $x_i, y_i=f(x_i)$，求 $f(n)$

```c++
int Lagrange(int k, int *x, int *y, int n) { //k 是多项式次数，n 是想求 f(n)
    int ret = 0;
    for (int i = 1; i <= k; ++i) {
        int up = y[i], dn = 1;
        for (int j = 1; j <= k; ++j) {
            if (i == j) continue;
            up = Mul(up, Sub(n, x[j]));
            dn = Mul(dn, Sub(x[i], x[j]));
        }
        Inc(ret, Mul(up, Inv(dn)));
    }
    return ret;
}
```

线性版

已知 $f(0), f(1)...f(k)$，求 $f(n)$

```c++
int Lagrange(int *f, int n) {
    pre[0] = n % MOD;
    for (int i = 1; i <= k; ++i) {
        pre[i] = Mul(pre[i - 1], (n - i) % MOD);
    }
    suf[k] = (n - k) % MOD;
    for (int i = k - 1; i >= 0; --i) {
        suf[i] = Mul(suf[i + 1], (n - i) % MOD);
    }
    int ret = 0;
    for (int i = 0; i <= k; ++i) {
        int cur = f[i];
        if (i > 0) cur = Mul(cur, pre[i - 1]);
        if (i < d) cur = Mul(cur, suf[i + 1]);
        cur = Mul(cur, Mul(finv[i], finv[k - i]));
        ((k - i) & 1)? Dec(ret, cur) : Inc(ret, cur);
    }
    return ret;
}
```

#### NTT

```c++
int len, rev[MAXN], w1[MAXN], w2[MAXN];

void prework(int m) {
    for (len = 1; len < m; len <<= 1);
    int Gi = 3, Gv = Inv(Gi);
    for (int i = 1; i < len; i <<= 1) {
        int wk1 = Ksm(Gi, (MOD - 1) / (i << 1)), wnk1 = 1;
        int wk2 = Ksm(Gv, (MOD - 1) / (i << 1)), wnk2 = 1;
        for (int k = 0; k < i; ++k) {
            w1[i + k] = wnk1;
            wnk1 = Mul(wnk1, wk1);
            w2[i + k] = wnk2;
            wnk2 = Mul(wnk2, wk2);
        }
    }
}

void GetRev(int m) {
    int bit = -1;
    for (len = 1; len < m; len <<= 1) ++bit;
    for (int i = 0; i < len; ++i) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << bit);
    }
}

void NTT(int *a, int type) {
    int *w = (type == 1? w1 : w2);
    for (int i = 0; i < len; ++i) {
        if (i < rev[i]) {
            swap(a[i], a[rev[i]]);
        }
    }
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                int x = a[j + k], y = Mul(a[j + k + i], w[i + k]);
                a[j + k] = Add(x, y);
                a[j + k + i] = Sub(x, y);
            }
        }
    }
    if (type == 1) return;
    int lenv = Inv(len);
    for (int i = 0; i < len; ++i) {
        a[i] = Mul(a[i], lenv);
    }
}
```

#### 多项式求逆、开根、ln、exp

```c++
void Poly_Inv(int n, int *a, int *b) {
    if (n == 1) {
        b[0] = Inv(a[0]);
        return;
    }
    Poly_Inv((n + 1) >> 1, a, b);
    GetRev(n << 1);
    copy(a, a + n, C);
    fill(C + n, C + len, 0);
    NTT(b, 1); NTT(C, 1);
    for (int i = 0; i < len; ++i) {
        b[i] = Mul(Sub(2, Mul(b[i], C[i])), b[i]);
    }
    NTT(b, -1);
    fill(b + n, b + len, 0);
}

void Poly_Sqrt(int n, LL *a, LL *b){
    if (n == 1) {
        b[0] = a[0];
    }
    Sqrt((n + 1)>> 1, a, b);
    Inv(n, b, D);
    GetRev(n << 1);
    copy(a, a + n, C);
    fill(C + n, C + len, 0);
    NTT(D, 1); NTT(C, 1);
    for (int i = 0;i < len; ++i) {
        D[i] = Mul(D[i], C[i]);
    }
    NTT(D, -1);
    for (int i = 0; i < n; ++i) {
        b[i] = Mul(INV2, Add(b[i], D[i]));
    }
    fill(D + 0, D + len, 0);
}

void Poly_Direv(int n, int *a, int *b) {
    for (int i = 1; i < n; ++i) {
        b[i - 1] = Mul(a[i], i);
    }
    b[n - 1] = 0;
}

void Poly_Integ(int n, int *a, int *b) {
    for (int i = 1; i < n; ++i) {
        b[i] = Mul(a[i - 1], inv[i]);
    }
    b[0] = 0;
}

void Poly_Ln(int n, int *a, int *b) {
    Poly_Direv(n, a, A);
    Poly_Inv(n, a, B);
    GetRev(n << 1);
    NTT(A, 1); NTT(B, 1);
    for (int i = 0; i < len; ++i) {
        A[i] = Mul(A[i], B[i]);
    }
    NTT(A, -1);
    Poly_Integ(n, A, b);
    fill(A, A + len, 0);
    fill(B, B + len, 0);
}

void Poly_Exp(int n, int *a, int *b) {
    if (n == 1) {
        b[0] = 1;
        return;
    }
    Poly_Exp((n + 1) >> 1, a, b);
    Poly_Ln(n, b, C);
    GetRev(n << 1);
    for (int i = 0; i < n; ++i) {
        C[i] = Sub(a[i], C[i]);
    }
    Inc(C[0], 1);
    fill(C + n, C + len, 0);
    NTT(b, 1); NTT(C, 1);
    for (int i = 0; i < len; ++i) {
        b[i] = Mul(b[i], C[i]);
    }
    NTT(b, -1);
    fill(b + n, b + len, 0);
}
```

#### 分治NTT

```c++
typedef vector<int> poly;

void NTT(poly &a, int type) {
    a.resize(len);
    int *w = (type == 1? w1 : w2);
    for (int i = 0; i < len; ++i) {
        if (i < rev[i]) {
            swap(a[i], a[rev[i]]);
        }
    }
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                int x = a[j + k], y = Mul(a[j + k + i], w[i + k]);
                a[j + k] = Add(x, y);
                a[j + k + i] = Sub(x, y);
            }
        }
    }
    if (type == 1) return;
    int lenv = Inv(len);
    for (int i = 0; i < len; ++i) {
        a[i] = Mul(a[i], lenv);
    }
}

poly Poly_Mul(poly F, poly G) {
    int deg = F.size() + G.size() - 1;
    if (deg <= 100) {
        poly H(deg, 0);
        int n = F.size(), m = G.size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                Inc(H[i + j], Mul(F[i], G[j]));
            }
        }
        return H;
    }
    GetRev(deg);
    NTT(F, 1); NTT(G, 1);
    for (int i = 0; i < len; ++i) {
        F[i] = Mul(F[i], G[i]);
    }
    NTT(F, -1);
    F.resize(deg);
    return F;
}

poly Poly_Prod(int l, int r) {
    if (l == r) {
        poly F;
        F.push_back(1);
        F.push_back(a[l]);
        return F;
    }
    int mid = (l + r) >> 1;
    return Poly_Mul( Poly_Prod(l, mid), Poly_Prod(mid + 1, r) );
}
```

#### 多点求值

```c++
poly Poly_Inv(int n, poly &F) {
    if (n == 1) {
        poly ret;
        ret.push_back(Inv(F[0]));
        return ret;
    }
    poly X = Poly_Inv((n + 1) >> 1, F);
    poly C(F.begin(), F.begin() + n);
    GetRev(n << 1);
    NTT(X, 1); NTT(C, 1);
    for (int i = 0; i < len; ++i) {
        X[i] = Mul(X[i], Sub(2, Mul(X[i], C[i])));
    }
    NTT(X, -1);
    X.resize(n);
    return X;
}

poly Poly_Mod(poly &F, poly &G) {
    int n = F.size(), m = G.size();
    if (n < m) return F;
    poly FR = F, GR = G;
    reverse(FR.begin(), FR.end());
    reverse(GR.begin(), GR.end());
    GR.resize(n - m + 1);
    poly Q = Poly_Mul(FR, Poly_Inv(n - m + 1, GR));
    Q.resize(n - m + 1);
    reverse(Q.begin(), Q.end());
    poly R = Poly_Mul(G, Q);
    R.resize(m - 1);
    for (int i = 0; i < m - 1; ++i) {
        R[i] = Sub(F[i], R[i]);
    }
    return R;
}

poly seg[MAXN << 2];

void Build(int rt, int l, int r) {
    if (l == r) {
        seg[rt].push_back(MOD - pts[l]);
        seg[rt].push_back(1);
        return;
    }
    int mid = (l + r) >> 1;
    Build(lc, l, mid);
    Build(rc, mid + 1, r);
    if (rt > 1) seg[rt] = Poly_Mul(seg[lc], seg[rc]);
}

void Poly_MultiVal(int rt, int l, int r, poly F) {
    if (r - l <= 100) {
        int deg = F.size() - 1;
        for (int i = l; i <= r; ++i) {
            int cur = 0;
            for (int j = deg; j >= 0; --j) {
                cur = Add(Mul(cur, pts[i]), F[j]);
            }
            val[i] = cur;
        }
        return;
    }
    int mid = (l + r) >> 1;
    Poly_MultiVal(lc, l, mid, PolyMod(F, seg[lc]));
    Poly_MultiVal(rc, mid + 1, r, PolyMod(F, seg[rc]));
}
```

#### 牛顿迭代

已知一个函数 $G(x)$，求一个多项式 $F(x)$，满足 $G(F(x)) \equiv 0 \pmod {x^n}$ 

设已经求出 $F_0(x)$，满足 $G(F_0(x)) \equiv 0 \pmod {x^\frac{n}{2}}$，则有
$$
F(x) = F_0(x) - \frac{G(F_0(x))}{G'(F_0(x))}
$$

#### 拉格朗日反演

对于一个多项式 $F(x)$，若存在一个 $F^{-1}(x)$ 使得 $F^{-1}(F(x)) = x$，那么显然也满足 $F(F^{-1}(x)) = x$，那么我们称 $F(x)$ 和 $F^{-1}(x)$ 互为复合逆。

使用拉格朗日反演可以快速求出复合逆的某一项
$$
[x^n]F(x)=\frac{1}{n}[x^{n-1}](\frac{x}{F^{-1}(x)})^n
$$
使用拓展拉格朗日反演可以算一些更复杂的东西
$$
[x^n]G(F(x))=\frac{1}{n}[x^{n-1}]G'(x)(\frac{x}{F^{-1}(x)})^n
$$

#### MTT

```c++
struct comp {
    LD r, i;
    comp() {}
    comp(LD _r, LD _i): r(_r), i(_i) {}
    friend comp operator + (comp a, comp b) {
        return comp(a.r + b.r, a.i + b.i);
    }
    friend comp operator - (comp a, comp b) {
        return comp(a.r - b.r, a.i - b.i);
    }
    friend comp operator * (comp a, comp b) {
        return comp(a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r);
    }
    friend comp operator / (comp a, LD b) {
        return comp(a.r / b, a.i / b);
    }
};

int len, rev[MAXN];
comp w[MAXN], P[MAXN], Q[MAXN], R[MAXN], S[MAXN];

void FFT_init(int m) {
    int bit = -1;
    for (len = 1; len <= m; len <<= 1) ++bit;
    for (int i = 0; i < len; ++i) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << bit);
    }
    for (int i = 1; i < len; i <<= 1) {
        comp wn(cos(PI / i), sin(PI / i)), wnk(1, 0);
        for (int k = 0; k < i; ++k) {
            w[i + k] = wnk;
            wnk = wnk * wn;
        }
    }
}

void DFT(comp *a) {
    for (int i = 0; i < len; ++i) {
        if (i < rev[i]) {
            swap(a[i], a[rev[i]]);
        }
    }
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                comp x = a[j + k], y = w[i + k] * a[j + k + i];
                a[j + k] = x + y;
                a[j + k + i] = x - y;
            }
        }
    }
}

void IDFT(comp *a) {
    DFT(a);
    reverse(a + 1, a + len);
    for (int i = 0; i < len; ++i) {
        a[i] = a[i] / len;
    }
}

inline LL toll(LD x) {
    return (LL)(x + 0.5);
}

void Mul(int n, LL *a, LL *b) {
    FFT_init(n + n);
    for (int i = 0; i <= n; ++i) {
        P[i] = comp(a[i] % M, a[i] / M);
        Q[i] = comp(b[i] % M, b[i] / M);        
    }
    for (int i = n + 1; i < len; ++i) {
        P[i] = Q[i] = comp(0, 0);
    }
    DFT(P); DFT(Q);
    for (int i = 0; i < len; ++i) {
        int j = (len - i) & (len - 1);
        R[i] = comp((P[i].r + P[j].r) * 0.5, (P[i].i - P[j].i) * 0.5) * Q[i];
        S[i] = comp((P[i].i + P[j].i) * 0.5, (P[j].r - P[i].r) * 0.5) * Q[i];
    }
    IDFT(R); IDFT(S);
    for (int i = 0; i < len; ++i) {
        LL ac = toll(R[i].r), ad = toll(R[i].i), bc = toll(S[i].r), bd = toll(S[i].i);
        a[i] = (ac % mod + (ad + bc) % mod * M % mod + bd % mod * M * M % mod) % mod;
    }
}
```

#### FWT

```c++
void FWT_or(int *a) {
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                Inc(a[j + k + i], a[j + k]);
            }
        }
    }
}

void iFWT_or(int *a) {
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                Dec(a[j + k + i], a[j + k]);
            }
        }
    }
}

void FWT_and(int *a) {
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                Inc(a[j + k], a[j + k + i]);
            }
        }
    }
}

void iFWT_and(int *a) {
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                Dec(a[j + k], a[j + k + i]);
            }
        }
    }
}

void FWT_xor(int *a) {
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                int x = a[j + k], y = a[j + k + i];
                a[j + k] = Add(x, y);
                a[j + k + i] = Sub(x, y);
            }
        }
    }
}

void iFWT_xor(int *a) {
    for (int i = 1; i < len; i <<= 1) {
        for (int j = 0; j < len; j += (i << 1)) {
            for (int k = 0; k < i; ++k) {
                int x = a[j + k], y = a[j + k + i];
                a[j + k] = Mul(Add(x, y), INV2);
                a[j + k + i] = Mul(Sub(x, y), INV2);
            }
        }
    }
}
```

## 线性代数

#### 高斯消元

```c++
void Gauss() {
    for (int i = 1; i <= n; ++i) {
        int p = i;
        while (p <= n && ma[p][i] == 0) ++p;
        if (p > i) {
            for (int j = i; j <= n + 1; ++j) {
                swap(ma[i][j], ma[p][j]);
            }
        }
        int iv = Inv(ma[i][i]);
        for (int j = i; j <= n + 1; ++j) {
            ma[i][j] = Mul(ma[i][j], iv);
        }
        for (int j = i + 1; j <= n; ++j) {
            int tmp = ma[j][i];
            for (int k = i; k <= n + 1; ++k) {
                Dec(ma[j][k], Mul(tmp, ma[i][k]));
            }
        }
    }
    for (int i = n; i >= 1; --i) {
        for (int j = 1; j < i; ++j) {
            Dec(ma[j][n + 1], Mul(ma[j][i], ma[i][n + 1]));
        }
    }
}
```

#### 矩阵树定理

注意是基尔霍夫矩阵去掉最后一行和最后一列后的行列式

```c++
int Det() {
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (e[i][j]) {
                Inc(a[i][i], 1);
                Inc(a[j][j], 1);
                Dec(a[i][j], 1);
                Dec(a[j][i], 1);
            }
        }
    }
    bool flag = 0;
    for (int i = 1; i < n; ++i) {
        int p = i;
        while (p < n && !a[p][i]) ++p;
        if (p >= n) return 0;
        if (p > i) {
            swap(a[i], a[p]);
            flag ^= 1;
        }
        int iv = Inv(a[i][i]);
        for (int j = i + 1; j < n; ++j) {
            int tmp = Mul(a[j][i], iv);
            for (int k = i; k < n; ++k) {
                Dec(a[j][k], Mul(a[i][k], tmp));
            }
        }
    }
    int ret = 1;
    for (int i = 1; i < n; ++i) {
        ret = Mul(ret, a[i][i]);
    }
    if (flag) return Sub(0, ret);
    return ret;
}
```

模合数的情况

```c++
int Det() {
    bool flag = 0;
    for (int i = 1; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            while (a[j][i]) {
                int tmp = a[i][i] / a[j][i];
                for (int k = i; k < n; ++k) {
                    Dec(a[i][k], Mul(tmp, a[j][k]));
                    swap(a[i][k], a[j][k]);
                }
                flag ^= 1;
            }
        }
    }
    int ret = 1;
    for (int i = 1; i < n; ++i) {
        ret = Mul(ret, a[i][i]);
    }
    if (flag) ret = (MOD - ret) % MOD;
    return ret;
}
```

#### Best 定理

有向图的欧拉回路计数：先保证联通且每个点入度 = 出度

记 $T_x$ 表示以 $x$ 为根的内向生成树个数，则 $ec(G) = T_x*deg_x!*\prod_{u\neq x} (deg_u - 1)! $

内向树计数还是用 Matrix Tree 即可。

#### 循环矩阵行列式

设 $f(x) = a_1 + a_2x + a_3x^2 + ... + a_nx^{n-1}$，则 $|A| = \prod_{i=1}^{n}f(\omega_i)$，$\omega_i$ 是 $B(x) = x^n-1$ 的根。

这里我们使用一个叫 Resultant 的东西。设 $A(x)$ 是 $n$ 次多项式，$B(x)$ 是 $m$ 次多项式，$A$ 的根是 $\lambda_1...\lambda_n$，$B$ 的根是 $\mu_1...\mu_m$，则
$$
R(A,B) = b_m^n\prod_{i=1}^{m}A(\mu_i) = b_m^na_n^m\prod_{i=1}^{m}\prod_{j=1}^{n}(\mu_i-\lambda_j) \\= (-1)^{nm}a_n^mb_m^n\prod_{i=1}^{n}\prod_{j=1}^{m}(\lambda_i - \mu_j) = (-1)^{nm}a_n^m\prod_{i=1}^{n}B(\lambda_i) = (-1)^{nm}R(B,A)
$$
我们要求的就是 $R(A,B)$。从定义不难发现当 $R(A,B) = R(A-CB,B) * b_m^{deg(A) - deg(A-CB)}$，可以通过辗转相减递归求出 $R(A,B)$。

```c++
int Resultant(int n, int *a, int m, int *b) {
    int ret = 1;
    while (1) {
        if (n < m) {
            swap(n, m);
            for (int i = 0; i <= n; ++i) {
                swap(a[i], b[i]);
            }
            if ((n * m) & 1) {
                ret = (MOD - ret) % MOD;
            }
        }
        if (!m) return Mul(ret, Ksm(b[0], n));
        int tmp = Mul(a[n], Inv(b[m]));
        for (int i = m, j = n; i >= 0; --i, --j) {
            Dec(a[j], Mul(tmp, b[i]));
        }
        while (n >= 0 && !a[n]) {
            ret = Mul(ret, b[m]);
            --n;
        }
        if (n < 0) return 0;
    }
    return ret;
}
```

#### 线性基

QueryKth: 查询能被某个子集异或出来的第 k 小的数

```c++
struct LinearBase {
    LL base[70];

    void Insert(LL x) {
        for (int i = 62; i >= 0; --i) {
            if ((x >> i) & 1) {
                if (base[i]) {
                    x ^= base[i];
                } else {
                    base[i] = x;
                    return;
                }
            }
        }
    }

    LL QueryMax(LL x) {
        for (int i = 62; i >= 0; --i) {
            if ((x ^ base[i]) > x) {
                x ^= base[i];
            }
        }
        return x;
    }

    int cnt; LL base2[70];

    void Rebuild() {
        for (int i = 1; i <= 62; ++i) {
            for (int j = 0; j < i; ++j) {
                if ((base[i] >> j) & 1) {
                    base[i] ^= base[j];
                }
            }
        }
        cnt = 0;
        for (int i = 0; i <= 62; ++i) {
            if (base[i]) {
                base2[cnt++] = base[i];
            }
        }
    }

    LL QueryKth(LL k) {
        if (k >= (1LL << cnt)) return -1;
        LL ret = 0;
        for (int i = 0; i <= 62; ++i) {
            if ((k >> i) & 1) {
                ret ^= base2[i];
            }
        }
        return ret;
    }
}
```

支持删除的线性基：

对每个被插入过的数 $x$（无论是否插入成功）维护一个向量 $b_x$ 表示在插入结束后它由哪些原始的数异或组成。

删除 $x$ 时找一个包含 $x$ 的向量，优先找插入失败的数的向量，如果没有就尽量找线性基中较低位的向量。把这个向量异或到其它包含 $x$ 的向量中，这样就从整个线性基中把 $x$ 的所有影响都消除了，且尽量不影响高位。

如果我们被迫选了一个线性基中的向量，那么最后它自己也要清空，且线性基的秩要 -1。

```c++
void Insert(int i) {
    for (int j = s; j; --j) {
        if (!a[i][j]) continue;
        if (!base[j]) {
            base[j] = i;
            pos[i] = j;
            ++r;
            return;
        } else {
            a[i] ^= a[base[j]];
            b[i] ^= b[base[j]];
        }
    }
    pos[i] = 0;
}

int Erase(int i) {
    int mn = s + 1, x = 0;
    for (int j = 1; j <= p; ++j) {
        if (b[j][i] && mn > pos[j]) {
            mn = pos[j]; x = j;
        }    
    }
    for (int j = 1; j <= p; ++j) {
        if (b[j][i] && j != x) {
            a[j] ^= a[x];
            b[j] ^= b[x];
        }
    }
    if (pos[x]) {
        --r;
        base[pos[x]] = 0;
        pos[x] = 0;
    }
    a[x].reset();
    b[x].reset();
    return x;
}
```

## 其它

#### BM求递推式

```c++
int n, f[MAXN], fail[MAXN], delta[MAXN], cnt;
vector<int> bm[MAXN];

void BM() {
    for (int i = 1; i <= n; ++i) {
        int tmp = 0;
        for (int j = 0; j < bm[cnt].size(); ++j) {
            Inc(tmp, Mul(bm[cnt][j], f[i - j - 1]));
        }
        delta[i] = Sub(f[i], tmp);
        if (!delta[i]) continue;
        fail[cnt] = i;
        if (!cnt) {
            bm[++cnt].resize(i);
            continue;
        }
        int val = Mul(MOD - delta[i], Ksm(delta[fail[cnt - 1]], MOD - 2));
        ++cnt; bm[cnt].resize(i - fail[cnt - 2] - 1);
        bm[cnt].push_back(MOD - val);
        for (int j = 0; j < bm[cnt - 2].size(); ++j) {
            bm[cnt].push_back(Mul(val, bm[cnt - 2][j]));
        }
        if (bm[cnt].size() < bm[cnt - 1].size()) {
            bm[cnt].resize(bm[cnt - 1].size());
        }
        for (int j = 0; j < bm[cnt - 1].size(); ++j) {
            Inc(bm[cnt][j], bm[cnt - 1][j]);
        }
    }
    while (!bm[cnt].back()) bm[cnt].pop_back();
    printf("%d\n", bm[cnt].size());
    for (int i = 1; i <= bm[cnt].size(); ++i) {
        printf("%d, ", f[i]);
    }
    printf("\n");
    for (int i = 0; i < bm[cnt].size(); ++i) {
        printf("%d, ", bm[cnt][i]);
    }
}
```

#### 常系数齐次线性递推

```c++
void Conv(int *f, int *g) {
    mset(tmp, 0);
    for (int i = 0; i < k; ++i) {
        if (!f[i]) continue;
        for (int j = 0; j < k; ++j) {
            Inc(tmp[i + j], Mul(f[i], g[j]));
        }
    }
    for (int i = k + k; i >= k; --i) {
        if (!tmp[i]) continue;
        for (int j = 1; j <= k; ++j) {
            Inc(tmp[i - j], Mul(tmp[i], b[j]));
        }
    }
    for (int i = 0; i < k; ++i) {
        f[i] = tmp[i];
    }
}

int F(int n) {
    mset(x, 0); mset(res, 0);
    res[0] = x[1] = 1;
    for (; n; n >>= 1)
        if (n & 1) Conv(res, x);
        Conv(x, x);
    }
    int ret = 0;
    for (int i = 0; i < k; ++i) {
        Inc(ret, Mul(res[i], a[i]));
    }
    return ret;
}
```

多项式优化

```c++
poly X, A, D, DV;

poly Poly_Mod(poly &F) {
    int n = F.size(), m = D.size();
    if (n < m) return F;
    poly FR = F;
    reverse(FR.begin(), FR.end());
    poly Q = Poly_Mul2(FR, DV);
    Q.resize(n - m + 1);
    reverse(Q.begin(), Q.end());
    poly R = Poly_Mul2(D, Q);
    R.resize(m - 1);
    for (int i = 0; i < m - 1; ++i) {
        R[i] = Sub(F[i], R[i]);
    }
    return R;
}

void solve() {
    read(n); read(k);
    prework((k + 1) << 2); //注意这里的范围
    D.resize(k + 1);
    X.resize(k);
    A.resize(k);
    D[k] = 1;
    for (int i = 1, f; i <= k; ++i) {
        read(f);
        f = (f % MOD + MOD) % MOD;
        D[k - i] = MOD - f;
    }
    DV = D;
    reverse(DV.begin(), DV.end());
    DV.resize(k - 1);
    DV = Poly_Inv(k - 1, DV);
    A[0] = X[1] = 1;
    for (; n; n >>= 1) {
        if (n & 1) {
            poly tmp = Poly_Mul2(A, X);
            A = Poly_Mod(tmp);
        }
        poly tmp = Poly_Mul1(X);
        X = Poly_Mod(tmp);
    }
    int ans = 0;
    for (int i = 0, a; i < k; ++i) {
        read(a);
        a = (a % MOD + MOD) % MOD;
        Inc(ans, Mul(a, A[i]));
    }
    printf("%d\n", ans);
}
```

#### 单位根反演

单位根反演用于处理形如
$$
\sum_{i=0}^{n}[k|i]f(i) \pmod p
$$
的求和问题。前提是要满足 p 为质数，$p \equiv 1 \pmod k$ 

本质上就是要找一个表达式 $a(i)$，使得当 $i\%k=0$ 时 $a(i) = 1$，否则 $a(i) = 0$

p 的原根 g 满足只有 $g^0 = g^{p-1} = 1$，构造 $\omega_k = g^{\frac{p-1}{k}}$，那么只有 $i\%k = 0$ 时，$\omega^i \equiv 1 \pmod p$。由此构造 a
$$
a(i) = \sum_{j=0}^{k-1}\omega_k^{ij}
$$
显然 $i\%k = 0$ 时 $a(i) = k$， 其余 $a(i)=0$。这样我们所求的东西可以转化为
$$
\frac{1}{k}\sum_{i=0}^{n}a(i)f(i)
$$
一般的问题往往会在 f 中放一个组合数，先考虑最基础的形态
$$
\sum_{i=0}^{n/k}{n\choose i*k}=\sum_{i=0}^{n}{n\choose i}[k|i]= \frac{1}{k}\sum_{i=0}^{n}{n\choose i}\sum_{j=0}^{k-1}\omega_k^{ij} = \frac{1}{k}\sum_{j=0}^{k-1}\sum_{i=0}^{n}{n\choose i}\omega_k^{ij}= \frac{1}{k}\sum_{j=0}^{k-1}(1+w_k^j)^n
$$

#### 概率生成函数

求在一个初始为空的序列不断 append 随机字符直到出现给定的字符串的期望步数。

设 $F(x) = \sum_{i=0}^{\infty}f_ix^i$，$f_i$ 表示 $i$ 秒后结束的概率，$G(x) = \sum_{i=0}^{\infty}g_ix^i$，$g_i$ 表示 $i$ 秒后还没结束的概率。根据期望的定义可得答案为 $G(1)$。在一个没有结束的序列后面再加一位，可能结束可能没结束。
$$
F(x) + G(x) = 1+xG(x) \Rightarrow F'(1) = G(1)
$$
强行在一个没结束的序列后面 append 一个完整的给定字符串，则有可能提早结束当且仅当加了一个 border
$$
G(x) * (\dfrac{1}{m}x)^L = \sum_{i=1}^{L}[isborder(i)] F(x)(\dfrac{1}{m}x)^{L-i}
$$
求导可得 $G(1) = \sum_{i=1}^{L}[isborder(i)]m^i$



#### Continued Fractions

$r = [a_0;a_1,a_2,...a_k]$ if and only if $r = a_0 + \frac{1}{a_1+\frac{1}{...+\frac{1}{a_k}}}$

Only two ways to present $[a_0;a_1,a_2,...a_k]$ and $[a_0;a_1,a_2,...a_k - 1,1]$  **zero** is special case.



##### convergent

Define $r_i=[a_0;a_1,...a_i]=\frac{p_i}{q_i}$  the i-th convergent of r.

Then $r_0,r_1...$ are called the convergents of r.

$p_{-1} = 1,q_{-1}=0$

$p_{-2} = 0, q_{-2} = 1$

$\frac{p_k}{q_k}=\frac{a_kp_{k-1} + p_{k-2}}{a_kq_{k-1}+q_{k-2}}$



##### Deviations

$|\frac{p_k}{q_k}-r|\leq\frac{1}{q_kq_{k+1}}\leq\frac{1}{q_k^2}$



##### Lattice hulls

![image-20220717204425996](C:\Users\zhaoxiao\AppData\Roaming\Typora\typora-user-images\image-20220717204425996.png)

偶数是下凸包

奇数是上凸包

所以总数是$O(logn)$个



##### semi-convergent

$[a_0;a_1..,a_{k-1},t]$ 的形式被称作semi-convergent

$找分母绝对值<B的最接近一个实数x的分数$

convergents (p,q) 是 |p-xq|最小的

$可以证明这个值一定是一个semi-convergent$ 二分即可。





# 网络流

#### Hopcroft Karp

```++
int n1, n2, m, cx[MAXN], cy[MAXN], dx[MAXN], dy[MAXN], vis[MAXN], id[MAXN];
vector< pair<int, int> > e[MAXN];
int q[MAXN], head, tail;
 
int Match(int u) {
    for (auto edge : e[u]) {
        int v = edge.first;
        if (!vis[v] && dy[v] == dx[u] + 1) {
            vis[v] = 1;
            if (cy[v] == -1 || Match(cy[v])) {
                cx[u] = v;
                cy[v] = u;
                id[u] = edge.second;
                return 1;
            }
        }
    }
    return 0;
}
 
bool BFS() {
    mset(dx, 0);
    mset(dy, 0);
    head = 1; tail = 0;
    for (int i = 1; i <= n1; ++i) {
        if (cx[i] == -1) {
            q[++tail] = i;
            dx[i] = 1;
        }
    }
    bool flag = 0;
    while (head <= tail) {
        int u = q[head++];
        for (auto edge : e[u]) {
            int v = edge.first;
            if (dy[v] == 0) {
                dy[v] = dx[u] + 1;
                if (cy[v] == -1) {
                    flag = true;
                } else {
                    dx[cy[v]] = dy[v] + 1;
                    q[++tail] = cy[v];
                }
            }
        }
    }
    return flag;
}
 
void solve() {
    read(n1); read(n2); read(m);
    for (int i = 1, u, v; i <= m; ++i) {
        read(u); read(v);
        e[u].push_back(make_pair(v, i));
    }
    int ans = 0;
    mset(cx, -1);
    mset(cy, -1);
    while (BFS()) {
        mset(vis, 0);
        for (int i = 1; i <= n1; ++i) {
            if (cx[i] == -1) {
                ans += Match(i);
            }
        }
    }
}
```

#### 网络流 Dinic + 当前弧优化

```c++
struct MaxFlow {
    int n, S, T;
    int level[MAXN], q[MAXN], head, tail;

    struct Edge {
        int v, nxt; LL w;
    } e[MAXM << 1];
    int first[MAXN], used[MAXN], eCnt;

    inline void AddEdge(int u, int v, LL w) {
        e[++eCnt].v = v;
        e[eCnt].w = w;
        e[eCnt].nxt = first[u];
        first[u] = eCnt;
    }

    inline void Add(int u, int v, LL w) {
        AddEdge(u, v, w);
        AddEdge(v, u, 0);
    }

    void Init(int _n) {
        n = _n;
        for (int i = 1; i <= n; ++i) {
            first[i] = 0;
        }
        eCnt = 1;
    }

    bool BFS() {
        for (int i = 1; i <= n; ++i) {
            level[i] = -1;
            used[i] = first[i];
        }
        level[S] = 0;
        q[head = tail = 1] = S;
        while (head <= tail) {
            int u = q[head++];
            if (u == T) return 1;
            for (int i = first[u]; i; i = e[i].nxt) {
                int v = e[i].v;
                if (e[i].w && level[v] == -1) {
                    level[v] = level[u] + 1;
                    q[++tail] = v;
                }
            }
        }
        return 0;
    }

    LL DFS(int u, LL flow) {
        if (u == T) return flow;
        LL ret = 0;
        for (int i = used[u]; i; i = e[i].nxt, used[u] = i) {
            int v = e[i].v;
            if (e[i].w && level[v] == level[u] + 1) {
                LL tmp = DFS(v, min(flow, e[i].w));
                e[i].w -= tmp;
                e[i ^ 1].w += tmp;
                flow -= tmp;
                ret += tmp;
                if (!flow) break;
            }
        }
        if (!ret) level[u] = -1;
        return ret;
    }

    LL Dinic(int s, int t) {
        S = s; T = t;
        LL ret = 0;
        while (BFS()) {
            ret += DFS(S, INF);
        }
        return ret;
    }
} Flow;
```

#### 最小割树

```c++
struct GomoryHu {
    //网络流部分同上
 
    int Dinic(int s, int t) {
        S = s; T = t;
        for (int i = 2; i <= eCnt; i += 2) {
            e[i].w += e[i ^ 1].w;
            e[i ^ 1].w = 0;
        }
        int ret = 0;
        while (BFS()) {
            ret += DFS(S, INF);
        }
        return ret;
    }

 
    int p[maxn], p2[maxn];
    void Build(int l, int r) {
        if (l >= r) return;
        int cut = Dinic(p[l], p[l + 1]);
        e2[++m] = (Edge2){p[l], p[l + 1], cut};
        int tl = l - 1, tr = r + 1;
        for (int i = l; i <= r; ++i) {
            if (level[p[i]] != -1) {
                p2[++tl] = p[i];
            } else {
                p2[--tr] = p[i];
            }
        }
        for (int i = l; i <= r; ++i) {
            p[i] = p2[i];
        }
        Build(l, tl);
        Build(tr, r);
    }
 
    LL solve() {
        m = 0;
        for (int i = 1; i <= n; ++i) {
            p[i] = i;
        }
        Build(1, n);
        //
    }
} Flow;
```

#### 费用流 ZKW

```c++
struct CostFlow {
    int n, S, T;
    int dis[MAXN], inq[MAXN], vis[MAXN], tim;
    int maxflow, mincost;

    struct Edge {
        int v, w, c, nxt;
    } e[MAXM << 1];
    int first[MAXN], used[MAXN], eCnt;

    inline void AddEdge(int u, int v, int w, int c) {
        e[++eCnt].v = v;
        e[eCnt].w = w;
        e[eCnt].c = c;
        e[eCnt].nxt = first[u];
        first[u] = eCnt;
    }

    inline void Add(int u, int v, int w, int c) {
        AddEdge(u, v, w, c);
        AddEdge(v, u, 0, -c);
    }

    void Init(int _n) {
        n = _n;
        maxflow = mincost = 0;
        for (int i = 1; i <= n; ++i) {
            first[i] = 0;
        }
        eCnt = 1;
    }

    bool SPFA() {
        for (int i = 1; i <= n; ++i) {
            dis[i] = INF;
            used[i] = first[i];
        }
        dis[T] = 0;
        queue<int> q;
        q.push(T);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = 0;
            for (int i = first[u]; i; i = e[i].nxt) {
                int v = e[i].v;
                if (e[i ^ 1].w > 0 && dis[v] > dis[u] - e[i].c) {
                    dis[v] = dis[u] - e[i].c;
                    if (!inq[v]) {
                        q.push(v);
                        inq[v] = 1;
                    }
                }
            }
        }
        return (dis[S] < INF);
    }

    int DFS(int u, int flow) {
        if (u == T) return flow;
        vis[u] = tim;
        int ret = 0;
        for(int i = used[u]; i; i = e[i].nxt, used[u] = i) {
            int v = e[i].v;
            if(vis[v] != tim && e[i].w && dis[v] == dis[u] - e[i].c) {
                int tmp = DFS(v, min(flow, e[i].w));
                e[i].w -= tmp;
                e[i ^ 1].w += tmp;
                flow -= tmp;
                ret += tmp;
                if (!flow) break;
            }
        }
        return ret;
    }

    void ZKW(int s, int t) {
        S = s; T = t;
        while (SPFA()) {
            ++tim;
            int flow = DFS(S, INF);
            maxflow += flow;
            mincost += flow * dis[S];
        }
    }
} Flow;
```

#### 费用流 Dijkstra

```c++
struct CostFlow {
    int n, S, T;
    int inq[MAXN], pre[MAXN];
    LL h[MAXN], dis[MAXN];
    LL maxflow, mincost;

    //Edge, AddEdge, Add同上

    void Init(int _n) {
        n = _n;
        maxflow = mincost = 0;
        for (int i = 1; i <= n; ++i) {
            first[i] = 0;
        }
        eCnt = 1;
    }

    bool Dijkstra() {
        for (int i = 1; i <= n; ++i) {
            if (dis[i] < INF2) {
                h[i] += dis[i];
                dis[i] = INF2;
            }
            inq[i] = pre[i] = 0;
            used[i] = first[i];
        }
        priority_queue<pli, vector<pli>, greater<pli> > pq;
        dis[S] = 0;
        pq.push(make_pair(0, S));
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (inq[u]) continue;
            inq[u] = 1;
            for (int i = first[u]; i; i = e[i].nxt) {
                int v = e[i].v;
                if (e[i].w > 0 && dis[v] > dis[u] + e[i].c + h[u] - h[v]) {
                    dis[v] = dis[u] + e[i].c + h[u] - h[v];
                    pre[v] = i;
                    pq.push(make_pair(dis[v], v));
                }
            }
        }
        return (dis[T] < INF2);
    }

    void Augment() {
        int flow = INF;
        for (int i = pre[T]; i; i = pre[e[i ^ 1].v]) {
            flow = min(flow, e[i].w);
        }
        for (int i = pre[T]; i; i = pre[e[i ^ 1].v]) {
            e[i].w -= flow;
            e[i ^ 1].w += flow;
        }
        maxflow += flow;
        mincost += (dis[T] + h[T]) * flow;
    }

    void MCMF(int s, int t) {
        S = s; T = t;
        while (Dijkstra()) {
            Augment();
        }
    }
} Flow;
```



#### 费用流 ZKW + 原始对偶

```c++
struct CostFlow {
    int n, S, T;
    int h[MAXN], dis[MAXN], inq[MAXN], vis[MAXN], tim;
    int maxflow, mincost;

    //Edge, AddEdge, Add同上
    
    void Init(int _n) {
        n = _n;
        maxflow = mincost = 0;
        for (int i = 1; i <= n; ++i) {
            first[i] = 0;
        }
        eCnt = 1;
    }

    void SPFA() {
        for (int i = 1; i <= n; ++i) {
            dis[i] = INF;
            h[i] = 0;
        }
        deque<int> dq;
        dis[T] = 0;
        dq.push_back(T);
        while (!dq.empty()) {
            int u = dq.front();
            dq.pop_front();
            inq[u] = 0;
            for (int i = first[u]; i; i = e[i].nxt) {
                int v = e[i].v;
                if (e[i ^ 1].w > 0 && dis[v] > dis[u] - e[i].c) {
                    dis[v] = dis[u] - e[i].c;
                    if (!inq[v]) {
                        if (!dq.empty() && dis[v] < dis[dq.front()]) {
                            dq.push_front(v);
                        } else {
                            dq.push_back(v);
                        }
                        inq[v] = 1;
                    }
                }
            }
        }
    }

    bool Dijkstra() {
        for (int i = 1; i <= n; ++i) {
            if (dis[i] < INF) {
                h[i] += dis[i];
                dis[i] = INF;
            }
            inq[i] = 0;
            used[i] = first[i];
        }
        priority_queue<pii, vector<pii>, greater<pii> > pq;
        dis[T] = 0;
        pq.push(make_pair(0, T));
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (inq[u]) continue;
            inq[u] = 1;
            for (int i = first[u]; i; i = e[i].nxt) {
                int v = e[i].v;
                if (e[i ^ 1].w > 0 && dis[v] > dis[u] - e[i].c + h[u] - h[v]) {
                    dis[v] = dis[u] - e[i].c + h[u] - h[v];
                    pq.push(make_pair(dis[v], v));
                }
            }
        }
        return (dis[S] < INF);
    }

    int DFS(int u, int flow) {
        if (u == T) return flow;
        vis[u] = tim;
        int ret = 0;
        for(int i = used[u]; i; i = e[i].nxt, used[u] = i) {
            int v = e[i].v;
            if(vis[v] != tim && e[i].w && dis[v] == dis[u] - e[i].c + h[u] - h[v]) {
                int tmp = DFS(v, min(flow, e[i].w));
                e[i].w -= tmp;
                e[i ^ 1].w += tmp;
                flow -= tmp;
                ret += tmp;
                if (!flow) break;
            }
        }
        return ret;
    }

    void ZKW(int s, int t) {
        S = s; T = t;
        SPFA();
        while (Dijkstra()) {
            ++tim;
            int flow = DFS(S, INF);
            maxflow += flow;
            mincost += flow * (dis[S] + h[S]);
        }
    }
} Flow;
```

#### 上下界费用流 支持负权

```c++
struct BoundCostFlow {
    int n, S, T, in[MAXN];
    int dis[MAXN], inq[MAXN], pre[MAXN];
    int maxflow, mincost;

    struct Edge {
        int v, w, c, nxt;
    } e[MAXM << 1];
    int first[MAXN], eCnt;

    inline void AddEdge(int u, int v, int w, int c) {
        //printf("add %d %d %d %d\n", u, v, w, c);
        e[++eCnt].v = v;
        e[eCnt].w = w;
        e[eCnt].c = c;
        e[eCnt].nxt = first[u];
        first[u] = eCnt;
    }

    inline void Add(int u, int v, int w, int c) {
        if (c < 0) {
            mincost += w * c;
            in[u] -= w;
            in[v] += w;
            swap(u, v);
            c = -c;
        }
        AddEdge(u, v, w, c);
        AddEdge(v, u, 0, -c);
    }

    inline void Add2(int u, int v, int l, int r, int c) {
        Add(u, v, r - l, c);
        in[u] -= l;
        in[v] += l;
        mincost += l * c;
    }

    void Init(int _n) {
        n = _n + 2;
        maxflow = mincost = 0;
        for (int i = 1; i <= n; ++i) {
            first[i] = in[i] = 0;
        }
        eCnt = 1;
    }

    bool SPFA() {
        for (int i = 1; i <= n; ++i) {
            dis[i] = INF;
            pre[i] = 0;
        }
        dis[S] = 0;
        queue<int> q;
        q.push(S);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = 0;
            for (int i = first[u]; i; i = e[i].nxt) {
                int v = e[i].v;
                if (e[i].w > 0 && dis[v] > dis[u] + e[i].c) {
                    dis[v] = dis[u] + e[i].c;
                    pre[v] = i;
                    if (!inq[v]) {
                        q.push(v);
                        inq[v] = 1;
                    }
                }
            }
        }
        return (dis[T] < INF);
    }

    void Augment() {
        int flow = INF;
        for (int i = pre[T]; i; i = pre[e[i ^ 1].v]) {
            flow = min(flow, e[i].w);
        }
        for (int i = pre[T]; i; i = pre[e[i ^ 1].v]) {
            e[i].w -= flow;
            e[i ^ 1].w += flow;
        }
        maxflow += flow;
        mincost += flow * dis[T];
    }

    void MCMF(int s, int t) {
        S = s; T = t;
        while (SPFA()) {
            Augment();
        }
    }

    void BoundFlow(int _n, int s, int t) {
        int SS = _n + 1, TT = SS + 1;
        for (int i = 1; i <= _n; ++i) {
            if (in[i] > 0) {
                Add(SS, i, in[i], 0);
            } else if (in[i] < 0) {
                Add(i, TT, -in[i], 0);
            }
        }
        Add(t, s, INF, 0);
        MCMF(SS, TT);
    }
} Flow;
```

#### 上下界流建图方法

1、无源汇上下界可行流：原图每条边流量为上界 - 下界，设 $d_u$ 表示 $u$ 的入边下界和 - 出边下界和，$d_u > 0$ 则连边 $(S, u, d_u)$，否则连边 $(u, T, -d_u)$。

2、有源汇上下界可行流：额外连边 $(T, S, \infty)$ 即可。

3、有源汇上下界最大流：先求可行流，然后删掉 $(T,S,\infty)$ 跑一遍 $S\rightarrow T$ 的最大流，两次之和即答案。

4、有源汇上下界最小流：先不加 $(T,S,\infty)$ 跑最大流，再加上跑最大流，答案为这条边的实际流量。

5、有源汇上下界最小费用可行流：同有源汇上下界可行流，记得加上达到下界的费用。

如果有负权则转成循环流后会有负环。处理办法是把这条边拆成上下界都为容量的一条边（强制满流）和一条上界为容量的反向退流边。

#### 一些神秘的费用流

##### 流量平衡建图

例：志愿者招募

若干个区间，选一个区间覆盖一次代价 $c_i$，每个点至少被覆盖 $a_i$，求最小代价

连边 $(S,1,\infty)$，$(i,i+1,\infty - a_i)$，$(n,T,\infty - a_n)$，$(l_i, r_i+1,c_i)$，跑最小费用流。如果有解必定满流，且对每个 $i$ 都有至少 $a_i$ 的流量是从最后一种边过去的。

##### LP对偶费用流

例：防守战线

每个位置建一个塔有代价 $c_i$，有若干个区间，要满足每个区间至少有 $d_i$ 个塔，求最小代价。

对偶：$\min\{c^{\Tau}x | A^{\Tau}x \geq d\} = \max\{d^{\Tau}x | Ax \leq c\}$

问题转化为每个区间覆盖一次收益 di，每个位置被覆盖次数最多 ci，求最大收益

建图：设 $c_{n+1}=0$，若 $c_i > c_{i-1}$ 则连边 $(S,i, c_i-c_{i-1})$，否则连边 $(i, T, c_{i-1}-c_i)$，最后再连一条链 $(i,i+1,INF)$，此时源点和汇点的流量是恰好相同的。然后每个区间连边 $(l,r+1,d_i)$ 表示可以选一些区间覆盖并获得收益，如果出现某个位置被覆盖次数超出，则一定无法满流，所以这种情况必定不会出现。



## 奇技淫巧

#### O(nB) 背包

```
int n, x, a[MAXN];
int dp[MAXN], pre[MAXN];

int Knapsack(int C) {
    int W = a[n], s = 0, b = 0;
    while (b < n && s + a[b] <= C) {
        s += a[b++];
    }
    mset(dp, -1);
    dp[W + s - C] = b;
    for (int t = b; t <= n; ++t) {
        for (int u = W + 1; u <= W + W; ++u) {
            pre[u] = dp[u];
        }
        for (int u = W; u >= 1; --u) {
            cmax(dp[u + a[t]], dp[u]);
        }
        for (int u = W + W; u > W; --u) {
            for (int s = pre[u]; s < dp[u]; ++s) {
                cmax(dp[u - a[s]], s);
            }
        }
    }
    for (int u = W; u >= 1; --u) {
        if (dp[u] >= 0) {
            return C - W + u;
        }
    }
    return -1e9;
}
```

#### 杨表

选出 k 个上升子序列，长度和最大

```c++
struct Young {
    int a[105][MAXN];

    void Insert(int x, int y, int num) {
        if (x > 100) return;
        y = min(y, a[x][0]);
        while (y && a[x][y] > num) --y;
        ++y;
        if (a[x][y] == 0) {
            a[x][y] = num;
            ++a[x][0];
        } else {
            Insert(x + 1, y, a[x][y]);
            a[x][y] = num;
        }
    }

    int GetAns(int k) {
        int ret = 0;
        for (int i = 1; i <= k; ++i) {
            ret += a[i][0];
        }
        return ret;
    }
} YT;
```





## FastIO



```C++
#define getchar() (S==T&&(T=(S=BB)+fread(BB,1,1<<15,stdin),S==T)?EOF:*S++)
char BB[1<<15],*S=BB,*T=BB;;
int f;
int read()
{
    f=0; register char c; 
    while(c=getchar(),c<=47||c>=58);f=(f<<3)+(f<<1)+c-48;
    while(c=getchar(),c>=48&&c<=57) f=(f<<3)+(f<<1)+c-48;
    return f;
}
inline void write(int x) {
	if(x < 0) {putchar('-'); x = -x;}	
    if(x >= 10) write(x / 10);
	putchar(x % 10 + '0');
}
```





