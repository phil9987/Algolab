// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>
#include <list>

// Namespaces
using namespace std;

set<int> res;
vector<int> visited;
vector<vector<int> > adj;
vector<int> temperatures;
size_t path_length;
size_t max_risk;
size_t total_num_nodes;
size_t current_num_nodes;

void dfs(int node, multiset<int> current_min_max, list<pair<size_t, size_t> > current_window, size_t len) {
    cout << "node " << ++current_num_nodes << " / " << total_num_nodes << endl;
    if(len == path_length) {
        // remove previous head
        pair<size_t, size_t> last_head = current_window.front();
        cout << "erasing " << last_head.first << endl;
        current_min_max.erase(current_min_max.find(last_head.first));
        current_window.pop_front();
        len--;
    } 
    // add current node to front
    current_window.push_back(make_pair(temperatures.at(node), node));
    cout << "inserting " << temperatures.at(node) << endl;
    current_min_max.insert(temperatures.at(node));
    len++;

    if(len == path_length) {
        // this is potentially a valid path
        int min = *current_min_max.begin();
        int max = *current_min_max.rbegin();
        /*for(auto it = current_window.begin(); it != current_window.end(); ++it) {
            cout << (*it).first << " , " << (*it).second << " ";
        }
        cout << endl;
        for(auto it = current_min_max.begin(); it!= current_min_max.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
        cout << "min=" << min << " max=" << max << endl;*/
        size_t current_risk = max - min;
        if (current_risk <= max_risk) {
            // this is a valid path!
            //cout << "found valid path starting at " << current_window.front().second << endl;
            //cout << "current risk: " << current_risk << " max_risk: " << max_risk << endl;
            res.insert(current_window.front().second);   // add current start to res vector
        }
    }


    for(int u : adj.at(node)) {
        if(visited.at(u) == 0) {
            visited.at(u) = 1;
            dfs(u, current_min_max, current_window, len);
        }
    }
}

// Functions
// ========= 
void testcase() {
	// Read and build graph
	int num_vertices;	
	cin >> num_vertices >> path_length >> max_risk;
    total_num_nodes = num_vertices;
    current_num_nodes = 0;
    vector<int> temps(num_vertices);
    vector<vector<int> > adj_(num_vertices);
    visited.clear();
    for (int i = 0; i < num_vertices; i++) {
        cin >> temps.at(i);
        visited.push_back(0);
    }
    vector<int> num_pointing(num_vertices, 0);
    for (int i = 0; i < num_vertices - 1; i ++) {
        int from, to;
        cin >> from >> to;
        adj_.at(from).push_back(to);
        num_pointing.at(to)++;
    }
    adj = adj_;
    temperatures = temps;
    res.clear();
    multiset<int> mset;
    list<pair<size_t, size_t> > window;
    visited.at(0) = 1;
    dfs(0, mset, window, 0);
    if(res.size() > 0) {
        for(int start : res) {
            cout << start << " ";
        }
    }
    else {
        cout << "Abort mission";
    }
    cout << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcase();
	return 0;
}

