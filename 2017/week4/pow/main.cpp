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
	Graph G(num_cities);	// creates an empty graph on V vertices
	WeightMap weightmap = get(edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
	vector<int> river_road(2);
	Edge river_road_edge;
	vector<Edge> river_roads;
	//int river_weight;

	for(int i = 0; i < num_roads; i++) {
		cin >> city1 >> city2 >> travel_time >> has_river;
		Edge e;	bool success;
		Vertex u = city1;
		Vertex v = city2;
		tie(e, success) = add_edge(u, v, G);
		weightmap[e] = travel_time;
		assert(source(e,G) == u && target(e, G) == v);
		if (has_river)  {
			river_road = {city1,city2};
			river_road_edge = e;
			river_roads.push_back(e);
			//river_weight = travel_time;
		}
		if(! success) {
			cout << "ERROR" << endl;
		}
	}
	//cout << "found the following river road: " << river_road[0] << " " << river_road[1] << endl;
	
	vector<Vertex> predmap(num_cities);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	vector<int> distmap(num_cities);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start = start_city;
	Vertex end = end_city;

	int min_distance = INT_MAX;

	for (auto e : river_roads) {
		int river_weight = weightmap[e];
		int river_start = source(e, G);
		int river_end = target(e, G);
		//cout << "initialized parameters for dijkstra" << endl;
		dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
			predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).	// predecessor map as Named Parameter
			distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter
		//cout << "dijkstra1 done." << endl;
		int min_dist_start_river0 = distmap[river_start];
		int min_dist_start_river1 = distmap[river_end];
		//cout << "found shortest dist to river: " << min_dist_to_river << endl;

		dijkstra_shortest_paths(G, end, // We MUST provide at least one of the two maps
			distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter
		//cout << "dijkstra2 done." << endl;
		
		int min_dist_end_river0 = distmap[river_start];
		int min_dist_end_river1 = distmap[river_end];
		//cout << "found shortest dist to end: " << min_dist_from_river_to_end << endl;
		
		int min_dist = min(min_dist_start_river0 + min_dist_end_river1 + river_weight, min_dist_start_river1 + min_dist_end_river0 + river_weight);
		if (min_dist < min_distance) min_distance = min_dist;
	}
	
		cout << min_distance << endl;

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	testcases();
	return 0;
}

