// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
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
        //std::cout << "adding edge " << u << "<->" << v <<"flow:" << c <<" cost:" << w << std::endl;
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

void do_testcase() {

    unsigned b, s, p;
    std::cin >> b >> s >> p;
    const int N=b+s+2;
    const int v_source = N-1;
    const int v_target = N-2;

    // Create Graph and Maps
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    std::vector<std::vector<int> > boat_to_sailor(b, std::vector<int>(s, 0));

    for(size_t i = 0; i < p; i++) {
        Vertex bi, si;
        std::cin >> bi >> si;
        //si += b;
        int c;
        std::cin >> c;
        //eaG.addEdge(bi, si, 1, -c);
        boat_to_sailor.at(bi).at(si) = c;
    }

    for(size_t i = 0; i < b; i++) {
        for(size_t j = 0; j < s; j++) {
            eaG.addEdge(i, j + b, 1, -boat_to_sailor.at(i).at(j));
        }
    }

    for(size_t bi = 0; bi < b; bi++) {
        eaG.addEdge(v_source, bi, 1, 0);
    }
    for(size_t si = b; si < b+s; si++) {
        eaG.addEdge(si, v_target, 1, 0);
    }
    
    // Run the algorithm

    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    std::cout << -cost1 << std::endl; // 12

    /*// Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
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
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
            << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        t_flow += rescapacitymap[*e] - capacitymap[*e];
    }
    std::cout << "t-in flow " << t_flow << std::endl; // 5
    std::cout << "-----------------------" << std::endl;*/

}

int main() {
    int T; std::cin >> T;

    for(int t = 0; t < T; t++) {
        do_testcase();
    }
    return 0;
}
