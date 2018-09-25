#include <iostream>
#include <vector>
std::vector<std::vector<int> > edges;
std::vector<int> discovery_timestamp;
std::vector<int> finished_timestamp;
std::vector<int> visited;
int current_time;


void dfs(int node) {
    discovery_timestamp.at(node) = current_time;
    current_time++;
    
    for(int i = 0; i < visited.size(); i++) {
        if(edges.at(node).at(i) == 1 && visited.at(i) == 0) {
            visited.at(i) = 1;
            dfs(i);
        }
    }
    finished_timestamp.at(node) = current_time;
    current_time++;
}
void testcase() {
    int n_vertices, n_edges, start; std::cin >> n_vertices >> n_edges >> start;
    edges = std::vector<std::vector<int> >(n_vertices);
    for(int i = 0; i < n_vertices; i++) {
        edges.at(i) = std::vector<int>(n_vertices, 0);
    }
    discovery_timestamp = std::vector<int>(n_vertices, -1);
    finished_timestamp = std::vector<int>(n_vertices, -1);
    visited = std::vector<int>(n_vertices, 0);
    discovery_timestamp.at(start) = 0;
    for(int i = 0; i < n_edges; i++) {
        int from, to; std::cin >> from >> to;
        edges.at(from).at(to) = 1;
        edges.at(to).at(from) = 1;
    }
    int it = 0;
    visited.at(start) = 1;
    current_time = 0;
    dfs(start);

    for(int d: discovery_timestamp) {
        std::cout << d << ' ';
    }
    std::cout << '\n';
    for(int f: finished_timestamp) {
        std::cout << f << ' ';
    }
    std::cout << '\n';
}   

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}