/* idea: DP where T.at(i).at(k) returns a tuple (min_cost, max_drinks) where min_cost is the minimum cost with which k liters can be reached using max_drinks different beverages. The important point is that k does not need to be reached exactly.
*/

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <iomanip>

// Namespaces
using namespace std;

typedef pair<long, int> P;

void testcase() {
    int n, k;
    cin >> n >> k;
    vector<int> cost(n);
    vector<int> volume(n);
    for(size_t i = 0; i < n; i++) {
        cin >> cost.at(i);
        cin >> volume.at(i);
    }

    vector<vector<P > > T(n, vector<P>(k+1));
    T.at(0).at(0) = make_pair(0,0);
    for(int liters = 1; liters < k+1; liters++) {
        long c = cost.at(0) * ((liters + volume.at(0)-1) / volume.at(0));
        T.at(0).at(liters) = make_pair(c, 1);
    }

    for(int drink_idx = 1; drink_idx < n; drink_idx++) {
        T.at(drink_idx).at(0) = T.at(drink_idx-1).at(0);

        for(int liters = 1; liters < k+1; liters++) {
            // option 0: don't take this drink:
            auto dont_take = T.at(drink_idx-1).at(liters);
            long min_cost;
            int max_drinks;            
            if( liters < volume.at(drink_idx)) {
                // option 1: only take this drink
                min_cost = cost.at(drink_idx);
                max_drinks = 1;
            } else {
                // option 2: take this drink with others or only this drink
                auto take = T.at(drink_idx).at(liters - volume.at(drink_idx));
                min_cost = cost.at(drink_idx) + take.first;
                max_drinks = take.second;
                if(T.at(drink_idx -1).at(liters - volume.at(drink_idx)) == take) {
                    max_drinks += 1;
                }
            }
            if(dont_take.first < min_cost) {
                min_cost = dont_take.first;
                max_drinks = dont_take.second;
            } else if(min_cost == dont_take.first) {
                max_drinks = max(max_drinks, dont_take.second);
            }
            T.at(drink_idx).at(liters) = make_pair(min_cost, max_drinks);

        }
    }
    P res = T.at(n-1).at(k);

    //cout << T << endl;
    cout << res.first << " " << res.second << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
