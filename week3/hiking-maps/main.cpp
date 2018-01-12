#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Triangle_2 T;

using namespace std;

inline bool contains(const vector<P>& t, const P& p) {
    return !CGAL::right_turn(t[0],t[1],p) && !CGAL::right_turn(t[2],t[3],p) && !CGAL::right_turn(t[4],t[5],p);
}

void do_testcase() {
    int m, num_map_parts; cin >> m >> num_map_parts;
    int num_legs = m-1;
    vector<P> path(m);
    vector<T> maps(num_map_parts);
    vector<vector<P> > triangles(num_map_parts);
    for(int i = 0; i < m; i++) {
        cin >> path.at(i);
    }

    for(int i = 0; i < num_map_parts; i++) {
        vector<P> points(6);
        for(int j = 0; j < 6; j++) {
            cin >> points.at(j);
        }
        // ensure correct order for orientation tests
        for (std::size_t j = 0; j < 6; j+=2) {
            if (CGAL::right_turn(points[j],points[j+1],points[(j+2)%6])) swap(points[j],points[j+1]);
        }
        triangles.at(i) = points;
    }

    vector<vector<bool>> contains_path(num_map_parts, vector<bool>(num_legs, false));
    for(int map_idx = 0; map_idx < num_map_parts; map_idx++) {
        for(int path_idx = 0; path_idx < num_legs; path_idx++) {
            T triangle = maps.at(map_idx);
            if(contains(triangles.at(map_idx), path.at(path_idx)) && contains(triangles.at(map_idx), path.at(path_idx +1))) {
                contains_path.at(map_idx).at(path_idx) = true;
                //clog << "map "  << map_idx << " contains path " << path_idx << endl;
            }
        }
    }

    size_t start = 0, end = 0, result = num_map_parts;
    vector<int> covered(num_legs, 0);

    while(end < num_map_parts) {
        bool cover = true;

        // add next map
        for(int i = 0; i < num_legs; i++) {
            if(contains_path.at(end).at(i)) {
                covered.at(i)++;
            } else {
                if(covered.at(i) == 0) {
                    cover = false;
                }
            }
        }

        while(cover && start <= end) {
            if((end+1) - start < result) {
                result = end+1 - start;
            }
            start++;
            for(int i = 0; i < num_legs; i++) {
                if(contains_path.at(start-1).at(i)) {
                    covered.at(i)--;
                    if(covered.at(i) == 0) {
                        cover = false;
                    }
                }
            }
        }
        end++;
    }
    cout << result << endl;
}

int main()
{
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
    int T; cin >> T;
    for(int t=0; t<T; t++) {
        do_testcase();
    }
}
