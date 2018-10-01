#include <iostream> 
 
int M[201][201]; 
int pM[201][201];
int main(){
    int T; 
    std::cin >> T;
    while (T > 0){ 
        int n; std::cin >> n; 
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                std::cin >> M[i][j];
        for (int i = 0; i <= n; ++i) {
            pM[0][i] = 0;
            pM[i][0] = 0;
        }
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                pM[i][j] = pM[i-1][j] + pM[i][j-1] - pM[i-1][j-1] + M[i][j];
                
        int solution = 0;
        for (int i1 = 1; i1 <= n; ++i1){
            for (int i2 = i1; i2 <= n; ++i2){
                // We reduce the problem to one dimension.
                int S[201]; // We do Even Pairs on array S.
                int pS[201]; // pS contains partial sums of S.
                pS[0] = 0;
                for (int k = 1; k <= n; ++k){
                    S[k] = pM[i2][k] - pM[i2][k-1] - pM[i1-1][k] + pM[i1-1][k-1];
                    pS[k] = pS[k-1] + S[k];
                }// Do Even Pairs O(n) algorithm on array S.
                int onedim_sol = 0;
                int even = 0, odd = 0; 
                for (int j = 1; j <= n; ++j){
                    // even = # of partial sums of array (S[1], ..., S[j-1]) that are even.
                    // odd = # of partial sums of array (S[1], ..., S[j-1]) that are odd. 
                    if (pS[j] % 2 == 0){ 
                        onedim_sol += even + 1;
                        ++even;
                        } else { 
                            onedim_sol += odd; 
                            ++odd; 
                        } 
                }
                solution += onedim_sol;
            }
        }
    std::cout << solution << std::endl;
    --T;
    }
    return 0;
}