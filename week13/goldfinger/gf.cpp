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

bool feasible(vector<P>& mpe_position, vector<K::FT>& mpe_radius, vector<pair<P, size_t> >& sensor, size_t max_intensity, size_t num_mpe) {
    
    // nonnegative linear program with Ax >= b
    Program lp (CGAL::LARGER, true, 0, false, 0);
    size_t num_sensors = sensor.size();

    for(size_t sens_idx = 0; sens_idx < num_sensors; sens_idx++) {
        // sum(ip) >= deactivation_energy
        P sens_pos = sensor.at(sens_idx).first;
        lp.set_b(sens_idx, sensor.at(sens_idx).second);
        for(size_t mpe_idx = 0; mpe_idx < num_mpe; mpe_idx++) {
            K::FT dist = CGAL::squared_distance(mpe_position.at(mpe_idx), sens_pos);
            if(mpe_radius.at(mpe_idx) == -1 || dist <= mpe_radius.at(mpe_idx)) {
                // only add it to equation if it's reachable or if radius == -1 (infinity)
                lp.set_a(mpe_idx, sens_idx,  ET(1)/dist);  
            }
        }
    }
    lp.set_b(num_sensors, max_intensity);
    lp.set_r(num_sensors, CGAL::SMALLER); // override default >= to <=
    for(size_t i = 0; i < num_sensors; i++) {
        lp.set_a(i, num_sensors, 1);
        lp.set_c(i, 1);        // objective function: minimize sum(ip)
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));
    if (s.status() == CGAL::QP_INFEASIBLE) {
        return false;
    } else {
        return true;
    }
}

void run_testcase() {
    size_t num_sensors, num_mpe, num_henchmen, max_intensity;
    cin >> num_sensors >> num_mpe >> num_henchmen >> max_intensity;
    vector<pair<P, size_t> > sensor(num_sensors);

    for(size_t i = 0; i < num_sensors; i++) {
        P pos;
        cin >> pos;
        size_t deactivation_energy; cin >> deactivation_energy;
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


    for(size_t k = 1; k <= num_mpe; k++) {
        if(feasible(mpe_position, mpe_radius, sensor, max_intensity, k)) {
            cout << k << endl;
            return;
        }
    }
    cout << "impossible" << endl;
}

int main() {
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) run_testcase();
    return 0;
}