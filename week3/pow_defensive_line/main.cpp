#include <iostream>
#include <vector>
#include <cmath>
#include <climits>


void testcase() {
    int num_defenders, num_attackers, attack_strength;
    std::cin >> num_defenders >> num_attackers >> attack_strength;
    std::vector<int> defender(num_defenders + 1);
    for(int i = 1; i < num_defenders+1; i++) {
        std::cin >> defender.at(i);
    }
    //std::vector<std::pair<int,int> > intervals;
    std::vector<int> intervals(num_defenders+1, 0);
    int start = 1;
    int end = 1;
    int current_defend_value = defender.at(start);
    while (start <= end && end < num_defenders+1) {
        if(current_defend_value <= attack_strength) {
            if(current_defend_value == attack_strength) {
                // valid interval!
                intervals.at(end) = start;
            }
            end++;
            if(end < num_defenders+1) {
                current_defend_value += defender.at(end);
            }
        } else {
            // defend value > attack_strength, decrease window
            if(start < end) {
                current_defend_value -= defender.at(start);
                start++;
            } else {
                current_defend_value -= defender.at(start);
                start++;
                end++;
                if(end < num_defenders+1) {
                    current_defend_value += defender.at(end);
                }
            }
        }
    }

    std::vector<std::vector<int> > T(num_attackers+1, std::vector<int>(num_defenders+1, 0));
    for(int i = 1; i < num_attackers+1; i++) {
        T.at(i).at(0) = INT_MIN;
    }
    for(int j = 1; j < num_attackers+1; j++) {
        for(int i = 1; i < num_defenders+1; i++) {
            if(intervals.at(i) > 0) {
                // there is an interval ending at i
                T.at(j).at(i) = std::max(T.at(j).at(i-1), T.at(j-1).at(intervals.at(i)-1) + i-intervals.at(i)+1);
            } else {
                T.at(j).at(i) = T.at(j).at(i-1);
            }
        }
    }

    if(T.at(num_attackers).at(num_defenders) > 0) {
        std::cout << T.at(num_attackers).at(num_defenders) << std::endl;
    } else {
        std::cout << "fail" << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}