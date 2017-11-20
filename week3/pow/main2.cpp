#include <iostream>
#include <vector>

using namespace std;
int main() {
	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		int N, M; cin >> N >> M;

		vector<pair<int, int> > jedis(N);
		vector<int> segments(M);

		for (int i = 0; i < N; i++) {
			int a, b; cin >> a >> b;
			jedis[i] = make_pair(a,b);

		}

		// find optimal starting point


	}

}