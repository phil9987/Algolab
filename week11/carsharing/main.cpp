#include <vector>
#include <iostream>
#include <cstdlib>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

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
    //return p;
    return max_p - p;
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
    vector<int> starts(num_booking_req);
    vector<int> targets(num_booking_req);
    vector<int> departure_times(num_booking_req);
    vector<int> arrival_times(num_booking_req);
    vector<int> profits(num_booking_req);
    int max_profit = 0;    // max. profit, used to invert the profit, because mincostmaxflow minimizes the cost
    int max_time = 0;

    set<int> times;

    for(int i = 0; i < num_booking_req; i++) {
        int start, target, dep_time, arrival_time, profit;
        cin >> start >> target >> dep_time >> arrival_time >> profit;
        dep_time = dep_time;
        arrival_time = arrival_time;
        max_profit = max(profit, max_profit);
        max_time = max(arrival_time, max_time);
        starts.at(i) = start-1;     // input goes from 1..S
        targets.at(i) = target-1;   // input goes from 1..S
        departure_times.at(i) = dep_time;
        arrival_times.at(i) = arrival_time;
        profits.at(i) = profit;
        times.insert(dep_time);
        times.insert(arrival_time);
    }
    map<int, int>   time_to_id;
    vector<int>     id_to_time(times.size());
    size_t i = 0;

    for(auto it = times.begin(); it != times.end(); it++) {
        time_to_id[*it] = i;
        id_to_time.at(i) = *it;
        i++;
    }
    max_time = times.size();
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
    for(int i=0; i < num_booking_req; i++) {
        int dep_time = time_to_id[departure_times.at(i)];
        int arr_time = time_to_id[arrival_times.at(i)];
        int skipper_cost = (arr_time - dep_time-1)*max_profit;     
        // if somebody borrows car from time a until time b with cost c the total cost for this will be c while not moving the car costs (b-a)*max_profit (likely to be more expensive). To adjust this, we add a skipper_cost for the (b-a-1) which are "skips"
        eaG.addEdge(dep_time*num_stations + starts.at(i), arr_time*num_stations + targets.at(i), 1, inverse_p(profits.at(i), max_profit)+skipper_cost);
    }

    // add edges from one timestep to next with max capacity and no profit -> car stays at same place
    for(int i = 0; i < max_time; i++) {
        int from_base = i*num_stations;
        int to_base = from_base + num_stations;
        for(int j = 0; j < num_stations; j++) {
            eaG.addEdge(from_base+j, to_base+j, num_cars, inverse_p(0, max_profit));      
        }
    }

    // add edges from source to first timestep and from last timestep to sink
    for(int i = 0; i < num_stations; i++) {
        eaG.addEdge(v_source, i, cars.at(i), 0);
        eaG.addEdge(N-3-i, v_target, num_cars, 0);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    
    int flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
    
    cout << max_time*max_profit*flow-cost << endl;
    
}
int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        do_testcase();
    }
    return 0;
}