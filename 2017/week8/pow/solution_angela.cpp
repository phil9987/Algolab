#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <utility>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

// map gangster to information triple and if an agent already watches him
//	gangster i: P(x, y) -> ui, vi, wi, aj
//for each agent find nearest gangster using triangulation
// agent j: ( fee j, nearest gangster)
//sort agents according their fee, look at it as only agent can observe one gangster and not two at the same time 
//		=> use the agent with smallest fee first
//get equations
//	minimize sum of (zi * hi)  -- fee of agent i * hours he has to work
//	hi >= 0
//	hi <= 24
//	sum(hi * ui) >= u
//	sum(hi * vi) >= v
//	sum(hi * wi) >= w

// WTF MOMENTS
// - nonnegative solver removes all lower and upper bound!!!!!!
// - compare function needs strict order


typedef CGAL::Gmpq ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

bool compare(pair<int, P> a1, pair<int, P> a2) {
	return a1.first < a2.first;
}

double ceil_to_double(const SolT& x)
{
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void lestrade() {
	double sherlock_fee;
	int u, v, w;
	cin >> sherlock_fee >> u >> v >> w;

	int num_agents, num_gangsters;
	cin >> num_agents >> num_gangsters;
	
	vector<P> g_pts(num_gangsters); //used for fast read in triangulation
	map<P, vector<int> > gangsters;
	
	for (int i = 0; i < num_gangsters; i++) {
		P p;
		cin >> p;
		g_pts.at(i) = p;
		
		vector<int> g(4);
		cin >> g.at(0) >> g.at(1) >> g.at(2);
		g.at(3) = -1;
		gangsters[p] = g;
	}
	Triangulation triangulation;
	triangulation.insert(g_pts.begin(), g_pts.end());
	
	vector<pair<int, P> > agents(num_agents); //[fee/hour, coord of nearest gangster]

	for (int i = 0; i < num_agents; i++) {
		P a;
		cin >> a;
		int agent_fee;
		cin >> agent_fee;

		P nearest_gangster = triangulation.nearest_vertex(a) -> point();
		agents.at(i) = make_pair(agent_fee, nearest_gangster);
	}

	sort(agents.begin(), agents.end(), compare);

	Program lp (CGAL::SMALLER, true, 0, true, 24);

	for (int i = 0; i < num_agents; i++) {
		vector<int> gangster = gangsters.at(agents.at(i).second);
		
		if (gangster.at(3) != -1) {
			continue;
		} else {
			gangsters.at(agents.at(i).second).at(3) = i;
		}
		lp.set_a(i, 0, - gangster.at(0)); 
		lp.set_a(i, 1, - gangster.at(1));
		lp.set_a(i, 2, - gangster.at(2));
		lp.set_c(i, agents.at(i).first);

	}
	lp.set_b(0, -u);
	lp.set_b(1, -v);
	lp.set_b(2, -w);

	Solution solution = CGAL::solve_linear_program(lp, ET());

	if (solution.is_optimal() && ceil_to_double(solution.objective_value()) <= sherlock_fee) {
		cout << "L" << endl;
	} else {
		cout << "H" << endl;
	}
	
}

int main () {
	std::ios_base::sync_with_stdio(false);
	
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		lestrade();
	}
	return 0;
}
