#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>

int dfs(int v, int current_time,
         std::vector<int> &discovered, 
         std::vector<int> &finished, 
         std::vector<int> &visited, 
         std::vector<std::vector<int> > &edges) {
             // do something for v 
            int t = current_time + 1;
            std::cout << "visiting node " << v << " at time " <<t << std::endl;
            for (int u : edges.at(v)) { 
                if (visited.at(u) == 0) { 
                     visited.at(u) = 1; 
                     discovered.at(u) = t;
                     t = dfs(u, t, discovered, finished, visited, edges); 
                } 
            }
            finished.at(v) = t;
            return t + 1;
}

void testcase() {
    int n_vertices, n_edges, start; std::cin >> n_vertices >> n_edges >> start;
    std::vector<int> visited(n_vertices, 0);
    std::vector<std::vector<int> > edges(n_vertices);
    std::vector<int> discovery_timestamp(n_vertices, -1);
    std::vector<int> finished_timestamp(n_vertices, -1);
    discovery_timestamp.at(start) = 0;
    for(int i = 0; i < n_edges; i++) {
        int from, to; std::cin >> from >> to;
        edges.at(from).push_back(to);
    }
    int it = 0;
    for(auto adj : edges) {
        std::sort(adj.begin(), adj.end());
        edges.at(it) = adj;
        it++;
    }
    std::cout << "start: " << start << std::endl;
    it = 0;
    for(auto v: edges) {
        std::cout << "edges from " << it << ": " << std::endl;
        for(auto el: v) {
            std::cout << el << ' ';
        }
        std::cout << std::endl;
        it++;
    }
    int total_time = dfs(start, 0, discovery_timestamp, finished_timestamp, visited, edges);
    std::cout << "total time passed = " << total_time << std::endl;
    for(int d: discovery_timestamp) std::cout << d << " ";
    std::cout << std::endl;
    for(int f: finished_timestamp) std::cout << f << " ";
    std::cout << std::endl;
}   

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}