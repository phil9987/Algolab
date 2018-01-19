// ALGOLAB BGL Tutorial 1
// Code snippets demonstrating 
// - graph definitions
// - several algorithms (components, distance-based algorithms, maximum matching)
// - how to pass exterior property maps
// - use of iterators

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <bitset>

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

using namespace std;

int best_score(vector<vector<int> >& A, int n) {
    vector<vector<int> > B(n, vector<int>(n));
    B.at(0).at(0) = A.at(0).at(0);
    for(int i = 1; i < n; i++ ){
        B.at(i).at(0) = A.at(i).at(0) + B.at(i-1).at(0);
        B.at(0).at(i) = A.at(0).at(i) + B.at(0).at(i-1);
    }
    for(int i = 1; i < n; i++) {
        for(int j = 1; j < n; j++) {
            B.at(i).at(j) = max(B.at(i-1).at(j) + A.at(i).at(j), B.at(i).at(j-1) + A.at(i).at(j));
        }
    }
    return B.at(n-1).at(n-1);
}

int cost(int cost, int max) {
    return max - cost;
}

void testcases() {
    int n; cin >> n;
    vector<vector<int> > A(n, vector<int>(n));

    int max_c = 0;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> A.at(i).at(j);
            max_c = max(A.at(i).at(j), max_c);
        }
    }
    long N = n*n;
    // Create Graph and Maps
    Graph G(2*N+2);
    Vertex v_source = 2*N;
    Vertex v_target = 2*N+1;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++) {
            int cur = i*n + j;
            eaG.addEdge(cur, N + cur, 1, cost(A.at(i).at(j), max_c));
            eaG.addEdge(cur, N + cur, 1, cost(0, max_c));
        }
    }    

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            // right direction
            int cur = i*n + j;
            if(j < n-1) {
                int right = cur + 1;
                eaG.addEdge(N + cur, right, 2, 0);
            }
            if(i < n-1) {
                int down = cur + n;
                eaG.addEdge(N + cur, down, 2, 0);
            }
            // down direction
        }
    }
    eaG.addEdge(v_source, 0, 2, 0);
    eaG.addEdge(2*N-1, v_target, 2, 0);

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    cout << ((2*n-1)*2 )*max_c - cost2 << endl;

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	testcases();
	return 0;
}

  