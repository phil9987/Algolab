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
typedef adjacency_list<vecS, vecS, undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,								// interior properties of vertices	
		property<edge_weight_t, int> 				// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator			EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


// Functions
// ========= 
void testcases() {
	// Create Graph, Vertices and Edges
	// ================================
	int num_cities, num_roads, min_nb_rivers, start_city, end_city; cin >> num_cities >> num_roads >> min_nb_rivers >> start_city >> end_city;
    int city1, city2, travel_time, has_river;
    int num_layers = min_nb_rivers+1;
	Graph G(num_cities * num_layers);	// creates an empty graph on V vertices
	WeightMap weightmap = get(edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
	for(int i = 0; i < num_roads; i++) {
        cin >> city1 >> city2 >> travel_time >> has_river;
        Edge e;	bool success;
        Vertex u, v;
        for (int j = 0; j < num_layers; j++) {
            int offset = num_cities*j;
            u = city1 + offset;
            v = city2 + offset;
            tie(e, success) = add_edge(u, v, G);
            weightmap[e] = travel_time;

            if(has_river && j < num_layers - 1) {
                tie(e, success) = add_edge(u, v + num_cities, G);
                weightmap[e] = travel_time;
                tie(e, success) = add_edge(v, u + num_cities, G);
                weightmap[e] = travel_time;
            }
        }
    }
    /*EdgeIt ebeg, eend;    
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {	// edges(G) returns a pair of iterators which define a range of all edges. 
		// For undirected graphs, each edge is visited once, with some orientation.
		// ebeg is EdgeIterator, *ebeg is EdgeDescriptor
		Vertex u = source(*ebeg, G);
		Vertex v = target(*ebeg, G);
		cout << u << " -- " << v << " (weight " << weightmap[*ebeg] << ")\n";

	}*/

	//cout << "found the following river road: " << river_road[0] << " " << river_road[1] << endl;
	
	vector<int> distmap(num_cities * num_layers);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start = start_city;
    Vertex end = end_city + min_nb_rivers*num_cities;

    //cout << "distmap size = " << num_layers << endl;
    
    //cout << "start: " << start << ", end: " << end << endl;

	//cout << "initialized parameters for dijkstra" << endl;
    dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
        distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter
    //cout << "dijkstra1 done." << endl;
    int min_distance = distmap[end];
	
	cout << min_distance << endl;

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	testcases();
	return 0;
}

