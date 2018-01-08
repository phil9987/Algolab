#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef K::Point_2 P;


using namespace std;

// computes squared distance between p1 and p2
long sq_dist(P p1, P p2) {
	long a = p1.x() - p2.x();
	long b = p1.y() - p2.y();
	return a * a + b * b;
}

vector<int> winners(vector<P> const &lamps, vector<pair<P, long> > const &participants, int last_lamp_idx, int num_participants, long lamp_radius) {
	Triangulation t;
	t.insert(lamps.begin(), lamps.begin()+last_lamp_idx+1);
	vector<int> w;
	
	for(int i = 0; i < num_participants; i++) {
		P participant_location = participants.at(i).first;
		long participant_radius = participants.at(i).second;
		P nearest_lamp = t.nearest_vertex(participant_location)->point();
		long min_dist = (lamp_radius + participant_radius) * (lamp_radius + participant_radius);
		long actual_dist = sq_dist(nearest_lamp, participant_location);
		if(actual_dist >= min_dist){
			// participant i is not hit by any lamp!
			w.push_back(i);
		}
	}
	return w;
}

bool hitsAll(vector<P> const &lamps, vector<pair<P, long> > const &participants, int last_lamp_idx, int num_participants, long lamp_radius) {
		Triangulation t;
	t.insert(lamps.begin(), lamps.begin()+last_lamp_idx+1);
	
	for(int i = 0; i < num_participants; i++) {
		P participant_location = participants.at(i).first;
		long participant_radius = participants.at(i).second;
		P nearest_lamp = t.nearest_vertex(participant_location)->point();
		long min_dist = (lamp_radius + participant_radius) * (lamp_radius + participant_radius);
		long actual_dist = sq_dist(nearest_lamp, participant_location);
		if(actual_dist >= min_dist){

			// participant i is not hit by any lamp!
			return false;
		}
	}
	return true;
}


void do_testcase() {
	int num_participants, num_lamps;
	cin >> num_participants >> num_lamps;
	
	vector<pair<P, long> > participants(num_participants);
	for(int i = 0; i < num_participants; i++) {
		long x, y, r;
		cin >> x >> y >> r;
		P p(x,y);
		participants.at(i) = make_pair(p,r);
	}
	
	vector<P> lamps(num_lamps);
	
	int lamp_height; cin >> lamp_height;
	
	for(int i = 0; i < num_lamps; i++) {
		cin >> lamps.at(i);
	}
	vector<int> ws = winners(lamps, participants, num_lamps-1, num_participants, lamp_height);
	if(ws.size() > 0) {
		for(int w: ws) cout << w << " ";
	} else {
		// there is no winner, find the ones which are eliminated last...

		vector<int> ws0 = winners(lamps, participants, 0, num_participants, lamp_height);
		if(ws0.size() > 0) {
			// do binary search to find lamp index where there are still participants left
			int lo = 0;
			int up = num_lamps - 1;
			while(lo < up-1) {
				int mid = lo + ((up - lo) /2);
				if(hitsAll(lamps, participants, mid, num_participants, lamp_height)) {
					up = mid;
				} else {
					lo = mid;
				}
			}
			vector<int> ws = winners(lamps, participants, lo, num_participants, lamp_height);
			assert(ws.size() > 0);
			for(int w: ws) cout << w << " ";

		} else {
			// everybody is a winner
			for(int i = 0; i < num_participants; i++) {
				cout << i << " ";
			}
		}
	}
	
	cout << endl;
}

int main() {
	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		do_testcase();
	}
}
