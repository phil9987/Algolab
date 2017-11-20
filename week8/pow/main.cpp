#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <map>
#include <string>

struct Gangm {
	int u; int v; int w; bool observed;
};

struct Agent {
	int x; int y; int z; std::string gangmKey;
};

typedef struct Gangm Gangm;
typedef struct Agent Agent;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 P;


#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <algorithm>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


using namespace std;

int main()
{
	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		
		int z,u,v,w; cin >> z >> u >> v >> w;
		int a,g; cin >> a >> g;
		map<P, Gangm> gangmMap;
        vector<P> pts(g);

        // read in gang members and create triangulation
		for(int i = 0; i < g; i++) {
			P loc;
			int ui,vi,wi;
			cin >> loc >> ui >> vi >> wi;
			Gangm member = {ui, vi, wi, false};
			gangmMap[loc] = member;
            pts.at(i) = loc;
			//cout << loc << endl;
		}

        Triangulation triangulation;
	    triangulation.insert(pts.begin(), pts.end());
		
		vector<pair<int, P> > agents(a); //[fee/hour, coord of nearest gangster]
        // read in agents and select closest gang member for every member
		for (int i = 0; i < a; i++) {
			P agnt_coordinate;
			cin >> agnt_coordinate;
			//cout << agnt_coordinate << endl;
			int agent_fee;
			cin >> agent_fee;
            P nearest_gangster = triangulation.nearest_vertex(agnt_coordinate) -> point();
			//cout << "nearest gangster found: " << nearest_gangster << endl;

			agents.at(i) = make_pair(agent_fee, nearest_gangster);
		}

		std::sort(agents.begin(), agents.end(), [](pair<int,P> a, pair<int,P> b) {
        	return a.first < b.first;   
    		});
		//cout << "sorted" << endl;

		Program lp (CGAL::LARGER, true, 0, true, 24);
		int var_i = 0;
		for(auto agnt: agents) {
			//cout << "looking for gangster at position " << agnt.second << endl;
			Gangm gangster = gangmMap.at(agnt.second);
			if(gangster.observed) {
				continue;
			} else {
				//cout << "gangster at " << agnt.second << " = " << gangmMap[agnt.second].observed << endl;
				gangmMap.at(agnt.second).observed = true;
			}
			lp.set_a (var_i, 0, gangster.u);
			lp.set_a (var_i, 1, gangster.v);
			lp.set_a (var_i, 2, gangster.w);
			lp.set_a (var_i, 3, -agnt.first);
			lp.set_c (var_i, agnt.first);
			var_i++;
		}
		//cout << "." << endl;
		lp.set_b(0, u);
		lp.set_b(1, v);
		lp.set_b(2, w);
		lp.set_b(3, -z);

		Solution s = CGAL::solve_linear_program(lp, ET());
  		assert (s.solves_linear_program(lp));

		if(s.status() == CGAL::QP_INFEASIBLE) {
			cout << "H" << endl;
		} else {
			cout << "L" << endl;
		}
	}
}