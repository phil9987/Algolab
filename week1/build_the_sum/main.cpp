#include <iostream>

using namespace std;

int main() {
	int T;
	cin >> T;
	for (int t = 0; t < T; t++){
		int N;
		cin >> N;
		double val = 0.0;
		for (int n = 0; n < N; n++){
			double v;
			cin >> v;
			val += v;
		}
		cout << val << endl;
	}
}