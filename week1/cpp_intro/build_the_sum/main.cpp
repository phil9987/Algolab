#include <iostream>

void testcase() {
    int result = 0;
    int n; std::cin >> n;
    for(int i = 0; i < n; i++) {
        int tmp; std::cin >> tmp;
        result += tmp;
    }
    std::cout << result << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
}