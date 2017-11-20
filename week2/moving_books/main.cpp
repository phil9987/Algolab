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
		sort(boxes.begin(), boxes.end()); 
		sort(friends.begin(), friends.end());

		int numCarryRounds = 0;
		int numReturnRounds = 0;
		int boxesCarried = 0;
		bool possible = true;

		vector<int> friendsRange(numFriends, 0);
		int currentFriend = 0;

		if (boxes.back() > friends.back()) {
			cout << "impossible" << endl;
		} else {
			for(auto w: boxes) {
				if (w <= friends[currentFriend]) {
					friendsRange[currentFriend] ++;
				} else {
					currentFriend++;
					friendsRange[currentFriend] = friendsRange[currentFriend -1] +1;
				}
			}
			while(currentFriend < numFriends) {
				currentFriend++;
				friendsRange[currentFriend] = friendsRange[currentFriend -1];
			}

			for (auto f: friendsRange) {
				cout << f << endl;
			}

			int boxesCarried = 0;
			int rounds = 0;
			int leftBound = 0;
			int carry = 0;

			for (int i = 0; i < numFriends; i++) {
				cout << "processing i=" << i << " with range=" << friendsRange[i] << " rounds so far=" << rounds << " carry = " << carry << endl;
				int boxesCarried = carry - friendsRange[i];
				if (friendsRange[i] >= carry) {
					boxesCarried = carry;
				}
				cout << "boxesCarried=" << boxesCarried << endl;
				friendsRange[i] -= (boxesCarried + carry);
				carry += boxesCarried;
				cout << "leftover range=" << friendsRange[i] << endl;
				if (friendsRange[i] > 0) {
					rounds += friendsRange[i];
					carry += friendsRange[i];
					cout << "new rounds=" << rounds << endl;
					friendsRange[i] = 0;
				}
				
			}
			cout << (rounds * 2) + (rounds -1) << endl;;
			
		}

		

		/*while (boxesCarried < numBoxes) {
			int boxesPerRound = 0;
			for(auto w: friends) {
				int i = 0;
				while(i < numBoxes && (boxes[i] == 0 || boxes[i] > w)) i++;
				if (i < numBoxes) {
					// friend can carry a box
					boxes[i] = 0;
					boxesPerRound++;
				}
			}
			boxesCarried+=boxesPerRound;
			if (boxesPerRound == 0 && boxesCarried < numBoxes) {
				// There are boxes which nobody can carry
				possible = false;
				break;
			} else {
				if (numCarryRounds > 0) {
					numReturnRounds++;
				}
				numCarryRounds++;
			}
		}*/

	}

}