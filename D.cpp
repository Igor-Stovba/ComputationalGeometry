
#include <bits/stdc++.h>
#include <complex>
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

ull sdist(const P& p1, const P& p2) {
    P tmp = p2 - p1;
    ull x_ = std::abs(tmp.X);
    ull y_ = std::abs(tmp.Y);
    return x_*x_ + y_*y_;
}

ull sdist(const P& p) {
    ll x_ = p.X;
    ll y_ = p.Y;
    return static_cast<ull>(x_) * x_ + static_cast<ull>(y_) * y_;
}

int n;
vector<P> hull, tmp;
P start;

void before() {
    int ind = 0;
    for (int i = 0; i < tmp.size(); ++i) {
        if ((tmp[i].X  < tmp[ind].X) || (tmp[i].X == tmp[ind].X && tmp[i].Y < tmp[ind].Y)) {
            ind = i;
        }
    }
    std::rotate(tmp.begin(), tmp.begin() + ind, tmp.end());
    n--;
    for (int i = 0; i < n; ++i) {
        hull[i] = tmp[i+1] - tmp[0];
    }
    start = tmp[0];
}

bool point_in_triangle(const P& a, const P& b, const P& p) {
    P c;
    c.imag(0);
    c.real(0);
    ull s1 = std::abs(cross_product(b - a, c - a));
    ull s2 = std::abs(cross_product(c - p, a - p)) + 
            std::abs(cross_product(c - p, b - p)) + 
            std::abs(cross_product(a - p, b - p));
    return s1 == s2;
}

bool contains(P& p) {
    auto sign = [](ll t) {
        return t > 0 ? 1 : (t == 0 ? 0 : -1); 
    };

    p -= start;
    ll cross_right = cross_product(p, hull[0]);
    ll cross_left = cross_product(p, hull[n-1]);
    if (cross_left != 0 && (sign(cross_product(hull[n-1], p)) != sign(cross_product(hull[n-1], hull[0])))) return false;
    if (cross_right != 0 && (sign(cross_product(hull[0], p)) != sign(cross_product(hull[0], hull[n-1])))) return false;

    if (cross_right == 0) {
        return sdist(p) <= sdist(hull[0]); 
    }

    int l = 0;
    int r = n - 1;

    while (r - l > 1) {
        int m = (l + r) / 2;
        if (cross_product(hull[m], p) >= 0) {
            l = m;
        } else {
            r = m;
        }
    }
    // determine if point in triangle

    return point_in_triangle(hull[l], hull[l+1], p);
}
 
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // freopen("input.txt", "r", stdin);
 
    int m, k;
    cin >> n >> m >> k;
    hull.resize(n-1);
    tmp.reserve(n);
    for (int i = 0; i < n; ++i) {
        ll x,y;
        cin >> x >> y;
        tmp.emplace_back(x, y);
    }

    before();

    int x_, y_;
    int counter = 0;
    for (int i = 0; i < m; ++i) {
        P point;
        cin >> x_ >> y_;
        point.real(x_);
        point.imag(y_);
        if (contains(point)) {
            counter++;
        }
        // debug(i, counter);
    }
    // debug(counter);
    if (counter >= k) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
    

    return 0;
}