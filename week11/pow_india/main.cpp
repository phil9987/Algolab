// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
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

void testcase() {
    int num_cities, num_guides, total_budget, start, end;
    std::cin >> num_cities >> num_guides >> total_budget >> start >> end;

    // Create Graph and Maps
    Graph G(num_cities + 1);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    for(int i = 0; i < num_guides; i++) {
        int from, to, cost, num_elephants;
        std::cin >> from >> to >> cost >> num_elephants;
        eaG.addEdge(from, to, num_elephants, cost);
    }

    // Run the algorithm
    boost::successive_shortest_path_nonnegative_weights(G, start, end);
    int max_cost = boost::find_flow_cost(G);
    //std::cout << "cost " << max_cost << std::endl;
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(start,G), G); e != eend; ++e) {
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    //std::cout << "flow " << s_flow << std::endl; 
    if(max_cost <= total_budget) {
        // we are done!
        std::cout << s_flow << std::endl;        
    } else {
        // find max flow that still fits budget
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(num_cities, start, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(start, num_cities, G);
        capacitymap[e] = --s_flow;
        weightmap[e] = 0;
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = 0; 
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e;         
        /*boost::successive_shortest_path_nonnegative_weights(G, num_cities, end);
        int current_cost = boost::find_flow_cost(G);
        while(current_cost > total_budget) {
            capacitymap[e] = --s_flow;
            //std::cout << s_flow << std::endl;
            capacitymap[rev_e] = 0;
            boost::successive_shortest_path_nonnegative_weights(G, num_cities, end);
            current_cost = boost::find_flow_cost(G);
        }*/

        int lmin = 0, lmax = s_flow;
        int current_cost = max_cost;
        //while (too_small(lmax)) lmax *= 2;
        while (lmin < lmax) {
            int p = (lmin + lmax+1)/2;
            capacitymap[e] = p;
            capacitymap[rev_e] = 0;
            boost::successive_shortest_path_nonnegative_weights(G, num_cities, end);
            current_cost = boost::find_flow_cost(G);
            if (current_cost > total_budget)
                lmax = p-1;
            else
                lmin = p;
            //std::cout << lmin << ' ' << p << ' ' << lmax << ' ' << current_cost << ' ' << total_budget << std::endl;
        }
        /*capacitymap[e] = lmin;
        capacitymap[rev_e] = 0;
        boost::successive_shortest_path_nonnegative_weights(G, num_cities, end);
        current_cost = boost::find_flow_cost(G);
        if(current_cost > total_budget)
            lmin--;*/
        std::cout << lmin << std::endl;
    }

}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;	// First input line: Number of testcases.
	while(T--)	testcase();
	return 0;
}

