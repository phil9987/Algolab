#include <iostream>
#include <queue>
#include <vector>

void testcase() {
    int num_vertices, num_edges, start; std::cin >> num_vertices >> num_edges >> start;
    //std::cout << num_vertices << " " << num_edges << " starting at: " << start << std::endl;
    std::queue<int> q; 
    std::vector<int> visited(num_vertices, 0);
    std::vector<std::vector<int> > edges(num_vertices);
    std::vector<int> dist(num_vertices, -1);

    for(int i = 0; i < num_edges; i++) {
        int from, to; std::cin >> from >> to;
        // pretty catchy: edges are NOT directed!!
        edges.at(from).push_back(to);
        edges.at(to).push_back(from);
    }

    q.push(start); 
    visited.at(start) = 1;
    dist.at(start) = 0;
    int num_iterations = 0;
    while (!q.empty()) { 
        num_iterations++;
        int v = q.front(); 
        q.pop(); 
        for (int u : edges.at(v)) { 
            if (visited.at(u) == 0) { 
                q.push(u); 
                visited.at(u) = 1; 
                dist.at(u) = dist.at(v) + 1;
            } 
        }
    }
    for(int d : dist) {
        std::cout << d << ' ';
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