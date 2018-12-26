// idea: The adversary starts at planet 0. From planet_idx 1..n we want to see if we find a connected component of size planet_idx. Because planet_idx indicates how many time steps we have until the emperor reaches one of our planets, we cannot reach more planets than planet_idx. The maximum number of planets we can reach is num_planets/2+1, because at every timestep the emperor can conquer one and we can conquer one.

#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
using namespace std;
using namespace boost;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator Edge_it;
typedef Triangulation::Vertex_handle Vertex_handle;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property>	Graph;
typedef graph_traits<Graph>::edge_descriptor		                        Edge;		




int max_connected_comp(vector<pair<K::Point_2, int> >& planets, size_t l, size_t max_sq_dist) {
    size_t V = planets.size()-l;
    Triangulation t;
    t.insert(planets.begin()+l, planets.end());

    Graph G(V);

    for(Edge_it e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
        Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
        Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);

        if(CGAL::squared_distance(v1->point(), v2->point()) <= max_sq_dist) {
            size_t source = v1->info() - l;
            size_t target = v2->info() - l;
            Edge e;	bool success;
            tie(e, success) = add_edge(source, target, G);
        }
    }

    vector<int> componentmap(V);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int ncc = connected_components(G, make_iterator_property_map(componentmap.begin(), get(vertex_index, G))); 
	vector<int> componentsize(ncc);
	// Iterate over all vertices
	for (int i = 0; i < V; ++i)
		++componentsize[componentmap[i]];
	
    return *max_element(componentsize.begin(), componentsize.end());
}

void do_testcase() {
    size_t num_planets, range; cin >> num_planets >> range;
    vector<pair<K::Point_2, int> > planets(num_planets);   // pairs of (point, already_used)

    for(int i = 0; i < num_planets; i++) {
        int x, y; cin >> x >> y;
        K::Point_2 p(x,y);
        planets.at(i) = make_pair(p, i);
    }
    size_t max_sq_dist = range*range;
    int k_left = 1;                     // connected component of size 1 is always possible
    int k_right = num_planets/2 + 1;    // max_connected_comp(i)  < i for i = num_planets/2+1 .. num_planets
    while(k_right - k_left > 1) {
        int k = k_left + (k_right - k_left) / 2;
        int m = max_connected_comp(planets, k, max_sq_dist);
        if(k <= m) {
            k_left = k;
        } else {
            k_right = k;
        }
    } 
    cout << k_left << endl;
}

int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        do_testcase();
    }

    return 0;
}