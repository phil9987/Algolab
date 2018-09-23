// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;


// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void do_testcase(int n, int d) {
    // by default, we have an LP with Ax <= b and no bounds for
    // the four variables alpha, beta, gamma, delta
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    for(size_t i = 0; i < n; i++) {
        double a_sum = 0;
        for(size_t j = 0; j < d; j++) {
            long a_ij;
            cin >> a_ij;
            lp.set_a (j, i, a_ij);  // a_ij * c_ij
            a_sum += a_ij*a_ij;
        }
        int norm = floor(sqrt(a_sum));
        lp.set_a (d, i, norm);      // ||a||*radius
        int bi; cin >> bi;
        lp.set_b (       i, bi);
    }

    // objective function: -radius (the solver minimizes)
    lp.set_c(d, -1);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    // output exposure center and radius, if they exist
    if(s.is_unbounded()) {
        cout << "inf" << endl;
    } else if(s.is_infeasible() || s.objective_value() > 0) {
        cout << "none" << endl;
    } else {
        cout << - s.objective_value().numerator() / s.objective_value().denominator() << endl;
    }
}

int main() {

    int n, d;
    cin >> n;
    while(n > 0) {
        cin >> d;
        do_testcase(n, d);
        cin >> n;
    }

    return 0;
}