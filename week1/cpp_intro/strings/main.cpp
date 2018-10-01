#include <iostream>
#include <string>

void reverseString(std::string &str) {
    int len = str.length();

    for(int i = 0; i < len/2; i++) {
        std::swap(str[i], str[len-1-i]);
    }
}

void testcase() {
    std::string s1, s2;
    std::cin >> s1 >> s2;
    std::cout << s1.length() << " " << s2.length() << "\n";
    std::cout << s1 + s2 << "\n";
    reverseString(s1);
    reverseString(s2);
    std::swap(s1[0], s2[0]);
    std::cout << s1 << " " << s2 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
}