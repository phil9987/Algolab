#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <boost/optional/optional_io.hpp>
#include <CGAL/Exact_integer.h>
#include <cmath>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
using namespace std;

double floor_to_double(const K::FT& x) {
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}

int main()
{

	struct CompareDistanceToRayOrigin {
		P origin;

	    CompareDistanceToRayOrigin(P origin) { this->origin = origin; }
	    bool operator () (S a, S b) { 
	    	bool aSourceFirst = CGAL::has_smaller_distance_to_point (origin, a.source(), a.target());
			bool bSourceFirst = CGAL::has_smaller_distance_to_point (origin, b.source(), b.target());
			if (aSourceFirst) {
				// source of a is closer to origin than target of a
				if (bSourceFirst) {
					// source of b is closer to origin than target of b
					return CGAL::has_smaller_distance_to_point (origin, a.source(), b.source());

				} else {
					// target of b is closer to origin than source of b
					return CGAL::has_smaller_distance_to_point (origin, a.source(), b.target());
				}
			} else {
				// target of a is closer to origin than source of a
				if (bSourceFirst) {
					// source of b is closer to origin than target of b
					return CGAL::has_smaller_distance_to_point (origin, a.target(), b.source());

				} else {
					// target of b is closer to origin than source of b
					return CGAL::has_smaller_distance_to_point (origin, a.target(), b.target());
				}
			}
	    }
	};



	int n; cin >> n;
	while (n > 0) {
		// reading in ray
		//cout << "handling testcase with n=" << n << endl;
		long long x, y, a, b;
		cin >> x >> y >> a >> b;
		P rayStart(x,y);
		P rayDirection(a,b);
		R ray(rayStart, rayDirection);
		bool hit = false;	
		P closest_intersection(0,0);
		

		for (int i = 0; i < n; i++) {
			//cout << "handling i=" << i << endl;
			long long e, f, g, h;
			cin >> e >> f >> g >> h;
			P start(e, f);
			P end(g, h);
			S seg(start, end);
			if(CGAL::do_intersect(ray, seg)){
				auto o = CGAL::intersection(ray,seg);
				if (const P* op = boost::get<P>(&*o)) {
					// intersection in point
					if (CGAL::has_smaller_distance_to_point (rayStart, *op, closest_intersection) || hit == false) {
						closest_intersection = *op;
					}
				}
				else if (const S* os = boost::get<S>(&*o)) {
					//cout << "intersection in segment" << endl;
					if (CGAL::has_smaller_distance_to_point (rayStart, os->source(), os->target())) {
						// source of segment is hit first
						if(CGAL::has_smaller_distance_to_point(rayStart, os->source(), closest_intersection) || hit == false) {
							closest_intersection = os->source();
						}
	
					} else {
						// target of segment is hit first
						if(CGAL::has_smaller_distance_to_point(rayStart, os->target(), closest_intersection) || hit == false) {
							closest_intersection = os->target();
						}
					}
				}
				else // how could this be? -> error
					throw std::runtime_error("strange segment intersection");
				
				hit = true;
			}
		}
		if (hit) {
			//sort(hitters.begin(), hitters.end(),CompareDistanceToRayOrigin(rayStart));
			//S firstHit = hitters[0];
			cout << fixed;
			cout << setprecision(0);
	  		cout << floor_to_double(closest_intersection.x()) << " " << floor_to_double(closest_intersection.y()) << "\n";
		} else {
			cout << "no" << endl;
		}

		cin >> n;
	}
}