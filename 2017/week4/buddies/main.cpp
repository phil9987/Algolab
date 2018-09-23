// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <map>
#include <string>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
typedef adjacency_list<vecS, vecS, undirectedS>                 Graph; 
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;

// Functions
// =========
// Function for an individual testcase
void testcase() {
    size_t n, c, f; cin >> n >> c >> f;
    map<string, int> ctoid;     // map 'characteristic' -> id
    size_t next_id = 0;         // the next free id if a characteristic is not yet in the map
    vector<set<int> > students(n);
    
    for(size_t i = 0; i < n; i++) {
        set<int> charset;
        for(size_t j = 0; j < c; j++) {
            string characteristic; cin >> characteristic;
            if(ctoid.find(characteristic) == ctoid.end()) {
                ctoid[characteristic] = next_id++;      // add the missing characteristic to the map and increment next_id
            }
            charset.insert(ctoid[characteristic]);      // insert the id of the characteristic into student's charset
        }
        students.at(i) = charset;
    }
    Graph G(n);
    for(size_t i = 0; i < n; i++) {
        for(size_t j = i+1; j < n; j++) {
            vector<int> v_intersection;
            set_intersection(students.at(i).begin(), students.at(i).end(), students.at(j).begin(), students.at(j).end(), back_inserter(v_intersection));
            if(v_intersection.size() > f) {     // in that case we can find a better matching, if existent
                add_edge(i, j, G);
            }
        }
    }
    // from slides week 4
	vector<Vertex> matemap(n);
    edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
    int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));

    if(matchingsize == n/2) {
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
