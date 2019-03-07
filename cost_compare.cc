#include <algorithm>
#include <random>
#include "da_location.h"

int main () {
    std::string name, path; // caminho final onde estao as informacoes para comparacoes
    std::cin >> name;
    int N;
    std::cin >> N;
    double v;
    std::vector<std::vector<long double> > c_ji;
    for (int i = 0; i<N; ++i) {
        c_ji.push_back(std::vector<long double>());
        for (int j = 0; j<N; ++j) {
            c_ji[i].push_back(0.0);
            // std::cin >> c_ji[i][j];
            scanf("%Lf,", &c_ji[i][j]);
            std::cout << c_ji[i][j] << "\t";
        }
    }
    std::ofstream file;
    std::ostringstream f_name;
    std::cin >> path;
    f_name << path << name << ".sol";
    file.open(f_name.str().c_str(), std::ofstream::out);
    std::vector<int> sol_pro (DA_Rangarajan(c_ji, N));
    for (int i = 0; i<N; ++i) {
        file << sol_pro[i] << ",";
    }
    file << std::endl;
    std::vector<int> sol_exh_uav (exhaustive (c_ji, N, 1));
    for (int i = 0; i<N; ++i) {
        file << sol_exh_uav[i] << ",";
    }
    file << std::endl;
    file.close();
}