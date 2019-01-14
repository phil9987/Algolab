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
// Functions
// ========= 
void testcase() {
	// Read and build graph
	size_t num_vertices, path_length, max_risk;	
	cin >> num_vertices >> path_length >> max_risk;
    vector<int> temps(num_vertices);
    vector<vector<int> > adj_(num_vertices);
    vector<int> visited(num_vertices);
    for (size_t i = 0; i < num_vertices; i++) {
        cin >> temps.at(i);
    }
    vector<pair<size_t, size_t> > backwards;
    for (size_t i = 0; i < num_vertices - 1; i ++) {
        int from, to;
        cin >> from >> to;
        backwards.push_back(make_pair(from, to));
    }

    set<int> res;
    sort(backwards.begin(), backwards.end(), [](pair<size_t, size_t> a, pair<size_t, size_t> b) {
        return a.first < b.first;   
    });
    for(size_t i = 0; i < num_vertices; i++) {
        if(visited.at(i) == 0) {
            visited.at(i) = 1;
            size_t current_len = 1;
            int to = i;
            multiset<size_t> min_max;
            list<pair<size_t, size_t> > window;
            min_max.insert(temps.at(to));
            window.push_back(make_pair(to, temps.at(to)));
            bool end = false;
            while(current_len <= path_length && !end) {
                int from = backwards.at(to);
                if(from != to) {
                    current_len++;
                    to = from;
                } else {
                    break;
                }
                min_max.insert(temps.at(to));
                window.push_back(make_pair(to, temps.at(to)));
                if(current_len == path_length) {
                    size_t min = *min_max.begin();
                    size_t max = *min_max.rbegin();
                    pair<size_t, size_t> last_head = window.front();
                    visited.at(last_head.first) = 1;
                    if (max - min <= max_risk) {
                        res.insert(to);
                    }
                    current_len--;
                    min_max.erase(min_max.find(last_head.second));
                    window.pop_front();
                }
            }
        }
    }
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

