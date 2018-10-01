#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
using namespace std;

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
		long long x, y, a, b;
		cin >> x >> y >> a >> b;
		P rayStart(x,y);
		P rayDirection(a,b);
		R ray(rayStart, rayDirection);
		bool hit = false;	
		vector<S> hitters;

		for (int i = 0; i < n; i++) {
			long long e, f, g, h;
			cin >> e >> f >> g >> h;
			P start(e, f);
			P end(g, h);
			S seg(start, end);
			if(CGAL::do_intersect(ray, seg)){
				hit = true;
				hitters.push_back(seg);
			}
		}
		if (hit) {
			sort(hitters.begin(), hitters.end(),CompareDistanceToRayOrigin(rayStart));
			S firstHit = hitters[0];
			auto o = CGAL::intersection(ray,firstHit);
        	if (const P* op = boost::get<P>(&*o))
	  			std::cout << round((*op).x()) << " " << round((*op).y()) << "\n";
        	else if (const S* os = boost::get<S>(&*o))
        		// segment is parallel to ray, get point closer to origin of ray
        		if (CGAL::has_smaller_distance_to_point (rayStart, os->source(), os->target())) {
        			// source of segment is hit first
        			std::cout << round(os->source().x()) << " " << round(os->source().y()) << endl;

        		} else {
        			// target of segment is hit first
        			std::cout << round(os->target().x()) << " " << round(os->target().y()) << endl;

        		}
        	else // how could this be? -> error
          		throw std::runtime_error("strange segment intersection");
    		//cout << "yes" << endl;
		} else {
			cout << "no" << endl;
		}

		cin >> n;
	}
}