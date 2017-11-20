#include <iostream>
#include <vector>

using namespace std;
vector<vector<int> > *colors;
vector<vector<int> > *memo;
int N;


// for first testset only where number of stacks <= 2
int MP(vector<int> is) {
	bool any_zero = false;
	for (auto e: is){
		if (e == 0){
			any_zero = true;
			break;
		}
	}
	if(any_zero && N == 2) {
		return 0;
	} else if(N > 2) {
		N--;
	}
	if (N == 2) {
		vector<int> is1 = is;
		is1[0]-=1;
		vector<int> is2 = is;
		is2[1]-=1;
		if ((*colors)[0][is[0]] == (*colors)[1][is[1]]){
			//cout << "colors are equal.." << endl;
			int m1 = max(MP(is1), MP(is2));
			is2[0] -=1;
			return max(m1, 1 + MP(is2));
		} else {
			//cout << "colors are not equal.." << endl;
			return max(MP(is1), MP(is2));
		}
	} else if ( N == 3) {
		cout << "n = 3" << endl;
		vector<int> is1 = is;
		is1[0]-=1;
		vector<int> is2 = is;
		is2[1]-=1;
		vector<int> is3 = is;
		is2[2]-=1;
		int m1 = max(MP(is1), MP(is2));
		int m2 = max(MP(is3), m1);
		if((*colors)[0][is[0]] == (*colors)[1][is[1]] && (*colors)[1][is[1]] == (*colors)[2][is[2]]) {
			is2[0] -=1;
			is2[2] -=1;
			return max(m2, 2 + MP(is2));
		} else if ((*colors)[0][is[0]] == (*colors)[1][is[1]]){
			//cout << "colors are equal.." << endl;
			is2[0] -=1;
			return max(m2, 1 + MP(is2));
		} else if((*colors)[1][is[1]] == (*colors)[2][is[2]]){
			//cout << "colors are equal.." << endl;
			is2[0] -=1;
			return max(m2, 1 + MP(is2));
		
		} else if((*colors)[0][is[0]] == (*colors)[2][is[2]]) {
			//cout << "colors are equal.." << endl;
			is1[0] -=1;
			return max(m2, 1 + MP(is1));
		} else {
			//cout << "colors are not equal.." << endl;
			return m2;
		}
	} else if (N == 4) {
	
	} else if (N == 5) {
	
	}
	else {
		cout << "not supported, only N <= 5!!" << endl;
	}
}

int main(){
	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		int n; cin >> n;
		N = n;
		vector<int> M(n);
		for (int i = 0; i < n; i++) {
			cin >> M[i];
		}
		
		vector<vector<int> > C(n);
		vector<vector<vector<vector<vector<int> > > > > DP;

		
		for (int i = 0; i < 5; i++) {

			C[i] = vector<int>(M[i] + 1, 0);
			for (int j = 1; j <= M[i]; j++) {
				cin >> C[i][j]; 
			}
		}
		vector<vector<int> > Memo(M[0] + 1);
		for(int i = 0; i <= M[0]; i++) {
			Memo[i] = vector<int>(M[1] + 1, -1);
		}
		colors = &C;
		memo = &Memo;
		
		/*for (auto v: *colors) {
			for(auto e: v) {
				cout << e << " ";
			}
			cout << endl;
		}
		for (auto v: *memo) {
			for(auto e: v) {
				cout << e << " ";
			}
			cout << endl;
		}*/

		int max_points = 0;
		//cout << "calculating max points for m0=" << M[0] << ", m1=" << M[1] << endl;
		max_points = MP(M);
	
		cout << max_points << endl;
	}
}
