// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

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

using namespace std;

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

    pair<Edge, Edge> addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
        return make_pair(e, rev_e);
    }
};

pair<long, long> min_cost_max_flow(Graph& G, Vertex v_source, Vertex v_target, Edge src_edge, int new_flow, EdgeCapacityMap& capacitymap, ResidualCapacityMap& rescapacitymap) {
    // returns a pair <cost, flow>
    OutEdgeIt e, eend;
    capacitymap[src_edge] = new_flow;
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    long cost = boost::find_flow_cost(G);
    long s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    return make_pair(cost, s_flow);
}

void testcase() {
    /*
        idea: find shortest path sp over length
        then model min-cost-max-flow graph as follows:
            width: capacity, length: cost
        add v_source and connect it to start with capacity x and calculate min-cost-max-flow.
        the returned cost = total_distance*x
        Hence we need to check whether cost / x = sp
        We want to find the edge, i.e. where the cost / x < sp, then decrese x by 1 and we have the desired result
    */
    int n, m, s, f;
    cin >> n >> m >> s >> f;
    // Create Graph and Maps
    Graph G(n + 1);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    Vertex v_source = n;
    Vertex v_target = f;

    for(size_t i = 0; i < m; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        eaG.addEdge(a, b, c, d);
        eaG.addEdge(b, a, c, d);
    }

    pair<Edge, Edge> src_edge = eaG.addEdge(v_source, s, 1, 0);     // to find shortest path we allow a flow of 1 (equal to 1 runner), hence the max_cost will be the shortest path

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int shortest_path = boost::find_flow_cost(G);

    long lmin = 1, lmax = 1;
    while(1) {
        lmax *= 2;
        pair<long, long> cost_flow = min_cost_max_flow(G, v_source, v_target, src_edge.first, lmax, capacitymap, rescapacitymap);
        long flow = cost_flow.second;
        long cost = cost_flow.first;
        if (flow < lmax || (cost + flow-1) / flow > shortest_path) break;
    } 

    while (lmin != lmax) {
        long p = lmin + (lmax-lmin)/2;
        pair<long, long> cost_flow = min_cost_max_flow(G, v_source, v_target, src_edge.first, p, capacitymap, rescapacitymap);
        long flow = cost_flow.second;
        long cost = cost_flow.first;
        if (    flow < p                                        // if flow is smaller than possible flow p        
            ||  (cost + flow-1) / flow > shortest_path) {       // OR if cost / flow > shortest_path (cost = num_runners*length,                                                          hence we need to divide it by length. If the length is longer                                                          than the shortest path, no runner would take it so we can stop)
            lmax = p;     
        }
        else lmin = p+1; 
    }
    cout << lmin-1 << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
