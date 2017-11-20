#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <functional>

using namespace std;
bool pairCompare(const std::pair<int, int>& firstElem, const std::pair<int, int>& secondElem) {
	if(firstElem.second == secondElem.second){
		return firstElem.first > secondElem.first;
	}else{
		return firstElem.second < secondElem.second;
	}
}

// The problem is that there is not a segment which is protected by exactly one jedi... 
// But there is a segment which is protected by no jedi at all. So this means we start there but there might be multiple jedis to check...
void max_jedis(){
	int n; cin >> n;
	int m; cin >> m;

	vector<pair<int,int> > jedis(n);
	vector<pair<int,int> > startend(2*n);


	for (int i = 0; i < n; i++){
		int a, b; cin >> a >> b;
		jedis[i] = make_pair(a,b);
		startend[i] = make_pair(a, i);
		startend[n+i] = make_pair(b, n+i);
	}
	std::sort(jedis.begin(), jedis.end(), pairCompare);
	vector<int> started(n, 0);
	int offset = 0;
	for(int i = 0; i < 2*n; i++) {
		int id = startend[i].second;
		if (id > n) {
			// it's an end, so increase offset by 1 and set interval to started
			offset++;
			started[id-n] = 1;
		} else {
			// it's the first start, so break loop
			break;
		}
	}
	pair<int, int> lowestOverlap = make_pair(0, offset);
	int currentOverlap = offset;

	for(int i = 0; i < 2*n; i++) {
		int id = startend[i].second;
		if(id > n) {
			// it's an end of interval
			currentOverlap--;
			if(currentOverlap < lowestOverlap.second && startend[i+1].first != startend[i].first) {
				// update lowest overlap
				lowestOverlap = make_pair(i, currentOverlap);
			}

		} else {
			// it's a start of interval
			currentOverlap++;
			if(currentOverlap < lowestOverlap.second && startend[i+1].first != startend[i].first){
				lowestOverlap = make_pair(i, currentOverlap);
			}
		}
	}
	cout << lowestOverlap.first << " " << lowestOverlap.second << endl;
	cout << "lowest overlap" << endl;

	// find optimal start
	sort(startend.begin(), startend.end());
	for(auto s: startend) {
		if (s.second < n) {
			// it's a start
			started[s.first] = 1;
		} else {
			// it's an end
			if (started[s.first] == 1) {
				// all good, the interval was recorded and ends here, decreasing overlap-count
				started[s.first] = 0;


			} else {
				// we have a problem here.. this interval started already before and we didn't account for it in overlap-count, adjusting overlap count...
			}
		}
	}







	int curr_finish = -1;
	vector<pair<int,int> > selected_jedis;
	int num_jedis = 0;
	int earliest_start = 1; 
	for (int i = 0; i < n; i++){
		int a, b;
		a = jedis[i].first;
		b = jedis[i].second;
		if (a > b){
			jedis.push_back(make_pair(a, m + b));	// this jedi overlaps over 1, so we push it to the end, make it start later
		}
		else{		// this jedi is not ignored and the next finish-line!!
			if (curr_finish == -1){ // first earliest finish time that is not ignored
				curr_finish = b;
				earliest_start = a;
				selected_jedis.push_back(jedis[i]);
				num_jedis += 1;
			}
			else{
				if(curr_finish < a){		//start time of current jedi is larger than end time of latest selected jedi -> select jedi
					curr_finish = b;
					selected_jedis.push_back(jedis[i]);
					num_jedis += 1;
				}								// else: don't select jedi, do nothing
			}
		}
	}
	int new_n = jedis.size();
	earliest_start += m;
	for(int i=n; i < new_n; i++){
		int a, b;
		a = jedis[i].first;
		b = jedis[i].second;
		if(curr_finish < a && b < earliest_start){
			selected_jedis.push_back(jedis[i]);
			num_jedis += 1;
			curr_finish = b;
		}

	}

	/*for (auto j: selected_jedis) {
		cout << j.first << " " << j.second << endl;
	}*/


	cout << num_jedis << endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	for (int t = 0; t < T; t++){
		//cout << t << " of " << (T-1) << endl;
		max_jedis();
	}
}