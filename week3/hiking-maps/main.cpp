#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Triangle_2 T;

using namespace std;

void do_testcase() {
    int m, num_map_parts; cin >> m >> num_map_parts;
    int num_legs = m-1;
    vector<P> path(m);
    vector<T> maps(num_map_parts);
    for(int i = 0; i < m; i++) {
        cin >> path.at(i);
    }

    for(int i = 0; i < num_map_parts; i++) {
        vector<L> lines(3);
        for(int j = 0; j < 3; j++) {
            vector<P> ps(2);
            for(int k = 0; k < 2; k++) cin >> ps.at(k);
            lines.at(j) = L(ps.at(0), ps.at(1));
        }
        vector<P> vertices(3);
        for(int j = 0; j < 3; j++) {
            // calculate line intersections
            auto o = CGAL::intersection(lines.at(j),lines.at((j+1)%3));
            if (const P* op = boost::get<P>(&*o)){
                vertices.at(j) = *op;
            } else cerr << "ERROR: two lines do not intersect!" << endl;
        }
        maps.at(i) = T(vertices.at(0), vertices.at(1), vertices.at(2));
    }

    vector<vector<bool>> contains_path(num_map_parts, vector<bool>(num_legs, false));
    vector<vector<bool>> starting_at_map(num_map_parts, vector<bool>(num_legs, false));
    vector<int> starting_at_map_stop_idx(num_map_parts, -1);
    vector<int> cost(num_map_parts);
    int min_cost = INT_MAX;
    for(int map_idx = 0; map_idx < num_map_parts; map_idx++) {
        for(int path_idx = 0; path_idx < num_legs; path_idx++) {
            T triangle = maps.at(map_idx);
            if(!triangle.has_on_unbounded_side(path.at(path_idx)) && !triangle.has_on_unbounded_side(path.at(path_idx+1))) {
                contains_path.at(map_idx).at(path_idx) = true;
                //clog << "map "  << map_idx << " contains path " << path_idx << endl;

                for(int i = map_idx; i < num_map_parts; i++) {
                    starting_at_map.at(i).at(path_idx) = true;
                    //clog << "setting path " << path_idx << " for map " << i << endl;
                }
            }
        }
    }

    // binary search over possible lengths:
    size_t l = 1;
    size_t u = num_map_parts-1;
    size_t res = 0;
    while(l <= u) {
        size_t p = l + ((u - l) / 2);
        clog << p << endl;
        // check if sequence of length p exists
        size_t start = 0;
        bool found = false;
        /*for(int i = 0; i < num_map_parts; i++) {
            clog << "map" << i << ":\t";
            for(int j = 0; j < num_legs;j ++) {
                clog << contains_path.at(i).at(j) << ",";
            }
            clog << endl;
        }*/
        while(start + p <= num_map_parts && !found) {
            vector<bool> paths(num_legs, false);
            int covered = 0;
            for(size_t i = start; i < start + p; i++) {
                //clog << "start = " << start << endl;
                for(size_t j = 0; j < num_legs; j++) {
                    //clog << "paths.at(j): " << paths.at(j) << " contains_path.at(" << i << ").at(" << j << "): " << contains_path.at(i).at(j) << endl;
                    if(contains_path.at(i).at(j) == true && paths.at(j) == false) {
                        paths.at(j) = true;
                        covered++;
                    }
                    //for(int h = 0; h < num_legs; h++) clog << paths.at(h) << " ";
                    //clog << endl << "covered: " << covered << endl;
                }
                if(covered == num_legs){
                    found = true;
                    break;
                }
            }
            start++;
        }
        if(found == true) {
            // there exists a sequence of length p, search in [l, p) for sequence
            u = p-1;
            //clog << "there exists a sequence of length " << p << endl;
        } else {
            // there doesn't exist a sequence of length p, search in (p,u) for sequence
            l = p+1;
            //clog << "there doesnt exist a sequence of length " << p << endl;
        }
        //clog << "start=" << start << " p=" << p << endl;
    }

    // l is the minimal length 


    cout << l << endl;


}

int main()
{
	ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
    int T; cin >> T;
    for(int t=0; t<T; t++) {
        do_testcase();
    }
}
