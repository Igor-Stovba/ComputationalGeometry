#include <bits/stdc++.h>
#include <complex>
#include <tgmath.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using P = complex<long long>;
#define X real()
#define Y imag()

 template<typename A, typename B>
string to_string(pair<A,B> p) {
    return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";
}
 
template<typename A, typename B, typename C>
string to_string(tuple<A,B,C> p) {
    return "(" + to_string(get<0>(p)) + ", " + to_string(get<1>(p)) + ", " + to_string(get<2>(p)) + ")";
}
 
template<typename A, typename B, typename C, typename D>
string to_string(tuple<A,B,C,D> p) {
    return "(" + to_string(get<0>(p)) + ", " + to_string(get<1>(p)) + ", " + to_string(get<2>(p)) + ", " + to_string(get<3>(p)) + ")";
}
 
string to_string(P p) {
    return "<" + to_string(p.real()) + " ^ " + to_string(p.imag()) + "i>";
}
 
string to_string(const string& s) {
    return '"' + s + '"';
}
 
string to_string(const char* s) {
    return to_string((string) s);
}
 
string to_string(bool b) {
    return (b ? "true" : "false");
}
 
string to_string(vector<bool> v) {
    bool first = true;
    string res = "{";
    for (int i = 0; i < static_cast<int>(v.size()); i++) {
        if (!first) {
            res += ", ";
        }
        first = false;
        res += to_string(v[i]);
    }
    res += "}";
    return res;
}
 
template <size_t N>
string to_string(bitset<N> v) {
    string res = "";
    for (size_t i = 0; i < N; i++) {
        res += static_cast<char>('0' + v[i]);
    }
    return res;
}
 
template <typename A>
string to_string(A v) {
    bool first = true;
    string res = "{";
    for (const auto& x: v) {
        if (!first) {
            res += ", ";
        }
        first = false;
        res += to_string(x);
    }
    res += "}";
    return res;
}
 
void debug_out() {cerr << endl;}
 
template<typename Head, typename... Tail>
void debug_out(Head H, Tail... T) {
    cerr << " " << to_string(H);
    debug_out(T...);
}
 
#define LOCAL
 
#ifdef LOCAL
#define debug(...) cerr << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
#else
#define debug(...) 42
#endif

ll cross_product(const P& p1, const P& p2) {
    return p1.X*p2.Y - p1.Y*p2.X;
}
 
bool make_left_turn(const P& p1, const P& p2, const P& p3) {
    return cross_product(p2 - p1, p3 - p2) > 0;
}
 
bool make_right_turn(const P& p1, const P& p2, const P& p3) {
    return cross_product(p2 - p1, p3 - p2) < 0;
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
 
ull len_square(const P& p) {
    ull x_ = std::abs(p.X);
    ull y_ = std::abs(p.Y);
    return (ull)x_*x_ + (ull)y_*y_;
}

ld dist(const P& p1, const P& p2) {
    P tmp = p2 - p1;
    ull x_ = std::abs(tmp.X);
    ull y_ = std::abs(tmp.Y);
    return std::sqrt((ull)x_*x_ + (ull)y_*y_);
}

ull sdist(const P& p1, const P& p2) {
    P tmp = p2 - p1;
    ull x_ = std::abs(tmp.X);
    ull y_ = std::abs(tmp.Y);
    return (ull)x_*x_ + (ull)y_*y_;
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
    cout.precision(15);

    if (n == 1) {
        cout << fixed << 0.0 << std::endl;
        return 0;
    }
    if (n == 2) {
        std::cout << fixed << dist(v[0], v[1]) << std::endl;
        return 0;
    }
    
    for (int i = 1; i < n; ++i) {
        if ((v[i].X < v[0].X) || (v[i].X == v[0].X && v[i].Y < v[0].Y)) 
            swap(v[i], v[0]);
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
        while (hull.size() >= 2 && !make_left_turn(
            hull[hull.size()-2],
            hull[hull.size()-1],
            v[i]
        )) {
            hull.pop_back();
        }
        hull.push_back(v[i]);
    }
    hull.pop_back();


    // find start positions of calipers
    int i_ = 0;
    int j_ = 0;
    for (int k = 0; k < hull.size(); ++k) {
        if (hull[k].X < hull[i_].X || (hull[k].X == hull[i_].X && hull[k].Y < hull[i_].Y)) i_ = k;
        if (hull[k].X > hull[j_].X || (hull[k].X == hull[j_].X && hull[k].Y < hull[j_].Y)) j_ = k;
    }

    auto f = [&hull](int v) {return v % hull.size();};
    ull d = sdist(hull[f(i_)], hull[f(j_)]);;
    // debug(hull);
    // debug(i_, j_);

    int hs = hull.size();
    int i, j;
    for (i = i_, j = j_; i < (i_ + hs);) {
        // debug(i, j);
        if (cross_product(hull[f(i+1)] - hull[f(i)], hull[f(j+1)] - hull[f(j)]) >= 0) {
            ++j;
        } else {
            ++i;
        }
        d = std::max(d, sdist(hull[f(i)], hull[f(j)]));
    }
    d = std::max(d, sdist(hull[f(i)], hull[f(j)]));

    cout << fixed << std::sqrt(static_cast<ld>(d)) << std::endl;

    return 0;
}