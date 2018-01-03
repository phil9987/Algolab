#include <vector>
#include <iostream>
#include <cstdlib>
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
        //std::cout << "adding edge " << u << " -> " << v << std::endl;
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

int inverse_p(int p, int max_p) {
    return p;
    //return max_p - p;
}

int reverse_inv_p(int p, int max_p) {
    return p;
    //return -(p - max_p);
}

void do_testcase() {
    int num_booking_req;        // N; <=10000
    int num_stations;           // S; <= 10
    std::cin >> num_booking_req >> num_stations;
    int num_cars = 0;
    std::vector<int> cars(num_stations);
    for(int i = 0; i < num_stations; i++) {
        std::cin >> cars.at(i);
        num_cars += cars.at(i);
    }
    std::vector<std::vector<int> > booking_reqs(num_booking_req);
    long max_profit = 0;    // max. profit, used to invert the profit, because mincostmaxflow minimizes the cost
    long max_time = 0;

    for(int i = 0; i < num_booking_req; i++) {
        std::vector<int> tmp(5);
        for(int j = 0; j < 5; j++){
            std::cin >> tmp.at(j);   // start_station, target_station, departure_time, arrival_time, profit
        }        
        max_profit = std::max((long)tmp.at(4), max_profit);
        long m = std::max(tmp.at(2), tmp.at(3));
        max_time = std::max(m, max_time);
        tmp.at(0) -=1;  // input goes from 1..num_stations
        tmp.at(1) -=1;  // input goes from 1..num_stations
        booking_reqs.at(i) = tmp;
    }

    const int N=num_stations*(max_time+1)+2;
    const int v_source = N-2;
    const int v_target = N-1;
    // Create Graph and Maps
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    //std::cout << "adding edges" << std::endl;
    for(int i=0; i < num_booking_req; i++) {
        //std::cout << "adding edge for booking request " << i << std::endl;
        std::vector<int> r = booking_reqs.at(i);
        //std::cout << r.size() << std::endl;
        eaG.addEdge(r.at(2)*num_stations + r.at(0), r.at(3)*num_stations + r.at(1), 1, -inverse_p(r.at(4), max_profit));
        //std::cout << ".." << std::endl;
    }

    // add edges from one timestep to next if car is not moved with max capacity and no profit -> cars stays at same place
    for(int i = 0; i < max_time; i++) {
        int from_base = i*num_stations;
        int to_base = from_base + num_stations;
        for(int j = 0; j < num_stations; j++) {
            eaG.addEdge(from_base+j, to_base+j, num_cars, inverse_p(0, max_profit));      
        }
    }

    // add edges from source to first timestep and from last timestep to sink
    for(int i = 0; i < num_stations; i++) {
        eaG.addEdge(v_source, i, cars.at(i), inverse_p(0, max_profit));
        eaG.addEdge(N-3-i, v_target, num_cars, inverse_p(0, max_profit));
    }

    int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << std::endl;
    std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << std::endl;
    std::cout << "flow " << flow1 << std::endl; 
    std::cout << "cost " << -cost1 << std::endl;
    
}
int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        do_testcase();
    }
    return 0;
}