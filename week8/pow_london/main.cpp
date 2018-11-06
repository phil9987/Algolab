// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
// using namespace boost;


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
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;


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
        //std::cout << "adding edge " << from << " -> " << to << " (" << capacity << ")" << std::endl;
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

int charToInt(char c) {
    return (int)(c-'A');
}


// Functions
// =========
// Function for an individual testcase
void testcases() {
    int height, width;
    std::cin >> height >> width;
    std::string targetString;
    std::cin >> targetString;
    std::vector<std::vector<int> > newspaperFront(height, std::vector<int>(width));
    std::vector<std::vector<int> > newspaperBack(height, std::vector<int>(width));
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            char c; std::cin >> c;
            newspaperFront.at(i).at(j) = charToInt(c);
        }
    }
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            char c; std::cin >> c;
            newspaperBack.at(i).at(j) = charToInt(c);
        }
    }	


    int alphabetSize = 26;
    int newspaperOffset = alphabetSize*alphabetSize;
    int V = newspaperOffset + alphabetSize;
    std::vector<int> targetChars(26, 0);
    int targetFlow = targetString.length();
    for(int i = 0; i < targetFlow; i++) {
        targetChars.at(charToInt(targetString.at(i)))++;
    }
    std::vector<int> newspaperInput(newspaperOffset, 0);
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            int frontLetter = newspaperFront.at(i).at(j);
            int backLetter = newspaperBack.at(i).at(width-j-1);
            newspaperInput.at(frontLetter*alphabetSize + backLetter)++;
        }
    }
	// Create Graph and Maps
	Graph G(V);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
    for(int firstLetter = 0; firstLetter < alphabetSize; firstLetter++) {
        for(int secondLetter = 0; secondLetter < alphabetSize; secondLetter++) {
            int newspaperVertex = firstLetter*alphabetSize + secondLetter;
            eaG.addEdge(source, newspaperVertex, newspaperInput.at(newspaperVertex));
            eaG.addEdge(newspaperVertex, newspaperOffset + firstLetter, newspaperInput.at(newspaperVertex));
            eaG.addEdge(newspaperVertex, newspaperOffset + secondLetter, newspaperInput.at(newspaperVertex));
        }
    }
    for(int i = 0; i < alphabetSize; i++) {
        eaG.addEdge(newspaperOffset + i, target, targetChars.at(i));
    }

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
	long flow = boost::push_relabel_max_flow(G, source, target);
	//long flow2 = boost::edmonds_karp_max_flow(G, source, target);
	//std::cout << "(push relabel max flow) " << flow1 << " == " << flow2 << " (Edmonds Karp max flow)" << std::endl;
    if(flow == targetFlow) {
        std::cout << "Yes" << '\n';
    } else {
        std::cout << "No" << '\n';
    }

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcases();
	return 0;
}
