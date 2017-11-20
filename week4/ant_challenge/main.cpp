// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
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
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices	
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        int n,num_edges,s,a,b;
        cin >> n >> num_edges >> s >> a >> b;
        Graph G(n);	// creates an empty graph on n vertices
        vector<Graph> graphs(s, Graph(n));
        vector<WeightMap> weightmaps(s);
        for(int i = 0; i < s; i++) {
            graphs[i] = Graph(n);
            weightmaps[i] = get(edge_weight, graphs[i]);
        }
        WeightMap weightmap = get(edge_weight, G);	

        for (int i = 0; i < num_edges; i++) {
            int t1, t2; cin >> t1 >> t2;
            int tmp_weight;
            for(int j = 0; j < s; j++) {
                cin >> tmp_weight;
                Edge e;	bool success;
                Vertex u = t1;
                Vertex v = t2;
                tie(e, success) = add_edge(u, v, graphs[j]);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
                                    // Caveat: if u or v don't exist in G, G is automatically extended!
                weightmaps[j][e] = tmp_weight;			// Otherwise it is false in case of failure when the edge is a duplicate
                assert(source(e, graphs[j]) == u && target(e, graphs[j]) == v);	// This shows how to get the vertices of an edge
            }
        }
        vector<int> hives(s);
        for (int i = 0; i < s; i ++) cin >> hives[i];
        std::unordered_map<int, vector<pair<int, int> > > chosen_edges;
        for (int sp = 0; sp < s; sp++) {
            // Prim minimum spanning tree
            // ==========================
            vector<Vertex> primpredmap(n);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
            Vertex start = hives[sp];
            prim_minimum_spanning_tree(graphs[sp], make_iterator_property_map(primpredmap.begin(), get(vertex_index, graphs[sp])),	root_vertex(start));
            // iterate over all vertices
            for (int i = 0; i < n; ++i) {
                Vertex u = i;
                OutEdgeIt oebeg, oeend;
                for (tie(oebeg, oeend) = out_edges(u, graphs[sp]); oebeg != oeend; ++oebeg) {
                    Vertex v = target(*oebeg, G);	// source(*oebeg, G) is guaranteed to be u, even in an undirected graph.
                    Vertex first, second;
                    if (u < v) {
                        first = u; second = v;
                    } else {
                        first = v; second = u;
                    }
                    if (primpredmap[u] == v) {
                        if(chosen_edges.find(u) == chosen_edges.end()) {
                             chosen_edges[u] = vector<pair<int,int> >(1);
                             chosen_edges[u][0] = make_pair(v, weightmaps[sp][*oebeg]);
                        } else {
                            chosen_edges[u].push_back(make_pair(v, weightmaps[sp][*oebeg]));
                        }
                    }
                }
            }
        }

        for(auto ent1 : chosen_edges) {
            //cout << "chosen edges for v " << ent1.first;
            //for (auto el: ent1.second) cout << ": " << el.second << " w:" << el.first << endl;
            vector<pair<int, int > > edges = ent1.second;
            sort(edges.begin(), edges.end(), [](pair<int,int> a, pair<int,int> b) {
                return a.first <= b.first && a.second <= b.second;   
            });
            int current_e = -1;
            Vertex u = ent1.first;                                            
            for (auto p: edges) {
                if (p.first != current_e) {
                    Edge e;	bool success;
                    Vertex v = p.first;
                    tie(e, success) = add_edge(u, v, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
                                        // Caveat: if u or v don't exist in G, G is automatically extended!
                    weightmap[e] = p.second;			// Otherwise it is false in case of failure when the edge is a duplicate
                    assert(source(e, G) == u && target(e, G) == v);	// This shows how to get the vertices of an edge

                    current_e = p.first;
                }

            }
        }
        //cout << "graph created" << endl;


        // Dijkstra shortest paths
        // =======================
        vector<int> distmap(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
        Vertex start_ = a;
        dijkstra_shortest_paths(G, start_, // We MUST provide at least one of the two maps
            distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter
        int dist = distmap[b];
        if (dist == INT_MAX) {
            cout << "not reachable!?" << endl;
        }
        cout << dist << endl;
    }
}