#include "da_location.h"

int main () {
    std::string name; // caminho final onde estao as informacoes para comparacoes
    std::cin >> name;
    int N;
    std::cin >> N;
    double v;
    std::vector<std::vector<long double> > c_ji;
    for (int i = 0; i<N; ++i) {
        c_ji.push_back(std::vector<long double>());
        for (int j = 0; j<N; ++j) {
            c_ji[i].push_back(0.0);
            std::cin >> c_ji[i][j];
        }
    }

    long double s_exaustive = 0.0;
    long double s_proposed = 0.0;
    long double s_random = 0.0;
    std::ofstream file;
    std::ostringstream path;
    path << name << "_out.txt";
    file.open(path.str().c_str(), std::ofstream::out);

    std::vector<int> sol_exh (exhaustive (c_ji, N));
    std::vector<int> sol_pro (da_positioning(c_ji, N));
    std::vector<int> sol_rnd (N,0);

    srand(time(NULL));
    for (int i = 0; i<N; ++i) {
     sol_rnd[i] = rand() % N; 
     s_random += c_ji[i][sol_rnd[i]];
    }    

    file << N << std::endl;
    for (int i = 0; i<N; ++i) {
        file << sol_exh[i] << "\t";
        s_exaustive += c_ji[i][sol_exh[i]];
    }
    file << std::endl;
    file << s_exaustive << std::endl;
    for (int i = 0; i<N; ++i) {
        file << sol_pro[i] << "\t";
        s_proposed += c_ji[i][sol_pro[i]];
    }
    file << std::endl;
    file << s_proposed << std::endl;
    for (int i = 0; i<N; ++i) {
        file << sol_rnd[i] << "\t";
    }
    file << std::endl;
    file << s_random << std::endl;

    file.close();

}