#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		int numFriends; cin >> numFriends;
		int numBoxes; cin >> numBoxes;
		vector<int> friends(numFriends);
		vector<int> boxes(numBoxes);
		for (int i = 0; i < numFriends; i++) {
			cin >> friends[i];
		}
		for (int i = 0; i < numBoxes; i++) {
			cin >> boxes[i];
		}


		sort(boxes.begin(), boxes.end(), greater<int>()); 
		sort(friends.begin(), friends.end(), greater<int>());

		vector<vector<int> > friends_;
		vector<vector<int> > boxes_;
		int last = 0;
		int numSoFar = 0;
		for(auto b: boxes) {
			if(last == b) {
				numSoFar++;
			} else {
				if (numSoFar > 0) {
					boxes_.push_back({last, numSoFar});
					last = b;
					numSoFar = 1;
				} else {
					last = b;
					numSoFar = 1;
				}
			}
		}
		boxes_.push_back({last, numSoFar});

		last = 0;
		numSoFar = 0;
		for(auto f: friends) {
			if(last == f) {
				numSoFar++;
			} else {
				if (numSoFar > 0) {
					friends_.push_back({last, numSoFar});
					last = f;
					numSoFar = 1;
				} else {
					last = f;
					numSoFar = 1;
				}
			}
		}
		friends_.push_back({last, numSoFar});

		if (boxes.front() > friends.front()) {
			cout << "impossible" << endl;
		} else {
			int rounds = 0;
			int boxesCarried = 0;
			while(boxesCarried < numBoxes) {
				for(auto f: friends_) {
					int friendStrength = f[0];
					int friendsLeft = f[1];
					for(int i = 0; i < boxes_.size(); i++) {
						int weight = boxes_[i][0];
						int n = boxes_[i][1];
						if(weight <= friendStrength && n > 0) {
							int boxCapacity = min(n, friendsLeft);
							friendsLeft -= boxCapacity;
							boxes_[i][1] -= boxCapacity;
							boxesCarried += boxCapacity;
							if (friendsLeft == 0) break;
						}
					}
				}
				rounds++;
			}
			cout << (rounds * 2) + (rounds -1) << endl;;
			
		}
	}

}