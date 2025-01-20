#include <bits/stdc++.h>
#include <fstream>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;

int n;
enum class TYPE {
    OPEN = 0, CLOSED = 1
};

struct P {
    ll x, y;
    P(ll x, ll y): x(x), y(y) {}
};

struct Seg {
    P start, finish;
    int id;
    Seg(ll x1, ll y1, ll x2, ll y2, int id): start(x1, y1), finish(x2, y2), id(id) {}

    ld get_y(ld x) const {
        if (start.x == finish.x) {
            return start.y;
        }
        return start.y + ((finish.y - start.y) * (x - start.x)) / (finish.x - start.x);
    }
};

bool operator<(const Seg& s1, const Seg& s2) {
    ld x = max(min(s1.start.x, s1.finish.x), min(s2.start.x, s2.finish.x));
    return s1.get_y(x) < s2.get_y(x);
}

struct Event {
    ll x;
    int id;
    TYPE type;
    Event(ll x, int id, TYPE t): x(x), id(id), type(t) {}
};

vector<Event> events;
vector<Seg> segs;
set<Seg> s;
vector<set<Seg>::iterator> where;

set<Seg>::iterator prev(set<Seg>::iterator it) {
    if (it == s.begin()) {
        return s.end();
    }
    return --it;
}
set<Seg>::iterator next(set<Seg>::iterator it) {
    return ++it;
}

struct V {
    ll x, y;
    V(ll s_x, ll s_y, ll f_x, ll f_y): x(f_x - s_x), y(f_y - s_y) {}
    V(const P & p1, const P & p2): x(p2.x - p1.x), y(p2.y - p1.y) {}

    ld len() const {
        return std::sqrt(x*x + y*y);
    }
};

ll cross_prod(const V & v1, const V & v2) {
    return v1.x * v2.y - v1.y * v2.x;
}
ll scalar_prod(const V & v1, const V & v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

bool p_belongs_to_seg(const P & p, const Seg & s) {
    return (cross_prod(V (s.start.x, s.start.y, p.x, p.y), V (s.start.x, s.start.y, s.finish.x, s.finish.y)) == 0) && 
    (scalar_prod(V (p.x, p.y, s.start.x, s.start.y), V (p.x, p.y, s.finish.x, s.finish.y)) <= 0);
}

int sign(ll v) {
    if (v > 0)
        return 1;
    if (v == 0)
        return 0;
    return -1;
}

bool intersect1d(ll l1, ll r1, ll l2, ll r2) {
    if (l1 > r1)
        swap(l1, r1);
    if (l2 > r2)
        swap(l2, r2);
    return max(l1, l2) <= min(r1, r2);
}

bool intersect(const Seg& a, const Seg& b)
{
    int sign1 = sign(cross_prod(V (a.start, a.finish), V (a.start, b.start)));
    int sign2 = sign(cross_prod(V (a.start, a.finish), V (a.start, b.finish)));
    int sign3 = sign(cross_prod(V (b.start, b.finish), V (b.start, a.start)));
    int sign4 = sign(cross_prod(V (b.start, b.finish), V (b.start, a.finish)));
    return intersect1d(a.start.x, a.finish.x, b.start.x, b.finish.x) &&
           intersect1d(a.start.y, a.finish.y, b.start.y, b.finish.y) &&
           (sign1 * sign2) <= 0 && (sign3 * sign4) <= 0;
}

pair<int, int> go() {
    s.clear();
    where.resize(segs.size()); // returns iterator by a given id

    for (int i = 0; i < events.size(); ++i) {
        int seg_id = events[i].id-1;
        if (events[i].type == TYPE::OPEN) {
            auto next_it = s.lower_bound(segs[seg_id]);
            auto prev_it = prev(next_it);
            if (next_it != s.end() && intersect(*next_it, segs[seg_id]))
                return {next_it->id, seg_id+1};
            if (prev_it != s.end() && intersect(*prev_it, segs[seg_id]))
                return {prev_it->id, seg_id+1};
            where[seg_id] = s.insert(next_it, segs[seg_id]);
        } else { // TYPE::CLOSED
            auto prev_it = prev(where[seg_id]);
            auto next_it = next(where[seg_id]);
            if (prev_it != s.end() && next_it != s.end() && intersect(*prev_it, *next_it)) 
                return {prev_it->id, next_it->id};
            s.erase(where[seg_id]);
        }
    }

    return {-1, -1};
}

int main() {
    // freopen("input.txt", "r", stdin);
    cin >> n;
    events.reserve(2*n);
    segs.reserve(n);
    ll x1,y1,x2,y2;

    // initialize vector of segs
    for (int i = 0; i < n; ++i) {
        cin >> x1 >> y1 >> x2 >> y2;
        segs.emplace_back(x1, y1, x2, y2, i+1);
    }
    
    // fill vector of Events
    for (auto& seg: segs) {
        events.emplace_back(min(seg.start.x, seg.finish.x), seg.id, TYPE::OPEN);
        events.emplace_back(max(seg.start.x, seg.finish.x), seg.id, TYPE::CLOSED);
    }

    std::sort(events.begin(), events.end(), [](const Event& e1, const Event& e2) {
        if (e1.x == e2.x) {
            return e1.type < e2.type;
        }
        return e1.x < e2.x;
    });

    pair<int, int> ret = go();
    if (ret.first == -1 && ret.second == -1) {
        cout << "NO\n";
    } else {
        cout << "YES\n";
        cout << ret.first << " " << ret.second << '\n';
    }

    return 0;
}