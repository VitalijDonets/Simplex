#include <iomanip>
#include "SimplexTable.h"

RationalNumber SimplexTable::_det(RationalNumber& rosv_elem, RationalNumber& diag_elem, RationalNumber& rosv_row_elem, RationalNumber& diag_row_elem) {
    return (rosv_elem * diag_elem) - (rosv_row_elem * diag_row_elem);
}

SimplexTable::SimplexTable(SimplexTable& other) {
    for (int i = 0; i < other._basis.size(); i++) {
        this->_basis.push_back(other._basis[i]);
    }
    for (int i = 0; i < other._nonbasis.size(); i++) {
        this->_nonbasis.push_back(other._nonbasis[i]);
    }
    for (int i = 0; i < other._func_coef.size(); i++) {
        this->_func_coef.push_back(other._func_coef[i]);
    }
    for (int i = 0; i < other._table.size(); i++) {
        this->_table.push_back(other._table[i]);
    }
}
SimplexTable::SimplexTable(LPProblem& problem) {
    _table = problem.convertLimitation();
    _basis = problem.getBasis();
    _nonbasis = problem.getNonBasis();
    auto temp_coef = problem.getFuncCoef();    
    for (int i = _basis.size(); i < _basis.size() + _nonbasis.size(); i++) {
        _func_coef.push_back(temp_coef[i]);
    }
    _func_coef.push_back(temp_coef[temp_coef.size() - 1]);
    for (int i = 0; i < _basis.size(); i++) {
        for (int j = 0; j < _nonbasis.size(); j++) {
            _func_coef[j] += -_table[i][j] * temp_coef[i];
        }
    }
    for (int i = 0; i < _table.size(); i++) {
        _func_coef[_func_coef.size() - 1] += _table[i][_table[i].size() - 1] * temp_coef[i];
    }
    for (int i = 0; i < _func_coef.size() - 1; i++) {
        _func_coef[i] *= -1;
    }
}
std::ostream& operator<<(std::ostream& out, SimplexTable& table) {
    std::cout << std::setw(20);
    for (int i = 0; i < table._nonbasis.size(); i++) {
        std::cout << 'x' << table._nonbasis[i] << std::setw(10);
    }
    std::cout << std::endl;
    for (int i = 0; i < table._table.size(); i++) {
        std::cout << 'x' << table._basis[i] << std::setw(10);
        for (int j = 0; j < table._table[i].size(); j++) {
            std::cout << table._table[i][j] << std::setw(10);
        }
        std::cout << std::endl;
    }
    std::cout << std::setw(21);
    for (int i = 0; i < table._func_coef.size(); i++) {
        std::cout << table._func_coef[i] << std::setw(10);
    }
    std::cout << std::endl;
    return out;
}

bool SimplexTable::is_startVec() {
    for (int i = 0; i < _table.size(); i++) {
        if (_table[i][_table[i].size() - 1] < 0) {
            return false;
        }
    }
    return true;
}

bool SimplexTable::is_opt() {
    for (int i = 0; i < _func_coef.size() - 1; i++) {
        if (_func_coef[i] < 0) return false;
    }
    return true;
}

bool SimplexTable::is_incorrect() {
    for (int i = 0; i < _table.size(); i++) {
        if (_table[i][_table[i].size() - 1] < 0) {
            bool incorrect = true;
            for (int j = 0; j < _table[i].size() - 1; j++) {
                if (_table[i][j] < 0) {
                    incorrect = false;
                    break;
                }
            }
            if (incorrect) {
                return true;
            }
        }
    }
    return false;
}

bool SimplexTable::is_unlimit() {
    for (int i = 0; i < _func_coef.size() - 1; i++) {
        if (_func_coef[i] < 0) {
            bool unlimit = true;
            for (int j = 0; j < _table.size(); j++) {
                if (_table[j][i] > 0) {
                    unlimit = false;
                    break;
                }
            }
            if (unlimit) {
                return true;
            }
        }
    }
    return false;
}

