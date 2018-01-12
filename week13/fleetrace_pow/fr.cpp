// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <map>



using namespace std;
vector<vector<pair<int, int> > > boat_sailor;
int num_boats;
map<string, int> memo;

int next_valid_sailor_idx(int startidx, vector<pair<int,int> > sailors, set<int> available_sailors) {

    while(startidx < sailors.size() && available_sailors.find(sailors.at(startidx).first) == available_sailors.end()) {
        startidx++;
    }
    return startidx;
}

string create_key(int bi, int si, set<int> sailors) {
    string out = to_string(bi) + "_" + to_string(si) + "_";
    for(int e: sailors) {
        out += to_string(e) + "_";
    }
    return out;
}



int largest_sum(int bi, int si, set<int> available_sailors) {
    //cout << "bi=" << bi << " si=" << si << " b="<<b<<" available_sailors="<<available_sailors.size() <<endl;
    if(bi == -1 || available_sailors.size() == 0) {
        return 0;
    } else {
        // take sailor pair (bi, si) --> remove sailor from available_sailors, increase bi, add spectacle_coeff
        // don't take sailor pair (bi, si) --> keep sailor in available_sailors, increase si
        set<int> available_sailors_ = available_sailors;
        int next_sailor_idx = next_valid_sailor_idx(si, boat_sailor.at(bi), available_sailors);
        if(next_sailor_idx < boat_sailor.at(bi).size()) {
            int sailor_idx = boat_sailor.at(bi).at(next_sailor_idx).first;
            int spectacle_coeff = boat_sailor.at(bi).at(next_sailor_idx).second;
            int r = available_sailors_.erase(sailor_idx);

            string s1 = create_key(bi-1, 0, available_sailors_);     
            string s2 = create_key(bi, next_sailor_idx-1, available_sailors);
            if (memo.find(s1) == memo.end()) {
                memo[s1] = largest_sum(bi-1, 0, available_sailors_);
            }
            int res1 = memo[s1];
            if (memo.find(s2) == memo.end()){
                memo[s2] = largest_sum(bi, next_sailor_idx+1, available_sailors);
            }
            int res2 = memo[s2];

            //cout << res1 << " " << res2 << endl;

            return max(res1 + spectacle_coeff, res2);
        }
        else {
            string s3 = create_key(bi-1, 0, available_sailors);
            if(memo.find(s3) == memo.end()) {
                memo[s3] = largest_sum(bi-1, 0, available_sailors);
            }
            int res3 = memo[s3];
            return res3;
        }
    }
}

void do_testcase() {
    memo.clear();

    unsigned s, p;
    std::cin >> num_boats >> s >> p;
    boat_sailor = vector<vector<pair<int, int> > >(num_boats);
    set<int> sailors;
    for(int i = 0; i < s; i++) sailors.insert(i);
  
    for(size_t i = 0; i < p; i++) {
        int bi, si;
        std::cin >> bi >> si;
        int c;
        std::cin >> c;
        boat_sailor.at(bi).push_back(make_pair(si, c));
    }

    cout << largest_sum(num_boats-1, 0, sailors) << endl;
}

int main() {
    int T; std::cin >> T;

    for(int t = 0; t < T; t++) {
        do_testcase();
    }
    return 0;
}
