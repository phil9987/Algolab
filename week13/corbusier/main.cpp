// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

// Namespaces
using namespace std;

// Functions
// =========
// Function for an individual testcase
void testcase() {
    unsigned n, i, k;
    cin >> n >> i >> k;
    vector<unsigned> disks(n);

    for(size_t a = 0; a < n; a++) {
        unsigned tmp; cin >> tmp;
        tmp = tmp % k;
        disks.at(a) = tmp;
    }
    bool found = false;
    for ( int s = 1; s < 1 << n && !found ; ++ s ){ // iterate through all subsets
        int sum = 0;
        bitset<30> bits(s);
        for ( int i = 0; i < 30 ; ++ i ){
            if(bits.test(i)) sum += disks.at(i);    // if i - th element in subset
        }
        sum = sum % k;
        if ( sum == i ){
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
