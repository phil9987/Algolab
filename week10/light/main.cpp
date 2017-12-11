#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;


using namespace std;

long sq_dist(K::Point_2 p1, K::Point_2 p2) {
	long a = p1.x() - p2.x();
	long b = p1.y() - p2.y();
	return a * a + b * b;
}

void do_testcase() {
	int num_participants, num_lamps;
	cin >> num_participants >> num_lamps;
	
	vector<K::Point_2 > participants_points(num_participants);
	vector<long> participants_radius(num_participants);
	for(int i = 0; i < num_participants; i++) {
		long x, y, r;
		cin >> x >> y >> r;
		K::Point_2 p(x,y);
		participants_points.at(i) = p;
		participants_radius.at(i) = r;
	}
	
	vector<K::Point_2> lamps(num_lamps);
	
	int lamp_height; cin >> lamp_height;
	
	for(int i = 0; i < num_lamps; i++) {
		cin >> lamps.at(i);
	}
	
	// construct triangulation
	Triangulation t;
	t.insert(lamps.begin(), lamps.end());
	
	for(int i = 0; i < num_participants; i++) {
		K::Point_2 nearest_lamp = t.nearest_vertex(participants_points.at(i))->point();
		long min_dist = (lamp_height + participants_radius.at(i)) * (lamp_height + participants_radius.at(i));
		long actual_dist = sq_dist(nearest_lamp, participants_points.at(i));
		//cout << "nearest_lamp: " << nearest_lamp << " participant: " << participants_points.at(i) << endl;
		//cout << "min_dist= " << min_dist << " actual_dist= " << actual_dist << endl;
		//if(CGAL::squared_distance(nearest_lamp,participants_points.at(i)) >= min_dist) {
		if(actual_dist >= min_dist){
			//cout << "hit by lamp!" << endl;
			cout << i << " ";
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