void SimplexTable::rebuild() {
    int rosv_row = -1, rosv_column = -1;
    for (int i = 0; i < _table.size(); i++) {
        if (_table[i][_table[i].size() - 1] < 0) {
            for (int j = 0; j < _table[i].size() - 1; j++) {
                if (_table[i][j] < 0) {
                    rosv_column = j;
                    break;
                }
            }
            if (rosv_column != -1) {
                break;
            }
        }
    }
    if (rosv_column == -1) {
        for (int i = 0; i < _func_coef.size() - 1; i++) {
            if (_func_coef[i] < 0) {
                rosv_column = i;
                break;
            }
        }
    }
    struct divide {
        RationalNumber div;
        int index;
    };
    std::vector<divide> divides;
    for (int i = 0; i < _table.size(); i++) {
        if ((_table[i][_table[i].size() - 1] >= 0 && _table[i][rosv_column] > 0)
            || (_table[i][_table[i].size() - 1] < 0 && _table[i][rosv_column] < 0)) {
            divides.push_back({ _table[i][_table[i].size() - 1] / _table[i][rosv_column], i });
        }
    }
    divide min = divides[0];
    for (int i = 1; i < divides.size(); i++) {
        //std::cout << min.div << ' ' << divides[i].div << '\t' << (min.div > divides[i].div) << std::endl;
        if (min.div > divides[i].div) {
            min = divides[i];
        }
    }
    rosv_row = min.index;
    //std::cout << "rosv_row: " << rosv_row << ", rosv_column: " << rosv_column << std::endl;
    SimplexTable oldTable(*this);
    std::swap(this->_basis[rosv_row], this->_nonbasis[rosv_column]);
    RationalNumber rosv_elem = this->_table[rosv_row][rosv_column];
    this->_table[rosv_row][rosv_column] = 1 / rosv_elem;
    for (int i = 0; i < _table[rosv_row].size(); i++) {
        if (i != rosv_column) {
            this->_table[rosv_row][i] = oldTable._table[rosv_row][i] / rosv_elem;
        }
    }
    //std::cout << "current:\n" << *this << std::endl;
    //std::cout << "old:\n" << oldTable << std::endl;
    for (int i = 0; i < _table.size(); i++) {
        if (i != rosv_row) {
            this->_table[i][rosv_column] = -1 * oldTable._table[i][rosv_column] / rosv_elem;
        }
    }
    this->_func_coef[rosv_column] = -1 * oldTable._func_coef[rosv_column] / rosv_elem;
    for (int i = 0; i < _table.size(); i++) {
        if (i != rosv_row) {
            for (int j = 0; j < _table[i].size(); j++) {
                if (j != rosv_column) {
                    auto t = _det(rosv_elem, oldTable._table[i][j], oldTable._table[i][rosv_column], oldTable._table[rosv_row][j]);
                    //std::cout << t << ' ' << rosv_elem << std::endl;
                    this->_table[i][j] = t / rosv_elem;
                }
            }
        }
    }
    for (int i = 0; i < _func_coef.size(); i++) {
        if (i != rosv_column) {
            this->_func_coef[i] = _det(rosv_elem, oldTable._func_coef[i], oldTable._table[rosv_row][i], oldTable._func_coef[rosv_column]) / rosv_elem;
        }
    }
}

std::vector<RationalNumber> SimplexTable::getCurOptVector() {
    std::vector<RationalNumber> vec;
    for (int i = 0; i < _basis.size() + _nonbasis.size(); i++) {
        vec.push_back(0);
    }
    for (int i = 0; i < _basis.size(); i++) {
        vec[_basis[i] - 1] = _table[i][_table[i].size() - 1];
    }
    for (int i = 0; i < _nonbasis.size(); i++) {
        vec[_nonbasis[i] - 1] = 0;
    }
    return vec;
}