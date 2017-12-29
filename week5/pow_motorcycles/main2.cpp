#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> Q;
typedef tuple<long, int, Q> Biker;

// round down to next double
double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}


void do_testcase() {
    //cout << "new test" << endl;
    int num_bikers; cin >> num_bikers;
    vector<Biker> bikers(num_bikers);
    for(int i=0; i < num_bikers; i++) {
        long y0, x1, y1; cin >> y0 >> x1 >> y1;
        Q q(y1 - y0, x1);
        bikers.at(i) = make_tuple(y0, i, q);
    }
    sort(bikers.begin(), bikers.end(), [](Biker a, Biker b) {
        return get<0>(a) > get<0>(b);       // bikers with higher y are first
    });
    Q shallowest_m = get<2>(bikers.at(0));
    vector<int> res;
    res.push_back(get<1>(bikers.at(0)));
    for(int i = 1; i < num_bikers; i++) {
        Biker b = bikers.at(i);
        if(get<2>(b) <= shallowest_m){
            shallowest_m = get<2>(b);
            //cout << get<1>(b) << " ";
            res.push_back(get<1>(b));
        } else {
            // biker's jurney ends here...
        }
    }
    sort(res.begin(), res.end());
    for(int i: res) cout << i << " ";
    cout << endl;
}

int main() {
    int T; cin >> T;
    for(int i = 0; i < T; i++) {
        do_testcase();
    }
    return 0;
}