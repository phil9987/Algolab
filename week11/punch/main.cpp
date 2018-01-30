// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

// Namespaces
using namespace std;

typedef pair<int, int> P;

int MAX = 100000000;

bool is_b_better(P a, P b) {
    return b.first < a.first || (a.first == b.first && b.second > a.second);
}

void testcase() {
    int n, k;
    cin >> n >> k;
    vector<int> cost(n);
    vector<int> volume(n);
    for(size_t i = 0; i < n; i++) {
        cin >> cost.at(i);
        cin >> volume.at(i);
    }
    int max_volume = *max_element(volume.begin(), volume.end());
    max_volume += k+1;
    vector<vector<pair<int, int> > > T(n, vector<pair<int, int> >(max_volume, pair<int,int>{MAX, 0}));
    for(int i = 1; i < max_volume/volume.at(0); i++) {
        T.at(0).at(volume.at(0)*i) = make_pair(cost.at(0)*i, 1);
    }
    P res = make_pair(INT_MAX, 0);

    for(int i = 1; i < n; i++) {
        for(int j = 0; j < max_volume; j++) {
            if(j < volume.at(i)) {
                T.at(i).at(j) = T.at(i-1).at(j);
            } else {
                vector<P> res(3);
                if(j == volume.at(i)) {
                    // option 0: take only ith element
                    res.at(0) = (make_pair(cost.at(i), 1));
                } else {
                    res.at(0) = make_pair(MAX, 0);
                }
                res.at(1) = (T.at(i-1).at(j));
                P r = T.at(i).at(j-volume.at(i));
                if(T.at(i-1).at(j-volume.at(i)) == r) {
                    r.second++;
                }
                r.first += cost.at(i);
                res.at(2) = r;

                P optimum = *max_element(res.begin(), res.end(), is_b_better);
                T.at(i).at(j) = optimum;
            }
            if(i == n-1 && j >= k && is_b_better(res, T.at(i).at(j))) {
                res = T.at(i).at(j);
            }
        }
    }

    //cout << T << endl;
    cout << res.first << " " << res.second << endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
