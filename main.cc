#include "da_location.h"

int main () {
    std::string name;
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
    std::vector<int> exhaustive;
    std::vector<int> random;
    std::vector<int> sequential;
    for (int i = 0; i<N; ++i) {
        exhaustive.push_back(0.0);
        std::cin >> exhaustive[i];
    }
    for (int i = 0; i<N; ++i) {
        random.push_back(0.0);
        std::cin >> random[i];
    }
    for (int i = 0; i<N; ++i) {
        sequential.push_back(0.0);
        std::cin >> sequential[i];
    }

    std::vector<int> proposed (da_positioning(c_ji, N, name));

    std::ofstream file;
    std::ostringstream path;
    path << name << "_out.txt";
    file.open(path.str().c_str(), std::ofstream::out);

    long double s_exaustive = 0.0;
    long double s_proposed = 0.0;
    long double s_random = 0.0;
    long double s_sequential = 0.0;
    for (int i = 0; i<N; ++i) {
        s_exaustive += c_ji[i][exhaustive[i]];
        s_random += c_ji[i][random[i]];
        s_sequential += c_ji[i][sequential[i]];
        s_proposed += c_ji[i][proposed[i]];
    }
    
    file << "EX:\t\t" << s_exaustive << "\t[\t";
    for (int i = 0; i<N; ++i) {
        file << exhaustive[i] << "\t";
    }
    file << "]\nPR:\t\t" << s_proposed << "\t[\t";
    for (int i = 0; i<N; ++i) {
        file << proposed[i] << "\t";
    }
    file << "]\nRD:\t\t" << s_random << "\t[\t";
    for (int i = 0; i<N; ++i) {
        file << random[i] << "\t";
    }
    file << "]\nSQ:\t\t" << s_sequential<< "\t[\t";
    for (int i = 0; i<N; ++i) {
        file << sequential[i] << "\t";
    }
    file << "]\n";
    file.close();

    return 0;
}