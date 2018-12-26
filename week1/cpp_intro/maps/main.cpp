#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

void testcase() {
    int q; std::cin >> q;
    int a; std::string b;
    std::multimap<std::string, int> mmap;
    for(int i = 0; i < q; i++) {
        std::cin >> a >> b;
        if(a == 0) {
            mmap.erase(b);
        } else {
            mmap.insert(std::make_pair(b, a));
        }
    }
    std::string s; std::cin >> s;
    auto range = mmap.equal_range(s);
    if(range.first != mmap.end()) {
        std::vector<int> res;
        for (auto i = range.first; i != range.second; ++i) {
            res.push_back(i->second);
        }
        sort(res.begin(), res.end());
        for(int i : res) {
            std::cout << i << " ";
        }
    } else {
        std::cout << "Empty";
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