#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
using namespace std;

void do_testcase() {
    int num_planets, range; cin >> num_planets >> range;
    vector<pair<K::Point_2, bool> > planets(num_planets);   // pairs of (point, already_used)

    for(int i = 1; i <= num_planets; i++) {
        int x, y; cin >> x >> y;
        K::Point_2 p(x,y);
        planets.at(num_planets - i) = make_pair(p, false);
    }
    int max_planets = 0;
    for(int i = 0; i < num_planets; i++) {
        if(planets.at(i).second) continue;
        planets.at(i).second = true;
        int curr_max_planets = 1;
        int empire_planet_index = num_planets -1;
        for(int j = 0; j < empire_planet_index; j++) {
            if(planets.at(j).second || i == j) continue;
            if(CGAL::squared_distance(planets.at(i).first, planets.at(j).first) <= range*range) {
                //cout << "conquering planet " << j << " because it is reachable and empire_planet_index=" << empire_planet_index
                curr_max_planets++;
                empire_planet_index--;
                planets.at(j).second = true;
            }
        }
        max_planets = max(curr_max_planets, max_planets);
    }

    cout << max_planets << endl;
}

int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        do_testcase();
    }

    return 0;
}