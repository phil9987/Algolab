#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> Q;
typedef tuple<long, int, Q, Q, bool> Biker;    // y0, idx, slope, abs(slope), stopped

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
        Q q2 = CGAL::abs(q);
        bikers.at(i) = make_tuple(y0, i, q, q2, false);
    }
    /*sort(bikers.begin(), bikers.end(), [](Biker a, Biker b) {
        return get<0>(a) > get<0>(b);       // bikers with higher y are first
    });*/
    sort(bikers.begin(), bikers.end(), [](Biker a, Biker b) {
        return get<3>(a) <= get<3>(b);       // bikers with lower abs(slope) are first (they are faster)
    });
    /*for(auto b: bikers) {
        cout << get<3>(b) << " " <<get<1>(b) << endl;
    }*/
    //Q shallowest_m = get<2>(bikers.at(0));
    vector<int> res;
    //res.push_back(get<1>(bikers.at(0)));
    for(int i = 0; i < num_bikers; i++) {
        Biker b = bikers.at(i);
        Q m = get<2>(b);
        bool add = !get<4>(b);      // init to !stopped
        //cout << m << endl;
        //cout << get<1>(b) << " add: " << add << endl;
        for(int j = i+1; j < num_bikers && add; j++){
            // because bikers is sorted we know that all bikers have a slope >= m
            //cout << "looking at biker " << j << " of " << num_bikers << endl;
            Biker b1 = bikers.at(j);
            Q m1 = get<2>(b1);

            if(get<4>(b1) || i == j) {
                //cout << "biker " << get<1>(b1) << " has already stoppedstops" << endl;
                continue;       // biker b1 has stopped already
            } 

            if(get<0>(b1) > get<0>(b)) {
                // b1 starts above b
                if((m == 0 && m1 < 0) || ((m1 < 0) &&!(m1 == -m))){
                    // b rides vertically (always wins) or m1 < 0 and because bikers is sorted we know that m < m1
                    // b will ride to sunrise forever -> b1 stops
                    get<4>(b1) = true;       // b1 stops
                    bikers.at(j) = b1;
                    //cout << "biker " << get<1>(b1) << " stops0" << endl;
                } /*else if(get<2>(b) == -get<2>(b1)) {
                    // right before left: b1 stops
                    get<4>(b1) = true;       // b1 stops
                    bikers.at(j) = b1;
                    //cout << "biker" << get<1>(b) << " stops2" << endl;
                }*/
            } else {
                // b1 starts below b
                if((m == 0 && m1 > 0) || (m1 > 0 && !(m1 == -m))){
                    // b will ride to sunrise forever -> b1 stops
                    get<4>(b1) = true;       // b1 stops
                    bikers.at(j) = b1;
                    //cout << "biker " << get<1>(b1) << " stops1" << endl;
                } /*else if(get<2>(b) == -get<2>(b1)) {
                    // right before left: b stops
                    add = false;
                    get<4>(b) = true;
                    bikers.at(i) = b;
                    //cout << "biker" << get<1>(b) << " stops3" << endl;
                }*/
            }
        }
        if(add) {
            res.push_back(get<1>(b));
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