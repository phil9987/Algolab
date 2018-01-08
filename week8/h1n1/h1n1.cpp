#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef K::Point_2 P;

using namespace std;

void do_testcase(size_t num_infected_people) {
    // read points
    vector<P> infection_coord(num_infected_people);
    for (std::size_t i = 0; i < num_infected_people; i++) {
        cin >> infection_coord.at(i);
    }
    // construct triangulation
    Triangulation t;
    t.insert(infection_coord.begin(), infection_coord.end());
    
    size_t num_users; cin >> num_users;
    vector<P> user_coord(num_users);
    vector<size_t> user_dist(num_users);
    for(size_t i = 0; i < num_users; i++) {
        cin >> user_coord.at(i) >> user_dist.at(i);
    }
    // output all edges
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        std::cout << t.segment(e) << "\n";
}

int main() {
    size_t n; cin >> n;
    while(n > 0) {
        do_testcase(n);
        cin >> n;
    }
    return 0;
}

