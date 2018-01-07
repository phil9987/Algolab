#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Delaunay_triangulation_2.h>


// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;


using namespace std;

bool feasible(const vector<P>& mpe_position, const vector<K::FT>& mpe_radius, const vector<pair<P, int> >& sensor, size_t max_intensity, size_t max_mpe) {
    
    // nonnegative linear program with Ax >= b
    Program lp (CGAL::LARGER, true, 0, false, 0);
    size_t num_sensors = sensor.size();

    for(size_t sens_idx = 0; sens_idx < num_sensors; sens_idx++) {
        // sum(ip) >= deactivation_energy
        P sens_pos = sensor.at(sens_idx).first;
        lp.set_b(sens_idx, sensor.at(sens_idx).second); //lp.set_r(sens_idx, CGAL::LARGER);
        for(size_t mpe_idx = 0; mpe_idx < max_mpe; mpe_idx++) {
            K::FT dist = CGAL::squared_distance(mpe_position.at(mpe_idx), sens_pos);
            if(mpe_radius.at(mpe_idx) == -1 || dist <= mpe_radius.at(mpe_idx)) {
                // only add it to equation if it's reachable or if radius == -1 (infinity)
                lp.set_a(mpe_idx, sens_idx,  ET(1)/dist);  
            }
        }
    }
    lp.set_b(num_sensors, max_intensity);
    lp.set_r(num_sensors, CGAL::SMALLER); // override default >= to <=
    for(size_t i = 0; i < max_mpe; i++) {
        lp.set_a(i, num_sensors, 1);
        //lp.set_c(i, 1);        // objective function: minimize sum(ip)
    }

    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));
    if (s.status() == CGAL::QP_OPTIMAL) {
        return true;
    } else {
        return false;
    }
}

void run_testcase() {
    size_t num_sensors, num_henchmen, max_intensity;
    int num_mpe;
    cin >> num_sensors >> num_mpe >> num_henchmen >> max_intensity;
    vector<pair<P, int> > sensor(num_sensors);

    for(size_t i = 0; i < num_sensors; i++) {
        P pos;
        cin >> pos;
        int deactivation_energy; cin >> deactivation_energy;
        sensor.at(i) = make_pair(pos, deactivation_energy);
    }
    vector<P> mpe_position(num_mpe);

    for(size_t i = 0; i < num_mpe; i++) {
        cin >> mpe_position.at(i);
    }

    vector<P> henchmen_pos(num_henchmen);

    for(size_t i = 0; i < num_henchmen; i++) {
        cin >> henchmen_pos.at(i);
    }
    // construct triangulation
    vector<K::FT> mpe_radius(num_mpe, -1);
    if(henchmen_pos.size() > 0) {
        Triangulation t;
        t.insert(henchmen_pos.begin(), henchmen_pos.end());
        for(size_t i = 0; i < num_mpe; i++) {
            Triangulation::Vertex_handle vh = t.nearest_vertex(mpe_position.at(i)); // get closest henchman == maximum possible radius for mpe_i
            P nearest_henchman = vh->point();       // get K::Point_2 from vertex_handle
            mpe_radius.at(i) = CGAL::squared_distance(mpe_position.at(i), nearest_henchman);        // calculate squared radius
        }
    }

    size_t l = 1;
    size_t r = num_mpe;
    // increase l as much as possible
    while(l*2 < r && !feasible(mpe_position, mpe_radius, sensor, max_intensity, l*2)) {
        l *= 2;
    }
    if(l*2 < r) {
        // decrease r as much as possible
        r = l*2;
    }
    while(l <= r) {
        size_t p = l + ((r-l) / 2);
        if(feasible(mpe_position, mpe_radius, sensor, max_intensity, p)) {
            r = p - 1;
        } else {
            l = p + 1;
        }
    }
    if(l > num_mpe) {
        cout << "impossible" << endl;
    } else {
        cout << l << endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) run_testcase();
    return 0;
}