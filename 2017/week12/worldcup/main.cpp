#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Circle_2 C;

#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;

// choose exact integral type
typedef CGAL::Gmpq ET;
typedef CGAL::Quotient<ET> SolT;


typedef CGAL::Quadratic_program<ET> Program;
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
    vector<P> warehouse_location(num_warehouses);
    vector<P> stadium_location(num_stadiums);
	vector<pair< long, long> > stadiums(num_stadiums);
	vector<vector<int> > rewards(num_warehouses);
    vector<unsigned> warehouse_contour_count(num_warehouses, 0);
    vector<unsigned> stadium_contour_count(num_stadiums, 0);
    vector<vector<int> > contour_count(num_stadiums, vector<int>(num_warehouses, 0));
	for(int i = 0; i < num_warehouses; i++) {
        P loc;
		long supply, alcohol;
		cin >> loc >> supply >> alcohol;
		warehouses.at(i) = make_pair(supply, alcohol);
        warehouse_location.at(i) = loc;
	}
	for(int i = 0; i < num_stadiums; i++){
		long demand, max_alcohol;
        P loc;
		cin >> loc >> demand >> max_alcohol;
		stadiums.at(i) = make_pair(demand, max_alcohol);
        stadium_location.at(i) = loc;
	}
	for(int i = 0; i < num_warehouses; i++) {
		vector<int> tmp_rewards(num_stadiums);
		for(int j = 0; j < num_stadiums; j++) {
			cin >> tmp_rewards.at(j);		
		}
		rewards.at(i) = tmp_rewards;
	}

    Triangulation t;
    t.insert(warehouse_location.begin(), warehouse_location.end());
    t.insert(stadium_location.begin(), stadium_location.end());
    
	for(int cont = 0, num_filled_contour_lines = 0; cont < num_contourlines && num_filled_contour_lines <= 100; cont++) {
		long radius;
        P loc;
		cin >> loc >> radius;
        
        long radius2 = radius*radius;
        
        P nearest = t.nearest_vertex(loc)->point();
        if(CGAL::squared_distance(nearest, loc) < radius2) {
            // there is something inside contourline!
            C c(loc, radius2);
            vector<int> w_contains_cont(num_warehouses, -1);
            for(size_t s = 0; s < num_stadiums; s++) {
                bool s_in_cont = c.has_on_bounded_side(stadium_location.at(s));
                for(size_t w = 0; w < num_warehouses; w++) {
                    if(w_contains_cont.at(w) == -1) {
                        w_contains_cont.at(w) = c.has_on_bounded_side(warehouse_location.at(w));
                    }
                    bool w_in_cont = w_contains_cont.at(w);
                    if((w_in_cont && !s_in_cont) || (!w_in_cont && s_in_cont)) {
                        contour_count.at(s).at(w)++;
                    }
                }
            }
        }
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
            lp.set_c(s*num_warehouses + w, -(ET(rewards.at(w).at(s))/1 - ET(contour_count.at(s).at(w))/100 ));
        }
    }
    
    //clog << "preparation done" << endl;

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    // output exposure center and radius, if they exist
    if (s.is_optimal()) {
        
        cout << floor_to_double(-s.objective_value()) << endl;

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
