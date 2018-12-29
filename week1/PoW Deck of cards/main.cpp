#include <iostream>
#include <vector>
#include <cmath>

void testcase() {
    int n, k; std::cin >> n >> k;
    std::vector<int> nums(n+1);
    std::vector<int> psum(n+1, 0);
    for(int i = 1; i <= n; i++) {
        std::cin >> nums.at(i);
        psum.at(i) = psum.at(i-1) + nums.at(i);
    }
    int i = 1;
    int j = 1;
    int best_i = 1; 
    int best_j = 1;
    int min_difference = std::abs(psum.at(0) - k);
    while(min_difference > 0 && i <= n && j <= n) {
        int sum = psum.at(j) - psum.at(i-1);
        int diff = std::abs(sum - k);
        if(diff < min_difference) {
            best_i = i;
            best_j = j;
            min_difference = diff;
        }
        if(sum < k) {
            // move right border
            j++;
        } else {
            if(i < j) {
                // move left border
                i++;
            } else {
                // move both left and right border
                i++;
                j++;
            }
        }
    }
    std::cout <<  best_i-1 << " " << best_j-1 << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}