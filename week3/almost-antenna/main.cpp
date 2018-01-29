#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <algorithm>
#include <set>
#include <cassert>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2 P;

using namespace std;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}
 
int main() {
    std::ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n > 0) {      // process all testcases

        set<K::Point_2> points;        

        for (int i = 0; i < n; ++i) {
            long x, y; cin >> x >> y;
            K::Point_2 p(x, y);
            points.insert(p);
        }
        Min_circle mc(points.begin(), points.end(), true);
        K::FT min_sq_radius = mc.circle().squared_radius();
        for (auto support_point = mc.support_points_begin(); support_point != mc.support_points_end(); ++support_point) {
            points.erase(*support_point);
            Min_circle mc1(points.begin(), points.end(), true);
            min_sq_radius = min(min_sq_radius, mc1.circle().squared_radius());
            points.insert(*support_point);
        }
        K::FT d = sqrt(min_sq_radius);
        std::cout << (long) ceil_to_double(d) << std::endl;
        
        cin >> n;
    }
}