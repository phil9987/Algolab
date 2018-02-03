// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

set<vector<int> > enumerate_all_possibilities(set<vector<int> > s, int n, int N) {
	if(n < 0) return s;
	else {
		auto snew = s;
		if(s.size() == 0) {
			vector<int> b0(N, 0);
			vector<int> b1(N, 0);
			b1.at(n) = 1;
			vector<int> b2(N, 0);
			b2.at(n) = 2;
			snew.insert(b0);
			snew.insert(b1);
			snew.insert(b2);
		} else {
			vector<int> b(N, 0);
			for(auto seq: s) {
				for(int i = 0; i < 3; i++) {
					b = seq;
					b.at(n) = i;
					snew.insert(b);
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

	int n1 = n/2;
	int n2 = n - n1;

	vector<pair<long, int> > possible_sums1;

	for(auto b : enumerate_all_possibilities({}, n1-1, n1)) {
		long sum_a = 0;
		long sum_b = 0;
		int k_count = 0;
		bool valid = true;
		for(int i = 0; i < n1; i++) {
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
		if(valid) {
			possible_sums1.push_back(make_pair(sum_a - sum_b, k_count));
		}
	}

	sort(possible_sums1.begin(), possible_sums1.end());		// sort ascending
	vector<long> sums1;
	vector<int> k1;
	for(auto el: possible_sums1) {
		sums1.push_back(el.first);
		k1.push_back(el.second);
	}

	size_t num_possibilities = 0;
	for(auto b : enumerate_all_possibilities({}, n2-1, n2)) {
		long sum_a = 0;
		long sum_b = 0;
		int k_count = 0;
		bool valid = true;
		for(int i = 0; i < n2; i++) {
			int el = b.at(i);
			if(el == 0) {
				sum_a += skills.at(n1+i);
			} else if(el == 1) {
				sum_b += skills.at(n1+i);
			} else {
				k_count++;
				if(k_count > k) {
					valid = false;
					break;
				}
			}
		}
		if(valid) {
			long q = -(sum_a - sum_b);
			auto it = lower_bound(sums1.begin(), sums1.end(), q);

			while(it != sums1.end() && *it == q && k1.at(it-sums1.begin()) + k_count <= k) {
				num_possibilities++;
				it++;
			}

		}

	}
	cout << num_possibilities << endl;

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
