// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <map>
#include <string>

using namespace std;

int binary_search(long max_age, vector<long>& age, vector<int>& path) {

    if(age.at(path.at(0)) <= max_age) {
        return path.at(0);
    }
    size_t l = 0, r = path.size()-1;
    while(l <= r) {
        size_t p = (l+r) / 2;
        long curr_age = age.at(path.at(p));
        if(curr_age == max_age) {
            l = p; r = p-1;
        } else if(curr_age <= max_age) {
            // we are too far right
            r = p-1;
        } else {
            l = p+1;
        }
    }
    return path.at(l);
}

void dfs(int root, vector<vector<int> >& tree, vector<long>& age, vector<int>& path, vector<int>& result, vector<vector<pair<long, int> > >& queries) {

    // do binary search in path for all queries of species root
    for(pair<long, int> q : queries.at(root)) {
        //clog << "performing binary serach for max_age=" << q.first << " for node "<< root << endl;
        result.at(q.second) = binary_search(q.first, age, path);
    }

    // continue dfs
    //clog << "continuing dfs..." << endl;
    for(int v: tree.at(root)) {
        path.push_back(v);
        dfs(v, tree, age, path, result, queries);
    }
    path.pop_back();
}

void do_testcase() {
    int n, q; cin >> n >> q;
    map<string, int> species;
    vector<string> species_rev(n);
    vector<long> age(n);
    vector<vector<int> > tree(n);       // the graph parent -> child

    for(int i = 0; i < n; i++) {
        string s; cin >> s;
        species[s] = i;
        species_rev.at(i) = s;
        cin >> age.at(i);
    }

    for(int i = 0; i < n-1; i++) {
        string child, parent;
        cin >> child >> parent;
        tree.at(species[parent]).push_back(species[child]);  // s is an immediate ofspring of p
    }
    vector<vector<pair<long, int> > > queries(n);

    for(int i = 0; i < q; i++) {
        // handle the queries
        string s;
        long max_age;
        cin >> s >> max_age;
        queries.at(species[s]).push_back(make_pair(max_age, i));
    }

    int root = max_element(age.begin(), age.end()) - age.begin();
    vector<int> result(q);
    vector<int> path;
    path.push_back(root);

    dfs(root, tree, age, path, result, queries);
    for(int r: result) cout << species_rev.at(r) << " ";
    cout << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	do_testcase();
	return 0;
}

