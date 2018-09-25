// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
// using namespace boost;
using namespace std;



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
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator


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
        //cout << "adding edge " << from << "->" << to << " cap=" << capacity << endl;

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
void testcase() {
    size_t num_zones, num_jobs;
    std::cin >> num_zones >> num_jobs;
    std::vector<size_t> cost(num_zones);
    for(size_t i = 0; i < num_zones; i++) {
        std::cin >> cost.at(i);
    }
    std::vector<size_t> reward(num_jobs);
    for(size_t i = 0; i < num_jobs; i++) {
        std::cin >> reward.at(i);
    }
    std::vector<std::vector<size_t> > tickets_per_job(num_jobs);
    for(size_t i = 0; i < num_jobs; i++) {
        size_t num_tickets; std::cin >> num_tickets;
        std::vector<size_t> tx(num_tickets);
        for(size_t j = 0; j < num_tickets; j++) {
            std::cin >> tx.at(j);
        }
        tickets_per_job.at(i) = tx;
    }
    
    const int N = num_zones + num_jobs + 2;
    const int v_source = N-1;
    const int v_target = N-2;
    // Create Graph and Maps
    Graph G(N);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
    
    for(size_t i = 0; i < num_zones; i++) {
        eaG.addEdge(v_source, i, cost.at(i));
    }
    for(size_t i = 0; i < num_jobs; i++) {
        eaG.addEdge(num_zones + i, v_target, reward.at(i));
        auto tix = tickets_per_job.at(i);
        for(auto t: tix) {
            eaG.addEdge(t, num_zones + i, INT_MAX);
        }
    }
    
	long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    long res = 0;
    OutEdgeIt ebeg, eend;
    for(int i = num_zones; i < num_zones + num_jobs; i++) {
        for (boost::tie(ebeg, eend) = boost::out_edges(i, G); ebeg != eend; ++ebeg) {
            // Only follow edges with spare capacity
            if(boost::target(*ebeg, G) == v_target) {
                //cout << capacitymap[*ebeg] <<  " rescap= " << rescapacitymap[*ebeg] << endl;
                res += rescapacitymap[*ebeg];
            }
        }
    }
    
    /*for(boost::tie(ebeg, eend) = boost::out_edges(v_target, G); ebeg != eend; ++ebeg) {
        std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G) 
            << " with capacity " << capacitymap[*ebeg] << " and residual capacity " << rescapacitymap[*ebeg] << std::endl;
        res += reward.at(boost::target(*ebeg, G) - num_zones) - rescapacitymap[*ebeg];
    }*/
    
    cout << res << endl;

}


// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
