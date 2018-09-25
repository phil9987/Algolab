#include <iostream>
#include <vector>

using namespace std;
vector<vector<int> > M;
vector<int> xs;

int MV(int i, int j) {
    if (M[i][j] == -1) {
        // element not yet calculated...
        int diff = j - i;
        if(diff == 0) {
            // only 1 element left
            M[i][j] = xs[i];
        } else if(diff == 1) {
            // 2 elements left, choose maximum
            if (xs[i] >= xs[j]) M[i][j] = xs[i];
            else M[i][j] = xs[j];
        } else {
            M[i][j] = max(xs[i] + min(MV(i+2, j), MV(i+1, j-1)), xs[j] + min(MV(i, j-2), MV(i+1, j-1)));     
        }
    }
    return M[i][j];
}

int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        int n; cin >> n;
        vector<int> values(n);
        for (int i = 0; i < n; i++) {
            cin >> values[i];
            vector<int> tmp(n, -1);
            M.push_back(tmp);
        }
        xs = values;

        cout << MV(0, values.size() -1) << endl;
        M.clear();
    }
}