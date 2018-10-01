#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
	int T;
	cin >> T;
	for (int t = 0; t < T; t++) {
		int N; cin >> N;
		int k; cin >> k;
		vector<int> bits(N);
		for (int i = 0; i < N; i++) {
			cin >> bits[i];
		}

		// precompute partial sums
		vector<int> S(N);
		S[0] = bits[0];
		for (int i = 1; i < N; i++) {
			S[i] = S[i-1] + bits[i];
		}

		// initialize with any element
		int closestDifference = abs(S[0] - k);
		int I = 0;
		int J = 0;
		int i = 0;
		int j = 0;

		while (closestDifference > 0 && i < N && j < N) {
			int sum;
			if (i == 0) {	// edge case
				sum = S[j];
			} else {
				sum = S[j] - S[i-1];
			}
			int diff = abs(sum - k);
			if (diff < closestDifference) {
				closestDifference = diff;
				I = i;
				J = j;
			}
			if (sum < k) {	// move right border of sliding window
				j++;
			} else {
				if (i < j) {
					i++;
				} else {
					i++;
					j++;
				}
			}
		}

		cout << I << " " << J << endl;
	}
}