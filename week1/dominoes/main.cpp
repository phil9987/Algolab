#include <iostream>
#include <vector>
#include <cmath>

void testcase() {
    int n; std::cin >> n;
    std::vector<int> dominoes(n);
    for(int i = 0; i < n; i++){
        std::cin >> dominoes.at(i);
    }
    int current_reach = 0;
    int current_stone = 0;
    while(current_reach >= current_stone && current_stone < n) {
        current_reach = std::max(current_reach, current_stone + dominoes.at(current_stone)-1);
        current_stone++;
    }
    std::cout << current_stone << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}