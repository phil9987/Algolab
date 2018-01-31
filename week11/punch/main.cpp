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

typedef pair<double, int> P;

double MAX = INT_MAX;

bool is_b_better(P& a, P& b) {
    return b.first < a.first || (a.first == b.first && b.second > a.second);
}

P better(P& a, P& b) {
    if(is_b_better(a, b)) return b;
    else return a;
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
    vector<vector<P > > T(n, vector<P >(max_volume, P{MAX, 0}));
    for(int i = 1; i < max_volume/volume.at(0); i++) {
        T.at(0).at(volume.at(0)*i) = make_pair(cost.at(0)*i, 1);
    }
    vector<P> tempres(4);
    for(int i = 1; i < n; i++) {
        for(int j = 0; j < max_volume; j++) {
            if(j < volume.at(i)) {
                T.at(i).at(j) = T.at(i-1).at(j);
            } else {      
                if(j == volume.at(i)) {
                    // option 0: take only ith element
                    tempres.at(0) = (make_pair(cost.at(i), 1));
                } else {
                    tempres.at(0) = make_pair(MAX, 0);
                }
                // option 1: don't take any element
                tempres.at(1) = (T.at(i-1).at(j));

                P r = T.at(i).at(j-volume.at(i));
                r.first += cost.at(i);
                tempres.at(2) = r;
                P r1 = T.at(i-1).at(j-volume.at(i));
                r1.first += cost.at(i);
                r1.second++;
                tempres.at(3) = r1;

                
                T.at(i).at(j) = *max_element(tempres.begin(), tempres.end(), is_b_better);
            }
        }
    }
    P res = *max_element(T.at(n-1).begin()+k, T.at(n-1).end(), is_b_better);

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
