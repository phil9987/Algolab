#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <queue>

enum Status { Unvisited = 0, Free = 2, Visited = 3 };

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Status,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Face_circulator Face_circulator;
typedef K::Point_2 P;

using namespace std;


bool touching(P p1, P p2, long long dist2) {
    return CGAL::squared_distance(p1, p2) < dist2;
}

bool can_escape(const Triangulation &t, P user, long dist2) {
    // init info fields of faces: set all infinite faces to Free, set all finite faces to Unvisited
    
    // get face where user is located
    Face_handle f1 = t.locate(user);
    // base case: user is too close to an infected person initially
    Vertex_handle v = t.nearest_vertex(user);
    if(touching(user, v->point(), dist2)) {
        //cout << "<touching>-";
        return false;
    } else if(f1->info() == Free) {
        // user is already in an infinite face
        //cout << "<infinite_face>-";
        return true;
    } else {
        // we are inside the triangulation, check  if he can escape
        bool escape = false;
        queue<Face_handle> q;
        vector<Face_handle> visited_faces;
        f1->info() = Visited;
        visited_faces.push_back(f1);
        q.push(f1);
        while(!q.empty() && !escape) {
            Face_handle f = q.front();
            q.pop();
            if(f->info() == Free) {
                // we reached an infinite face!
                escape = true;
                continue;
            }
            for(int i = 0; i < 3; i++) {
                P p1 = f->vertex(i)->point();
                P p2 = f->vertex((i+1)%3)->point();
                Face_handle f2 = f->neighbor((i+2)%3);
                if(!touching(p1, p2, 4*dist2) && f2->info() != Visited){       // if user fits through gap between vertex i and i+1
                    if(f2->info() == Free){
                         escape = true;
                         continue;
                     }
                    f2->info() = Visited;
                    visited_faces.push_back(f2);
                    q.push(f2);
                }
            }
        }
        
        //reset visited face-infos
        for(Face_handle fh: visited_faces) {
            fh->info() = Unvisited;
        }
        //cout << "<might_escape>-";

        return escape;
    }
}

void do_testcase(size_t num_infected_people) {
    // read points
    vector<P> infection_coord(num_infected_people);
    for (std::size_t i = 0; i < num_infected_people; i++) {
        cin >> infection_coord.at(i);
    }
    // construct triangulation
    Triangulation t;
    t.insert(infection_coord.begin(), infection_coord.end());
    Triangulation::Face_circulator fc = t.incident_faces(t.infinite_vertex());
    do {
        fc->info() = Free;
    } while (++fc != t.incident_faces(t.infinite_vertex()));
    for (Face_iterator fi = t.finite_faces_begin(); fi != t.finite_faces_end(); ++fi) {
        fi->info() = Unvisited;
    }
    
    size_t num_users; cin >> num_users;
    vector<P> user_coord(num_users);
    vector<long long> user_dist(num_users);
    for(size_t i = 0; i < num_users; i++) {
        cin >> user_coord.at(i) >> user_dist.at(i);
        if(can_escape(t, user_coord.at(i), user_dist.at(i))) {
            cout << 'y';
        } else {
            cout << 'n';
        }
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    size_t n; cin >> n;
    while(n > 0) {
        do_testcase(n);
        cin >> n;
    }
    return 0;
}

