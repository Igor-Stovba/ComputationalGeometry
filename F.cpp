#include <bits/stdc++.h>
#include <complex>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
#define X real()
#define Y imag()
using P = complex<long long>;

ll cross_product(const P& p1, const P& p2) {
    return p1.X*p2.Y - p1.Y*p2.X;
}
 
bool make_left_turn(const P& p1, const P& p2, const P& p3) {
    return cross_product(p2 - p1, p3 - p2) > 0;
}
 
bool make_right_turn(const P& p1, const P& p2, const P& p3) {
    return cross_product(p2 - p1, p3 - p2) <= 0;
}
 
pair<P, P> min_max_angles(const vector<P>& v, const P& start) {
    P min_angle = v[1];
    P max_angle = v[1];
 
    for (int i = 2; i < v.size(); ++i) {
        ll cross1 = cross_product(v[i] - start, min_angle - start);
        ll cross2 = cross_product(v[i] - start, max_angle - start);
        if (cross1 > 0) {
            min_angle = v[i];
        }
        if (cross2 < 0) {
            max_angle = v[i];
        }
    }
 
    return make_pair(min_angle, max_angle);
}
 
ll len_square(const P& p) {
    return p.X*p.X + p.Y*p.Y;
}
 
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // freopen("input.txt", "r", stdin);
 
    int n, x;
    cin >> n;
    vector<P> v(n);
    vector<P> hull;
    for (int i = 0; i < n; ++i) {
        ll x,y;
        cin >> x >> y;
        v[i].real(x);
        v[i].imag(y);
    }
    
    for (int i = 1; i < n; ++i) {
        if (v[i].X < v[0].X) {
            swap(v[i], v[0]);
        } else if (v[i].X == v[0].X && v[i].Y < v[0].Y) {
            swap(v[i], v[0]);
        }
    }
    P start = v[0];
    pair<P, P> tmp_pair = min_max_angles(v, start);
    P min_angle = tmp_pair.first;
    P max_angle = tmp_pair.second;
 
    sort(v.begin()+1, v.end(), [&](const P& p1, const P& p2) {
        P vec1 = p1 - start;
        P vec2 = p2 - start;
        ll cross = cross_product(vec1, vec2);
        if (cross == 0) {
            ll cross_min = cross_product(min_angle - start, vec1);
            ll cross_max = cross_product(max_angle - start, vec1);
            if (cross_min == 0) {
                return len_square(vec1) < len_square(vec2);
            } else if (cross_max == 0) {
                return len_square(vec1) > len_square(vec2);
            }
            return len_square(vec1) < len_square(vec2); // random here is possible
        } else {
            return cross > 0;
        }
    });
    v.push_back(v[0]);
 
    hull.push_back(v[0]);
    hull.push_back(v[1]);
 
    for (int i = 2; i < n+1; ++i) {
        while (hull.size() >= 2 && make_right_turn(
            hull[hull.size()-2],
            hull[hull.size()-1],
            v[i]
        )) {
            hull.pop_back();
        }
        hull.push_back(v[i]);
    }

    hull.pop_back();
 
    cout << hull.size() << endl;
    for (int i = 0; i < hull.size(); ++i) {
        cout << hull[i].X << " " << hull[i].Y << endl;
    }
    
    ull res = 0;
    for (int i = 2; i < hull.size(); ++i) {
        res += std::abs(cross_product(hull[i-1] - hull[0], hull[i]-hull[0]));
    }
    cout << (res/2);
    if (res % 2 == 1) {
        cout << ".5\n";
    }
    return 0;
}