#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

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

vector<int> change_switches(bitset<30> switches, int offset) {
    vector<int> end(num_rooms, 0);
    for(unsigned i = offset; i < num_switches; i++) {
        if(switches.test(i-offset)) {
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

    unsigned N1, N2;
    N1 = num_switches / 2;
    N2 = num_switches - N1;
    vector<vector<int> > L2(1<<N2);
    vector<pair<vector<int>, int> > L2_S(1<<N2);

    // precompute 2^N/2 possibilities for N2
    for(int i = 0; i < (1 << N2); i++) {
        bitset<30> to_switch(i);
        L2_S.at(i) = make_pair(change_switches(to_switch, N1), to_switch.count());
    }

    sort(L2_S.begin(), L2_S.end());
    for(int i = 0; i < 1<<N2; i++) L2.at(i) = L2_S.at(i).first;
    size_t min_count = SIZE_MAX;

    for(int i = 0; i < (1 << N1); i++) {
        bitset<30> to_switch(i);
        vector<int> l1 = change_switches(to_switch, 0);
        vector<int> l2_wanted(num_rooms);
        for(unsigned j = 0; j < num_rooms; j++) {
            l2_wanted.at(j) = target_brightness.at(j) - initial_brightness.at(j) - l1.at(j);
        }
        auto it = lower_bound(L2.begin(), L2.end(), l2_wanted);
        if(*it == l2_wanted) {
            int idx = it - L2.begin();
            min_count = min(min_count, L2_S.at(idx).second + to_switch.count());
        }
    }
    



    /*for(int i = 0; i < (1 << num_switches); i++) {
        bitset<30> to_switch(i);
        if(change_switches(initial_brightness, to_switch) == target_brightness) {
            min_count = min(min_count, to_switch.count());
        }
    }*/

    if(min_count == SIZE_MAX) {
        cout << "impossible" << endl;
    } else {
        cout << min_count << endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) do_testcase();
    return 0;
}