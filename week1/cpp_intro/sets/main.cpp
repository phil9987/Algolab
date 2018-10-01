#include <iostream>
#include <set>

void testcase() {
    int q; std::cin >> q;
    std::set<int> s;

    for(int i = 0; i < q; i++) {
        int a, b; std::cin >> a >> b;
        if(a == 0) {
            s.insert(b);
        } else {
            auto it = s.find(b);
            if(it != s.end())
                it = s.erase(it);
            //s.erase(b);
        }
    }
    if(s.size() == 0) {
        std::cout << "Empty" << std::endl;
    } else {
        for(int i : s) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}