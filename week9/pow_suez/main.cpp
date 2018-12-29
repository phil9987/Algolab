#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <math.h>


typedef CGAL::Gmpq ET;

// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

double ceil_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcases() {

    int n, m, h, w;
    std::cin >> n >> m >> h >> w;

    std::vector<K::Point_2> newPosters;
    newPosters.reserve(n);
    // read nail coordinates of new posters
    for (int i = 0; i < n; i++) {
        K::Point_2 p;
        std::cin >> p;
        newPosters.push_back(p);
    }
    std::vector<K::Point_2> oldPosters;
    oldPosters.reserve(m);
    // read nail coordinates of old posters
    for (int i = 0; i < m; i++) {
        K::Point_2 p;
        std::cin >> p;
        oldPosters.push_back(p);
    }

    // by default, we have a nonnegative LP with Ax <= b
    Program lp (CGAL::SMALLER, true, 1, false, 0);
    int constraint_idx = 0;
    for(int i = 0; i < n; i++) {
        for(int j = i+1; j < n; j++) {
            K::Point_2 a = newPosters[i];
            K::Point_2 b = newPosters[j];
            int x_dist = std::abs(a.x() - b.x());
            int y_dist = std::abs(a.y() - b.y());
            lp.set_a(i, constraint_idx, 1);
            lp.set_a(j, constraint_idx, 1);
            lp.set_b(constraint_idx, 2*CGAL::max(ET(x_dist)/w, ET(y_dist)/h));
            constraint_idx++;
        }
    }

    for(int i = 0; i < n; i++) {
        ET limit = 33554432;        // 2**25
        for(int j = 0; j < m; j++) {
            K::Point_2 a = newPosters[i];
            K::Point_2 b = oldPosters[j];
            int x_dist = std::abs(a.x() - b.x());
            int y_dist = std::abs(a.y() - b.y());
            ET current_limit = 2*CGAL::max(ET(x_dist)/w, ET(y_dist)/h) - 1;
            limit = CGAL::min(limit, current_limit);
        }
        lp.set_a(i, constraint_idx, 1);
        lp.set_b(constraint_idx, limit);
        constraint_idx++;
    }

    for(int i = 0; i < n; i++) {
        lp.set_c(i, -2*(w+h));
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));
    std::cout << ceil_to_double(-s.objective_value()) << std::endl;
    /*SolT totalPerimeter = 0;
    for(Solution::Variable_value_iterator opt = s.variable_values_begin(); opt < s.variable_values_end(); ++opt) {
        SolT a = opt->numerator() / opt->denominator();
        //std::cout << ceil_to_double(a) << " ";
        //std::cout << ceil_to_double(*opt) << " ";

        totalPerimeter += a*w*2 + a*h*2;
    }
    //std::cout << std::endl;

    // output solution
    std::cout << ceil_to_double(totalPerimeter) << std::endl;*/
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
    std::cout << std::setprecision(0) << std::fixed;
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcases();
	return 0;
}
