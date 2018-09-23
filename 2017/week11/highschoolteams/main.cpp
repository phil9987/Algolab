// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include "prettyprint.hpp"
using namespace std;

vector<pair<vector<int>, int> > enumerate_all_possibilities(vector<pair<vector<int>, int> > s, int n, int N, int k) {
	if(n < 0) return s;
	else {
		auto snew = s;
		if(s.size() == 0) {
			vector<int> b0(N, 0);
			vector<int> b1(N, 0);
			b1.at(n) = 1;
			vector<int> b2(N, 0);
			b2.at(n) = 2;
			snew.push_back(make_pair(b0, 0));
			snew.push_back(make_pair(b1, 0));
			if(k > 0) {
				snew.push_back(make_pair(b2, 1));
			}
		} else {
			vector<int> b(N, 0);
			for(auto tup: s) {
				auto seq = tup.first;
				auto k_count = tup.second;
				int max = 3;
				if(k_count == k) max = 2;
				for(int i = 0; i < max; i++) {
					b = seq;
					if(!(b.at(n) == i)) {
						b.at(n) = i;
						if(i == 2) k_count++;
						snew.push_back(make_pair(b, k_count));
					}
				}
			}
		}
		return enumerate_all_possibilities(snew, n-1, N, k);
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

	for(auto tup : enumerate_all_possibilities({}, n1-1, n1, k)) {
		auto b = tup.first;
		auto k_count = tup.second;
		long sum_a = 0;
		long sum_b = 0;
		for(int i = 0; i < n1; i++) {
			int el = b.at(i);
			if(el == 0) {
				sum_a += skills.at(i);
			} else if(el == 1) {
				sum_b += skills.at(i);
			}
		}
		possible_sums1.push_back(make_pair(sum_a - sum_b, k_count));
	}

	sort(possible_sums1.begin(), possible_sums1.end());		// sort ascending
	vector<long> sums1;
	vector<int> k1;
	for(auto el: possible_sums1) {
		sums1.push_back(el.first);
		k1.push_back(el.second);
	}

	size_t num_possibilities = 0;
	for(auto tup : enumerate_all_possibilities({}, n2-1, n2, k)) {
		auto b = tup.first;
		auto k_count = tup.second;
		long sum_a = 0;
		long sum_b = 0;
		for(int i = 0; i < n2; i++) {
			int el = b.at(i);
			if(el == 0) {
				sum_a += skills.at(n1+i);
			} else if(el == 1) {
				sum_b += skills.at(n1+i);
			}
		}
		long q = -(sum_a - sum_b);
		auto start_it = lower_bound(sums1.begin(), sums1.end(), q);
		if(start_it != sums1.end() && *start_it == q) {
			auto end_it = upper_bound(start_it, sums1.end(), q);
			size_t i_start = start_it - sums1.begin();
			size_t i_end = end_it - sums1.begin();
			if(k1.at(i_start) + k_count <= k) {
				int tmp_num_possibilities = 1;
				
				if(i_end - i_start > 1) {
					auto it2 = upper_bound(k1.begin()+i_start, k1.begin()+i_end, k-k_count);
					size_t i_end_real = it2 - k1.begin();
					tmp_num_possibilities = i_end_real - i_start;
				}
				num_possibilities += tmp_num_possibilities;
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
