#include <iostream>
#include <iomanip>
#include <fstream>
#include "LPProblem.h"
#include "RationalNumber.h"

int main() {
    std::ifstream file("test3.txt");
    LPProblem problem;
    //std::cin >> problem;
    file >> problem;
    std::cout << problem << std::endl;
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
        std::cout << std::endl << "f(x*) = " << problem.getFuncValue(t.result) << std::endl;
    }
    file.close();
}