#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose exact integral type
typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;


typedef CGAL::Quadratic_program<long> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

double floor_to_double(const SolT& x) {
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}

void do_testcase(){
	// by default, we have a nonnegative LP with Ax <= b
	Program lp (CGAL::SMALLER, true, 0, false, 0); 
	
	int num_warehouses, num_stadiums;
	long num_contourlines;
	cin >> num_warehouses >> num_stadiums >> num_contourlines;
	vector<pair< long, long> > warehouses(num_warehouses);
	vector<pair< long, long> > stadiums(num_stadiums);
	vector<vector<int> > rewards(num_warehouses);
	for(int i = 0; i < num_warehouses; i++) {
		long x, y, supply, alcohol;
		cin >> x >> y >> supply >> alcohol;
		warehouses.at(i) = make_pair(supply, alcohol);
	}
	for(int i = 0; i < num_stadiums; i++){
		long x, y, demand, max_alcohol;
		cin >> x >> y >> demand >> max_alcohol;
		stadiums.at(i) = make_pair(demand, max_alcohol);
	}
	for(int i = 0; i < num_warehouses; i++) {
		vector<int> tmp_rewards(num_stadiums);
		for(int j = 0; j < num_stadiums; j++) {
			cin >> tmp_rewards.at(j);		
		}
		rewards.at(i) = tmp_rewards;
	}
	for(int i = 0; i < num_contourlines; i++) {
		long x, y, radius;
		cin >> x >> y >> radius;
	}
	
	// (1) sum of supplies for stadium = demand of stadium

	// (2) sum of supplies of pure alcohol per stadium <= max total pure alcohol per stadium

	for(int s = 0; s < num_stadiums; s++) {
		for(int w = 0; w < num_warehouses; w++) {
			// every stadium has num_warehouses variables
			lp.set_a(s*num_warehouses + w, s, 1);		// (1)

			lp.set_a(s*num_warehouses + w, num_stadiums + s, warehouses.at(w).second);	// (2) set alcohol in %
		}
		lp.set_r(					   	   s, CGAL::EQUAL);    // (1) supply needs to meet demand exactly
		lp.set_b(                          s, stadiums.at(s).first);       // (1) demand
		
		lp.set_b(                          num_stadiums + s, stadiums.at(s).second * 100);	// (2)

	}

	// sum of outgoing supplies of warehouse <= max supply of warehouse
    for(int w = 0; w < num_warehouses; w++) {
        for(int s = 0; s < num_stadiums; s++) {
            lp.set_a(s*num_warehouses + w, (2*num_stadiums)+w, 1);	
        }
        lp.set_b(                          (2*num_stadiums)+w, warehouses.at(w).first);	// supply
    }

    
    
    // objective function: -sum of rewards (the solver minimizes)
    for(int s = 0; s < num_stadiums; s++) {
        for(int w = 0; w < num_warehouses; w++) {
            lp.set_c(s*num_warehouses + w, -rewards.at(w).at(s));
        }
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    // output exposure center and radius, if they exist
    if (s.is_optimal()) {

		if(s.objective_value() > 0) {
			// result is negative
			cout << -floor_to_double(s.objective_value()) << endl;
		} else {
			cout << floor_to_double(-s.objective_value()) << endl;
		}
    } else
        std::cout << "RIOT!" << endl;
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int T; cin >> T;
	cout << setiosflags(std::ios::fixed) << setprecision(0);
	for (int t = 0; t < T; t++) {
		do_testcase();
	}
	
}
