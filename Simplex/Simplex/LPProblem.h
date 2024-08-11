#ifndef _LPPROBLEM_H_
#define _LPPROBLEM_H_

#include <iostream>
#include <vector>

#include "Limitation.h"
#include "tableRow.h"

struct Result {
    std::vector<RationalNumber> result;
    bool incorrect = false, unlimit = false;
};

class LPProblem {
    int _count_variables = 0;
    std::vector<RationalNumber> _func_coef;
    std::string _extr;
    std::vector<ILimitation*> _limitation;
    int _count_limitation = 0;
    std::vector<int> _var_order;
    std::vector<int> _basis, _nonbasis;

    std::vector<std::vector<RationalNumber>> _getLimitsMatrix();
public:
    virtual ~LPProblem() {
        for (int i = 0; i < _limitation.size(); i++) {
            delete _limitation[i];
        }
    }
    friend std::ostream& operator<<(std::ostream& out, LPProblem& problem);
    friend std::istream& operator>>(std::istream& in, LPProblem& problem);
    std::vector<TableRow> convertLimitation();
    Result solve();
    std::vector<ILimitation*> getLimitation() {
        return _limitation;
    }
    std::vector<RationalNumber> getFuncCoef() {
        return _func_coef;
    }
    //void setNewLimitations(std::vector<ILimitation*>& newLimitations) {
    //    for (int i = 0; i < _limitation.size(); i++) {
    //        if (i < _limitation.size() && i < newLimitations.size()) {
    //            delete _limitation[i];
    //            _limitation[i] = newLimitations[i];
    //        }
    //    }
    //}
    //void setNewLimitation(int index, ILimitation*& newLimitation) {
    //    if (!(index < 0 || index > _count_limitation)) {
    //        delete _limitation[index];
    //        _limitation[index] = newLimitation->copy();
    //    }
    //}

    std::vector<int> getBasis() {
        return _basis;
    }

    std::vector<int> getNonBasis() {
        return _nonbasis;
    }

    std::vector<RationalNumber> getFree() {
        std::vector<RationalNumber> free;
        for (int i = 0; i < _count_limitation; i++) {
            free.push_back(RationalNumber(_limitation[i]->getRight()));
        }
        return free;
    }

    int getCountLimitation() {
        return _count_limitation;
    }

    int getCountVariable() {
        return _count_variables;
    }
};

#endif