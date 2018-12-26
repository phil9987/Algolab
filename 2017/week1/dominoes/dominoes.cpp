#include <iostream>
#include <vector>

using namespace std;

void do_testcase() {
    size_t n; cin >> n;
    vector<size_t> d(n);
    for(size_t i = 0; i < n; i++) {
        cin >> d.at(i);
    }
    size_t next_falling = d.at(0) -1;
    size_t num_falling = 1;

    for(size_t i = 1; i < n; i++) {
        if(next_falling > 0) {
            next_falling = max(next_falling -1, d.at(i)-1);
            num_falling++;
        } else {
            break;
        }
    }
    cout << num_falling << endl;
}

int main() {
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) {
        do_testcase();
    }

}