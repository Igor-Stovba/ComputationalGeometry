#include <bits/stdc++.h>
#include "seg_intersection_tests.h"
#include <boost/numeric/interval.hpp>
#include <gmpxx.h>

using namespace std;
using d = double;
using ld = long double;

struct V{
    d x1, y1, x2, y2;
    V(d x1, d y1, d x2, d y2): x1(x1), y1(y1), x2(x2), y2(y2) {}
};

bool intersect1d(ld l1, ld r1, ld l2, ld r2) {
    if (l1 > r1)
        swap(l1, r1);
    if (l2 > r2)
        swap(l2, r2);
    return max(l1, l2) <= min(r1, r2);
}

int cross_snd(ld ax, ld ay, ld bx, ld by, ld cx, ld cy) {
    mpf_set_default_prec(2048);
    mpf_t fax, fay, fbx, fby, fcx, fcy;
    mpf_init_set_d(fax, ax);
    mpf_init_set_d(fay, ay);
    mpf_init_set_d(fbx, bx);
    mpf_init_set_d(fby, by);
    mpf_init_set_d(fcx, cx);
    mpf_init_set_d(fcy, cy);

    mpf_t res, sub1, sub2, sub3, sub4, sub5, mul1, mul2;
    mpf_inits(res, sub1, sub2, sub3, sub4, sub5, mul1, mul2, nullptr);

    //Processing itself
    mpf_sub(sub1, fcx, fax);
    mpf_sub(sub2, fby, fay);
    mpf_sub(sub3, fcy, fay);
    mpf_sub(sub4, fbx, fax);
    mpf_mul(mul1, sub1, sub2);
    mpf_mul(mul2, sub3, sub4);
    mpf_sub(res, mul1, mul2);
    int s = mpf_sgn(res);

    mpf_clears(fax, fay, fbx, fby, fcx, fcy, res, sub1, sub2, sub3, sub4, sub5, mul1, mul2, nullptr);
    return s;
}

int sign(ld v) {
    return v > 0 ? 1 : (v == 0 ? 0 : -1);
}

int cross_fst(ld ax, ld ay, ld bx, ld by, ld cx, ld cy) { // a - common point
    // returns a sign: {-1, 0, 1}
    ld fst_part = (cx - ax)*(by - ay);
    ld snd_part = (cy - ay)*(bx - ax);
    ld E_hat = fst_part - snd_part;
    ld m_eps = std::numeric_limits<d>::epsilon();
    ld eps = (std::abs(fst_part) + std::abs(snd_part)) * (ld)8 * m_eps;
    
    if (std::abs(E_hat) > eps) { // the result is safe
        return sign(E_hat);
    }
    return cross_snd(ax, ay, bx, by, cx, cy);
}

bool intersect(const V& a, const V& b) {
    int sign1 = cross_fst(a.x1, a.y1, a.x2, a.y2, b.x1, b.y1);
    int sign2 = cross_fst(a.x1, a.y1, a.x2, a.y2, b.x2, b.y2);
    int sign3 = cross_fst(b.x1, b.y1, b.x2, b.y2, a.x1, a.y1);
    int sign4 = cross_fst(b.x1, b.y1, b.x2, b.y2, a.x2, a.y2);

    return intersect1d(a.x1, a.x2, b.x1, b.x2) &&
           intersect1d(a.y1, a.y2, b.y1, b.y2) &&
           (sign1 * sign2) <= 0 && (sign3 * sign4) <= 0;
}

int main() {
    int t;
    cin >> t;
    vector<double> points = genTest(t);
    int n = points.size() / 8;
    for (int i = 0; i < points.size(); i += 8) {
        V v1 = V(points[i], points[i+1], points[i+2], points[i+3]);
        V v2 = V(points[i+4], points[i+5], points[i+6], points[i+7]);
        if (intersect(v1, v2)) {
            cout << "Y";
        } else {
            cout << "N";
        }
    }
    cout << std::endl;
}