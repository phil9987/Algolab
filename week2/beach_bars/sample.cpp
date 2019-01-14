int start = 0;
int end = 0;
int num_umbrellas = 1;
int current_max = 0;
int min_longest_dist = INT_MAX;
vector<int> centers;
while(end < n) {
    int current_distance = abs(parasols.at(end) - parasols.at(start));
    if (num_umbrellas >= current_max && current_distance <= 200) {
        int dist = (current_distance + 1) / 2;
        if (num_umbrellas > current_max) {
            centers.clear();
            current_max = num_umbrellas;
            min_longest_dist = dist;
        }
        if (dist < min_longest_dist) {
            centers.clear();
            min_longest_dist = dist;
        }
        if (dist == min_longest_dist) {
            min_longest_dist = min(min_longest_dist, dist);
            centers.push_back(parasols.at(start) + dist);
            if(current_distance % 2 == 1) {
                centers.push_back(parasols.at(start) + dist - 1);
            }
        }
    }
    if (current_distance < 200) {
        end++;
        num_umbrellas++;
    } else {
        start++;
        num_umbrellas--;
    }
}