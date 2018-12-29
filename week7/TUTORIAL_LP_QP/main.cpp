#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <map>

typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;


void testcases() {

    int n, m, h, w;
    std::cin >> n >> m >> h >> w;
    std::map<K::Point_2, int> pointToIdx;

    std::vector<K::Point_2> newPosters;
    newPosters.reserve(n);
    // read nail coordinates of new posters
    for (int i = 0; i < n; i++) {
        K::Point_2 p;
        std::cin >> p;
        newPosters.push_back(p);
        pointToIdx[p] = i;
    }
    std::vector<K::Point_2> oldPosters;
    oldPosters.reserve(m);
    // read nail coordinates of old posters
    for (int i = 0; i < m; i++) {
        K::Point_2 p;
        std::cin >> p;
        oldPosters.push_back(p)
    }
    // by default, we have a nonnegative LP with Ax <= b
    /*Program lp (CGAL::SMALLER, true, 1, false, 0);
    Triangulation t;
    t.insert(newPosters.begin(), newPosters.end());

    for(int i = 0; i < n; i++) {
        // version 0, only x constraint because all nails are on y axis
        Comparison_result CGAL::compare_distance_to_point 	( 	const CGAL::Point_2< Kernel > &  	p,
		    const CGAL::Point_2< Kernel > &  	q,
		    const CGAL::Point_2< Kernel > &  	r 
	    ) 	
        lp.set_a(i, i, );

    }

    // now set the non-default entries
    const int X = 0;
    const int Y = 1;
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 7);  //  x + y  <= 7
    lp.set_a(X, 1, -1); lp.set_a(Y, 1, 2); lp.set_b(1, 4);  // -x + 2y <= 4
    lp.set_u(Y, true, 4);                                   //       y <= 4
    lp.set_c(Y, -32);                                       // -32y
    lp.set_c0(64);                                          // +64

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    // output solution
    std::cout << s;
return 0;*/
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcases();
	return 0;
}
