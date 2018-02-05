// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "prettyprint.hpp"

using namespace std;

void testcase() {
    int n, m; long k; cin >> n >> m >> k;
    vector<int> defense(n+1);
    for(int i = 1; i <= n; i++) {
        cin >> defense.at(i);
    }
    vector<long> sums(n + 1, 0);
    for(int i = 1; i <= n; i++) {
        sums.at(i) = sums.at(i-1) + defense.at(i);
    }
    vector<int> intervals(n+1, 0);     // intervals.at(i) = j == there is an interval starting at j and ending at i, [j,i]
    for(int lo = 1, hi=1; lo <= n && hi <= n;) {
        long cur_defense = sums.at(hi) - sums.at(lo-1);
        if(cur_defense <= k) {
            if(cur_defense == k) {
                // attack is feasible
                intervals.at(hi) = lo;      
            } 
            // increase window
            hi++;
        } else {
            // window is too big, increase lo
            lo++;
        }
        if(hi < lo) hi++; 
    }

    vector<vector<int> > T(m+1, vector<int>(n+1, 0));
    for(int i = 1; i < m+1; i++) {
        T.at(i).at(0) = INT_MIN;
    }
    for(int i = 0; i < n+1; i++) {
        T.at(0).at(i) = 0;
    }
    for(int j = 1; j < m+1; j++) {
        for(int i = 1; i < n+1; i++) {
            if(intervals.at(i) > 0) {
                // there is an interval ending at i
                T.at(j).at(i) = max(T.at(j).at(i-1), T.at(j-1).at(intervals.at(i)-1) + i-intervals.at(i)+1);
            } else {
                // there is no interval ending at i
                T.at(j).at(i) = T.at(j).at(i-1);
            }
        }
    }

    if(T.at(m).at(n) <= 0) {
        cout << "fail" << endl;
    } else {
        cout << T.at(m).at(n) << endl;
    }

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
