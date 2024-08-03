#pragma once

#include <vector>

class TableRow {
    std::vector<double> _table_coef;
    double _free = 0;
public:
    TableRow() = default;
    TableRow(std::vector<double> table_coef, double free_coef) {
        for (int i = 0; i < table_coef.size(); i++) {
            _table_coef.push_back(table_coef[i]);
        }
        _free = free_coef;
    }
    void push_coef(double elem) {
        _table_coef.push_back(elem);
    }
    void push_free(double elem) {
        _free = elem;
    }
    int size() {
        return _table_coef.size() + 1;
    }
    double& operator[](const int index) {
        if (0 <= index && index < _table_coef.size()) {
            return _table_coef[index];
        }
        else if (index == _table_coef.size()) {
            return _free;
        }
    }
};
