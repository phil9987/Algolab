#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;

typedef CGAL::Triangulation_vertex_base_2<K> Vb;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, IK> vertex_t;
typedef CGAL::Triangulation_face_base_2<IK> face_t;
typedef CGAL::Triangulation_data_structure_2<vertex_t,face_t> triangulation_t;
typedef CGAL::Delaunay_triangulation_2<IK,triangulation_t> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef IK::Point_2 P;
typedef K::FT SolT;
typedef IK::Segment_2 S;

using namespace std;

double ceil_to_double(const SolT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int testcase(size_t n){
    long l, b, r, t; cin >> l >> b >> r >> t;
    vector<P> corners(4);
    corners.at(0) = P(l, t);        // upper left
    corners.at(1) = P(r, t);        // upper right
    corners.at(2) = P(l, b);        // lower left
    corners.at(3) = P(r, b);        // lower right
    
    vector<S> segments(4);
    segments.at(0) = S(corners.at(0), corners.at(1));   // upper
    segments.at(1) = S(corners.at(1), corners.at(3));   // right
    segments.at(2) = S(corners.at(2), corners.at(3));   // lower
    segments.at(3) = S(corners.at(0), corners.at(2));   // left
    vector<IK::FT> sqdistances(n, SIZE_MAX);
    // read points
    std::vector<pair<P, int>> pts(n);
    for(size_t i = 0; i < n; i++) {
        P p; cin >> p;
        pts.at(i) = make_pair(p, i);       // todo add index to triangulation?! -> need to sort by min(distance_to_border, distance_to_closest_point)
        for(size_t j = 0; j < 4; j++) {
            IK::FT dist = 4*CGAL::squared_distance(segments.at(j), pts.at(i).first);
            sqdistances.at(i) = min(sqdistances.at(i),dist);
        }

    }
    if(n > 1) {
        // construct triangulation
        Triangulation tr;
        tr.insert(pts.begin(), pts.end());
        // output all edges
        for (Edge_iterator e = tr.finite_edges_begin(); e != tr.finite_edges_end(); ++e){
            Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
            Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
            //std::cout << "e = " << v1->point() << " <-> " << v2->point() << std::endl;
            P p1 = v1->point();
            P p2 = v2->point();
            IK::FT d = CGAL::squared_distance(p1, p2);
            int i = v1->info();
            int j = v2->info();
            sqdistances.at(i) = min(sqdistances.at(i), d);
            sqdistances.at(j) = min(sqdistances.at(j), d);
        }
    } 

    sort(sqdistances.begin(), sqdistances.end());
    //cout << sqdistances.size() << endl;
    SolT min_dist = sqdistances.front();
    SolT median_dist = sqdistances.at(n/2);
    SolT max_dist = sqdistances.back();

    SolT min_time = 0, median_time = 0, max_time = 0;
    if (min_dist > 0) min_time = sqrt((sqrt(min_dist) - 1) / 2);
    if (median_dist > 0) median_time = sqrt((sqrt(median_dist) - 1) / 2);
    if (max_dist > 0) max_time = sqrt((sqrt(max_dist) - 1) / 2);

    cout << ceil_to_double(min_time) << " " << ceil_to_double(median_time) << " " << ceil_to_double(max_time) << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	size_t n; cin >> n;
	while(n > 0) {
        testcase(n);
        cin >> n;
    }
	return 0;
}

