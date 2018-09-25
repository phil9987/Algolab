#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>

void testcase() {
    int n_vertices, n_edges, start; std::cin >> n_vertices >> n_edges >> start;
    std::vector<std::vector<int> > edges(n_vertices);
    std::vector<int> discovery_timestamp(n_vertices, -1);
    std::vector<int> finished_timestamp(n_vertices, -1);
    std::vector<int> visited(n_vertices, 0);
    discovery_timestamp.at(start) = 0;
    for(int i = 0; i < n_edges; i++) {
        int from, to; std::cin >> from >> to;
        edges.at(from).push_back(to);
    }
    int it = 0;
    for(auto adj : edges) {
        // sort descendants in descending order, such that when processed the bigger indices are pushed to stack first, hence smaller indices processed ferce
        std::sort(adj.begin(), adj.end(), std::greater<int>());
        edges.at(it) = adj;
        it++;
    }
    std::stack<int> stack;
    stack.push(start);
    int time = 0;
    while(!stack.empty()) {
        int current = stack.top();
        stack.pop();
        discovery_timestamp.at(current) = 

        if(!visited.at(current)) {
            // this node is finished

            visited.at(current) = 1;
        }

        for(int i: edges.at(current)) {
            if(!visited.at(i)) {
                stack.push(i);
            }
        }
    }
}   

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}