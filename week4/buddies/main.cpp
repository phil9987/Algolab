// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <bitset>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef adjacency_list<vecS, vecS, undirectedS> Graph; 
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;

// Functions
// =========
// Function for an individual testcase
void testcase() {
    size_t n, c, f; cin >> n >> c >> f;
    map<string, int> ctoid;
    size_t next_id = 0;
    vector<set<int> > students(n);
    
    for(size_t i = 0; i < n; i++) {
        set<int> charset;
        for(size_t j = 0; j < c; j++) {
            string characteristic; cin >> characteristic;
            if(ctoid.find(characteristic) == ctoid.end()) {
                ctoid[characteristic] = next_id++;
            }
            charset.insert(ctoid[characteristic]);
        }
        students.at(i) = charset;
    }
    vector<vector<int> > commonchar(n, vector<int>(n));
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            vector<int> v_intersection;
            set_intersection(students.at(i).begin(), students.at(i).end(), students.at(j).begin(), students.at(j).end(), back_inserter(v_intersection));
            commonchar.at(i).at(j) = v_intersection.size();
        }
    }

	// Create Graph and Maps
    size_t V = n;
	Graph G(V);
    for(size_t i = 0; i < n; i++) {
        for(size_t j = i+1; j < n; j++) {
            if(commonchar.at(i).at(j) > f) {
                add_edge(i, j, G);
            }
        }
    }


	vector<Vertex> matemap(V);
    // Use the vector as an Exterior Property Map: Vertex -> Matched mate
    edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
    // Look at the matching
    // Matching size
    int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));

    // unmatched vertices get the NULL_VERTEX as mate.
    const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();
    bool all_matched = matchingsize == n/2;
    /*for (int i = 0; i < V; ++i) {
        if (matemap[i] == NULL_VERTEX) {
            all_matched = false;
            break;
        }
    }*/
    if(all_matched) {
        cout << "not optimal" << endl;
    } else {
        cout << "optimal" << endl;
    }
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
