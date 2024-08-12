#ifndef _SIMPLEXTABLE_H_
#define _SIMPLEXTABLE_H_

#include <iostream>
#include <vector>

#include "LPProblem.h"
#include "tableRow.h"
#include "RationalNumber.h"

class SimplexTable {
    std::vector<int> _basis, _nonbasis;
    std::vector<RationalNumber> _func_coef;
    std::vector<TableRow> _table;

    RationalNumber _det(RationalNumber& rosv_elem, RationalNumber& diag_elem, RationalNumber& rosv_row_elem, RationalNumber& diag_row_elem);
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

    std::vector<RationalNumber> getCurOptVector();
};

#endif