#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iomanip>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef K::Point_2 P;

using namespace std;

// round up to next integer double
double ceil_to_double(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int testcase(long n)
{
    // read points
    std::vector<P> restaurants(n);
    for (std::size_t i = 0; i < n; ++i) {
        cin >> restaurants.at(i);
    }

    // construct triangulation
    Triangulation t;
    t.insert(restaurants.begin(), restaurants.end());

    long m; cin >> m;
    for(int i = 0; i < m; i++) {
        long x, y; cin >> x >> y;
        P p(x, y);
        Triangulation::Vertex_handle nearest_restaurant = t.nearest_vertex(p);
        double dist = CGAL::squared_distance(nearest_restaurant->point(), p);
        cout << dist << endl;
    }
}
// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
    cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
	long n;
    cin >> n;
	while(n > 0) {
        testcase(n);
        cin >> n;
    }
	return 0;
}
