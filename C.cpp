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
    
};


ull euclid_dist(const Vec& o1, const Vec& o2) {
    ull res = ((ull)(o1.x - o2.x))*((ull)(o1.x - o2.x));
    ull res2 = ((ull)(o1.y - o2.y))*((ull)(o1.y - o2.y));
    return res + res2;
}
 
pair<pair<Vec, Vec>, ull> go(const vector<Vec>& v, int start, int end) {
    if (end - start == 2){
        return {make_pair(v[start], v[start+1]), euclid_dist(v[start], v[start+1])};
    } 
    if (end - start == 3) {
        ull d01 = euclid_dist(v[start], v[start+1]);
        ull d12 = euclid_dist(v[start+1], v[start+2]);
        ull d02 = euclid_dist(v[start], v[start+2]);
        if (d01 < d12) {
            if (d01 < d02) {
                return {make_pair(v[start], v[start+1]), d01};
            } else {
                return {make_pair(v[start], v[start+2]), d02};
            }
        } else {
            if (d12 < d02) {
                return {make_pair(v[start+1], v[start+2]), d12};
            } else {
                return {make_pair(v[start], v[start+2]), d02};
            }
        }
    }
    
    int nums = end - start;
    int m = nums / 2;
    auto l = go(v, start, start + m); // l = ldist
    auto r = go(v, start + m, end);
 
    auto d = (l.second < r.second) ? l : r; // d = dist
    vector<Vec> tmp;
    for (int i = start; i < end; ++i) {
        if (llabs(v[i].x - v[start + m].x) <= pow(d.second, 0.5)) {
            tmp.push_back(v[i]);
        }
    }
    sort(tmp.begin(), tmp.end(), [](const Vec& o1, const Vec& o2) {
        if (o1.y == o2.y) return o1.x < o2.x;
        return o1.y < o2.y;
    });
    for (int i = 0; i < tmp.size()-1; ++i) {
        for (int j = 1; j <= 7; ++j) {
            if (i + j < tmp.size()) {
                ull tmp_dist = euclid_dist(tmp[i], tmp[i+j]);
                if (tmp_dist < d.second) {
                    d = make_pair(make_pair(tmp[i], tmp[i+j]), tmp_dist);
                }
            }
        }
    }
    return d;
}
 
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
 
    int n;
    cin >> n;
 
    vector<Vec>v(n);
 
    for (int i = 0; i < n; ++i) {
        ll x, y;
        cin >> x >> y;
        v[i] = Vec(x, y);
    }
 
    sort(begin(v), end(v), [](const Vec& o1, const Vec& o2) {
        if (o1.x == o2.x) return o1.y < o2.y;
        return o1.x < o2.x;
    });
 
    pair<pair<Vec, Vec>, ull> p = go(v, 0, v.size());
    cout << p.first.first.x << " " << p.first.first.y << '\n';
    cout << p.first.second.x << " " << p.first.second.y << '\n';
    
    return 0;
}