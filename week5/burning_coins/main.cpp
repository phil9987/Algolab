#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <boost/functional/hash.hpp>

using namespace std;
//vector<int> xs;
int N;
unordered_map<pair<int,int>, int, boost::hash<std::pair<int, int> > > memo;
vector<int> xs;

int MV(int i, int j) {
    pair<int, int> p = make_pair(i, j);
    if (memo.find(p) == memo.end()) {
        // element not yet calculated...
        int diff = j - i;
        if(diff == 0) {
            // only 1 element left
            memo[p] = xs[i];
        } else if(diff == 1) {
            // 2 elements left, choose maximum
            if (xs[i] >= xs[j]) memo[p] = xs[i];
            else memo[p] = xs[j];
        } else {
            memo[p] = max(xs[i] + min(MV(i+2, j), MV(i+1, j-1)), xs[j] + min(MV(i, j-2), MV(i+1, j-1)));     
        }
    }
    return memo[p];
}

int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        int n; cin >> n;
        N = n;
        vector<int> values(n);
        for (int i = 0; i < n; i++) {
            cin >> values[i];
        }
        xs = values;

        cout << MV(0, values.size() -1) << endl;
        memo.clear();
    }
}