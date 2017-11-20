#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main() {

    while(1) {
        // by default, we have an LP with Ax <= b and a lower bound for all variables
        Program lp (CGAL::SMALLER, true, 0, false, 0);

        int n; cin >> n;       // number of nutrients
        int m; cin >> m;       // number of products
        if (n == 0 && m == 0) {
            break;
        }

        vector<pair<int,int> >  minmax(n);
        vector<vector<int> > productnutrients(m, vector<int>(n));

        // read the nutrient min and max values
        for (int i=0; i<n; i++) {
            int min; cin >> min;
            int max; cin >> max;
            minmax.at(i) = make_pair(min, max);
        }

        // read the product information
        for (int i=0; i<m; i++) {
            int price; cin >> price;
            lp.set_c(i, price);     // objective funciton: minimize total price (product * amount)

            for (int j=0; j<n; j++) {
                cin >> productnutrients.at(i).at(j);
            }
        }

        for(int nutr = 0; nutr < n; nutr ++) {
            for(int prod = 0; prod < m; prod++) {
                lp.set_a(prod, nutr, productnutrients.at(prod).at(nutr));
                lp.set_a(prod, nutr + n, productnutrients.at(prod).at(nutr));

            }
            lp.set_b(nutr, minmax.at(nutr).second); // set <= max
            lp.set_b(nutr + n, minmax.at(nutr).first); // set >= min
            lp.set_r(nutr + n, CGAL::LARGER);
        }

        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_linear_program(lp, ET());
        assert (s.solves_linear_program(lp));

        // output exposure center and radius, if they exist
        if (s.is_optimal()) {
            cout << (int)to_double(s.objective_value()) << endl;
        } else {
            cout << "No such diet." << endl;
        }
    }
    
    return 0;
}
