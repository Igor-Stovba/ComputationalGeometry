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
string to_string(pair<A,B> p) {return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";}
string to_string(P p) {return "<" + to_string(p.real()) + " ^ " + to_string(p.imag()) + "i>";}
string to_string(const string& s) {return '"' + s + '"';}
string to_string(const char* s) {return to_string((string) s);}
string to_string(bool b) {return (b ? "true" : "false");}
 
template <typename A>
string to_string(A v) {
    bool first = true;
    string res = "{";
    for (const auto& x: v) {
        if (!first)
            res += ", ";
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
int n;
struct T {
    int ind;
    P p;
};

ll cross_prod(const P& p1, const P& p2) {return p1.X*p2.Y - p1.Y*p2.X;}
ll scalar_prod(const P& o1, const P& o2) {return (o1.X*o2.X + o1.Y*o2.Y);}
int sign(ll v) {return (v < 0) ? -1 : (v == 0 ? 0 : 1);}

bool Left(const P& a, const P& b, const P& c) {
    return cross_prod(b - a, c - a) > 0;
}

bool LeftOn(const P& a, const P& b, const P& c) {
    return cross_prod(b - a, c - a) >= 0;
}

bool Collinear(const P& a, const P& b, const P& c) {
    return cross_prod(b - a, c - a) == 0;
}

ll signed_area(vector<T>& v) {
    v.push_back(v[0]);
    ll result = 0;
    for (int i = 0; i < v.size()-1; ++i) 
        result += (v[i].p.Y + v[i+1].p.Y)*(v[i].p.X - v[i+1].p.X);
    v.pop_back();
    return result;
}

void ensure_counter_clockwise(vector<T>& v) {
    ll area = signed_area(v);
    if (area < 0)
        std::reverse(v.begin(), v.end());
}

bool Between(const P& a, const P& b, const P& c) {
    if (!Collinear(a, b, c))
        return false;
    
    if (a.X != b.X) {
        return ((a.X <= c.X) && (c.X <= b.X)) ||
         ((a.X >= c.X) && (c.X >= b.X));
    } else {
        return ((a.Y <= c.Y) && (c.Y <= b.Y)) ||
         ((a.Y >= c.Y) && (c.Y >= b.Y));
    }
}

bool intersectProp(const P& a, const P& b, const P& c, const P& d) {
    if (Collinear(a, b, c) ||
        Collinear(a, b, d) || 
        Collinear(c, d, a) ||
        Collinear(c, d, b)) {
            return false;
        }
    auto xorl = [](bool t1, bool t2) {return (!t1 ^ !t2);};
    return xorl(Left(a, b, c), Left(a, b, d)) && xorl(Left(c, d, a), Left(c, d, b));
}

bool intersect(const P& a, const P& b, const P& c, const P& d)
{
    if (intersectProp(a, b, c, d)) {
        return true;
    } else if (Between(a, b, c) ||
               Between(a, b, d) || 
               Between(c, d, a) || 
               Between(c, d, b)) {
        return true;
    } 
    return false;
}

struct Vertex {
    int index;
    P coord;
    bool ear = false;
    Vertex* next = nullptr;
    Vertex* prev = nullptr;

    Vertex(int index, const P& coord): index(index), coord(coord) {}
};

Vertex* head = nullptr;

bool Diagonalie(Vertex* a, Vertex* b) {
    Vertex *c = head;
    Vertex *c1;

    do {
        c1 = c->next;
        if (c != a && c != b && c1 != a && c1 != b &&
            intersect(a->coord, b->coord, c->coord, c1->coord)) {
            return false;
        }
        c = c->next;
    } while (c != head);
    return true;
}

bool inCone(Vertex* a, Vertex* b) {
    Vertex* a0;
    Vertex* a1;
    
    a1 = a->next;
    a0 = a->prev;
    if (LeftOn(a->coord, a1->coord, a0->coord)) 
        return Left(a->coord, b->coord, a0->coord) && Left(b->coord, a->coord, a1->coord);
    
    return !(LeftOn(a->coord, b->coord, a1->coord) && LeftOn(b->coord, a->coord, a0->coord));
}

bool Diagonal(Vertex* a, Vertex* b) {
    return inCone(a, b) && inCone(b, a) && Diagonalie(a, b);
}

void earInit() {
    Vertex* v0;
    Vertex* v1 = head;
    Vertex* v2;

    do {
        v0 = v1->prev;
        v2 = v1->next;
        v1->ear = Diagonal(v0, v2);
        v1 = v1->next;
    } while (v1 != head);
}

void triangulate() {
    Vertex* v0;
    Vertex* v1;
    Vertex* v2;
    Vertex* v3;
    Vertex* v4;
    int n_tmp = n;

    earInit();
    while (n_tmp > 3) {
        // debug("while n_tmp > 3");
        v2 = head;
        do {
            if (v2->ear) {
                v3 = v2->next;
                v4 = v3->next;
                v1 = v2->prev;
                v0 = v1->prev;

                cout << v1->index << " " << v3->index << endl; // it's a diagonal

                v1->ear = Diagonal(v0, v3);
                v3->ear = Diagonal(v1, v4);

                v1->next = v3;
                v3->prev = v1;
                head = v3;
                n_tmp--;
                break;
            }
            v2 = v2->next;
        } while (v2 != head);
    } 
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // freopen("input.txt", "r", stdin);
    cin >> n;

    vector<T> v(n);
    for (int i = 0; i < n; ++i) {
        ll x, y;
        cin >> x >> y;
        v[i].p.real(x);
        v[i].p.imag(y);
        v[i].ind = i;
    }

    if (n == 3) 
        return 0;

    // ensure that counter-clockwise
    ensure_counter_clockwise(v);
    
    Vertex* cur = nullptr;
    Vertex* start = nullptr;
    for (int i = 0 ;i < v.size(); ++i) {
        if (i == 0) {
            head = new Vertex(v[i].ind, v[i].p);
            start = head;
            cur = head;
        } else {
            cur->next = new Vertex(v[i].ind, v[i].p);
            cur->next->prev = cur;
            cur = cur->next;
        }
    }
    cur->next = head;
    head->prev = cur;
    triangulate();
    return 0;
}