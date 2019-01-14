// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

void testcase() {
    int n;
    cin >> n;
    vector<int> parasols(n);
    for(int i = 0; i < n; i++) {
        cin >> parasols.at(i);
    }
    sort(parasols.begin(), parasols.end());
    int start = 0;
    int end = 0;
    int num_umbrellas = 1;
    int current_max = 0;
    int min_longest_dist = INT_MAX;
    vector<int> centers;
    while(end < n) {
        int current_distance = abs(parasols.at(end) - parasols.at(start));
        //cout << "handling start=" << start << " (" << parasols.at(start) << ") end=" << end << "(" << parasols.at(end) << ") dist = " << current_distance << endl;
        if (num_umbrellas >= current_max && current_distance <= 200) {
            int dist = (current_distance + 1) / 2;
            if (num_umbrellas > current_max) {
                centers.clear();
                current_max = num_umbrellas;
                min_longest_dist = dist;
            }
            if (dist < min_longest_dist) {
                centers.clear();
                min_longest_dist = dist;
            }
            if (dist == min_longest_dist) {
                //cout << "found new optimal center(s): " << parasols.at(start) + dist << " dist=" << dist << endl;
                min_longest_dist = min(min_longest_dist, dist);
                centers.push_back(parasols.at(start) + dist);
                if(current_distance % 2 == 1) {
                    centers.push_back(parasols.at(start) + dist - 1);
                }
            }
        }
        if (current_distance < 200) {
            end++;
            num_umbrellas++;
        } else {
            start++;
            num_umbrellas--;
        }
    }
    sort(centers.begin(), centers.end());
    cout << current_max << ' ' << min_longest_dist <<'\n';
    for(auto el : centers) {
        cout << el << ' ';
    }
    cout << endl;

}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	testcase();
	return 0;
}

