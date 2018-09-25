#include <iostream>
#include <vector>

void testcase() {
    int n; std::cin >> n;
    std::vector<int> nums(n+1);
    std::vector<int> psums(n+1);

    int num_even = 0;
    int num_odd = 0;
    for(int i = 1; i <= n; i++) {
        std::cin >> nums.at(i);
        psums.at(i) = psums.at(i-1) + nums.at(i);
        if(psums.at(i) % 2 == 0) {
            num_even++;
        } else {
            num_odd++;
        }
    }
    
    int res = num_even*(num_even -1) / 2 + num_odd*(num_odd -1) /2 + num_even;
    std::cout << res << '\n';

    

}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}