#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void do_testcase() {
    size_t n; cin >> n;
    vector<int> parasols(n);
    for(size_t i = 0; i < n; i++) {
        cin >> parasols.at(i);
    }

    sort(parasols.begin(), parasols.end());     // sort locations ascending
    //for(auto p: parasols) clog << p << " ";
    //cout << endl;
    //clog << parasols.back() << " - " << parasols.front() << " = " << parasols.back() - parasols.front() << endl;

    size_t total_coverage = parasols.back() - parasols.front();
    vector<int> optimal_spots;
    size_t max_dist = SIZE_MAX;
    size_t max_nb_parasols = 0;
    if(total_coverage <= 200) {
        // all parasols are within reach
        max_nb_parasols = parasols.size();
        if(total_coverage % 2 == 0) {
            // there is 1 optimal spot for the beach bar
            max_dist = total_coverage / 2;
            optimal_spots.push_back(parasols.front() + max_dist);
        } else {
            // there are 2 optimal spots for the beach bar
            max_dist = total_coverage / 2 + 1;
            optimal_spots.push_back(parasols.back() - max_dist);
            optimal_spots.push_back(parasols.front() + max_dist);
        }

    } else {
        // we need to use a sliding window to find the possible locations
        size_t l = 0;
        size_t r = 1;
        size_t nb_parasols = 2;
        do {
            nb_parasols += 1;
            if(r < parasols.size() -1 ) {
                r++;
            } else {
                break;
            }
            size_t total_coverage = parasols.at(r) - parasols.at(l);
            while(total_coverage > 200 && l < parasols.size()) {
                nb_parasols -= 1;
                l++;
                total_coverage = parasols.at(r) - parasols.at(l);
            }
            //clog << "sliding window loop start: " << l << " " << r << " total_coverage = " <<  total_coverage << endl;
            
            if(nb_parasols > max_nb_parasols){
                //clog << "number of parasols is greater than max so far: " << nb_parasols << endl;
                // new candidate, clear best result so far
                optimal_spots.clear();
                max_nb_parasols = nb_parasols;
                if(total_coverage % 2 == 0) {
                    // there is 1 optimal spot for the beach bar
                    max_dist = total_coverage / 2;
                    optimal_spots.push_back(parasols.at(l) + max_dist);
                } else {
                    // there are 2 optimal spots for the beach bar
                    max_dist = total_coverage / 2 + 1;
                    optimal_spots.push_back(parasols.at(r) - max_dist);
                    optimal_spots.push_back(parasols.at(l) + max_dist);
                }
                

            } else if (nb_parasols == max_nb_parasols) {
                // new candidate
                size_t dist = 0;
                if(total_coverage % 2 == 0) {
                    // there is 1 optimal spot for the beach bar
                    dist = total_coverage / 2;
                    if(dist == max_dist) {
                        optimal_spots.push_back(parasols.at(l) + max_dist);
                    } else if(dist < max_dist) {
                        optimal_spots.clear();
                        max_dist = dist;
                        optimal_spots.push_back(parasols.at(l) + max_dist);
                    }
                } else {
                    // there are 2 optimal spots for the beach bar
                    size_t dist = total_coverage / 2 + 1;
                    if(dist == max_dist) {
                        optimal_spots.push_back(parasols.at(r) - max_dist);
                        optimal_spots.push_back(parasols.at(l) + max_dist);
                    } else if(dist < max_dist) {
                        optimal_spots.clear();
                        max_dist = dist;
                        optimal_spots.push_back(parasols.at(r) - max_dist);
                        optimal_spots.push_back(parasols.at(l) + max_dist);
                    }
                }
            }
            //clog << "sliding window loop end: " << l << " " << r<< endl;
        } while(l <= parasols.back() - 200); 
    }
    cout << max_nb_parasols << " " << max_dist << endl;
    for(auto s: optimal_spots) cout << s << " ";
    cout << endl;


}

int main() {
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) {
        do_testcase();
    }
    return 0;
}