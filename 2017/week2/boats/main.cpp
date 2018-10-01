#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int new_right_border(int right_border, pair<int,int> boat) {
	// assuming that the ring of the boat is on the right of right_border
	if (right_border <= boat.second) {
		if (right_border <= boat.second - boat.first) {
			// completely push boat towards left
			return boat.second;
		} else {
			// put boat at current border
			return right_border + boat.first;
		}
	} else {
		cerr << "ERROR: boat left of right_border" << endl;
		return -1;
	}
}


int main() {
	std::ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	for (int t = 0; t < T; t++) {
		int N; cin >> N;
		vector<pair<int, int> > lengthPos(N);
		vector<vector<int> > chosenBoats;
		for (int i = 0; i < N; i++) {
			int l; int p;
			cin >> l;
			cin >> p;
			lengthPos[i] = make_pair(l,p);
		}
		sort(lengthPos.begin(), lengthPos.end(), [](pair<int,int> a, pair<int,int> b) {
			return a.second < b.second;   
			});
		int current_right_end = lengthPos[0].second;
		int max_num_boats = 1;

		/*for(auto b: lengthPos) {
			cout << b.first << " "<< b.second << endl;
		}*/

		for(int i = 1; i < lengthPos.size(); i++) {
			//cout << "checking boat " << i << endl;
			if(lengthPos[i].second >= current_right_end) {
				int next_min_right_end = new_right_border(current_right_end, lengthPos[i]);
				int next_idx = i;
				//cout << "boat " << i << " has smallest right end so far: " << next_min_right_end << endl;				
				for(int j = i+1; j < lengthPos.size(); j++) {
					if(lengthPos[j].second > next_min_right_end) break;
					if(lengthPos[j].second >= current_right_end) {
						int tmp = new_right_border(current_right_end, lengthPos[j]);
						if (tmp < next_min_right_end) {
							next_min_right_end = tmp;
							//cout << "boat " << j << "has smallest right end so far: " << next_min_right_end << endl;
							next_idx = j;
						}
					}
				}
				//cout << "choosing boat " << next_idx << endl;
				i = next_idx;
				current_right_end = next_min_right_end;
				max_num_boats++;
			}
		}
		cout << max_num_boats << endl;
	}

}