// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>
#include "prettyprint.hpp"

// Namespaces
using namespace std;
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS, no_property>	Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex


map<pair<vector<int>, int>, int> memo;
// Functions
// =========
// Function for an individual testcase
bool tile_possible(size_t start_x, size_t start_y, vector<vector<int> >& floor, size_t h, size_t w) {

    if(start_x + 1 >= h || start_y + 1 >= w) {
        // we are at boundary, no tile can be placed starting at (i,j)
        return false;
    } else {
        // return true if 2x2 square starting at (i, j) is possible
        return (floor.at(start_x).at(start_y) == 1 && 
                floor.at(start_x+1).at(start_y) == 1 &&
                floor.at(start_x).at(start_y+1) == 1 &&
                floor.at(start_x+1).at(start_y+1) == 1);
    }
}

bool intersect(pair<int, int> t1, pair<int, int> t2) {
    return !(abs(t1.first - t2.first) >= 2 || abs(t1.second - t2.second) >= 2);
}

int solve(vector<pair<int, int> > & possible, vector<int> selected, size_t i) {
    //cout << selected << endl << i << endl;
    if(i == 0) {
        bool ith_el_possible = true;
        //cout << "base case i=0 " << selected << endl;
        for(int e: selected) {
            if(intersect(possible.at(e), possible.at(i))) {
                //cout << possible.at(e) << " collides with " << possible.at(i) << endl;
                ith_el_possible = false;
                break;
            }
        }
        return (int)ith_el_possible;
    } else {
        bool ith_el_possible = true;
        for(int e: selected) {
            if(intersect(possible.at(e), possible.at(i))) {
                ith_el_possible = false;
                break;
            }
        }
        if(ith_el_possible) {
            vector<int> sel = selected;
            sel.push_back(i);
            // either select it (increases result by 1) or not select it
            int v1, v2;
            pair<vector<int>, int> q1 = make_pair(sel, i-1);
            pair<vector<int>, int> q2 = make_pair(selected, i-1);
            auto res = memo.find(q1);
            if(res == memo.end()) {
                v1 = solve(possible, sel, i-1);
                memo[q1] = v1;
            } else {
                v1 = res->second;
            }
            res = memo.find(q2);
            if(res == memo.end()) {
                v2 = solve(possible, selected, i-1);
                memo[q2] = v2;
            } else{
                v2 = res->second;
            }
            return max(1 + v1, v2);
        } else {
            // continue without selecting ith element (not possible to select it with current selection)
            int v1;
            pair<vector<int>, int> q1 = make_pair(selected, i-1);
            auto res = memo.find(q1);
            if(res == memo.end()) {
                v1 = solve(possible, selected, i-1);
                memo[q1] = v1;
            } else {
                v1 = res->second;
            }
            return v1;
        }
    }
}

void testcase() {
    size_t h, w;
    cin >> h >> w;
    vector<vector<int> > floor(h, vector<int>(w));

    for(size_t i = 0; i < h; i++) {
        for(size_t j = 0; j < w; j++) {
            cin >> floor.at(i).at(j);
        }
    }
    // even, even
    vector<pair<int, int> > possible_positions;
    for(size_t i = 0; i < h; i++) {
        for(size_t j = 0; j < w; j++) {
            if(tile_possible(i, j, floor, h, w)) {
                possible_positions.push_back(make_pair(i,j));
            }
        }
    }
    size_t num_possible = possible_positions.size();
    //cout << possible_positions.size() << endl;
    /*if(num_possible > 0) {
        cout << solve(possible_positions, {}, num_possible-1) << endl;
    } else {
        cout << 0 << endl;
    }*/
    //cout << "." << endl;
    size_t V = num_possible;
    Graph G(V);
    Edge e;	bool success;
    for(size_t i = 0; i < num_possible; i++) {
        for(size_t j = 0; j < num_possible; j++) {
            if(i != j) {
                if(intersect(possible_positions.at(i), possible_positions.at(j))) {
                    // tile i "attacks" tile j
                    tie(e, success) = add_edge(i, j, G);
                    assert(source(e, G) == i && target(e, G) == j);	
                }
            }
        }
    }

    vector<Vertex> matemap(V);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
	edmonds_maximum_cardinality_matching(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	// Using the matemap 
	// =================
	const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();	// unmatched vertices get the NULL_VERTEX as mate.
	int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	cout << "A maximum matching has " << matchingsize << " edges " << " and " << (V-2*matchingsize) << " unmatched vertices.\n"; 
	cout << V - matchingsize << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
