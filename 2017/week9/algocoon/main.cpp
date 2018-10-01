/*
	Idea: every partition needs to contain at least 1 vertex. We can either go through all possible n^2 possible source, target pairs for the max-flow-min-cut calculation. Or, we can just check all the pairs (0,1), (1,2), (2,3), ...(n-2,n-1),(n-1,0)
	This works because 

*/

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

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
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

// Namespaces
using namespace std;

// Functions
// =========
pair<int, vector<int> > min_cut(Graph& G, Vertex v_source, Vertex v_target, int n, ResidualCapacityMap& rescapacitymap, EdgeCapacityMap& capacitymap) {

    boost::push_relabel_max_flow(G, v_source, v_target);

	// BFS to find vertex set S
	std::vector<int> vis(n, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[v_source] = true; // Mark the source as visited
	Q.push(v_source);
    vector<int> contained_figures;
    contained_figures.push_back(v_source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
            contained_figures.push_back(v);
			Q.push(v);
		}
	}

    int cost = 0;
	for (int i = 0; i < n; ++i) {
		if (vis[i]){
            OutEdgeIt ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(i, G); ebeg != eend; ++ebeg) {
                const int v = boost::target(*ebeg, G);
                if(!vis[v]) {
                    cost += capacitymap[*ebeg];
                }
            }
        }
	}
    sort(contained_figures.begin(), contained_figures.end());
    return make_pair(cost, contained_figures);
}

// Function for an individual testcase
void testcase() {
	int n, m;
    cin >> n >> m;
    // Create Graph and Maps
	Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    for(size_t i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        eaG.addEdge(a, b, c);
    }

    pair<int, vector<int> > min_res(INT_MAX, {});
    vector<pair<int, vector<int> > > results(n);
    for(int i = 0; i < n; i++) {
        Vertex v_source = i;
        Vertex v_target = (i+1)%n;
        pair<int, vector<int> > res =  min_cut(G, v_source, v_target, n, rescapacitymap, capacitymap);
        if(res.first < min_res.first) {
            min_res = res;
        }
    }
    
    int cost = min_res.first;
    vector<int> contained_figures = min_res.second;
	std::cout << cost << std::endl;
    sort(contained_figures.begin(), contained_figures.end());
    cout << contained_figures.size() << " ";
    for(int f: contained_figures) {
        cout << f << " ";
    }
    cout << endl;

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
