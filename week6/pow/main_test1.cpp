#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;
vector<vector<int> > *colors;
vector<vector<vector<vector<vector<int> > > > > *memo;

vector<vector<int> > getEqualityIndizes(vector<int> elements) {
	vector<vector<int> > res;
	int el_size = elements.size();
	//cout << el_size << endl;
	unordered_set<int> used_colors;
	for(int i = 0; i < el_size; i++) {
		int el = elements[i];
		if (el > 0 && used_colors.find(el) == used_colors.end()) {
			//cout << "running for loop for element " << i << " with color " << el << endl;
			// it is a color and color is not yet used
			used_colors.insert(el);
			vector<int> elements_until_now = {i};
			res.push_back(elements_until_now);
			int num_elements = 1;
			for(int j = i+1; j < el_size; j++) {
				//cout << "next_idx = " << j << endl;
				int next_el = elements[j];
				//cout << "color: " << next_el << endl;
				if (next_el == el) {
					num_elements++;	
					vector<int> single = {j};
					res.push_back(single);				
					if (num_elements == 2) {
						elements_until_now.push_back(j);
						//cout << "new element added to elements_until now:" << j << endl;
						res.push_back(elements_until_now);
						//cout << "new element added to res" << endl;
					} else if(num_elements == 3) {
						//cout << "adding 3rd element" << endl;
						for(auto idx: elements_until_now) {
							vector<int> tmp = {idx, j};
							//cout << "created new vector with elements ";
							//for(auto s: tmp) cout << s << " and ";
							//cout << endl;

							res.push_back(tmp);
						}
						elements_until_now.push_back(j);
						res.push_back(elements_until_now);
					} else if(num_elements == 4) {
						for(auto idx: elements_until_now) {
							vector<int> tmp = {idx, j};
							res.push_back(tmp);
						}
						vector<int> tmp = {elements_until_now[0], elements_until_now[1], j};
						res.push_back(tmp);
						tmp = {elements_until_now[0], elements_until_now[2], j};
						res.push_back(tmp);
						tmp = {elements_until_now[1], elements_until_now[2], j};

						res.push_back(tmp);
						elements_until_now.push_back(j);
						res.push_back(elements_until_now);
					} else if(num_elements == 5) {
						for(auto idx: elements_until_now) {
							vector<int> tmp = {idx, j};
							res.push_back(tmp);
						}
						vector<int> tmp = {elements_until_now[0], elements_until_now[1], j};
						res.push_back(tmp);
						tmp = {elements_until_now[0], elements_until_now[2], j};
						res.push_back(tmp);
						tmp = {elements_until_now[0], elements_until_now[3], j};
						res.push_back(tmp);
						tmp = {elements_until_now[1], elements_until_now[2], j};
						res.push_back(tmp);
						tmp = {elements_until_now[1], elements_until_now[3], j};
						res.push_back(tmp);
						tmp = {elements_until_now[2], elements_until_now[3], j};
						res.push_back(tmp);

						tmp = {elements_until_now[0], elements_until_now[1], elements_until_now[2], j};
						res.push_back(tmp);
						tmp = {elements_until_now[0], elements_until_now[1], elements_until_now[3], j};
						res.push_back(tmp);
						tmp = {elements_until_now[0], elements_until_now[2], elements_until_now[3], j};
						res.push_back(tmp);
						tmp = {elements_until_now[1], elements_until_now[2], elements_until_now[3], j};
						res.push_back(tmp);

						elements_until_now.push_back(j);
						res.push_back(elements_until_now);
					}
				}
			}
		} else {
			// it's not a color, this index is not to be considered
		}

	}
	return res;

}

int MP(vector<int> idxs) {
	if((*memo)[idxs[0]][idxs[1]][idxs[2]][idxs[3]][idxs[4]] == -1) {

		vector<vector<int> > indizes = getEqualityIndizes({(*colors)[0][idxs[0]], (*colors)[1][idxs[1]], (*colors)[2][idxs[2]], (*colors)[3][idxs[3]], (*colors)[4][idxs[4]]});
		vector<int> res_candidates;

		for( vector<int> vec : indizes) {
			int score = 0;
			int s = vec.size();
			if (s > 1) score = pow(2, vec.size()-2);
			//cout << "size=" << s << " score=" << score << endl;
			vector<int> new_idxs = idxs;
			for(auto i: vec) new_idxs[i] -=1;
			int temp_res = score + MP(new_idxs);
			res_candidates.push_back(temp_res);
		}
		if (res_candidates.size() > 0) {
			(*memo)[idxs[0]][idxs[1]][idxs[2]][idxs[3]][idxs[4]] = *max_element(res_candidates.begin(), res_candidates.end());//cout << (*memo)[idxs[0]][idxs[1]][idxs[2]][idxs[3]][idxs[4]] << " ";
			//for(auto i: idxs) cout << i << " ";
			//cout << endl;
		} else {
			(*memo)[idxs[0]][idxs[1]][idxs[2]][idxs[3]][idxs[4]] = 0;
		}
	}
	return (*memo)[idxs[0]][idxs[1]][idxs[2]][idxs[3]][idxs[4]];
}

int main(){
	int T; cin >> T;

	for (int t = 0; t < T; t++) {
		// reading in number of stacks n (n<=5)
		int n; cin >> n;
		vector<int> M(5,0);
		// reading in stack sizes
		for (int i = 0; i < n; i++) {
			cin >> M[i];
		}
		
		vector<vector<int> > C(5);

		for (int i = 0; i < n; i++) {
			// read in colors of i-th stack, C[i][0] is always empty (no color)
			C[i] = vector<int>(M[i] + 1, 0);
			for (int j = 1; j <= M[i]; j++) {
				cin >> C[i][j];
			}
		}
		// set colors of non-existent stacks to [0]
		for(int i = n; i < 5; i++) {
			C[i] = vector<int>(1,0);
		}
		vector<vector<vector<vector<vector<int> > > > > Memo(M[0] + 1, vector<vector<vector<vector<int> > > >(M[1] + 1, vector<vector<vector<int> > >(M[2] + 1, vector<vector<int> >(M[3] + 1, vector<int>(M[4] + 1, -1)))));
		colors = &C;
		memo = &Memo;

		int max_points = MP(M);
		cout << max_points << endl;
		Memo.clear();
		C.clear();
		/*vector<vector<int> > test = getEqualityIndizes({1,1,1,0,0});
		cout << test.size() << endl;
		for(auto v1: test) {
			for(auto el:v1) {
				cout << el << " ";
			}
			cout << endl;
		}*/
	}
}
