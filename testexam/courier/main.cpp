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
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator
typedef boost::graph_traits<Graph>::in_edge_iterator  InEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        //std::cout << "adding edge " << u << "->" << v << "cap: " << c << " cost: " << w << std::endl;
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

// Functions
// =========
// Function for an individual testcase
void testcase() {
    size_t num_zones, num_jobs;
    size_t max_reward = 0;
    std::cin >> num_zones >> num_jobs;
    std::vector<size_t> cost(num_zones);
    for(size_t i = 0; i < num_zones; i++) {
        std::cin >> cost.at(i);
    }
    std::vector<size_t> reward(num_jobs);
    for(size_t i = 0; i < num_jobs; i++) {
        std::cin >> reward.at(i);
        max_reward = std::max(reward.at(i), max_reward);
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
    
    const int N = num_zones + 2*num_jobs + 2;
    const int v_source = N-1;
    const int v_target = N-2;
    // Create Graph and Maps
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    const int tickets_offset = 2*num_jobs;

    for(size_t i = 0; i < num_jobs; i++) {
        std::vector<size_t> tix = tickets_per_job.at(i);

        eaG.addEdge(v_source, i, 1, 0);
        eaG.addEdge(i, num_jobs + i, 1, max_reward - reward.at(i));
        eaG.addEdge(i, v_target, 1, max_reward - 0);      // zero cost edge to v_target --> don't do job
        if(tix.size() == 0) {
            eaG.addEdge(num_jobs + i, v_target, 1, max_reward - 0);      // we can get this job for free!
        } else {
            for(size_t j = 0; j < tix.size(); j++) {
                eaG.addEdge(num_jobs + i, tickets_offset + tix.at(j), 1, 0);
            }
        }
    }
    
    for(size_t i = 0; i < num_zones; i++) {
        eaG.addEdge(tickets_offset + i, v_target, num_jobs, cost.at(i));
    }
    
    // Run the algorithm

    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << std::endl;
    std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << std::endl;
    std::cout << "cost " << cost2 << std::endl; // 12
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
            << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    std::cout << "s-out flow " << s_flow << std::endl; // 5
    // Or equivalently, you can do the summation at the sink, but with reversed sign.
    int t_flow = 0;

    int discount = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
            << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        int f = rescapacitymap[*e] - capacitymap[*e];
        if(f > 1) {
            discount += weightmap[*e]*(f - 1);
        }
    }
    std::cout << "-----------------------" << std::endl;
    
    int total_cost = num_jobs*max_reward - (cost2 - discount);
    std::cout << total_cost << std::endl;
}


// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
