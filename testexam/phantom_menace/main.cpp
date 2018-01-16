// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
// BGL includes
#include <boost/config.hpp>
#include <list>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iterator>

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}

using namespace std;
using namespace boost;

// Functions
// =========
// Function for an individual testcase
void testcase() {
    unsigned num_obj, num_links, num_starts, num_dest;
    cin >> num_obj >> num_links >> num_starts >> num_dest;
    typedef adjacency_list < vecS, vecS, undirectedS,
    no_property, property < edge_component_t, std::size_t > >graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_t;
    graph_t g(num_obj);
    
    vector<pair<size_t, size_t> > links(num_links);
    for(size_t i = 0; i < num_links; i++) {
        size_t from, to;
        cin >> from >> to;
        // directed edge from --> to
        links.at(i) = make_pair(from, to);
        add_edge(from, to, g);
    }
    vector<size_t> starts(num_starts);
    for(size_t i = 0; i < num_starts; i++) {
        cin >> starts.at(i);
    }
    vector<size_t> dest(num_dest);
    for(size_t i = 0; i < num_dest; i++) {
        cin >> dest.at(i);
    }
    
    property_map < graph_t, edge_component_t >::type
    component = get(edge_component, g);

    
    for(size_t i = 0; i < num_comps; i++) {
        // for every vertex: remove it and check if no start,dest pair is in same component
        // only works for min(num_start, num_dest) = 2
        
        std::size_t num_comps = biconnected_components(g, component);
        
        for(auto s: starts) {
            
            for(auto d: dest) {
                
            }
        }
        //std::vector<vertex_t> art_points;
        //articulation_points(g, std::back_inserter(art_points));


        graph_traits < graph_t >::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
        std::cout << (char)(source(*ei, g) + 'A') << " -- " 
                  << (char)(target(*ei, g) + 'A')
                  << "[label=\"" << component[*ei] << "\"]\n";
        std::cout << "}\n";
        
    }
    
    
    
    

    
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
