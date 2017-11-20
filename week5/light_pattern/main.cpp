#include <iostream>
#include <vector>

using namespace std;


u_int f(vector<bool> v,int current_idx, bool flipped) {
    int len = v.size();
    if (current_idx > len -1) return 0;
    if (current_idx == len - 1) return v[current_idx];
    else {
        bool bit = v[current_idx];
        if ((bit && !flipped) || (!bit && flipped)) {
            // min set bit to zero vs flip all bits until this bit
            ////cout << "is 1: "  << bit << " " << flipped << " " << current_idx << " " << endl;
            return min(f(v, current_idx + 1, !flipped) + 1, f(v, current_idx + 1, flipped) + 1);
        } else {
            // continue with next bit
            //cout << "is 0: "  << bit << " " << flipped << " " << current_idx << " " << endl;
            return f(v, current_idx + 1, flipped);
        }
    }
}

int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        u_int num_bulbs, num_per_pattern, x;
        cin >> num_bulbs >> num_per_pattern >> x;        

        //cout << all_1_value << endl;
        vector<bool> val(num_bulbs);
        for (u_int i = 0; i < num_bulbs; i++) {
            bool tmp; cin >> tmp;
            val[i] = tmp;
        }

        //for (auto b : val) cout << b << " ";
        //cout << endl;

        cout << f(val, 0, false) << endl;
    }
}