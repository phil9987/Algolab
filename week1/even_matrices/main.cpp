// STL includes
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
void testcase() {
    int n; cin >> n;
    vector<vector<long> > M(n+1, vector<long>(n+1, 0));
    vector<vector<long> > P(n+1, vector<long>(n+1, 0));
    for(size_t i = 1; i <= n; i++) {
        for(size_t j = 1; j <= n; j++) {
            cin >> M.at(i).at(j);
        }
    }
    for(size_t i = 1; i <= n; i++) {
        for(size_t j = 1; j <= n; j++) {
            P.at(i).at(j) = P.at(i-1).at(j) + P.at(i).at(j-1) - P.at(i-1).at(j-1) + M.at(i).at(j);
        }
    }
    size_t counter = 0;
    for(size_t i1 = 1; i1 <= n; i1++) {
        for(size_t j1 = 1; j1 <= n; j1++) {
            for(size_t i2 = i1; i2 <=n; i2++) {
                for(size_t j2 = j1; j2 <=n; j2++) {
                    long quadruple = P.at(i2).at(j2) - P.at(i2).at(j1-1) - P.at(i1-1).at(j2) + P.at(i1-1).at(j1-1);
                    if(quadruple % 2 == 0) {
                        counter++;
                    }
                }
            }
        }
    }
    cout << counter << endl;

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
