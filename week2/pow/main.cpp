#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	std::ios_base::sync_with_stdio(false);

	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		int N; cin >> N;
		vector<vector<int> > bombs(N, vector<int>(2));
		vector<int> times(N);
		for(int i = 0; i < N; i++) {
			int t;
			cin >> t;
			bombs[i] = {i, t};
			times[i] = t;
		}

		// sort bombs ascending by timer value
		sort(bombs.begin(), bombs.end(), [](vector<int> a, vector<int> b) {
        	return b[1] > a[1];   
    	});

    	int half = (N-1) / 2;
    	int time = 0;
    	bool exploded = false;
    	vector<bool> deactivated(N,false);

		for (auto b: bombs) {
			if (!deactivated[b[0]]) {
				// bomb is not yet deactivated, deactivate it...
				if (b[0] >= half) {
					// deactivate bomb
					deactivated[b[0]] = true;
					if(time >= b[1]) {
						exploded = true;
					}
					time++;
				}
				else {
					// deactivate bombs below first
					vector<int> indizes;
					int i = b[0];
					indizes.push_back(i);
					while (i < half) {
						i = (i*2) + 1;
						if(deactivated[i] && deactivated[i+1]){
							break;
						}
						indizes.push_back(i);
						indizes.push_back(i+1);
						i++;
					}
					reverse(indizes.begin(), indizes.end());
					for(auto idx: indizes) {
						if(!deactivated[idx]){
							deactivated[idx] = true;
							if(time >= times[idx]) {
								//cout << "time=" << time << " but bomb_time=" << times[idx] << endl;
								exploded = true;
							}
							time++;

						}
					}
				}
				//cout << "time=" << time << endl;
			}
		}

		if(exploded) {
			cout << "no" << endl;
		} else {
			cout << "yes" << endl;
		}
	}

}