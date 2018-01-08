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

    vector<pair<P, P> > jammers(n);
    vector<vector<P> > missions(m);

    for(int i = 0; i < n; i++) {
        // read in jammer positions
        // x, y
        P x, y;
        cin >> x >> y;
        jammers.at(i) = make_pair(x,y);
    }

    for(int i = 0; i < m; i++) {
        // read in missions
        // 4 integer coordinates xs, ys, xt, yt
        P xs, ys, xt, yt;
        cin >> xs >> ys >> xt >> yt;
        missions.at(i) = {xs, ys, xt, yt};
    }
}

int main() {
    int T; cin >> T;
    for(int t = 0; t < T; t++) do_testcase();
    return 0;
}