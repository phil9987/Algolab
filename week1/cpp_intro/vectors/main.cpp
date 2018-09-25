#include <iostream>
#include <vector>

void testcase() {
    int n; std::cin >> n;
    std::vector<int> vec(n);

    for(int i = 0; i < n; i++) {
        std::cin >> vec.at(i);
    }
    int idx_to_remove; std::cin >> idx_to_remove;
    int start, end; std::cin >> start >> end;
    vec.erase(vec.begin() + idx_to_remove);
    vec.erase(vec.begin() + start, vec.begin() + end +1);
    if(vec.size() == 0) {
        std::cout << "Empty";
    } else {
        for(int i : vec) {
            std::cout << i << " ";
        }
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