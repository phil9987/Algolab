#include <iostream>
#include <algorithm>
#include <vector>

void testcase() {
    int n; std::cin >> n;
    std::vector<int> vec(n);
    for(int i = 0; i < n; i++) {
        std::cin >> vec.at(i);
    }
    int x; std::cin >> x;
    if(x == 0) {
        // sort increasing
        std::sort(vec.begin(), vec.end());
    } else {
        //sort decreasing
        std::sort(vec.begin(), vec.end(), [](int a, int b) {
        return a > b;   
        });
    }

    for(int i : vec) {
        std::cout << i << " ";
    }
    std::cout << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}