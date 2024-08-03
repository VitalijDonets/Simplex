#ifndef _SIMPLEXTABLE_H_
#define _SIMPLEXTABLE_H_

#include <iostream>
#include <vector>

#include "LPProblem.h"
#include "tableRow.h"

class SimplexTable {
    std::vector<int> _basis, _nonbasis;
    std::vector<double> _func_coef;
    std::vector<TableRow> _table;

    double _det(double rosv_elem, double diag_elem, double rosv_row_elem, double diag_row_elem);
public:
    std::vector<int> getBasis() {
        return _basis;
    }
    std::vector<int> getNonbasis() {
        return _nonbasis;
    }
    SimplexTable(SimplexTable& other);
    SimplexTable(LPProblem& problem);
    friend std::ostream& operator<<(std::ostream& out, SimplexTable& table);

    bool is_startVec();
    bool is_incorrect();
    bool is_opt();
    bool is_unlimit();

    void rebuild();

    std::vector<double> getCurOptVector();
};

#endif