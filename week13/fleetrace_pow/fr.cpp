// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <set>

using namespace std;
vector<vector<pair<int, int> > > boat_sailor;
int b;

int next_valid_sailor_idx(int startidx, vector<pair<int,int> > sailors, set<int> available_sailors) {

    while(startidx < sailors.size() && available_sailors.find(sailors.at(startidx).first) == available_sailors.end()) {
        startidx++;
    }
    return startidx;
}



int largest_sum(int bi, int si, set<int> available_sailors) {
    //cout << "bi=" << bi << " si=" << si << " b="<<b<<" available_sailors="<<available_sailors.size() <<endl;
    if(bi == b || available_sailors.size() == 0) {
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

            //cout << "erased " << r << " element from available_sailors_: " << available_sailors_.size() << " old: " << available_sailors.size() << endl;
                // there is a next sailor for this boat
            return max(largest_sum(bi+1, 0, available_sailors_) + spectacle_coeff, largest_sum(bi, next_sailor_idx+1, available_sailors));
        }
        else {
            return largest_sum(bi+1, 0, available_sailors);
        }
    }
}

void do_testcase() {

    unsigned s, p;
    std::cin >> b >> s >> p;
    boat_sailor = vector<vector<pair<int, int> > >(b);
    set<int> sailors;
    for(int i = 0; i < s; i++) sailors.insert(i);
  
    for(size_t i = 0; i < p; i++) {
        int bi, si;
        std::cin >> bi >> si;
        int c;
        std::cin >> c;
        boat_sailor.at(bi).push_back(make_pair(si, c));
    }

    cout << largest_sum(0, 0, sailors) << endl;
}

int main() {
    int T; std::cin >> T;

    for(int t = 0; t < T; t++) {
        do_testcase();
    }
    return 0;
}
