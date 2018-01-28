// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
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
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

// Functions
// =========
// Function for an individual testcase

int mst_cost(Graph G, int V, Vertex start, WeightMap& weightmap) {
    vector<Vertex> primpredmap(V);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
	prim_minimum_spanning_tree(G, make_iterator_property_map(primpredmap.begin(), get(vertex_index, G)), root_vertex(start));
    	
	int totalweight = 0;
	for (int i = 0; i < V; ++i) {
		if (primpredmap[i] != i) {
			Edge e; bool success;
			tie(e, success) = edge(i, primpredmap[i], G);
			totalweight += weightmap[e];
		}
	}
    //clog << "Edges in Prim's minimum spanning:\n";	
	// iterate over all vertices
	for (int i = 0; i < V; ++i) {
		// OutEdgeIterators
		// ================
		Vertex u = i;
		OutEdgeIt oebeg, oeend;
		for (tie(oebeg, oeend) = out_edges(u, G); oebeg != oeend; ++oebeg) {
			Vertex v = target(*oebeg, G);	// source(*oebeg, G) is guaranteed to be u, even in an undirected graph.
			if (primpredmap[u] == v) {
				//clog << u << " -- " << v << " (weight " << weightmap[*oebeg] << ")\n";
			}
		}
	}
	return totalweight;
}
void testcase() {

    size_t num_planets, start_planet;
    cin >> num_planets >> start_planet;
    start_planet--;

    int V = num_planets;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = get(edge_weight, G);	
	for (size_t i = 0; i < V-1; ++i) {
        for(size_t j = i+1; j < V; ++j) {
            size_t c; cin >> c;
            //clog << "adding edge " << i << " <-> " << j << " cost: " << c << endl;
            Edge e;	bool success;
            tie(e, success) = add_edge(i, j, G);
            weightmap[e] = c;		
            assert(source(e, G) == i && target(e, G) == j);	
        }
	}
	// Prim minimum spanning tree
	// ==========================
	vector<Vertex> primpredmap(V);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
	Vertex start = start_planet;
	prim_minimum_spanning_tree(G, make_iterator_property_map(primpredmap.begin(), get(vertex_index, G)), root_vertex(start));
    	
	int totalweight_mst = 0;
	for (int i = 0; i < V; ++i) {
		if (primpredmap[i] != i) {
			Edge e; bool success;
			tie(e, success) = edge(i, primpredmap[i], G);
			totalweight_mst += weightmap[e];
		}
	}

	// iterate over all vertices
    vector<Edge> edges;
	for (int i = 0; i < V; ++i) {
		// OutEdgeIterators
		// ================
		Vertex u = i;
		OutEdgeIt oebeg, oeend;
        Edge edge_to_remove;
		for (tie(oebeg, oeend) = out_edges(u, G); oebeg != oeend; ++oebeg) {
			Vertex v = target(*oebeg, G);	// source(*oebeg, G) is guaranteed to be u, even in an undirected graph.
			if (primpredmap[u] == v) {
				edges.push_back(*oebeg);
                //clog << u << " -- " << v << " (weight " << weightmap[*oebeg] << ")\n";
			}
		}
	}
    //clog << "---" << endl;
    int othermintotalweight = INT_MAX;
    for(Edge e: edges) {
        Vertex s, t;
        s = source(e, G);
        t = target(e, G);
        int cost = weightmap[e];
        remove_edge(e, G);
        int c = mst_cost(G, V, start_planet, weightmap);
        othermintotalweight = min(othermintotalweight, c);
        if(othermintotalweight == totalweight_mst) {
            break;
        }
        Edge e_; bool success;
		tie(e_, success) = add_edge(s, t, G);
        weightmap[e_] = cost;
    }
    //clog << "mst_totalweight=" << totalweight_mst << endl;
    cout << othermintotalweight << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
