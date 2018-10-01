#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

void testcase() {
    int i;
    long long l;
    std::string s;
    double d;
    std::cin >> i >> l >> s >> d;
   // d = round(d*100.0)/100;
    std::cout << std::setprecision(2);
    std::cout << std::fixed;
    std::cout << i << " " << l << " " << s << " " << d << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;

    for(int i = 0; i < t; i++) {
        testcase();
    }
}