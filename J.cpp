#include <bits/stdc++.h>
#include <math.h>

using ll = long long;
using ull = unsigned long long;
using ld = long double;
using namespace std;

using P = complex<long long>;
using Pld = complex<long double>;
#define X real()
#define Y imag()

 template<typename A, typename B>
string to_string(pair<A,B> p) {return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";}
string to_string(P p) {return "<" + to_string(p.real()) + " ^ " + to_string(p.imag()) + "i>";}
string to_string(const string& s) {return '"' + s + '"';}
string to_string(const char* s) {return to_string((string) s);}
string to_string(bool b) {return (b ? "true" : "false");}
string to_string(Pld p) {
    return "<" + to_string(p.X) + "," + to_string(p.Y) + ">";
}
 
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

struct half_edge;

struct vertex {
    vertex(half_edge* rep, const Pld& coord): rep(rep), coord(coord) {}

    half_edge *rep; 
    Pld coord;
};

struct face {
    face(half_edge* rep): rep(rep) {}
    half_edge *rep;  
};

struct half_edge {
    half_edge(vertex* origin): origin(origin) {}

    half_edge *prev = nullptr;  
    half_edge *next = nullptr;  
    half_edge *twin = nullptr;  
    vertex *origin;     
    face *left = nullptr;
};

struct Line {
    ll a,b,c;
    P p1, p2;
    Line() = default;
    Line(const P& p1, const P& p2) {
        a = p1.Y - p2.Y;
        b = p2.X - p1.X;
        c = -a*p2.X - b*p2.Y;
        this->p1 = p1;
        this->p2 = p2;
    }
};

ll cross_prod(const P& p1, const P& p2) {return p1.X*p2.Y - p1.Y*p2.X;}
ll scalar_prod(const P& o1, const P& o2) {return (o1.X*o2.X + o1.Y*o2.Y);}
int sign(ll v) {return (v < 0) ? -1 : (v == 0 ? 0 : 1);}

bool intersect(const Pld& a, const Pld& b, const Line& l) { 
    ld fst = l.a * a.X + l.b * a.Y + l.c;
    ld snd = l.a * b.X + l.b * b.Y + l.c;
    return fst * snd < 1e-9;
}

Pld find_intersection(const Pld& A, const Pld& B, const Line& l) {
    ld dx = (B.X - A.X); ld dy = (B.Y - A.Y);
    ld denominator = l.a * dx + l.b * dy;
    ld t = -(l.a * A.X + l.b * A.Y + l.c) / denominator;
    ld x = A.X + t * dx;
    ld y = A.Y + t * dy;
    return {x, y};
}

struct DCEL {
    DCEL(const Pld& up_left, const Pld& up_right, const Pld& down_left, const Pld& down_right) {
        vs.push_back(new vertex(nullptr, up_left));
        vs.push_back(new vertex(nullptr, down_left));
        vs.push_back(new vertex(nullptr, down_right));
        vs.push_back(new vertex(nullptr, up_right));

        es.push_back(new half_edge(vs[0]));
        vs[0]->rep = es[0];
        es.push_back(new half_edge(vs[1]));
        vs[1]->rep = es[1];
        es.push_back(new half_edge(vs[2]));
        vs[2]->rep = es[2];
        es.push_back(new half_edge(vs[3]));
        vs[3]->rep = es[3];

        es.push_back(new half_edge(vs[1]));
        es.push_back(new half_edge(vs[2]));
        es.push_back(new half_edge(vs[3]));
        es.push_back(new half_edge(vs[0]));

        fs.push_back(new face(es[0])); // OK
        // дополним инфу в ребра
        es[0]->left = fs[0];
        es[0]->next = es[1];
        es[0]->prev = es[3];
        es[0]->twin = es[4];

        es[1]->left = fs[0];
        es[1]->next = es[2];
        es[1]->prev = es[0];
        es[1]->twin = es[5];

        es[2]->left = fs[0];
        es[2]->next = es[3];
        es[2]->prev = es[1];
        es[2]->twin = es[6];

        es[3]->left = fs[0];
        es[3]->next = es[0];
        es[3]->prev = es[2];
        es[3]->twin = es[7];


        es[4]->left = nullptr; //special face (infinite)
        es[4]->next = es[7];
        es[4]->prev = es[5];
        es[4]->twin = es[0];
        
        es[5]->left = nullptr; //special face (infinite)
        es[5]->next = es[4];
        es[5]->prev = es[6];
        es[5]->twin = es[1]; // es[1]
        
        es[6]->left = nullptr; //special face (infinite)
        es[6]->next = es[5];
        es[6]->prev = es[7];
        es[6]->twin = es[2];
        
        es[7]->left = nullptr; //special face (infinite)
        es[7]->next = es[6];
        es[7]->prev = es[4];
        es[7]->twin = es[3];
    }

    void add_line(const Line& l) {
        half_edge* start = nullptr;
        for (auto& it: es) {
            if (it->twin->left == nullptr && intersect(it->origin->coord, it->next->origin->coord, l)) {
                start = it;
                break;
            }
        }

        Pld inter_p = find_intersection(start->origin->coord, start->next->origin->coord, l);
        vs.push_back(new vertex(nullptr, inter_p));
        es.push_back(new half_edge(vs.back()));
        es.push_back(new half_edge(vs.back()));

        start->next->prev = es[es.size()-2];
        es[es.size()-2]->next = start->next;
        es[es.size()-2]->prev = start;
        es[es.size()-2]->left = start->left;
        es[es.size()-2]->twin = start->twin;
        start->next = es[es.size()-2];
        start->twin = es[es.size()-1];

        half_edge* t = es[es.size()-2]->twin;
        t->next->prev = es[es.size()-1];
        es[es.size()-1]->next = t->next;
        es[es.size()-1]->prev = t;
        es[es.size()-1]->left = t->left;
        es[es.size()-1]->twin = start;
        t->next = es[es.size()-1];
        t->twin = es[es.size()-2];

        start = start->next;

        half_edge* next = nullptr;
        while (start->left != nullptr) {
            next = start->next;
            while (!intersect(next->origin->coord, next->next->origin->coord, l)) {
                next = next->next;
            }
            inter_p = find_intersection(next->origin->coord, next->next->origin->coord, l);
        
            vs.push_back(new vertex(nullptr, inter_p));
            es.push_back(new half_edge(vs[vs.size()-1])); 
            es.push_back(new half_edge(vs[vs.size()-2])); 
            

            es.push_back(new half_edge(vs[vs.size()-1])); 
            es.push_back(new half_edge(vs[vs.size()-1])); 

            fs.push_back(new face(es[es.size()-4]));
            // forwarding relationships

            es[es.size()-4]->next = start; 
            es[es.size()-4]->prev = next; 
            es[es.size()-4]->twin = es[es.size()-3]; 
            es[es.size()-4]->left = fs[fs.size()-1]; 

            es[es.size()-3]->next = es[es.size()-2]; 
            es[es.size()-3]->prev = start->prev; 
            es[es.size()-3]->twin = es[es.size()-4];
            es[es.size()-3]->left = start->prev->left; 
            start->prev->next = es[es.size()-3];
            start->prev = es[es.size()-4];


            next->next->prev = es[es.size()-2];
            next->twin->next->prev = es[es.size()-1];
            es[es.size()-1]->next = next->twin->next;
            es[es.size()-1]->prev = next->twin;
            es[es.size()-1]->twin = next;
            es[es.size()-1]->left = es[es.size()-1]->prev->left;

            es[es.size()-1]->prev->next = es[es.size()-1];
            es[es.size()-1]->prev->twin = es[es.size()-2];

            es[es.size()-2]->next = next->next;
            es[es.size()-2]->prev = es[es.size()-3];
            es[es.size()-2]->twin = es[es.size()-1]->prev;
            es[es.size()-2]->left = es[es.size()-2]->next->left;

            next->next = es[es.size()-4];
            next->twin = es[es.size()-1];

            // изменить face as like as by 1-st 
            half_edge* tmp = es[es.size()-4]->next;
            tmp->left->rep = es[es.size()-4]->twin;
            while (tmp != es[es.size()-4]) {
                tmp->left = es[es.size()-4]->left;
                tmp = tmp->next;
            }
            start = next->twin;
        }
        // validate_dcel();
    }

    bool check_difference_edges(face* f1, face* f2) {
        /*
        method checks whether f1 and f2 have common edges. If it is so, then assertion should fail
        */
        half_edge* fst = f1->rep;
        half_edge* snd;
        do {
            snd = f2->rep;
            do {
                assert(snd->left != fst->left);
                if (snd == fst)
                    return false;
                snd = snd->next;
            } while (snd != f2->rep);
            fst = fst->next;
        } while (fst != f1->rep);
        return true;
    }

    bool is_outer(face* f) {
        half_edge* edge = f->rep;
        do {
            if (edge->twin->left == nullptr)
                return true;
            edge = edge->next;
        } while (edge != f->rep);
        return false;
    }

    ld compute_area(face* f) {
        // we assume that f is bounded face (in our terms -> inner is true)
        if (is_outer(f))
            return -1.0;
        ld res = 0;
        half_edge* rep = f->rep;
        vector<Pld> v;
        do {
            v.push_back(rep->origin->coord);
            rep = rep->next;
        } while (rep != f->rep);
        v.push_back(v[0]);
        v.push_back(v[1]);
        for (int i = 0; i < v.size()-2; ++i) 
            res += (v[i].Y + v[i+1].Y)*(v[i].X - v[i+1].X);
        
        return std::abs(res) / 2;
    }

    void print_face(face* f) {
        half_edge* rep = f->rep;
        debug("---------------------------");
        do {
            debug(rep->origin->coord);
            rep = rep->next;
        } while (rep != f->rep);        
    }
    void validate_dcel() {
        // debug("validation");
        for (auto& e : es) {
            assert(e->next->prev == e);
            assert(e->prev->next == e);
            assert(e->twin->twin == e);
            half_edge *tmp = e;
            do {
                assert(e->left == tmp->left);
                tmp = tmp->next;
            } while (tmp != e);
        }
        for (int i = 0; i < fs.size()-1; ++i) {
            for (int j = i + 1; j < fs.size(); ++j) {
                assert(check_difference_edges(fs[i], fs[j]));
            }
        }
    }

    vector<face*> fs;
    vector<vertex*> vs;
    vector<half_edge*> es;
};

Pld find_inter(const Line& l1, const Line& l2) {
    // we can assume the lines aren't parallel
    ll down = l2.b * l1.a - l1.b * l2.a;
    ll up_x = l1.b * l2.c - l2.b * l1.c;
    ll up_y = l1.c * l2.a - l2.c * l1.a;
    return {static_cast<ld>(up_x) / down, static_cast<ld>(up_y) / down};
}

bool parallel(const Line& l1, const Line& l2) {
    return (l2.b * l1.a - l1.b * l2.a) == 0;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // freopen("input.txt", "r", stdin);
    cin >> n;
    vector<Line> v(n);
    P tmp1, tmp2;
    ll x, y;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        tmp1.real(x);
        tmp1.imag(y);
        cin >> x >> y;
        tmp2.real(x);
        tmp2.imag(y);
        v[i] = Line(tmp1, tmp2);
    }

    if (n < 3) {
        cout << 0 << endl;
        return 0;
    }

    // Build DCEL for bounding box O(n^2)
    vector<Pld> inters;
    for (int i = 0; i < v.size()-1; ++i) {
        for (int j = i + 1; j < v.size(); ++j) {
            if (!parallel(v[i], v[j]))
                inters.push_back(find_inter(v[i], v[j]));
        }
    }
    if (inters.size() < 3) {
        std::cout << 0 << endl;
        return 0;
    }
    ld up_y = inters[0].Y;
    ld down_y = inters[0].Y;
    ld left_x = inters[0].X;
    ld right_x = inters[0].X;

    for (auto& i: inters) {
        if (i.Y > up_y)
            up_y = i.Y;
        if (i.Y < down_y)
            down_y = i.Y;
        if (i.X > right_x)
            right_x = i.X;
        if (i.X < left_x) 
            left_x = i.X;
    }
    up_y += 100000; down_y -= 100000;
    right_x += 100000; left_x -= 100000;

    DCEL dcel = DCEL({left_x, up_y}, {right_x, up_y}, {left_x, down_y}, {right_x, down_y}); 
    // dcel.validate_dcel();
    for (int i = 0; i < v.size(); ++i) {
        dcel.add_line(v[i]);
        // debug("added line: ", i);
    } 

    vector<ld> areas;
    for (int i = 0; i < dcel.fs.size(); ++i) {
            ld area = dcel.compute_area(dcel.fs[i]);
            if (area >= 1e-8) 
                areas.push_back(area);
            
    }

    cout << areas.size() << endl;
    std::sort(areas.begin(), areas.end());
    std::cout.precision(15);
    for (auto& i: areas) 
        cout << i << endl;
    
    
    return 0;
}