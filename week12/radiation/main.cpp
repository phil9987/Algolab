// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>


// choose exact integral type
typedef CGAL::Gmpzf ET;
typedef CGAL::Quotient<ET> SolT;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// Namespaces
using namespace std;

map<pair<int, int>, double> memo;
// Functions
// =========
// calculates a^b
double power(int a, int b) {
	double res = 1;
	pair<int,int> p = make_pair(a,b);
	if(memo.find(p) == memo.end()) {
		for(int i = b; i > 0; i--) {
			res *= a;
		}
		memo[p] = res;
	} else {
		res = memo[p];
	}
	
	return res;
}
bool polynomial_possible(int d, vector<int>& h_x, vector<int>& h_y, vector<int>& h_z,
								vector<int>& t_x, vector<int>& t_y, vector<int>& t_z) {
	bool res = false;
	int h = h_x.size();
	int t = t_x.size();
	if(d == 0) {
		res = (h == 0 || t == 0);
	} else {
		// check if a polynomial of degree d exists
		Program lp (CGAL::SMALLER, false, 0, false, 0);
		CGAL::Quadratic_program_options options;
		options.set_pricing_strategy(CGAL::QP_BLAND);
		int eq_idx = 0;

		for(int i = 0; i < h; i++) {
			int var_idx = 0;
			for(int a = 0; a <= d; a++) {
				for(int b = 0; b <= d; b++) {
					for(int c = 0; c <= d; c++) {
						if( a + b + c <= d) {		// we want to create a polynomial of degree d
							lp.set_a(var_idx++, eq_idx, power(h_x.at(i), a) * power(h_y.at(i), b) * power(h_z.at(i), c));
						}
					}
				}
			}
			lp.set_a(var_idx, eq_idx, 1);
			lp.set_b(eq_idx, 0);
			eq_idx++;
		}

		int delta_idx = 0;

		for(int i = 0; i < t; i++) {
			int var_idx = 0;
			for(int a = 0; a <= d; a++) {
				for(int b = 0; b <= d; b++) {
					for(int c = 0; c <= d; c++) {
						if(a + b + c <= d) {		// we want to create a polynomial of degree d
							lp.set_a(var_idx++, eq_idx, power(t_x.at(i), a) * power(t_y.at(i), b) * power(t_z.at(i), c));
						}
					}
				}
			}
			lp.set_b(eq_idx, 0);
			delta_idx = var_idx;
			lp.set_a(delta_idx, eq_idx, -1);
			lp.set_r(eq_idx, CGAL::LARGER);
			eq_idx++;
		}
		lp.set_u (delta_idx, true, 1);		// enforce a bounded problem
		lp.set_l (delta_idx, true, 0);
		lp.set_c(delta_idx, -1);			// minimize -delta

		Solution s = CGAL::solve_linear_program(lp, ET(), options);
		assert (s.solves_linear_program(lp));

		if(s.is_optimal() && s.objective_value() < 0) {
			res = true;
		} 
	}
	return res;
}


// Function for an individual testcase
void testcase() {
	int h,t; cin >> h >> t;
	vector<int> h_x(h);
	vector<int> h_y(h);
	vector<int> h_z(h);

	vector<int> t_x(t);
	vector<int> t_y(t);
	vector<int> t_z(t);


	for(int i = 0; i < h; i++) {
		// read in healthy cells
		cin >> h_x.at(i);
		cin >> h_y.at(i);
		cin >> h_z.at(i);
	}
	for(int i = 0; i < t; i++) {
		// read in tumor cells
		cin >> t_x.at(i);
		cin >> t_y.at(i);
		cin >> t_z.at(i);
	}

	int optimal_d = -1;
	
	/*for(int d = 0; d <= 30; d++) {
		if(polynomial_possible(d, h_x, h_y, h_z, t_x, t_y, t_z)) {
			optimal_d = d;
			cout  << d << endl;
		}else {
			cout << "!" << d << endl;
		}
	}
	if(optimal_d >= 0) {
		cout << optimal_d << endl;
	} else {
		cout << "Impossible!" << endl;
	}*/
	
	int lmin = 0, lmax = 1;
	while(lmax < 32 && !polynomial_possible(lmax, h_x, h_y, h_z, t_x, t_y, t_z)) lmax*=2;
	while (lmin != lmax) {
		int p = (lmin + lmax)/2;
		if (!polynomial_possible(p, h_x, h_y, h_z, t_x, t_y, t_z))
			lmin = p + 1;
		else
			lmax = p;
	}
	if(lmin <= 30) {
		cout << lmin << endl;
	} else {
		cout << "Impossible!" << endl;
	}
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}