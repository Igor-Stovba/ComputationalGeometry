#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;

struct P {
    ll x, y;
    P(ll x, ll y): x(x), y(y) {}
};

struct V {
    ll x, y;
    V(ll s_x, ll s_y, ll f_x, ll f_y): x(f_x - s_x), y(f_y - s_y) {}
    V(const P & p1, const P & p2): x(p2.x - p1.x), y(p2.y - p1.y) {}

    ld len() const {
        return std::sqrt(x*x + y*y);
    }
};

struct Segment {
    ll x1, y1, x2, y2;

    Segment(ll x1, ll y1, ll x2, ll y2): x1(x1), y1(y1), x2(x2), y2(y2) {}
    Segment(const P & p1, const P & p2): x1(p1.x), y1(p1.y), x2(p2.x), y2(p2.y) {}
};

struct Ray{
    ll x1, y1, x2, y2;
    Ray(ll x1, ll y1, ll x2, ll y2): x1(x1), y1(y1), x2(x2), y2(y2) {} 
    Ray(const P & p1, const P & p2): x1(p1.x), y1(p1.y), x2(p2.x), y2(p2.y) {}
};

struct Line{
    ll x1, y1, x2, y2;
    Line(ll x1, ll y1, ll x2, ll y2): x1(x1), y1(y1), x2(x2), y2(y2) {} 
    Line(const P & p1, const P & p2): x1(p1.x), y1(p1.y), x2(p2.x), y2(p2.y) {}
};

int sign(ll a) {
    if (a < 0)
        return -1;
    if (a == 0) 
        return 0;
    return 1;
}

ld dist(ll x1, ll y1, ll x2, ll y2) {
    ll delta_x = x2 - x1;
    ll delta_y = y2 - y1;
    return std::sqrt(delta_x*delta_x + delta_y*delta_y);
}

ld dist(const P & p1, const P & p2) {
    ll delta_x = p2.x - p1.x;
    ll delta_y = p2.y - p1.y;
    return std::sqrt(delta_x*delta_x + delta_y*delta_y);
}

