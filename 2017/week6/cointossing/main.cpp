// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
using namespace boost;


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
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Functions
// =========
// Function for an individual testcase
void testcases() {

    int n, m;
    std::cin >> n >> m;
    std::vector<int> A(m);
    std::vector<int> B(m);
    std::vector<int> C(m);
    for (int i = 0; i < m; i++) {
        std::cin >> A[i] >> B[i] >> C[i];
    }
    std::vector<int> S(n);
    for (int i = 0; i < n; i++) {
        std::cin >> S[i];
    }
	// Create Graph and Maps
	Graph G(n + m);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    Vertex source = boost::add_vertex(G);
    
    // Add edges
    for (int i = 0; i < m; i++) {
        eaG.addEdge(source, i, 10);
        if(C[i] == 1) {
            // A wins
            eaG.addEdge(i, m+A[i],10);
        } else if(C[i] == 2) {
            // B wins
            eaG.addEdge(i, m+B[i], 10);
        } else {
            // unsure
            eaG.addEdge(i, m+A[i], 10);
            eaG.addEdge(i, m+B[i], 10);
        }
    }

	// Add source and sink
	// Careful: The names 'source' and 'target' are already used for BGL's 
	// functions to get the two endpoints of an edge, use 'src' and 'sink'
	// in case you remove the namespace (which we don't recommend).
    Vertex target = boost::add_vertex(G);
    for(int i = 0; i < n; i++) {
        eaG.addEdge(m+i, target, S[i]*10);
    }

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
    long flow = boost::push_relabel_max_flow(G, source, target);
    //std::cout << flow << std::endl;
    int s = std::accumulate(S.begin(), S.end(), 0)*10;
    if (flow == s && s == m*10) {
        std::cout << "yes" << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcases();
	return 0;
}
