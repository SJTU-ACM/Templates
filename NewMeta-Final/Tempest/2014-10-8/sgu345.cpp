#include<cstring>
#include<cstdio>
#include<algorithm>
#include<cmath>
#include<cassert>
using namespace std;
int n;
double eps(1e-8);
int sign(const double & x) {
	return (x > eps) - (x + eps < 0);
}
struct Point {
	double x, y;
	void scan() {
		scanf("%lf%lf", &x, &y);
	}
	void print() {
		printf("%lf %lf\n", x, y);
	}
	Point() {
	}
	Point(const double & x, const double & y) : x(x), y(y) {
	}
};
Point operator + (const Point & a, const Point & b) {
	return Point(a.x + b.x, a.y + b.y);
}
Point operator - (const Point & a, const Point & b) {
	return Point(a.x - b.x, a.y - b.y);
}
Point operator * (const double & a, const Point & b) {
	return Point(a * b.x, a * b.y);
}
double operator * (const Point & a, const Point & b) {
	return a.x * b.y - a.y * b.x;
}
bool isUpper(const Point & a) {
	return sign(a.x) < 0 or sign(a.x) == 0 and sign(a.y > 0);
}
Point crs(const Point & as, const Point & at, const Point & bs, const Point & bt) {
	if(sign((at - as) * (bt - bs)) == 0) {
		return bs;
	}
	double lambda((bs - as) * (bt - bs) / ((at - as) * (bt - bs)));
	return as + lambda * (at - as);
}
struct reca {
	Point a[50000];
	double s[50000];
	Point & operator [] (int x) {
		assert(x % n < 50000);
		return a[x % n];
	}
	void init() {
		s[0] = a[0] * a[1];
		for(int i(1); i < n; i++) {
			s[i] = s[i - 1] + a[i] * (i == n - 1?a[0]:a[i + 1]);
		}
	}

	double getS(int le, int ri) {
		if(le > ri)
			return 0;
		le %= n;
		ri %= n;
		if(le <= ri) {
			return s[ri] - (le?s[le - 1]:0);
		}else {
			return getS(le, n - 1) + getS(0, ri);
		}
	}
} a;

int lowerBound(int le, int ri, const Point & dir) {
	while(le < ri) {
		int mid((le + ri) / 2);
		if(sign((a[mid + 1] - a[mid]) * dir) >= 0) {
			le = mid + 1;
		}else {
			ri = mid;
		}
	}
	return le;
}
int boundLower(int le, int ri, const Point & s, const Point & t) {
	while(le < ri) {
		int mid((le + ri + 1) / 2);
		if(sign((a[mid] - s) * (t - s)) >= 0) {
			le = mid;
		}else {
			ri = mid - 1;
		}
	}
	return le;
}
bool check(const Point & a, const Point & b, const Point & c, const Point & d) {
	return sign((a - c) * (d - c)) * sign((b - c) * (d - c)) <= 0;
}
bool f[55555];
int main() {
	scanf("%d", &n);
	for(int i(0); i < n; i++) {
		//printf("%d\n", n);
		a[i].scan();
		//return 0;
	}
	//return 0;
	for(int i(0); i < n; i++) {
		int d(sign((a[i + 1] - a[i]) * (a[i + 2] - a[i + 1])));
		if(d) {
			if(d < 0) {
				reverse(a.a, a.a + n);
			}
			break;
		}
	}
	for(int i(0); i < n; i++) {
		if(!sign(a[i].x - a[i + 1].x) and !sign(a[i].y - a[i + 1].y)) {
			f[i] = false;
		}else {
			f[i] = true;
		}
	}
	int n1(0);
	for(int i(0); i < n; i++) {
		if(f[i]) {
			a[n1++] = a[i];
		}
	}
	n = n1;
	//??????a??????????????????????????????
	a.init();
	int i1, j1;
	for(int i(0); i < n; i++) {
		if(isUpper(a[i + 1] - a[i])) {
			for(int j(i + 1); j != i; ++j %= n) {
				if(!isUpper(a[j + 1] - a[j])) {
					i1 = i; j1 = j;
					break;
				}
			}
			break;
		}
	}
	if(i1 > j1) {
		j1 += n;
	}
	int m;
	scanf("%d", &m);
	for(int i(0); i < m; i++) {
		Point s, t;
		s.scan(); t.scan();
		if(!isUpper(t - s)) {
			swap(t, s);
		}
		int i3(lowerBound(i1, j1, t - s));
		int j3(lowerBound(j1, i1 + n, s - t));
		int i4(boundLower(i3, j3, s, t));
		int j4(boundLower(j3, i3 + n, t, s));
		if(check(a[i4], a[i4 + 1], s, t)) {
			Point p1(crs(s, t, a[i4], a[i4 + 1]));
			Point p2(crs(s, t, a[j4], a[j4 + 1]));
			if(sign(p1.x - p2.x) or sign(p1.y - p2.y)) {
				assert(i4 % n != j4 % n);
				double area1(p1 * a[i4 + 1] + a.getS(i4 + 1, j4 - 1) + a[j4] * p2 + p2 * p1);
				double area2(p2 * a[j4 + 1] + a.getS(j4 + 1, i4 + n - 1) + a[i4] * p1 + p1 * p2);
				printf("%.6f\n", min(fabs(area1), fabs(area2)) / 2);
			}else {
				printf("0.000000\n");
			}
		}else {
			printf("0.000000\n");
		}
	}
}
