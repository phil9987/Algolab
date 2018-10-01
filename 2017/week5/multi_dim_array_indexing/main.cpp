#include <iostream>

using namespace std;

int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        int num_params, num_experiments, num_queries;
        cin >> num_params >> num_experiments >> num_queries;
        int as[num_params], bs[num_params];
        int dimensions[num_params];
        for (int i = 0; i < num_params; i++) {
            cin >> as[i] >> bs[i];
            dimensions[i] = bs[i] - as[i] + 1;
        }
        int dimensions_mult[num_params];
        dimensions_mult[0] = dimensions[0];
        for (int i = 1; i < num_params; i++) {
            dimensions_mult[i] = dimensions_mult[i-1]*dimensions[i];
        }
        int total_dim = 1;
        for (int i = 0; i < num_params; i++) {
            total_dim = total_dim * dimensions[i];
        }
        int results[total_dim];   
        for (int i = 0; i < total_dim; i ++) {
            results[i] = -1;
        }     

        for (int i = 0; i < num_experiments; i ++) {
            int params[num_params];
            int idx = 0;
            for (int j = 0; j < num_params; j++) {
                cin >> params[j];
                if (j == 0) {
                    idx += params[j];
                } else {
                    idx += params[j] * dimensions_mult[j-1];
                }
            }
            cin >> results[idx];              
        }

        for (int i = 0; i < num_queries; i++) {
            int query[num_params];
            int idx = 0;
            for (int j = 0; j < num_params; j++) {
                cin >> query[j];
                if (j == 0) {
                    idx += query[j];
                } else {
                    idx += query[j] * dimensions_mult[j-1];
                }
            }
            cout << results[idx] << endl;
        }
    }

}