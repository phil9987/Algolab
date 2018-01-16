// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

// Functions
// =========
// Function for an individual testcase
void testcase() {
    size_t num_zones, num_jobs;
    std::cin >> num_zones >> num_jobs;
    std::vector<size_t> cost(num_zones);
    for(size_t i = 0; i < num_zones; i++) {
        std::cin >> cost.at(i);
    }
    std::vector<size_t> reward(num_jobs);
    for(size_t i = 0; i < num_jobs; i++) {
        std::cin >> reward.at(i);
    }
    long total_reward = 0;
    vector<vector<size_t> > tickets_per_job(num_jobs);
    vector<pair<size_t, size_t> > jobs_start_stop_zone(num_jobs, {-1,-1});
    for(size_t i = 0; i < num_jobs; i++) {
        size_t num_tickets; std::cin >> num_tickets;
        std::vector<size_t> tx(num_tickets);
        if(num_tickets == 0) total_reward += reward.at(i);      // if no ticket is required we anyway get the reward
        for(size_t j = 0; j < num_tickets; j++) {
            std::cin >> tx.at(j);
            if( j == 0) jobs_start_stop_zone.at(i).first = tx.at(j);
            if( j == num_tickets-1) jobs_start_stop_zone.at(i).second = tx.at(j);
        }
        tickets_per_job.at(i) = tx;
    }
    
    long max_reward = total_reward;
    for(size_t start = 0; start < num_zones; start++) {
        for(size_t end = start; end < num_zones; end++) {
            long tmp_reward = total_reward;
            vector<int> ticket_used(num_zones, 0);
            for(size_t i = 0; i < num_jobs; i++) {
                auto j = jobs_start_stop_zone.at(i);
                if((j.first >= start && j.second <= end) && !(j.first == -1)) {
                    tmp_reward += reward.at(i);
                    for(int k = j.first; k <= j.second; k++) {
                        ticket_used.at(k)++;
                    }
                }
            }
            for(size_t i = start; i <= end; i++) {
                if(ticket_used.at(i))
                    tmp_reward -= cost.at(i);
            }
            max_reward = max(tmp_reward, max_reward);
        }
    }
    
    cout << max_reward << endl;
    
    
}


// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	std::cin >> T;
	for (; T > 0; --T)	testcase();
	return 0;
}

