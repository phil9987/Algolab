#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
using namespace std;

int main()
{
	int n; cin >> n;
	while (n > 0) {
		// reading in ray
		long long x, y, a, b;
		cin >> x >> y >> a >> b;
		P rayStart(x,y);
		P rayDirection(a,b);
		R ray(rayStart, rayDirection);
		bool hit = false;	

		for (int i = 0; i < n; i++) {
			long long e, f, g, h;
			cin >> e >> f >> g >> h;
			P start(e, f);
			P end(g, h);
			S seg(start, end);
			if(!hit && CGAL::do_intersect(ray, seg)){
				hit = true;
			}
		}
		if (hit) {
			cout << "yes" << endl;
		} else {
			cout << "no" << endl;
		}

		cin >> n;
	}
}