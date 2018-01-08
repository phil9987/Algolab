// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
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

int main() {

    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        
        int N, M, S; std::cin >> N >> M >> S;
        std::vector<int> state_max(S);
        std::vector<int> property_belongs_to(M);
        Graph G(N+M+S+2);
        const int v_source = N+M+S;
        const int v_target = N+M+S+1;
        EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
        EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
        ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
        ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
        for(int i = 0; i < S; i++) {
            std::cin >> state_max.at(i);
            eaG.addEdge(N+M+i, v_target, state_max.at(i), 0);   // add edge from state to target (no cost)
        }
        for(int i = 0; i < M; i++) {
            std::cin >> property_belongs_to.at(i);
            eaG.addEdge(N+i, N+M+property_belongs_to.at(i)-1, 1, 0);  // add edge from property to state (no cost)
        }
        for(int i = 0; i < N; i++) {

            eaG.addEdge(v_source, i, 1, 0);     // add edge from source to buyer (no cost)

            std::vector<int> bids(M);
            for(int j = 0; j < M; j++) {
                std::cin >> bids.at(j);
                eaG.addEdge(i, N+j, 1, -bids.at(j));    // add edge from buyer to property with negative cost (because we want to minimize it)
            }

        }

        int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
        boost::cycle_canceling(G);
        int cost1 = boost::find_flow_cost(G);
        std::cout << flow1 << " " << -cost1 << std::endl;

    }
}
