#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Circle_2 C;


using namespace std;

void do_testcase() {
    int n, m;
    long p;
    cin >> n >> m >> p;

    vector<P> jammers(n);
    vector<pair<P, P> > missions(m);

    for(int i = 0; i < n; i++) {
        // read in jammer positions
        cin >> jammers.at(i);
    }

    for(int i = 0; i < m; i++) {
        // read in missions
        // 4 integer coordinates xs, ys, xt, yt
        P s, t;
        cin >> s >> t;
        missions.at(i) = make_pair(s, t);
    }
}

int main() {
    int T; cin >> T;
    for(int t = 0; t < T; t++) do_testcase();
    return 0;
}