#include <algorithm>
#include <random>
#include "da_location.h"

int main () {
    std::string name;
    std::cin >> name;
    std::cout << name << std::endl;
    int N;
    std::cin >> N;
    std::cout << N << std::endl;
    double v;
    std::vector<std::vector<long double> > c_ji;
    std::vector<int> random;
    std::vector<int> seq;
    v = 0.0;    
    for (int i = 0; i<N; ++i) {
        c_ji.push_back(std::vector<long double>());
        for (int j = 0; j<N; ++j) {
            c_ji[i].push_back(0.0);
            std::cin >> c_ji[i][j];
            if (c_ji[i][j]>v) {
                v = c_ji[i][j];
            }
        }
    }
    // normalizando
    for (int i = 0; i<N; ++i) {
        random.push_back(i);
        seq.push_back(i);
        for (int j = 0; j<N; ++j) {
            c_ji[i][j] /= v;
            std::cout << c_ji[i][j] << "\n"; 
        }
    }
    // std::cout << "---------\n";
    std::vector<int> exh(exhaustive(c_ji, N, 1));
    for (int i = 0; i<N; ++i) {
        std::cout << exh[i] << "\n";
    }
    // std::cout << "----------\n";
    std::random_shuffle(std::begin(random), std::end(random));
    for (int i = 0; i<N; ++i) {
        std::cout << random[i] << "\n";
    }
    // std::cout << "----------\n";
    for (int i = 0; i<N; ++i) {
        std::cout << seq[i] << "\n";
    }
}