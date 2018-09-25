#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef boost::disjoint_sets_with_storage<>     UnionFind;

// Namespaces
using namespace std;

void do_testcase() {
    size_t n, m, p;
    cin >> n >> m >> p;

    vector<pair<P, size_t> > jammers(n);
    vector<pair<P, P> > missions(m);

    for(size_t i = 0; i < n; i++) {
        // read in jammer positions
        P loc;
        cin >> loc;
        jammers.at(i) = make_pair(loc, i);
    }
    Triangulation jammer_triag;
    jammer_triag.insert(jammers.begin(), jammers.end());

    vector<bool> initially_covered(m, true);        // indicates whether start and end location of mission i are covered under p-umbrella
    vector<pair<int, int> > missions_closest_center(m);

    vector<tuple<size_t, size_t, K::FT> > edges;

    for (Edge_iterator e = jammer_triag.finite_edges_begin(); e != jammer_triag.finite_edges_end(); ++e) {
        // get the vertices of e 
        Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
        Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
        K::FT dist = CGAL::squared_distance(v1->point(), v2->point());
        edges.push_back(make_tuple(v1->info(), v2->info(), dist));
    }

    sort(edges.begin(), edges.end(), [](tuple<size_t, size_t, K::FT> a, tuple<size_t, size_t, K::FT> b) {
        return get<2>(a) < get<2>(b);   
    });

    UnionFind UFP(n);

    for(auto e = edges.begin(); e != edges.end() && get<2>(*e) <= p; ++e) {
        UFP.union_set(get<0>(*e), get<1>(*e));
    }
    K::FT a = 0;
    K::FT b = 0;
    UnionFind UFA(n);
    UnionFind UFB(n);
    auto ai = edges.begin();
    auto bi = edges.begin();
    for(size_t i = 0; i < m; i++) {
        // read in missions
        // start and target position
        P s, t;
        cin >> s >> t;
        missions.at(i) = make_pair(s, t);

        Triangulation::Vertex_handle vs = jammer_triag.nearest_vertex(s);
        Triangulation::Vertex_handle vt = jammer_triag.nearest_vertex(t);
        P closest_jammer_to_s = vs->point();
        P closest_jammer_to_t = vt->point();
        K::FT dist = 4*max(CGAL::squared_distance(s, closest_jammer_to_s), CGAL::squared_distance(t, closest_jammer_to_t));


        if( dist <= p and UFP.find_set(vs->info()) == UFP.find_set(vt->info())) {
            // mission is possible with p-umbrella
            cout << 'y';
            //  make sure mission is also possible in b
            b = max(b, dist);
            for(; bi != edges.end() && UFB.find_set(vs->info()) != UFB.find_set(vt->info()); ++bi) {
                UFB.union_set(get<0>(*bi), get<1>(*bi));
            }
        } else {
            // mission is not possible with p-umbrella
            cout << 'n';
        }
        // make sure mission is possible in a
        a = max(a, dist);
        for(; ai != edges.end() && UFA.find_set(vs->info()) != UFA.find_set(vt->info()); ++ai) {
            UFA.union_set(get<0>(*ai), get<1>(*ai));
        }
    }
    if (ai != edges.begin() && get<2>(*(ai-1)) > a) a = get<2>(*(ai-1));
    if (bi != edges.begin() && get<2>(*(bi-1)) > b) b = get<2>(*(bi-1));
    cout << endl << a << endl << b << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cout << setiosflags(ios::fixed) << setprecision(0);

    int T; cin >> T;
    for(int t = 0; t < T; t++) do_testcase();
    return 0;
}