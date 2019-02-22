#include "da_location.h"

int main () {
    std::string path = "teste1";
    int N;
    std::cin >> N;
    // std::ostringstream name;
    // name << path << "/bij.txt";
    // std::ofstream file;
    // std::string line;
    // file.open(name.str().c_str(), std::ofstream::in);
    double v;
    std::vector<std::vector<long double> > b_ij;
    for (int i = 0; i<N; ++i) {
        b_ij.push_back(std::vector<long double>());
        for (int j = 0; j<N; ++j) {
            b_ij[i].push_back(0.0);
            std::cin >> b_ij[i][j];
        }
    }
    // file.close();
    // std::ostringstream os;
    // os << path << "/bij_teste.txt";
    // PrintMatrix (b_ij, os.str());

    // positioning(b_ij, N, path);
    da_positioning(b_ij, N, path);

    // exhaustive(b_ij, N);

    return 0;
}

/*
    3
        Best: 0[1] 1[1] 2[1] ---> 3
*/


/*
    4
1.00349
1.00549
1.00727
1.00557

1.00349
1.00549
1.00727
1.00557

1.00343
1.00547
1.00724
1.00551

1.00347
1.00542
1.0073
1.00547


4.0217000000
4.0198600000
ex [1, 2, 0, 3] = 1.00549 + 1.00727 + 1.00343 + 1.00547 = 4.02166
prop [2/3, 2/3, 1, 0] = 1.00727 + 1.00557 + 1.00547 + 1.00347 = 4.02178
seq [0, 1, 2, 3] = 1.00349 + 1.00549 + 1.00724 + 1.00547 = 4.02169
rand [1, 2, 0, 3] = 1.00549 + 1.00727 + 1.00343 + 1.0073 = 4.02349

1.07871e-68,7.30773e-69,0.5,0.5
1.07871e-68,7.30773e-69,0.5,0.5
2.26449e-51,1,4.93989e-94,6.92515e-94
1,2.26274e-51,4.8823e-94,6.97941e-94

*/


/*
5 
1.00491
1.00548
1.00971
1.00341
1.00613

1.00458
1.00427
1.00936
1.00461
1.00554

1.00174
1.00661
1.00574
1.00735
1.00244

1.0038
1.00884
1.00371
1.00951
1.00443

1.00241
1.0063
1.00702
1.00592
1.00397

loc [4, 1, 3, 0, 2] => 1.00241 + 1.00427 + 1.00371 + 1.00341 + 1.00244 = 5.0178
uav [3, 1, 4, 2, 0] => 1.00341 + 1.00427 + 1.00244 + 1.00371 + 1.00241 = 5.01624

exh[3, 1, 4, 2, 0] = 1.00341 + 1.00427 + 1.00244 + 1.00371 + 1.00241 = 5.01624 --- 0501710000


seq[0, 1, 2, 3, 4] = 1.00491 + 1.00427 + 1.00574 + 1.00951 + 1.00397 = 5.02840
ale[2, 4, 3, 0, 1] = 1.00971 + 1.00554 + 1.00735 + 1.0038 + 1.0063 = 5.03270
pro[2, 0, 1, 3, 4] = 1.00971 + 1.00458 + 1.00661 + 1.00951 + 1.00397 = 5.03438

2 0 1 3 4

1.46369e-72,1.38455e-72,1,1.694e-72,1.29946e-72
1,1.5579e-72,9.47585e-73,1.50713e-72,1.37647e-72
1.99277e-72,1,1.34987e-72,1.15349e-72,1.86167e-72
1.63088e-72,9.97073e-73,1.64524e-72,1,1.5338e-72
1.86711e-72,1.27808e-72,1.19129e-72,1.32637e-72,1

*/