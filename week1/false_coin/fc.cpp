#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

/*
idea: falsecoin is only there if weighting is not equal.

hence we can remove all candidates participating in the weighting if the result is equal
for the case of inequal weightings we keep 2 sets, a left and a right one. This is necessary because we don't know where the false coin is. We then always keep the intersection between the candidates_left and left and same for right
*/

void do_testcase(){
    int num_coins, num_weightings;
    cin >> num_coins >> num_weightings;
    set<int> candidates;
    for(int i = 1; i <= num_coins; i++) {
        candidates.insert(i);
    }
    set<int> candidates_left(candidates);
    set<int> candidates_right(candidates);
    for(size_t i = 0; i < num_weightings; i++) {
        int n; cin >> n;
        set<int> left;
        set<int> right;
        set<int> diff;
        set<int> intersect;
        for(size_t j = 0; j < n*2; j++) {
            int c; cin >> c;
            if(j < n) {
                left.insert(c);
            } else {
                right.insert(c);
            }
        }
        char s; cin >> s;

        switch (s) {
            case '<':   
            case '>':
                        set_intersection(candidates_left.begin(), candidates_left.end(), left.begin(), left.end(), inserter(intersect, intersect.begin()));
                        candidates_left = intersect;
                        intersect.clear();

                        set_intersection(candidates_right.begin(), candidates_right.end(), right.begin(), right.end(), inserter(intersect, intersect.begin()));
                        candidates_right = intersect;
                        intersect.clear();                      

                        break;

            case '=':   // remove all elements of this weighting from candidates_left and candidates_right
                        left.insert(right.begin(), right.end());
                        set_difference(candidates_left.begin(), candidates_left.end(), left.begin(), left.end(), inserter(diff, diff.begin()));
                        candidates_left = diff;
                        diff.clear();

                        set_difference(candidates_right.begin(), candidates_right.end(), left.begin(), left.end(), inserter(diff, diff.begin()));
                        candidates_right = diff;
                        diff.clear();
        }
    }
    if(candidates_left.size() == 1) {
        cout << *candidates_left.begin() << endl;
    } else if(candidates_right.size() == 1) {
        cout << *candidates_right.begin() << endl;
    } else {
        cout << 0 << endl;
    }  

}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) {
         try {
            do_testcase();
         } catch (const std::bad_alloc& e) {
        std::cout << "Allocation failed: " << e.what() << '\n';
        }
    }
}