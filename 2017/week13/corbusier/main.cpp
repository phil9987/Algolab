// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <set>

// Namespaces
using namespace std;

// Functions
// =========
// Function for an individual testcase
void testcase() {
    int n, i, k;
    cin >> n >> i >> k;
    vector<unsigned> disks(n);

    for(size_t a = 0; a < n; a++) {
        unsigned tmp; cin >> tmp;
        tmp = tmp % k;
        disks.at(a) = tmp;
    }
    bool found = false;

    vector<vector<bool> > DP(n, vector<bool>(k, false));
    DP.at(0).at(disks.at(0)) = true;
    for(size_t a = 1; a < n; a++) {
        unsigned cur_val = disks.at(a);
        DP.at(a).at(cur_val) = true;
        for(size_t b = 0; b < k; b++) {
            if(DP.at(a-1).at(b)){
                DP.at(a).at(b) = true;
                DP.at(a).at((cur_val + b) % k) = true;
            }
        }
    }
    for(size_t a = 0; a < n; a++) {
        if(DP.at(a).at(i)) {
            found = true;
            break;
        }
    }

    if(found) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }



}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
