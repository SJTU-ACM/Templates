// given first m a[i] and coef c[i] (0-based),
// calc a[n] mod p in O(m*m*log(n)).
// a[n] = sum(c[m-i]*a[n-i]), i = 1...m
// i.e. a[m] = sum(c[i]*a[i]), i = 0...m-1
int linear_recurrence(LL n, int m, int a[], int c[], int p) {
	LL v[M] = {1 % p}, u[M<<1], msk = !!n;
	for(LL i = n; i > 1; i >>= 1) msk <<= 1;
	for(LL x = 0; msk; msk >>= 1, x <<= 1) {
		fill_n(u, m<<1, 0);
		int b = !!(n & msk); x |= b;
		if(x < m) u[x] = 1 % p;
		else {
			for(int i = 0; i < m; ++i)
				for(int j = 0, t = i+b; j < m; ++j, ++t)
					u[t] = (u[t]+v[i]*v[j]) % p;
			for(int i = (m<<1)-1; i >= m; --i)
				for(int j = 0, t = i-m; j < m; ++j, ++t)
					u[t] = (u[t]+c[j]*u[i]) % p;
		}
		copy(u, u+m, v);
	}
	int an = 0;
	for(int i = 0; i < m; ++i) an = (an+v[i]*a[i]) % p;
	return an;
}
