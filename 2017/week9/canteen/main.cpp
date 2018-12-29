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

// Namespaces
using namespace std;
using namespace boost;

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
        //cout << u << " -> " << v << " capacity: " << c << " cost: " << w << endl;
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
void testcases() {
    size_t n; cin >> n;
    
    // Create Graph and Maps
    size_t V = n+2;
    Graph G(V);
    Vertex v_source = V-1;
    Vertex v_target = V-2;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    for(size_t i = 0; i < n; i++) {
        // ai: amount of menus for day i
        // ci: production cost per menu on day i
        int ai, ci; cin >> ai >> ci;
        eaG.addEdge(v_source, i, ai, ci);
    }
    size_t total_num_students = 0;
    int max_price = 0;
    vector<pair<int, int> > demand(n);
    for(size_t i = 0; i < n; i++) {
        // si: number of students for day i
        // pi: price per menu for day i
        int si, pi; cin >> si >> pi;
        demand.at(i) = make_pair(si, pi);
        max_price = max(pi, max_price);
        total_num_students += si;
    }
    for(size_t i = 0; i < n; i++) {
        // the price per menu is income, so we want to add it negatively. In order to only have positive cost, we subtract it form the max_price and subtract flow*max_price from the total_cost in the end in order to get the real cost
        eaG.addEdge(i, v_target, demand.at(i).first, max_price - demand.at(i).second);
    }
    for(size_t i = 0; i < n-1; i++) {
        // vi: number of menus which can be stored over night
        // ei: cooling energy cost per menu
        int vi, ei; cin >> vi >> ei;
        eaG.addEdge(i, i+1, vi, ei);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    int flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
    cost -= flow*max_price;     // subtract the max_price again

    if(flow == total_num_students) {
        cout << "possible";
    } else {
        cout << "impossible";
    }
    cout << " " << flow << " " << -cost << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcases();
	return 0;
}
