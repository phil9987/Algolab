// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Functions
// =========
bool attacks(int i, int j, int k, int l) {
    if(k == i-1 && l == j-2 ||
        k == i-1 && l == j+2 ||
        k == i+1 && l == j-2 ||
        k == i+1 && l == j+2 ||
        k == i-2 && l == j-1 ||
        k == i-2 && l == j+1 ||
        k == i+2 && l == j-1 ||
        k == i+2 && l == j+1)
        return true;
    else return false;
}
// 0 --> hole / under attack
// 1 --> available
// 2 --> knight
bool placable(int i, int j, vector<vector<int> >& board){
    if(board.at(i).at(j) == 1 && 
        board.at(i-1).at(j-2) != 2 && 
        board.at(i-1).at(j+2) != 2 && 
        board.at(i+1).at(j-2) != 2 && 
        board.at(i+1).at(j+2) != 2 && 
        board.at(i-2).at(j-1) != 2 && 
        board.at(i-2).at(j+1) != 2 &&
        board.at(i+2).at(j-1) != 2 &&
        board.at(i+2).at(j+1) != 2)
        return true;
    else return false;
}

void place(int i, int j, vector<vector<int> > & board) {
    board.at(i).at(j) = 2;
    board.at(i-1).at(j-2) = 0;
    board.at(i-1).at(j+2) = 0;
    board.at(i+1).at(j-2) = 0;
    board.at(i-2).at(j-1) = 0;
    board.at(i-2).at(j+1) = 0;
    board.at(i+2).at(j-1) = 0;
    board.at(i+2).at(j+1) = 0;
}

int max_knights(size_t n, size_t col, vector<vector<int> >& board) {
    if (col < 0) {
        return 0;
    }
    for(size_t i = 0; i < n; i++) {
        
    }
}

// Function for an individual testcase
void testcase() {
    
    size_t n; cin >> n;
    vector<vector<int> > board(n, vector<int>(n));
    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            cin >> board.at(i).at(j);
        }
    }
    
    cout << board.at(n-1).at(n-1) << endl;

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}
