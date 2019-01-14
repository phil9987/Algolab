// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <string>
#include <map>

// Namespaces
using namespace std;
// g++ -std=c++11 -O2 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in



// Functions
// ========= 
void testcase() {
	// Read and build graph
	size_t n, q;
    cin >> n >> q;
    vector<string> species(n);
    map<string, size_t> speciesm;
    vector<long> age(n);
    for(int i = 0; i < n; i++) {
        cin >> species.at(i);
        speciesm.insert(make_pair(species.at(i), i));
        cin >> age.at(i);
    }
    vector<vector<size_t> > tree(n);
    vector<int> tree_rev(n, -1);
    for(int i = 0; i < n-1; i++) {
        string child, parent;
        cin >> child >> parent;
        tree.at(speciesm.at(parent)).push_back(speciesm.at(child));
        tree_rev.at(speciesm.at(child)) = speciesm.at(parent);
    }
    for(int i = 0; i < q; i++) {
        // run queries
        string name;
        long max_age;
        cin >> name >> max_age;
        int next_id = speciesm.at(name);
        int id;
        long current_age = age.at(next_id);
        while(current_age <= max_age && next_id != -1) {
            id = next_id;
            //cout << "getting next_id for id " << id << endl;
            next_id = tree_rev.at(id);
            //cout << "next_id=" << next_id << endl;
            if(next_id >= 0)
                current_age = age.at(next_id);
        }
        if(id == -1)    id = next_id;
        cout << species.at(id) << " ";
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

