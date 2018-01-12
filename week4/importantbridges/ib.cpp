// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>

// BGL includes
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>

// Namespaces
using namespace std;
using namespace boost;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices	
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;

typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges




void do_testcase() {
    size_t num_islands, num_bridges;
    cin >> num_islands >> num_bridges;
	// Create Graph, Vertices and Edges
	// ================================
	int V = num_islands;
	Graph G(V);	// creates an empty graph on n vertices
	int E = num_bridges;
    vector<pair<int, int> > edges(E);
	for (int i = 0; i < E; ++i) {
		Edge e;	bool success;
		Vertex u, v;
        cin >> u >> v;
		tie(e, success) = add_edge(u, v, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.

        edges.at(i) = make_pair(u,v);
	}

	vector<int> componentmap(V);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
    vector<pair<int, int> > critical_bridges;
    for(int i = 0; i < E; ++i) {
        remove_edge(edges.at(i).first, edges.at(i).second, G);
        int ncc = connected_components(G, make_iterator_property_map(componentmap.begin(), get(vertex_index, G))); 
        if(ncc > 1) {
            Vertex u = edges.at(i).first, v = edges.at(i).second;
            if(u > v) {
                int tmp = u;
                u = v;
                v = tmp;
            }
            critical_bridges.push_back(make_pair(u, v));
        }
        add_edge(edges.at(i).first, edges.at(i).second, G);     // restore graph
    }

    /*EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {	
		Vertex u = source(*ebeg, G);
		Vertex v = target(*ebeg, G);
        remove_edge(edges.at(i).first, edges.at(i).second, G);
        int ncc = connected_components(G, make_iterator_property_map(componentmap.begin(), get(vertex_index, G))); 
        if(ncc > 1) {
            critical_bridges.push_back(make_pair(edges.at(i).first, edges.at(i).second));
        }
        add_edge(edges.at(i).first, edges.at(i).second, G);     // restore graph

	}*/
    cout << critical_bridges.size() << endl;
    sort(critical_bridges.begin(), critical_bridges.end());
    for(auto b: critical_bridges) {
        cout << b.first << " " << b.second << endl;
    }
}


int main() {
    size_t T; cin >> T;
    for(int t = 0; t < T; t++) {
        do_testcase();
    }
}