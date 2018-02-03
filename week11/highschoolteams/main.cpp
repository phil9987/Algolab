// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include "prettyprint.hpp"

using namespace std;
int B = 22;

vector<vector<int> > enumerate_all_possibilities(vector<vector<int> > s, int n, int N) {
	if(n < 0) return s;
	else {
		auto snew = s;
		if(s.size() == 0) {
			vector<int> b0(N, 0);
			vector<int> b1(N, 0);
			b1.at(n) = 1;
			vector<int> b2(N, 0);
			b2.at(n) = 2;
			snew.push_back(b0);
			snew.push_back(b1);
			snew.push_back(b2);
		} else {
			vector<int> b(N, 0);
			for(auto seq: s) {
				for(int i = 0; i < 3; i++) {
					b = seq;
					b.at(n) = i;
					snew.push_back(b);
				}
			}
		}
		return enumerate_all_possibilities(snew, n-1, N);
	}
}

void testcase() {

	int n, k;
	cin >> n >> k;
	vector<long> skills(n);
	for(int i = 0; i < n; i++) {
		cin >> skills.at(i);
	}

	set<vector<int> > valid_constellations;

	for(auto b : enumerate_all_possibilities({}, n-1, n)) {
		long sum_a = 0;
		long sum_b = 0;
		int k_count = 0;
		bool valid = true;
		for(int i = 0; i < n; i++) {
			int el = b.at(i);
			if(el == 0) {
				sum_a += skills.at(i);
			} else if(el == 1) {
				sum_b += skills.at(i);
			} else {
				k_count++;
				if(k_count > k) {
					valid = false;
					break;
				}
			}
		}
		if(valid && (sum_a - sum_b) == 0) {
			valid_constellations.insert(b);
		}

	}
	cout << valid_constellations.size() << endl;

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
