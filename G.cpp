#include <bits/stdc++.h>
#include <math.h>

using ll = long long;
using ull = unsigned long long;
using ld = long double;
using namespace std;

using P = complex<long long>;
#define X real()
#define Y imag()

 template<typename A, typename B>
string to_string(pair<A,B> p) {
    return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";
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
ld dist(const P& p1, const P& p2) {
    P tmp = p2 - p1;
    ull x_ = std::abs(tmp.X);
    ull y_ = std::abs(tmp.Y);
    return std::sqrt((ull)x_*x_ + (ull)y_*y_);
}

ll cross_prod(const P& p1, const P& p2) {
    return p1.X*p2.Y - p1.Y*p2.X;
}

ll scalar_prod(const P& o1, const P& o2) {
    return (o1.X*o2.X + o1.Y*o2.Y);
}

vector<P> get_sum_mink(const vector<P>& p1, const vector<P>& p2) {
    // find p1 + p2
    vector<P> result;
    int j = 0, i = 0;
    while (i < p1.size() - 2 || j < p2.size() - 2) {
        result.push_back(p1[i] + p2[j]);
        ll cross = cross_prod(p1[i+1] - p1[i], p2[j+1] - p2[j]);
        if (cross >= 0 && i < p1.size() - 2)
            ++i;
        if (cross <= 0 && j < p2.size() - 2)
            ++j;
    }

    return result;
}

bool point_in_seg(const P& tmp, const P& p1, const P& p2) {
    ll cross = cross_prod(tmp - p1, tmp - p2);
    ll scal = scalar_prod(tmp - p1, tmp - p2);
    return scal <= 0 && cross == 0;
}

bool p_in_bound(const P& p, const vector<P>& v) {
    for (int i = 1; i < v.size(); ++i) {
        if (point_in_seg(p, v[i-1], v[i])) return true;
    }
    return false;
}

bool contains_zero(const vector<P>& v) {
    P zero = {0, 0};
    if (p_in_bound(zero, v)) {
        return true;
    } else {
        double phi = 0;
        for (int i = 1; i < v.size(); ++i) {
            phi += atan2(cross_prod(zero - v[i-1], zero - v[i]),
                        scalar_prod(zero - v[i-1], zero - v[i]));
        }
        return fabs(phi) > M_PI;
    }
}

struct Segment {
    ll x1, y1, x2, y2;

    Segment(ll x1, ll y1, ll x2, ll y2): x1(x1), y1(y1), x2(x2), y2(y2) {}
    Segment(const P & p1, const P & p2): x1(p1.X), y1(p1.Y), x2(p2.X), y2(p2.Y) {}
};

ld point_to_segment(const P & p, const Segment & s) { // Checked
    P l_seg = {s.x1, s.y1};
    P r_seg = {s.x2, s.y2};

    auto len_p = [](const P& t) -> ld {
        return std::sqrt(static_cast<ld>(t.X*t.X + t.Y*t.Y));
    };
    if (scalar_prod(p - l_seg, r_seg - l_seg) >= 0 && scalar_prod(p - r_seg, l_seg - r_seg) >= 0) {
        return ((ld)std::abs(cross_prod(p - l_seg, r_seg - l_seg)) / len_p(r_seg - l_seg));
    } else {
        return std::min(len_p(p - l_seg), len_p(p - r_seg));
    }
}

ld closest_to_zero(const vector<P>& v) {
    P zero = {0, 0};
    ld d = std::numeric_limits<ld>::max();
    for (size_t i = 1; i < v.size(); ++i) 
        d = std::min(d, point_to_segment(zero, Segment(v[i], v[i-1])));
    return d;
}

int main() {
    std::cout.precision(15);
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // freopen("input.txt", "r", stdin);

    int sz1, sz2;
    cin >> sz1;
    vector<P> p1(sz1);

    for (size_t i = 0; i < sz1; ++i) {
        ll x,y;
        cin >> x >> y;
        p1[i].real(x);
        p1[i].imag(y);
    }

    cin >> sz2;
    vector<P> p2(sz2);

    for (size_t i = 0; i < sz2; ++i) {
        ll x,y;
        cin >> x >> y;
        p2[i].real(x);
        p2[i].imag(y);
    }
    // Warning n >= 1!!!
    if (sz1 == 1 && sz2 == 1) {
        std::cout << std::fixed << dist(p1[0], p2[0]) << std::endl;
        return 0;
    }


    for (size_t i = 0; i < p2.size(); ++i)
        p2[i] *= -1;

    // counter clockwise reordering
    std::reverse(p1.begin(), p1.end());
    std::reverse(p2.begin(), p2.end());

    // the first element in every array is the mostleft
    int i_ = 0;
    int j_ = 0;

    // the mostleft point in p1
    for (int i = 1; i < p1.size(); ++i) 
        if (p1[i].X < p1[i_].X || (p1[i].X == p1[i_].X && p1[i].Y < p1[i_].Y)) i_ = i;
    
    // the mostleft point in p2
    for (int i = 1; i < p2.size(); ++i) 
        if (p2[i].X < p2[j_].X || (p2[i].X == p2[j_].X && p2[i].Y < p2[j_].Y)) j_ = i;
    
    std::rotate(p1.begin(), p1.begin() + i_, p1.end());
    std::rotate(p2.begin(), p2.begin() + j_, p2.end());
    p1.push_back(p1[0]);
    p1.push_back(p1[1]);

    p2.push_back(p2[0]);
    p2.push_back(p2[1]);

    vector<P> sub_mink = get_sum_mink(p1, p2);
    // debug(sub_mink);
    sub_mink.push_back(sub_mink[0]);
    if (contains_zero(sub_mink)) {
        std::cout << std::fixed << 0.0 << std::endl;
    } else {
        std::cout << std::fixed << closest_to_zero(sub_mink) << std::endl;
    }

    return 0;
}