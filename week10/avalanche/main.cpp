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
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, directedS, no_property,property<edge_weight_t, int> > DirectedGraph;
typedef graph_traits<DirectedGraph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<DirectedGraph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef property_map<DirectedGraph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property > Graph;


bool max_cardinality_matching_of_size(vector<vector<int> >& min_time, size_t num_agents, size_t num_shelters, int max_time){
	int V = num_agents + num_shelters;
	Graph G(V);
	for(size_t a = 0; a < num_agents; a++) {
		for(size_t s = 0; s < num_shelters; s++) {
			int t = min_time.at(a).at(s);
			if(t <= max_time ) {
				add_edge(a, num_agents + s, G);
			}
		}
	}
	vector<Vertex> matemap(V);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
	edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	// Using the matemap 
	// =================
	const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();	// unmatched vertices get the NULL_VERTEX as mate.
	int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	return matchingsize == num_agents;
}

// Functions
// ========= 
void testcases() {
	// Create Graph, Vertices and Edges
	// ================================
    int n, m, a, s, c, d;
    cin >> n >> m >> a >> s >> c >> d;

	int V = n;
	DirectedGraph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = get(edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
	int E = m;
	for (int i = 0; i < E; ++i) {

		Edge e;	bool success;
		Vertex u, v;
        char type; cin >> type;
        cin >> u >> v;
        int time; cin >> time;
		tie(e, success) = add_edge(u, v, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
        weightmap[e] = time;			
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
	int overall_min_time = INT_MAX;
	vector<vector<int> > min_time_agent_shelter(a, vector<int>(s, INT_MAX));
	for(size_t i = 0; i < a; i++) {
		vector<int> distmap(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
		Vertex start = agents.at(i);
		dijkstra_shortest_paths(G, start, distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));
		for(size_t j = 0; j < s; j++) {
			min_time_agent_shelter.at(i).at(j) = distmap[shelters.at(j)];
			overall_min_time = min(overall_min_time, distmap[shelters.at(j)]);
		}
	}

	if (a == 1) {
		cout << overall_min_time + d << endl;
	} else {
		int lmin = 0, lmax = INT_MAX;
		//while(!max_cardinality_matching_of_size(min_time_agent_shelter, a, s, lmax-d)) lmax *= 2;
		while(lmin != lmax) {
			int p = lmin + (lmax - lmin)/2;
			if(!max_cardinality_matching_of_size(min_time_agent_shelter, a, s, p-d)) lmin = p + 1;
			else lmax = p;
		}
		cout << lmin << endl;
	}
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); 
	int T;	cin >> T;
	while(T--)	testcases();
	return 0;
}

