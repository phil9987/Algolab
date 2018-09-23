#include <iostream>
#include <vector>
#include <map>

using namespace std;

map<int,int> memo ;

int main() {
    memo[3] = 5;
    memo[1] = 2;
    if (memo[2] == memo.end())
        cout << "not in memo" << endl;
    else
        cout << memo[2] << endl;
}