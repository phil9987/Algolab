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

void testcases() {
    int n, q; cin >> n >> q;
    map<string, int> species;
    vector<string> species_rev(n);
    vector<long> age(n);
    map<int, int> ancestor;      //map id -> vector<ancestor_id, ancestor_age, expanded> 

    for(int i = 0; i < n; i++) {
        string s; cin >> s;
        species[s] = i;
        species_rev.at(i) = s;
        cin >> age.at(i);
    }

    for(int i = 0; i < n-1; i++) {
        string s, p;
        cin >> s >> p;
        int s_idx, p_idx;
        s_idx = species[s];
        p_idx = species[p];
        ancestor[s_idx] = p_idx;
    }

    for(int i = 0; i < q; i++) {
        // handle the queries
        string s;
        long max_age;
        cin >> s >> max_age;
        int s_idx = species[s];
        int best_idx = s_idx;
        auto it = ancestor.find(s_idx);
        while(it != ancestor.end()) {
            int a_idx = it->second;
            //cout << "finding ancestor of " << s << " which is " << species_rev.at(a_idx) << " with age " << age.at(a_idx) << endl;
            if(age.at(a_idx) <= max_age) {
                best_idx = a_idx;
                it = ancestor.find(best_idx);
            } else {
                it = ancestor.end();
            }
        }
        cout << species_rev.at(best_idx) << " ";
    }
    cout << endl;

	
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	testcases();
	return 0;
}

