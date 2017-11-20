#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;

using namespace std;

int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        int N; cin >> N;
        vector<P> starts(N);
        vector<P> ends(N);
        vector<pair<R,int> > rays(N);
        for (int i = 0; i < N; i++) {
            long long y0, x1, y1;
            cin >> y0 >> x1 >> y1;
            starts[i] = P(0, y0);
            ends[i] = P(x1, y1);
            rays[i] = make_pair(R(starts[i], ends[i]), i);
        }
        sort(rays.begin(), rays.end(), [](pair<R,int> a, pair<R,int> b) {
            return a.first.source().y() > b.first.source().y();
        });
        // pair <<winner, loser>, intersection_point>
        vector< pair<int,int> > intersections;
        for(int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                if (CGAL::do_intersect(rays[i].first, rays[j].first)) {
                    //auto o = CGAL::intersection(rays[i], rays[j]);
                    //const P* op = boost::get<P>(&*o);
                    auto compRes = CGAL::compare_slopes(rays[i].first.supporting_line(), rays[j].first.supporting_line());

                    if (compRes == CGAL::SMALLER) {
                        // ray i has shorter distance to intersection and wins
                        pair<int,int> p = (make_pair(rays[i].second,rays[j].second));
                        intersections.push_back(p);
                    } else if (compRes == CGAL::LARGER) {
                        // the two rays diverge, this case should never happen...
                        cout << "WTF, diverging rays intersect!!" << endl;
                    } else {
                        cout << "WTF, parallel rays intersect!!" << endl;
                    }
                    //double slopei = abs(starts[i].y() - ends[i].y()) / abs(ends[i].x());
                    //double slopej = abs(starts[j].y() - ends[j].y()) / abs(ends[j].x());
                    //cout << *op << endl;
                }
            }
        }

        vector<int> endedOnes;

        //cout << "intersection coordinates:" << endl;
        for(auto i: intersections) {
            //cout << i.second.x() << " " << i.second.y() << endl;
            endedOnes.push_back(i.second);
        }
        sort(endedOnes.begin(), endedOnes.end());
        //cout << "end" << endl;
        int i = 0;
        for(auto e: endedOnes) {
            cout << " (" << e << ") ";
            while (i < e) {
                cout << i << " ";
                i++;
            }
            if(i==e) i++;
        }
        while(i < N) {
            cout << i << " ";
            i++;

        }
        cout << endl;
    }

}


/*
  P p[] = { P(0,0), P(2,0), P(1,0), P(3,0), P(.5,1), P(.5,-1) };
  S s[] = { S(p[0],p[1]), S(p[2],p[3]), S(p[4],p[5]) };
  for (int i = 0; i < 3; ++i)
    for (int j = i+1; j < 3; ++j)
      if (CGAL::do_intersect(s[i],s[j])) {
	auto o = CGAL::intersection(s[i],s[j]);
        if (const P* op = boost::get<P>(&*o))
	  std::cout << "point: " << *op << "\n";
        else if (const S* os = boost::get<S>(&*o))
	  std::cout << "segment: " << os->source() << " " 
                    << os->target() << "\n";
        else // how could this be? -> error
          throw std::runtime_error("strange segment intersection");
      } else
	std::cout << "no intersection\n";
}
*/