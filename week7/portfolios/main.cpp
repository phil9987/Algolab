// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
//#include "prettyprint.hpp"

// choose exact integral type
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int testcase(size_t n, size_t m) {

    vector<pair<int, int> > assets(n);
    for(size_t i = 0; i < n; i++) {
        size_t c, r;
        cin >> c >> r;
        assets.at(i) = make_pair(c, r);
    }

    vector<vector<long> > cov_matrix(n, vector<long>(n));
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            cin >> cov_matrix.at(i).at(j);
        }
    }

    vector<int> max_cost(m);
    vector<int> min_return(m);
    vector<int> max_variance(m);
    for(size_t i = 0; i < m; i++) {
        cin >> max_cost.at(i);
        cin >> min_return.at(i);
        cin >> max_variance.at(i);
    }

    //cout << assets << endl;
    //cout <<cov_matrix << endl;
    //cout << max_cost << endl;
    //cout << min_return << endl;
    //cout << max_variance << endl;

    for(size_t p = 0; p < m; p++) {
        Program qp (CGAL::SMALLER, true, 0, false, 0);
        
        
        // expected return >= R
        for(size_t i = 0; i < n; i++) {
            qp.set_a(i, 0, assets.at(i).second);
        }
        qp.set_b(0, min_return.at(p));
        qp.set_r(0, CGAL::LARGER);

        // total cost <= C
        for(size_t i = 0; i < n; i++) {
            qp.set_a(i, 1, assets.at(i).first);
        }
        qp.set_b(1, max_cost.at(p));

        for(size_t i = 0; i < n; i++) {
            for(size_t j = 0; j <= i; j++) {
                    qp.set_d(i, j, 2*cov_matrix.at(i).at(j));
            }
        }
        assert(qp.is_nonnegative());
        Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
        if(s.is_optimal() && s.objective_value() <= max_variance.at(p)) {
            cout << "Yes." << endl;
        } else {
            cout << "No." << endl;
        }
    }

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	size_t n, m;
    cin >> n >> m;
    while(n > 0 && m > 0) {
        testcase(n, m);
        cin >> n >> m;
    }
	return 0;
}
