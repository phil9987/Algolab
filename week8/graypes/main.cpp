// If somebody from the CGAL development team reads this: Your library is really difficult to understand and use!

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

using namespace std;

double ceil_to_double(const K::FT& x){
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}
int main() {
    // read number of points
    std::size_t n;
    std::cin >> n;

    while(n > 0) {

        // read points
        std::vector<K::Point_2> pts;
        pts.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            K::Point_2 p;
            std::cin >> p;
            pts.push_back(p);
        }
        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        K::FT min_dist = K::FT(-1);
        // output all edges
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
            Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
            Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
            //std::cout << "e = " << v1->point() << " <-> " << v2->point() << std::endl;
            K::FT dist = squared_distance(v1->point(), v2->point());
            if(dist < min_dist || min_dist < K::FT(0)) {
                //cout << "new min_dist: " << dist << endl;
                min_dist = dist;
            }
        }
        cout << ceil_to_double(sqrt(min_dist) * 50) << endl;
        cin >> n;
    }

}
