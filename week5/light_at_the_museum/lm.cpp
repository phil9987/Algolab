#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

vector<vector<int > > switch_circuit;
unsigned num_switches, num_rooms;

vector<int> diff(vector<int> j, vector<int> k) {
    vector<int> res(j.size());
    for(size_t i = 0; i < k.size(); i++) {
        res.at(i) = j.at(i) + k.at(i);
    }
    return res;
}

bool is_zero(vector<int> j) {
    for(int e: j) if(e > 0) return false;
    return true;
}

vector<int> change_switches(vector<int> start, bitset<30> switches) {
    vector<int> end = start;
    for(unsigned i = 0; i < num_switches; i++) {
        if(switches.test(i)) {
            // change this switch
            for(unsigned j = 0; j < end.size(); j++) {
                end.at(j) += switch_circuit.at(i).at(j);
            }
        }
    }
    return end;
}

void do_testcase() {

    cin >> num_switches >> num_rooms;       // 1 <= x <= 30
    vector<int> target_brightness(num_rooms);
    vector<int> initial_brightness(num_rooms);
    switch_circuit.clear();


    for(size_t i = 0; i < num_rooms; i++) {
        cin >> target_brightness.at(i);
    }
    for(size_t i = 0; i < num_switches; i++) {
        vector<int> light_states(num_rooms);
        for(size_t j = 0; j < num_rooms; j++) {
            int on, off; cin >> on >> off;
            light_states.at(j) = off - on;
            initial_brightness.at(j) += on;
        }
        switch_circuit.push_back(light_states);
    }
    size_t min_count = SIZE_MAX;
    for(int i = 0; i < (1 << num_switches); i++) {
        bitset<30> to_switch(i);
        if(change_switches(initial_brightness, to_switch) == target_brightness) {
            min_count = min(min_count, to_switch.count());
        }
    }

    if(min_count == SIZE_MAX) {
        cout << "impossible" << endl;
    } else {
        cout << min_count << endl;
    }


    /*for(auto e: target_brightness) {
        clog << e << ", ";
    }
    clog << endl;
    auto a = switch_circuit.at(0);
    clog << "a.size() = " << a.size() << endl;
    for(auto e: switch_circuit.at(0)) {
        clog << e << "; ";
    }
    clog << endl;
    for(auto e: diff(target_brightness, switch_circuit.at(0))) {
        clog << e << " ";
    }
    clog << endl;
    clog << "." << endl;*/

}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) do_testcase();
    return 0;
}