// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
using namespace std;
using namespace boost;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property>	Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;			
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex


pair<bool, vector<int> > do_not_interfer(Graph& G, int n, vector<pair<P, int> > & stations, long r2) {
    vector<int> componentmap(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int ncc = connected_components(G, make_iterator_property_map(componentmap.begin(), get(vertex_index, G)));
    int current_comp = 0;
    int valid = true;
    vector<int> freq(n, 0);
    vector<P> stations1;
    vector<P> stations2;
    for(int i = 0; i < n; i++) {
        if(componentmap[i] == current_comp) {
            current_comp++;
            // run bfs on this component and check if we can find a correct assignment for all vertices
            queue<int> q;
            q.push(i);
            freq[i] = 1;
            stations1.push_back(stations.at(i).first);
            while (not q.empty() && valid) {
                int v = q.front();
                q.pop();
                OutEdgeIt oebeg, oeend;
                for (tie(oebeg, oeend) = out_edges(v, G); oebeg != oeend; ++oebeg) {
                    Vertex u = target(*oebeg, G);
                    if(freq[u] == 0) {
                        // assign freq[u] to the other frequency then freq[v]
                        if(freq[v] == 1) {
                            freq[u] = 2;
                            stations2.push_back(stations.at(u).first);
                        } else {
                            freq[u] = 1;
                            stations1.push_back(stations.at(u).first);
                        }
                        q.push(u);
                    } else if(freq[v] == freq[u]) {
                        // u already has a freq assigned and it is the same like v -> 2-coloring not possible
                        valid = false;
                    }
                }
            }
        }
    }

    // a triangulation might not contain the shortes edge between two points, so we need to verify if the shortest distance between any point in 1-freq is smaller than r2 and same for 2-freq
    Triangulation t1;
    t1.insert(stations1.begin(), stations1.end());
    Triangulation t2;
    t2.insert(stations2.begin(), stations2.end());
    for (Edge_iterator e = t1.finite_edges_begin(); e != t1.finite_edges_end() && valid; ++e){
        auto seg = t1.segment(e);
        if(CGAL::squared_distance(seg.source(), seg.target()) <= r2) {
            valid = false;
        }
    }
    for (Edge_iterator e = t2.finite_edges_begin(); e != t2.finite_edges_end() && valid; ++e){
        auto seg = t2.segment(e);
        if(CGAL::squared_distance(seg.source(), seg.target()) <= r2) {
            valid = false;
        }
    }
    return make_pair(valid, componentmap);
}

void testcase() {
    int n, m;
    long r;
    cin >> n >> m;
    cin >> r;
    long r2 = r*r;

    // read in stations
    vector<pair<P, int> > stations(n);
    for(int i = 0; i < n; i++) {
        P p; cin >> p;
        stations.at(i) = make_pair(p, i);
    }

    vector<pair<P, P> > clues(m);
    for(int i = 0; i < m; i++) {
        P sherlock, watson;
        cin >> sherlock >> watson;
        clues.at(i) = make_pair(sherlock, watson);
    }

    int V = n;
	Graph G(V);	// creates an empty graph on n vertices

    // construct triangulation
    Triangulation t;
    t.insert(stations.begin(), stations.end());
    // output all edges
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
        Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
        Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
        if(CGAL::squared_distance(v1->point(), v2->point()) <= r2) {
            // add edge to graph
            add_edge(v1->info(), v2->info(), G);
        }
    }
    auto res = do_not_interfer(G, n, stations, r2);
    bool valid = res.first;
    if(valid) {
        auto componentmap = res.second;
        for(int i = 0; i < m; i++) {
            P sherlock, watson;
            tie(sherlock, watson) = clues.at(i);
            if(CGAL::squared_distance(sherlock, watson) <= r2) {
                cout << "y";
            } else {
                Triangulation::Vertex_handle vsherlock = t.nearest_vertex(sherlock);
                Triangulation::Vertex_handle vwatson = t.nearest_vertex(watson);
                if( CGAL::squared_distance(vsherlock->point(), sherlock) <= r2 && 
                    CGAL::squared_distance(vwatson->point(), watson) <= r2 && 
                    componentmap[vsherlock->info()] == componentmap[vwatson->info()] ) {
                    cout << "y";
                } else {
                    cout << "n";
                }
            }
        }
        cout << endl;

    } else {
        for(int i = 0; i < m; i++) {
            cout << "n";
        }
        cout << endl;
    }
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
