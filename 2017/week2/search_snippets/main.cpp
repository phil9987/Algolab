#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
	std::ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	for (int t = 0; t < T; t++) {
		int N; cin >> N;
		vector<int> M(N);
		int numCounts = 0;
		for (int i = 0; i < N; i++) {
			cin >> M[i];
			numCounts += M[i];
		}
		vector< vector <int> > P(N);
		for (int i = 0; i < N; i++) {
			vector<int> tmp(M[i]);
			for (int j = 0; j < M[i]; j++) {
				cin >> tmp[j];
			}
			P[i] = tmp;
		}

		int smallesDistance = abs(P[0][0]-P[1][0]);
		int i = 0;
		int j = 0;
		while (i < P[0].size() && j < P[1].size()) {
			int tmpDist = abs(P[0][i] - P[1][j]);
			if (tmpDist < smallesDistance) {
				smallesDistance = tmpDist;
			}
			if (P[0][i] < P[1][j]) {
				i++;

			} else {
				j++;
			}
		}
		cout << smallesDistance + 1 << endl;
	}

}