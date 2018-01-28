// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <stack>
//#include "prettyprint.hpp"
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS,	
		no_property,				// interior properties of vertices	
		property<edge_weight_t, int> 		// interior properties of edges
		>											Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator			EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

// Functions
// =========
// Function for an individual testcase
void testcase() {

    size_t num_planets, start_planet;
    cin >> num_planets >> start_planet;
    start_planet--;

    int V = num_planets;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = get(edge_weight, G);	
	for (int i = 0; i < V-1; ++i) {
        for(int j = i+1; j < V; ++j) {
            int c; cin >> c;
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
	vector<vector<pair<int, int> > > mst_graph(V);
	int totalweight_mst = 0;
	for (int i = 0; i < V; ++i) {
		if (primpredmap[i] != i) {
			Edge e; bool success;
			tie(e, success) = edge(i, primpredmap[i], G);			// get edge from original G
			int cost = weightmap[e];
			totalweight_mst += cost;

			mst_graph.at(i).push_back(make_pair(primpredmap[i], cost));
			mst_graph.at(primpredmap[i]).push_back(make_pair(i, cost));
		}
	}

	vector<vector<int> > max_weight_edge(V, vector<int>(V, 0));
	for(int i = 0; i < V; i++) {
		// BFS to find heaviest edge from i to every other edge
		std::vector<int> vis(V, false); // visited flags
		std::stack<int> Q; // BFS queue (from std:: not boost::)
		vis[i] = true; // Mark the source as visited
		Q.push(i);
		while (!Q.empty()) {
			const int u = Q.top();
			Q.pop();
			for(pair<int, int> edge : mst_graph.at(u)) {
				const int v = edge.first;
				const int cost = edge.second;
				if(vis[v]) continue;
				max_weight_edge.at(i).at(v) = max(max_weight_edge.at(i).at(u), cost);
				vis[v] = true;
				Q.push(v);
			}
		}
	}

	// iterate over all vertices
	int min_additional_cost = INT_MAX;
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		Vertex i = source(*ebeg, G);
		Vertex j = target(*ebeg, G);
		if(!(primpredmap[i] == j || primpredmap[j] == i)) {
			// edge is not part of MST
				int cost = weightmap[*ebeg];
				min_additional_cost = min(min_additional_cost, cost - max_weight_edge.at(i).at(j));
		}
	}	
	/*	ATTENTION: This code does not pass test3 (timelimit). The problem is the lookup time for an edge (log(V/E) according to http://www.boost.org/doc/libs/1_39_0/libs/graph/doc/using_adjacency_list.html)
	for (int i = 0; i < V; ++i) {
		for(int j = i+1; j < V; ++j) {
			if(!(primpredmap[i] == j || primpredmap[j] == i)) {
				// edge is not part of MST
				Edge e; bool success;
				tie(e, success) = edge(i, j, G);
				if(success) {
					int cost = weightmap[e];
					min_additional_cost = min(min_additional_cost, cost - max_weight_edge.at(i).at(j));
				}
			}
		}
	}*/

	cout << totalweight_mst + min_additional_cost << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
