// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <map>

using namespace std;

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
        string s, p;
        cin >> s >> p;
        tree.at(species[p]).push_back(species[s]);  // s is an immediate ofspring of p
    }
    vector<vector<long> > queries(n);

    for(int i = 0; i < q; i++) {
        // handle the queries
        string s;
        long max_age;
        cin >> s >> max_age;
        queries.at(species[s]).push_back(max_age);

    }
    cout << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	do_testcase();
	return 0;
}