ll cross_prod(const V & v1, const V & v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

ll scalar_prod(const V & v1, const V & v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
bool p_belongs_to_seg(const P & p, const Segment & s) {
    return (cross_prod(V (s.x1, s.y1, p.x, p.y), V (s.x1, s.y1, s.x2, s.y2)) == 0) && 
    (scalar_prod(V (p.x, p.y, s.x1, s.y1), V (p.x, p.y, s.x2, s.y2)) <= 0);
}

bool intersect1d(ll l1, ll r1, ll l2, ll r2) {
    if (l1 > r1)
        swap(l1, r1);
    if (l2 > r2)
        swap(l2, r2);
    return max(l1, l2) <= min(r1, r2);
}

bool intersect(const Segment& a, const Segment& b)
{
    int sign1 = sign(cross_prod(V (a.x1, a.y1, a.x2, a.y2), V (a.x1, a.y1, b.x1, b.y1)));
    int sign2 = sign(cross_prod(V (a.x1, a.y1, a.x2, a.y2), V (a.x1, a.y1, b.x2, b.y2)));
    int sign3 = sign(cross_prod(V (b.x1, b.y1,b.x2, b.y2), V (b.x1, b.y1, a.x1, a.y1)));
    int sign4 = sign(cross_prod(V (b.x1, b.y1, b.x2, b.y2), V (b.x1, b.y1, a.x2, a.y2)));
    return intersect1d(a.x1, a.x2, b.x1, b.x2) &&
           intersect1d(a.y1, a.y2, b.y1, b.y2) &&
           (sign1 * sign2) <= 0 && (sign3 * sign4) <= 0;
}


ld point_to_point(const P & p1, const P & p2) { // Checked
    return dist(p1, p2);
}


ld point_to_segment(const P & p, const Segment & s) { // Checked
    V  lp = V (s.x1, s.y1, p.x, p.y);
    V  rp = V (s.x2, s.y2, p.x, p.y);
    V  lr = V (s.x1, s.y1, s.x2, s.y2);
    V  rl = V (s.x2, s.y2, s.x1, s.y1);
    if (scalar_prod(lp, lr) >= 0 && scalar_prod(rp, rl) >= 0) {
        return (std::abs(cross_prod(lp, lr)) / lr.len());
    } else {
        return std::min(lp.len(), rp.len());
    }
}


ld point_to_ray(const P & p, const Ray & r) { // Checked
    V  p1p2 = V (r.x1, r.y1, r.x2, r.y2);
    V  p1p = V (r.x1, r.y1, p.x, p.y);
    if (scalar_prod(p1p2, p1p) >= 0) {
        return std::abs(cross_prod(p1p2, p1p)) / p1p2.len();
    } else {
        return p1p.len();
    }
}


ld point_to_line(const P & p, const Line & l) { // Checked
    V  lp = V (l.x1, l.y1, p.x, p.y);
    V  lr = V (l.x1, l.y1, l.x2, l.y2);
    return std::abs(cross_prod(lp, lr)) / lr.len();
}


ld segment_to_segment(const Segment & s1, const Segment & s2) { 
    P  p1s = P (s1.x1, s1.y1);
    P  p1f = P (s1.x2, s1.y2);
    P  p2s = P (s2.x1, s2.y1);
    P  p2f = P (s2.x2, s2.y2);
    ld d1 = std::min(point_to_segment(p1s, s2),point_to_segment(p1f, s2));
    ld d2 = std::min(point_to_segment(p2s, s1),point_to_segment(p2f, s1));
    ld dd = std::min(d1, d2);
    if (intersect(s1, s2)) {
        return 0;
    }
    return dd;
}

ld segment_to_ray(const Segment & s, const Ray & r) {
    P  start = P (r.x1, r.y1);  // point of ray
    ll delta_x = r.x2 - r.x1;
    ll delta_y = r.y2 - r.y1;
    P  p = P (r.x1 + 40001*delta_x, r.y1 + 40001*delta_y); // another point of ray

    return segment_to_segment(s, Segment(start, p));
}

ld segment_to_line(const Segment & s, const Line & l) { // Checked
    V  l1l2 = V (l.x1, l.y1, l.x2, l.y2);
    V  l1sl = V (l.x1, l.y1, s.x1, s.y1);
    V  l1sr = V (l.x1, l.y1, s.x2, s.y2);
    if (sign(cross_prod(l1l2, l1sl))*sign(cross_prod(l1l2, l1sr)) <= 0) {
        // точки отрезка по обе стороны
        return 0;
    } else {
        return std::min(point_to_line(P (s.x1, s.y1), l),
                        point_to_line(P (s.x2, s.y2), l));
    }
}

ld ray_to_ray(const Ray & r1, const Ray & r2) {
    ll a1 = r1.y1 - r1.y2;
    ll b1 = r1.x2 - r1.x1;
    ll c1 = -a1*r1.x1-b1*r1.y1;

    ll a2 = r2.y1 - r2.y2;
    ll b2 = r2.x2 - r2.x1;
    ll c2 = -a2*r2.x1-b2*r2.y1;

    if (b2*a1 == b1*a2) { // coincident or parallel
        if ((a1*c2==a2*c1) && (b1*c2==b2*c1)) { // coincident, but they may not intersect
            // TODO
        } else { // parallel
            return std::min(point_to_ray(P(r1.x1, r1.y1), r2),
                            point_to_ray(P(r2.x1, r2.y1), r1));
        }
    } else {
        // there is point of intersection (BUT as lines!!!!!)
        ld x_ = static_cast<ld>((b1*c2 - b2*c1)) / (b2*a1 - b1*a2); // x coord of point of intersection
        ld y_ = static_cast<ld>((c1*a2 - c2*a1)) / (b2*a1 - b1*a2); // y coord of point of intersection
        // Now we check that (x_, y_) lies on good sides with respect to rays
        // use scalara prod, but with real arithmetic
        auto scalar_prodf = [](ld x1, ld y1, ld x2, ld y2) -> ld {
            return x1 * x2 + y1 * y2;
        };

        ld scal1 = scalar_prodf(r1.x2 - r1.x1, r1.y2 - r1.y1, x_ - r1.x1, y_ - r1.y1);
        ld scal2 = scalar_prodf(r2.x2 - r2.x1, r2.y2 - r2.y1, x_ - r2.x1, y_ - r2.y1);
        if (scal1 >= 0 && scal2 >= 0) {
            return 0;
        } else {
            return std::min(
                point_to_ray(P(r1.x1, r1.y1), r2),
                point_to_ray(P(r2.x1, r2.y1), r1)
            );
        }
    }
}


ld ray_to_line(const Ray & r, const Line & l) { 
    // split line as 2 rays and use ray_to_ray twice and take min of them 
    Ray r1 = Ray(l.x1, l.y1, l.x2, l.y2);
    Ray r2 = Ray(l.x2, l.y2, l.x1, l.y1);
    return std::min(
        ray_to_ray(r, r1),
        ray_to_ray(r, r2)
    );    
}

ld line_to_line(const Line & l1, const Line & l2) { // Checked
    ll a1 = l1.y1 - l1.y2;
    ll b1 = l1.x2 - l1.x1;
    ll c1 = -a1*l1.x1-b1*l1.y1;

    ll a2 = l2.y1 - l2.y2;
    ll b2 = l2.x2 - l2.x1;
    ll c2 = -a2*l2.x1-b2*l2.y1;

    if (b2*a1 == b1*a2) {
        if ((a1*c2==a2*c1) && (b1*c2==b2*c1)) { // coincident
            return 0.0;
        } else {
            return point_to_line(P (l1.x1, l1.y1), l2); // parallel
        }
    } 
    return 0.0;
}

int main() {
    // freopen("input.txt", "r", stdin);
    ll x_a, y_a, x_b, y_b, x_c, y_c, x_d, y_d;
    cin >> x_a >> y_a >> x_b >> y_b >> x_c >> y_c >> x_d >> y_d;
    P a(x_a, y_a);
    P b(x_b, y_b);
    P c(x_c, y_c);
    P d(x_d, y_d);

    cout << std::setfill('0') << std::setw(10) << std::fixed << std::setprecision(10);
    // от точки А до точки С
    cout << point_to_point(a, c) << '\n';

    // от точки A до отрезка CD
    cout << point_to_segment(a, Segment(c, d)) << '\n';

    // от точки A до луча CD
    cout << point_to_ray(a, Ray(c, d)) << '\n';

    // от точки A до прямой CD
    cout << point_to_line(a, Line(c, d)) << '\n';

    // от отрезка AB до точки C
    cout << point_to_segment(c, Segment(a, b)) << '\n';

    // от отрезка AB до отрезка CD
    cout << segment_to_segment(Segment(a,b), Segment(c, d)) << '\n';

    // от отрезка AB до луча CD
    cout << segment_to_ray(Segment(a, b), Ray(c, d)) << '\n';

    // от отрезка AB до прямой CD
    cout << segment_to_line(Segment(a, b), Line(c, d)) << '\n';

    // от луча AB до точки C
    cout << point_to_ray(c, Ray(a, b)) << '\n';

    // от луча AB до отрезка CD
    cout << segment_to_ray(Segment(c, d), Ray(a, b)) << '\n';

    //  от луча AB до луча CD
    cout << ray_to_ray(Ray(a,b), Ray(c, d)) << '\n';

    // от луча AB до прямой CD
    cout << ray_to_line(Ray(a, b), Line(c, d)) << '\n';

    // от прямой AB до точки C
    cout << point_to_line(c, Line(a, b)) << '\n';

    // от прямой AB до отрезка CD.
    cout << segment_to_line(Segment(c, d), Line(a, b)) << '\n';

    // от прямой AB до луча CD
    cout << ray_to_line(Ray(c, d), Line(a, b)) << '\n';

    // от прямой AB до прямой CD
    cout << line_to_line(Line(a, b), Line(c, d)) << '\n';

    return 0;
}