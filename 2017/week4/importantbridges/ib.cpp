// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

// BGL includes
#include <boost/graph/biconnected_components.hpp>
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

typedef boost::property_map<Graph, boost::edge_weight_t>::type ComponentMap;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex


int time_;

void do_testcase() {
    time_ = 0;
    size_t num_islands, num_bridges;
    cin >> num_islands >> num_bridges;
	// Create Graph, Vertices and Edges
	// ================================
	int V = num_islands;
	Graph G(V);	// creates an empty graph on n vertices
    ComponentMap componentmap = boost::get(boost::edge_weight, G);
	int E = num_bridges;
	for (int i = 0; i < E; ++i) {
		Edge e;	bool success;
		Vertex u, v;
        cin >> u >> v;
		tie(e, success) = add_edge(u, v, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
	}

    int ncc = boost::biconnected_components(G, componentmap);
    vector<int> componentsize(ncc);
    EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {	
        componentsize.at(componentmap[*ebeg])++;
	}
    set<int> critical_components;
    for(int i = 0; i < ncc; i++) {
        if(componentsize.at(i) == 1) {
            critical_components.insert(i);
        }
    }
    vector<pair<int, int> > critical_bridges;
	for (tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
        int c = componentmap[*ebeg];
        if(critical_components.find(c) != critical_components.end()) {
            Vertex u = boost::source(*ebeg, G), v = boost::target(*ebeg, G);
            if (u > v) {
                int tmp = u;
                u = v;
                v = tmp;
            }
            critical_bridges.push_back(make_pair(u, v));
        }
	}
    sort(critical_bridges.begin(), critical_bridges.end());
    cout << critical_bridges.size() << endl;
    for(auto b: critical_bridges) {
        cout << b.first << " " << b.second << endl;
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) {
        do_testcase();
    }
}