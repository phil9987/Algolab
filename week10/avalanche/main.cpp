// ALGOLAB BGL Tutorial 1
// Code snippets demonstrating 
// - graph definitions
// - several algorithms (components, distance-based algorithms, maximum matching)
// - how to pass exterior property maps
// - use of iterators

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

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
typedef adjacency_list<vecS, vecS, directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
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
void testcases() {
	// Create Graph, Vertices and Edges
	// ================================
    int n, m, a, s, c, d;
    cin >> n >> m >> a >> s >> c >> d;
	int V = n;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = get(edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
	int E = m;
	for (int i = 0; i < E; ++i) {

		Edge e;	bool success;
		Vertex u, v;
        char type; cin >> type;
        cin >> u >> v;
        int time; cin >> time;
		tie(e, success) = add_edge(u, v, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
        weightmap[e] = time;			// Otherwise it is false in case of failure when the edge is a duplicate
        assert(source(e, G) == u && target(e, G) == v);	// This shows how to get the vertices of an edge

        if(type == 'L') {
            // it is a lift, also add back edge!
            tie(e, success) = add_edge(v, u, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
            weightmap[e] = time;			// Otherwise it is false in case of failure when the edge is a duplicate
            assert(source(e, G) == v && target(e, G) == u);	// This shows how to get the vertices of an edge
        }
	}
    vector<int> agents(a);
    for(size_t i = 0; i < a; i++) {
        cin >> agents.at(i);
    }
    vector<int> shelters(s);
    for(size_t i = 0; i < s; i++) {
        cin >> shelters.at(i);
    }


	// Dijkstra shortest paths
	// =======================
	vector<Vertex> predmap(V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	vector<int> distmap(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start = agents.front();
	dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter

    int min_time = INT_MAX;
    for(size_t i = 0; i < s; i++) {
        min_time = min(min_time, distmap[shelters.at(i)]);
    }
    cout << min_time + a*d << endl;
	/*cout << "Dijkstra starting in node " << start << " reaches " << reachable << " out of " << V << " vertices.\n";
	cout << "The furthest reachable vertex is " << furthest << " with distance " << maxdist << ".\n";
	cout << "Edges in Dijkstra's shortest paths tree:\n";
	// EdgeIterators
	// =============
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges. 
		// For undirected graphs, each edge is visited once, with some orientation.
		// ebeg is EdgeIterator, *ebeg is EdgeDescriptor
		Vertex u = source(*ebeg, G);
		Vertex v = target(*ebeg, G);
		if (predmap[u] == v || predmap[v] == u)
			cout << u << " -- " << v << " (weight " << weightmap[*ebeg] << ")\n";

	}
	cout << "List of disconnected single nodes: ";
	for (int i = 0; i < V; ++i)
		if (predmap[i] == i && i != start)
			cout << i << " ";
	cout << "\n" << endl;
	


	// Edmonds' maximum cardinality matching
	// =====================================
	vector<Vertex> matemap(V);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
	edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	// Using the matemap 
	// =================
	const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();	// unmatched vertices get the NULL_VERTEX as mate.
	int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	cout << "A maximum matching has " << matchingsize << " edges " << " and " << (V-2*matchingsize) << " unmatched vertices.\n"; 
	cout << "Edges in the maximum matching:\n";
	for (int i = 0; i < V; ++i) 
		if (matemap[i] != NULL_VERTEX && i < matemap[i])	// i is matched && we only print the edge once
			cout << i << " -- " << matemap[i] << "\n";
	cout << "List of unmatched vertices: ";
	for (int i = 0; i < V; ++i) 
		if (matemap[i] == NULL_VERTEX)	// i is not matched
			cout << i << " ";
	cout << "\n" << endl;*/
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	testcases();
	return 0;
}

