// ALGOLAB BGL Tutorial 1
// Code snippets demonstrating 
// - graph definitions
// - several algorithms (components, distance-based algorithms, maximum matching)
// - how to pass exterior property maps
// - use of iterators

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <bitset>

using namespace std;

int best_score(vector<vector<int> >& A, int n) {
    vector<vector<int> > B(n, vector<int>(n));
    B.at(0).at(0) = A.at(0).at(0);
    for(int i = 1; i < n; i++ ){
        B.at(i).at(0) = A.at(i).at(0) + B.at(i-1).at(0);
        B.at(0).at(i) = A.at(0).at(i) + B.at(0).at(i-1);
    }
    for(int i = 1; i < n; i++) {
        for(int j = 1; j < n; j++) {
            B.at(i).at(j) = max(B.at(i-1).at(j) + A.at(i).at(j), B.at(i).at(j-1) + A.at(i).at(j));
        }
    }
    return B.at(n-1).at(n-1);
}

void testcases() {
    int n; cin >> n;
    vector<vector<int> > A(n, vector<int>(n));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> A.at(i).at(j);
        }
    }
    int best = 0;
    // there are 2*(n-1) edges from 0,0 to n-1,n-1, hence as many decision points
    for(long long i = 0; i < 1 << 2*(n-1); i++ ) {
        bitset<60> b(i);
        int v = A.at(0).at(0);
        vector<vector<int> > A_ = A;
        A_.at(0).at(0) = 0;
        // calculate path value
        pair<int, int> pos = make_pair(0, 0);
        for(int p = 0; p < 2*(n-1); p++) {
            if(b.test(p)) {
                // we go right
                if(pos.second < n-1) pos.second++;
                else pos.first++;
            } else {
                // we go down
                if(pos.first < n-1) pos.first++;
                else pos.second ++;
            }
            v += A_.at(pos.first).at(pos.second);
            A_.at(pos.first).at(pos.second) = 0;
        }
        int tmp_max = v + best_score(A_, n);
        best = max(tmp_max, best);
    }

    cout << best << endl;

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	testcases();
	return 0;
}

  