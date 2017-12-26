// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, directedS
typedef adjacency_list<vecS, vecS, directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices	
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

class EdgeAdder {
	Graph &G;
	WeightMap	&weightmap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, WeightMap &weightmap):
		G(G), weightmap(weightmap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long weight) {
		//cout << "adding edge " << from << "->" << to << " w:" << weight <<  endl;
		Edge e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		weightmap[e] = weight;
	}
};

// calculates the startidx of layer i, starting at idx 0
int layer_i_startidx(int i) {
	return i*(i+1) / 2;
}

void do_testcase() {
    int num_layers; std::cin >> num_layers;
    int num_balls = (num_layers * (num_layers+1)) / 2;
	int V = num_balls*2;
	// cout << "num_layers: " << num_layers << " num_balls: " << num_balls << " num vertices: " << V << endl;

    Graph G(V);		// for each ball there is an in-vertex and an out-vertex
	WeightMap weightmap = get(edge_weight, G);
	EdgeAdder eaG(G, weightmap);
	vector<int> weights(num_balls);

	// Add edges
    for(int i = 0; i < num_layers; i++) {
        for(int j = 0; j <= i; j++ ) {
			int curr_in = layer_i_startidx(i) + j;
			int curr_out = num_balls+curr_in;
			int w; cin >> w;
			weights.at(layer_i_startidx(i) + j) = w;
			//cout << "weight " << w << endl;
			eaG.addEdge(curr_in, curr_out, w);
			if(j > 0 && j < i) {
				//cout << "adding left & right neigbors" << endl;
				eaG.addEdge(curr_out, curr_in+1, 0);	// right neighbor
				eaG.addEdge(curr_out, curr_in-1, 0);	// left neighbor
			} else if (j == 0 && j < i) {
				//cout << "adding right neighbor" << endl;
				eaG.addEdge(curr_out, curr_in+1, 0);	// right neighbor
			} else if (j == i && j > 0) {
				//cout << "adding left neighbor" << endl;
				eaG.addEdge(curr_out, curr_in-1, 0);	// left neighbor
			}
			if(i < num_layers-1) {		// for every row, except for the last one, add edges to lower ones
				//cout << "adding 2 neighbors below" << endl;
				eaG.addEdge(curr_out, layer_i_startidx(i+1) + j, 0);		// one layer below, ball touching to the left
				eaG.addEdge(curr_out, layer_i_startidx(i+1) + j + 1, 0);	// one layer below, ball touching to the right
			}
			if(i > 0) { // for every row except for the first one, add edges to previous ones
				if(j == 0) {
					//cout << "adding 1 previous neighbor (left border)" << endl;
					eaG.addEdge(curr_out, layer_i_startidx(i-1) + j, 0);
				} else if(j == i){
					//cout << "adding 1 previous neighbor (right border)" << endl;
					eaG.addEdge(curr_out, layer_i_startidx(i-1) + j - 1, 0);
				} else {
					//cout << "adding 2 previous neighbors" << endl;
					eaG.addEdge(curr_out, layer_i_startidx(i-1) + j, 0);
					eaG.addEdge(curr_out, layer_i_startidx(i-1) + j -1,0);
				}
			}
        }
    }
	// cout << "done adding the edges" << endl;

	// Dijkstra shortest paths
	// =======================
	vector<int> distmap1(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start1 = 0;
	// cout << "dijkstra1 init done: " << start1 << endl;
	dijkstra_shortest_paths(G, start1, // We MUST provide at least one of the two maps
		distance_map(make_iterator_property_map(distmap1.begin(), get(vertex_index, G))));	// distance map as Named Parameter
	// cout << "dijkstra1 done" << endl;

	vector<int> distmap2(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start2 = layer_i_startidx(num_layers-1);	// left corner of triangle is in last layer on the left side
	// cout << "dijkstra2 init done: " << start2 << endl;
	dijkstra_shortest_paths(G, start2, // We MUST provide at least one of the two maps
		distance_map(make_iterator_property_map(distmap2.begin(), get(vertex_index, G))));	// distance map as Named Parameter
	//cout << "dijkstra2 done" << endl;

	vector<int> distmap3(V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start3 = start2 + num_layers -1;
	//cout << "dijkstra3 init done: " << start3 << endl;
	dijkstra_shortest_paths(G, start3, // We MUST provide at least one of the two maps
		distance_map(make_iterator_property_map(distmap3.begin(), get(vertex_index, G))));	// distance map as Named Parameter
	//cout << "dijkstra3 done" << endl;


	int mindist = INT_MAX;
	for (int i = 1; i < num_balls; ++i) {
		int tmp_dist = distmap1.at(i) + distmap2.at(i) + distmap3.at(i) + weights.at(i);
		if(tmp_dist < mindist) {
			mindist = tmp_dist;
		}
	}
	// cout << "optimal middle vertex: " << threemin << endl;
	cout << mindist << endl;

}


int main() {

    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        do_testcase();
    }
}