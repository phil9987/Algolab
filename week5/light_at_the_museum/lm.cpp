#include <iostream>
#include <vector>

using namespace std;

void do_testcase() {
    size_t num_switches, num_rooms;
    cin >> num_switches >> num_rooms;       // 1 <= x <= 30
    vector<size_t> target_brightness(num_rooms);

    for(size_t i = 0; i < num_rooms; i++) {
        cin >> target_brightness.at(i);
    }
}

int main() {
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) do_testcase;
    return 0;
}