#include <bits/stdc++.h>

using ll = long long;
using ull = unsigned long long;
using namespace std;

class Vec {
public:
    ll x;
    ll y;
    Vec() = default;
    Vec(ll x, ll y): x(x), y(y) {}
    double vec_len() {
        return pow(x*x + y*y, 0.5);
    }
};
 
ll cross_prod(const Vec& o1, const Vec& o2) {
    return (o1.x*o2.y - o1.y*o2.x);
}
 
ll scalar_prod(const Vec& o1, const Vec& o2) {
    return (o1.x*o2.x + o1.y*o2.y);
}
 
Vec make_vec(const Vec& start, const Vec& end) {
    return Vec(end.x - start.x, end.y - start.y);
}

bool point_in_seg(const Vec& tmp, const Vec& p1, const Vec& p2) {
    ll cross = cross_prod(make_vec(p1, tmp), make_vec(p2, tmp));
    ll scal = scalar_prod(make_vec(p1, tmp), make_vec(p2, tmp));
    return scal <= 0 && cross == 0;
}

bool p_in_bound(const Vec& p, vector<Vec>& v) {
    for (int i = 1; i < v.size(); ++i) {
        if (point_in_seg(p, v[i-1], v[i])) return true;
    }
    return false;
}

const double PI = 3.14159265358979323846;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
 
    int n, xa, ya;
    cin >> n >> xa >> ya;
 
    vector<Vec>v(n);
    for (int i = 0; i < n; ++i) {
        ll x, y;
        cin >> x >> y;
        v[i] = Vec(x, y);
    }
    v.push_back(Vec(v[0].x, v[0].y));
 
    Vec p(xa, ya);
        
    if (p_in_bound(p, v)) {
        cout << "YES\n";
    } else {
        double phi = 0;
        for (int i = 1; i < v.size(); ++i) {
            phi += atan2(cross_prod(make_vec(v[i-1], p), make_vec(v[i], p)),
                        scalar_prod(make_vec(v[i-1], p), make_vec(v[i], p)));
        }
        if (fabs(phi) < PI) {
            cout << "NO\n";
        } else {
            cout << "YES\n";
        }
    }
    
    return 0;
}