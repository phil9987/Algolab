#include <iostream>
#include <vector>

using namespace std;

int choose (int n, int k) {
	if (k == 0) return 1;
	return (n * choose (n-1, k-1)) / k;
}

int main() {
	int T;
	cin >> T;
	for (int t = 0; t < T; t++) {
		int N; cin >> N;
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

		int numEven = 0;
		int E = 0;
		int O = 0;

		for (int i = 0; i < N; i++) {
			if (S[i] % 2  == 0) {
				E++;
			} else {
				O++;
			}
		}

		numEven = choose(E,2) + choose(O, 2) + E;


		/*for (int i = 1; i < N; i++) {
			for (int j = i; j < N; j++) {
				if ((S[j] - S[i-1]) % 2 == 0) {
					numEven ++;
				}
			}
		}
		for (int i = 0; i < N; i++) {
			if (S[i] % 2 == 0) {
				numEven++;
			}
		}*/

		cout << numEven << endl;
	}
}