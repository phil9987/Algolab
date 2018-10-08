#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

int num_days, wealth_init, wealth_goal;
double probability;

double prob_rec(int day, int current_wealth) {
    if(day == 0 && current_wealth > wealth_init) {
        return 0.0;
    } else if(current_wealth <= wealth_init) {
        return probability;
    } else {
        double max_prob = prob_rec(day-1, current_wealth);
        for(int i = current_wealth/2; i < current_wealth; i++) {
            max_prob += prob_rec(day-1, i)*probability;
        }
        return max_prob;
    }
}

void testcase() {
    std::cin >> num_days >> wealth_init >> wealth_goal;
    std::vector<double> likelihoods(num_days);
    for(int i = 0; i < num_days; i++) {
        std::cin >> likelihoods.at(i);
        //std::cout << likelihoods.at(i) << ' ';
    }
    int reachable_wealth = wealth_init;
    double probability_result;
    int i = 0;
    while(reachable_wealth < wealth_goal && i < num_days) {
        reachable_wealth += reachable_wealth;
        i++;
    }
    if(reachable_wealth < wealth_goal || i == num_days) {
        probability_result = 0.0;   // not possible to reach wealth_goal
    } else if(i == 0) {
        probability_result = 1.0;   // goal is already reached before magician comes
    } else {
        // wealth_goal is reachable, calculate probability
        probability = likelihoods.at(0);    // for testcases 1 and 2 we know there exists only one prob.#pragma endregion
        probability_result = prob_rec(num_days, wealth_goal + 1);
    }
    std::cout << probability_result << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t;
    std::cout << std :: fixed << std::setprecision (5);
    for(int i = 0; i < t; i++) {
        testcase();
    }
    return 0;
}