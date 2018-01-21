/*
    Idea: a knight on a white square can only attack black squares and vice versa. Hence we have a maximum independent set problem.
    From each black square we add an edge to all squares it attacks.
    From v_source we add an edge to each black square and from each white square we add an edge to v_target.
    We have a bipartite graph. According to the slides (König's theorem):
    In a bipartite graph, the number of edges in a bipartite matching is equal to the number of vertices in a minimum vertex cover.
    And we know that minimum vertex cover and maximum independent set are complementary, meaning we can obtain MIS = V - MVC
    Done.
*/
// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
// Namespaces
using namespace std;
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
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		    EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		        ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			            Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			                Edge;


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
size_t idx(size_t i, size_t j, size_t n) {
    return i*n + j;
}

// Function for an individual testcase
void testcase() {
    size_t n; cin >> n;
    vector<vector<int> > board(n, vector<int>(n));

    for(size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            cin >> board.at(i).at(j);       // board.at(i).at(j) == 1 if spot is available
        }
    }

    //clog << "input read" << endl;
    size_t V = n*n+2;
    Graph G(V);
    Vertex v_source = V-1;
    Vertex v_target = V-2;
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
    size_t num_possible_squares = 0;

    //clog << "graph initialized" << endl;
    //clog << board.size() << " x " << board.at(0).size() << endl;

    for(size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if(board.at(i).at(j)) {
                num_possible_squares++;
                if((i+j)%2 == 0) {
                    // black square

                    eaG.addEdge(v_source, idx(i,j,n), 1);

                    // add edges to all position which knight placed on (i,j) attacks
                    if(i >=1 && j >=2)      eaG.addEdge(idx(i,j,n), idx(i-1, j-2, n), 1);
                    if(i >=1 && j < n-2)    eaG.addEdge(idx(i,j,n), idx(i-1, j+2, n), 1);
                    if(i < n-1 && j >=2)    eaG.addEdge(idx(i,j,n), idx(i+1, j-2, n), 1);
                    if(i < n-1 && j < n-2)  eaG.addEdge(idx(i,j,n), idx(i+1, j+2, n), 1);
                    if(i >=2 && j >=1)      eaG.addEdge(idx(i,j,n), idx(i-2, j-1, n), 1);
                    if(i >=2 && j < n-1)    eaG.addEdge(idx(i,j,n), idx(i-2, j+1, n), 1);
                    if(i < n-2 && j >=1)    eaG.addEdge(idx(i,j,n), idx(i+2, j-1, n), 1);
                    if(i < n-2 && j < n-1)  eaG.addEdge(idx(i,j,n), idx(i+2, j+1, n), 1);

                } else {
                    // white square
                    eaG.addEdge(idx(i,j,n), v_target, 1);
                }

            }
        }
    }

    long min_vertex_cover = boost::push_relabel_max_flow(G, v_source, v_target);
    long max_indep_set = num_possible_squares - min_vertex_cover;
    cout << max_indep_set << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
