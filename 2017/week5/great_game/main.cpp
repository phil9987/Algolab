#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <climits>

using namespace std;

unordered_map<int, vector<int> > transition_map;
unordered_map<int, int> even_memo;
unordered_map<int, int> odd_memo;
int N;

int steps(int i, bool even) {
    if (i == N) {
        return 0;
    }
    if(even) {
        if(even_memo.find(i) == even_memo.end()) {
            // not yet in memory...
            //cout << "not yet in even cache: " << i << endl;
            vector<int> ts = transition_map[i];
            int smallest_val = INT_MAX;
            for (int t: ts) {
                int new_val = steps(t, !even);
                if (new_val < smallest_val) {
                    smallest_val = new_val;
                }
            }
            even_memo[i] = smallest_val + 1;
        }
        return even_memo[i];
        
    } else {
        if(odd_memo.find(i) == odd_memo.end()) { 
            //cout << "not yet in odd cache: " << i << endl;
            // not yet in memory...
            vector<int> ts = transition_map[i];
            int largest_val = 0;
            for(int t: ts) {
                int new_val = steps(t, !even);
                if (new_val > largest_val) {
                    largest_val = new_val;
                }
            }
            odd_memo[i] = largest_val + 1;
        }
        return odd_memo[i];
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        int n, m, r, b;
        cin >> n >> m >> r >> b;
        N = n;
        vector< pair<int, int> > transitions (m);
        for (int i = 0; i < m ; i++) {
            int u, v; cin >> u >> v;
            transitions[i] = make_pair(u, v);
        }

        
        sort(transitions.begin(), transitions.end());
        int cur_target = 1;
        for (int i = 0; i < m;) {
            if(transitions[i].first == cur_target){
                vector<int> ts;
                ts.push_back(transitions[i].second);
                int j = i+1;
                while (transitions[j].first == cur_target){
                    ts.push_back(transitions[j].second);
                    j++;
                }
                transition_map[cur_target] = ts;
                i = j;
                cur_target = transitions[j].first;
            }
        }

        int sherlok = steps(r, true);
        //even_memo.clear();
        //odd_memo.clear();
        //cout << "." << endl;
        int moriarty = steps(b, true);
        bool holmes_wins  = (sherlok < moriarty) || ((sherlok == moriarty) && (sherlok % 2));
        //cout << sherlok << endl << moriarty << endl;
        cout << (!holmes_wins) << endl;
        even_memo.clear();
        odd_memo.clear();

    }

}