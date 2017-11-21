// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Main
int main() {

	int T; std::cin >> T;
	for(int t=0; t < T; t++) {
		// build Graph
		int n, m, k, l;
		std::cin >> n >> m >> k >> l;
		Graph G(2*n);		// we create the same graph twice and connect it at the photo-vertices
		EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
		ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
		ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
		EdgeAdder eaG(G, capacitymap, revedgemap);

		std::vector<int> polices(k);
		std::vector<int> photos(l);
		
		// read in police locations
		for(int i = 0; i < k; i++) {
			std::cin >> polices.at(i);
		}
		// read in photo locations
		for(int i = 0; i < l; i++) {
			std::cin >> photos.at(i);
		}
		// add edges
		for(int i = 0; i < m; i++) {
			int x, y;
			std::cin >> x >> y;
			eaG.addEdge(x, y, k);			// add edge into original part
			eaG.addEdge(x+n, y+n, 1);		// add edge into replicated part
		}
		
		// add source and sink vertices, actually our graph has now 2*n + 2 vertices
		Vertex src = boost::add_vertex(G);
		Vertex sink = boost::add_vertex(G);
		
		for(int i = 0; i < k; i++) {
			eaG.addEdge(src, polices.at(i), 1);			// add a vertex from source to each police station
			eaG.addEdge(polices.at(i) + n, sink, 1);	// add a vertex from each police station of the the replicate part to sink 
		}
		for(int i = 0; i < l;i++) {
			eaG.addEdge(photos.at(i), photos.at(i) + n, 1);	// for every photo vertex connect the original part with replicate part
		}
		
		
		// Find a min cut via maxflow
		int flow = boost::push_relabel_max_flow(G, src, sink);		// calculate max flow
		std::cout << flow << std::endl;								// output result
	
	
	}

}