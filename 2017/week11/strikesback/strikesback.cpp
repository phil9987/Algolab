#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <algorithm>
#include <CGAL/Delaunay_triangulation_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;


using namespace std;

// choose exact integral type
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef pair<P, int> Asteroid;

struct Closer_to_l{
    Closer_to_l(P l) { this->l = l; }
    P l;
    bool operator()(P a, P b) {
        return CGAL::squared_distance(l, a) <= CGAL::squared_distance(l, b);   
    }
};

void do_testcase() {
    size_t num_asteroids, num_lasershots, num_bhunters;
    cin >> num_asteroids >> num_lasershots >> num_bhunters;
    size_t energy; cin >> energy;

    // by default, we have a nonnegative LP with Ax <= b
    Program lp (CGAL::SMALLER, true, 0, false, 0);

    vector<Asteroid> asteroids(num_asteroids);
    for(size_t i = 0; i < num_asteroids; i++) {
        P coordinate;
        cin >> coordinate;
        int density; cin >> density;
        asteroids.at(i) = make_pair(coordinate, density);
    }

    vector<P> lasershots(num_lasershots);
    for(size_t i = 0; i < num_lasershots; i++) {
        cin >> lasershots.at(i);
    }

    vector<P> bhunters(num_bhunters);
    for(size_t i = 0; i < num_bhunters; i++) {
        cin >> bhunters.at(i);
    }
    vector<K::FT> radiusses(num_lasershots, -1);
    // calculate radius for every laser
    if(bhunters.size() > 0) {
        Triangulation tr;
        tr.insert(bhunters.begin(), bhunters.end());

        for(size_t i = 0; i < num_lasershots; i++) {
            Triangulation::Vertex_handle v = tr.nearest_vertex(lasershots.at(i));
            P nearest_bhunter = v->point();
            radiusses.at(i) = CGAL::squared_distance(lasershots.at(i), nearest_bhunter);
        }
    }

    vector<vector<ET> > distances_per_asteroid(num_asteroids, vector<ET>(num_lasershots));
    for(size_t i = 0; i < num_asteroids; i++) {
        Asteroid a = asteroids.at(i);
        lp.set_b(i, a.second); lp.set_r(i, CGAL::LARGER);  // sum of energy per lasershot is >= density of asteroid
        for(size_t j = 0; j < num_lasershots; j++) {
            K::FT d = CGAL::squared_distance(a.first, lasershots.at(j));
            if(d < 1) { // d = max(1,d)
                d = 1;
            }
            if(d <= radiusses.at(j) || radiusses.at(j) == -1) {
                lp.set_a(j, i, ET(1)/d); 
            }
        }
    }

    // sum(e_i) <= energy
    for(size_t i = 0; i < num_lasershots; i++) {
        lp.set_a(i, num_asteroids, 1);
        lp.set_c(i, 1); // objective function: sum of energy
    }
    lp.set_b(num_asteroids, energy);

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));
    if(s.status() == CGAL::QP_OPTIMAL) {
        cout << "y" << endl;
    } else {
        cout << "n" << endl;
    }

}

int main() {
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) {
        do_testcase();
    }
    return 0;
}