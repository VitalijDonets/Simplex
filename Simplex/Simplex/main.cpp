#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "SimplexTable.h"

//void convertToCanon(LPProblem& problem) {
//    auto limits = problem.getLimitation();
//    int counter = 0;
//    std::vector<int> indexes;
//    for (int i = 0; i < limits.size(); i++) {
//        if (limits[i]->getSign() != "=") {
//            indexes.push_back(i);
//            counter++;
//        }
//    }
//    for (int i = 0, j = 0; i < limits.size(); i++) {
//
//    }
//}

int main()
{
    std::ifstream file("test5.txt");
    LPProblem problem;
    //std::cin >> problem;
    file >> problem;
    std::cout << problem << std::endl;
    if (problem.getCountLimitation() < problem.getCountVariable()) {
        auto t = problem.solve();
        if (t.incorrect) {
            std::cout << "No solution: incorrect limitation system\n";
        }
        else if (t.unlimit) {
            std::cout << "No solution: unlimit function\n";
        }
        else {
            std::cout << "Solution: ";
            for (int i = 0; i < t.result.size(); i++) {
                std::cout << t.result[i] << ' ';
            }
            std::cout << std::endl;
        }
    }
    file.close();
}