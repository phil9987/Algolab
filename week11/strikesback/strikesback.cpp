#include <iostream>
#include <vector>

using namespace std;
typedef vector<long> Asteroid;
typedef pair<long, long> Coordinate;

void do_testcase() {
    size_t num_asteroids, num_lasershots, num_bhunters;
    cin >> num_asteroids >> num_lasershots >> num_bhunters;
    size_t energy; cin >> energy;

    vector<Asteroid> asteroids(num_asteroids);
    for(size_t i = 0; i < num_asteroids; i++) {
        Asteroid tmp(3);
        cin >> tmp.at(0) >> tmp.at(1) >> tmp.at(2); // reading x, y, d
    }

    vector<Coordinate> lasershots(num_lasershots);
    for(size_t i = 0; i < num_lasershots; i++) {
        long x, y; cin >> x >> y;
        lasershots.at(i) = make_pair(x,y);
    }

    vector<Coordinate> bhunters(num_bhunters);
    for(size_t i = 0; i < num_bhunters; i++) {
        long x, y; cin >> x >> y;
        bhunters.at(i) = make_pair(x,y);
    }
}

int main() {
    size_t T; cin >> T;
    for(size_t t = 0; t < T; t++) {
        do_testcase();
    }
    return 0;
}