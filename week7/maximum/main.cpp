#include <iostream>
#include <fstream>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


using namespace std;

// round up to next integer double
double ceil_to_double(const SolT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

// round down to next integer double
double floor_to_double(const SolT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}
int main() {
    int p, a, b; cin >> p;
    while(p > 0) {
        cin >> a >> b;
        if(p == 1) {
            // by default, we have a nonnegative QP with Ax >= b
            Program qp (CGAL::LARGER, true, 0, false, 0); 
            
            // now set the non-default entries: 
            const int X = 0; 
            const int Y = 1;

            qp.set_a(X, 0, 1);
            qp.set_a(Y, 0, 1);
            qp.set_b(    0, 4);
            qp.set_r(0, CGAL::SMALLER);

            qp.set_a(X, 1, 4);
            qp.set_a(Y, 1, 2);
            qp.set_b(1, a * b);
            qp.set_r(1, CGAL::SMALLER);

            qp.set_a(X, 2, -1);
            qp.set_a(Y, 2, 1);
            qp.set_b(2, 1);
            qp.set_r(2, CGAL::SMALLER);

            qp.set_d(X, X, 2*a);
            qp.set_c(Y, -b);

            assert(qp.is_nonnegative());
            Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
            assert(s.solves_quadratic_program(qp));

            assert (s.solves_quadratic_program(qp));
            switch (s.status()){
                case CGAL::QP_INFEASIBLE:
                    cout << "no" << endl;
                    break;

                case CGAL::QP_UNBOUNDED:
                    cout << "unbounded" << endl;
                    break;

                case CGAL::QP_OPTIMAL:
                    int result = 0;
                    ET object =  s.objective_value().numerator() / s.objective_value().denominator();
                
                    object = - object;
                    result = floor_to_double(object);
                    cout << result << endl;
                    break;

            }
        } else {
             Program qp (CGAL::LARGER, false, 0, false, 0); 
            
            // now set the non-default entries: 
            const int X = 0; 
            const int Y = 1;
            const int Z = 2;

            qp.set_a(X, 0, 1);
            qp.set_a(Y, 0, 1);
            qp.set_a(Z, 0, 0);
            qp.set_b(    0, -4);

            qp.set_a(X, 1, 4);
            qp.set_a(Y, 1, 2);
            qp.set_a(Z, 1, 1);
            qp.set_b(1, -a * b);

            qp.set_a(X, 2, -1);
            qp.set_a(Y, 2, 1);
            qp.set_a(Z, 2, 0);
            qp.set_b(2, -1);

            qp.set_d(X, X, 2*a);
            qp.set_d(Z, Z, 2);
            qp.set_c(Y, b);
            qp.set_l(Z, true, 0);
            qp.set_u(X, true, 0);
            qp.set_u(Y, true, 0);
            Solution s = CGAL::solve_quadratic_program(qp, ET());
            assert(s.solves_quadratic_program(qp));

            assert (s.solves_quadratic_program(qp));
            switch (s.status()){
                case CGAL::QP_INFEASIBLE:
                    cout << "no" << endl;
                    break;

                case CGAL::QP_UNBOUNDED:
                    cout << "unbounded" << endl;
                    break;

                case CGAL::QP_OPTIMAL:
                    int result = 0;
                    ET object =  s.objective_value().numerator() / s.objective_value().denominator();
                    result = ceil_to_double(object);
                    cout << result << endl;
                    break;

            }
            

        }

        cin >> p;
    }
}