#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

int num_days, wealth_init, wealth_goal;
double probability;

void testcase() {
    std::cin >> num_days >> wealth_init >> wealth_goal;
    std::vector<double> likelihoods(num_days+1);
    for(int i = 1; i <= num_days; i++) {
        std::cin >> likelihoods.at(i);
        //std::cout << likelihoods.at(i) << ' ';
    }
    std::vector<std::vector<double> > T(wealth_goal + 1, std::vector<double>(num_days + 1, 0.0));
    // init
    for(int d = 0; d <= num_days; d++) {
        T.at(wealth_goal).at(d) = 1.0;
    }
    for(int w = wealth_goal - 1; w*2 >= wealth_goal; w--) {
        T.at(w).at(num_days) = likelihoods.at(num_days);
    }
    for(int d = num_days-1; d > 0; d--) {
        for(int w = wealth_goal-1; w > 0; w--) {
            double tmp_max_prob = T.at(w).at(d+1);
            for(int bet = 1; bet <= w; bet++) {
                int bet_win = std::min(wealth_goal, w+bet);
                tmp_max_prob = std::max(tmp_max_prob, likelihoods.at(d)*T.at(bet_win).at(d+1) + (1-likelihoods.at(d))*T.at(w-bet).at(d+1));
            }
            T.at(w).at(d) = tmp_max_prob;
        }
    }
    std::cout << T.at(wealth_init).at(1) << std::endl;
    
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