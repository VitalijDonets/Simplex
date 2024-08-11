#pragma once

#include <vector>

#include "RationalNumber.h"

class TableRow {
    std::vector<RationalNumber> _table_coef;
    RationalNumber _free = 0;
public:
    TableRow() = default;
    TableRow(std::vector<double> table_coef, double free_coef) {
        for (int i = 0; i < table_coef.size(); i++) {
            RationalNumber number(table_coef[i]);
            _table_coef.push_back(number);
        }
        _free = free_coef;
    }
    void push_coef(RationalNumber elem) {
        _table_coef.push_back(elem);
    }
    void push_free(RationalNumber elem) {
        _free = elem;
    }
    int size() {
        return _table_coef.size() + 1;
    }
    RationalNumber& operator[](const int index) {
        if (0 <= index && index < _table_coef.size()) {
            return _table_coef[index];
        }
        else if (index == _table_coef.size()) {
            return _free;
        }
    }
};
