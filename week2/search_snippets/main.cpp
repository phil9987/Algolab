#include <iostream>
#include <vector>
#include <algorithm>

bool contains_all_words(std::vector<int> & vec, int start, int end) {
    return std::all_of(vec.begin() + start, vec.begin() + end + 1, [](int i){ return i > 0; });
}

int interval_size(std::vector<std::pair<int, int> > & Occ, int start, int end) {
    int start_ = Occ.at(start).first;
    int end_ = Occ.at(end).first;
    return end_ - start_ + 1;
}

void testcase() {
    int n; std::cin >> n;
    std::vector<int> M(n);
    int N = 0;

    for(int i = 0; i < n; i++) {
        std::cin >> M.at(i);
        N += M.at(i);
    }
    std::vector<std::pair<int, int> > Occ;  // list of pair<pos, word_idx>
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < M.at(i); j++) {
            int pos; std::cin >> pos;
            Occ.push_back(std::make_pair(pos, i));
        }
    }
    std::sort(Occ.begin(), Occ.end());  // sort ascending by first element, positions are distinct
    std::vector<int> counts(n, 0);     // how many times does word i occur in current interval
    int start = 0;                      // current interval start (inclusive)
    int end = 0;                        //current interval end (inclusive)
    

    // find initial interval that contains every word at least once
    counts.at(Occ.at(0).second) = 1;
    while(!contains_all_words(counts, 0, n-1)) {
        end++;
        if(end < N) {
            counts.at(Occ.at(end).second)++;
        } else {
            std::cout << "ERROR: not all words are contained" << std::endl;
        }
    }
    int smallest_valid_interval = interval_size(Occ, start, end);

    while(start <= end && start < N && end < N) {
        int start_idx = Occ.at(start).second;
        if(counts.at(start_idx) > 1) {
            // start can be shifted to the right
            counts.at(start_idx)--;
            start++;
            smallest_valid_interval = std::min(smallest_valid_interval, interval_size(Occ, start, end));
        } else {
            // end has to be shifted to the right
            end++;
            if(end < N) {
                counts.at(Occ.at(end).second)++;
            }
        }
    }

    std::cout << smallest_valid_interval << std::endl;
    




}


int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}