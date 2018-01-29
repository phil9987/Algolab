// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <set>

// Namespaces
using namespace std;

// Functions
// =========
// Function for an individual testcase
void testcase() {
    int n, i, k;
    cin >> n >> i >> k;
    vector<unsigned> disks(n);

    for(size_t a = 0; a < n; a++) {
        unsigned tmp; cin >> tmp;
        tmp = tmp % k;
        disks.at(a) = tmp;
    }
    bool found = false;
    unsigned n1 = n/2;
    unsigned n2 = n - n1;

    //vector<unsigned> L1(1 << n1);
    set<unsigned> L1;
    for ( size_t s = 1; s < 1 << n1; ++s ){ // iterate through all subsets
        int sum = 0;
        bitset<30> bits(s);
        for ( int j = 0; j < min(n, 30); ++j ){
            if(bits.test(j)) sum += disks.at(j);    // if i - th element in subset
        }
        L1.insert(sum % k);
    }

    for (size_t s = 1; s < 1 << n2 && !found; ++s) {
        int sum = 0;
        bitset<30> bits(s);
        for(size_t j = 0; j < min(n, 30); ++j) {
            if(bits.test(j)) sum += disks.at(n1+j);
        }
        sum = sum %k;
        int target = i-sum;
        if(target < 0) {
            target = k + target;
        } else {
            target = target % k;
        }

        auto it = lower_bound(L1.begin(), L1.end(), target);
        if(it != L1.end() && (*it +  sum) % k == i) {
            found = true;
        }
    }

    if(found) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }



}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
