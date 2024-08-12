#include "LPProblem.h"

#include "SimplexTable.h"

#define TEST

#ifdef TEST
#include <iomanip>
#endif

Result LPProblem::solve() {
    this->_convertToCanon();
    Result res;
    std::cout << *this << std::endl;
    SimplexTable table(*this);
    std::cout << table << std::endl;
    while (!table.is_startVec()) {
        if (table.is_incorrect()) {
            res.incorrect = true;
            break;
        }
        else {
            table.rebuild();
        }
        std::cout << table << std::endl;
    }
    if (!res.incorrect) {
        while (!table.is_opt()) {
            if (table.is_unlimit()) {
                res.unlimit = true;
                break;
            }
            else {
                table.rebuild();
            }
            std::cout << table << std::endl;
        }
        res.result = table.getCurOptVector();
        for (int i = _start_count_variables; i < _count_variables; i++) {
            res.result.erase(res.result.begin() + _start_count_variables, res.result.end());
        }
    }
    return res;
}

std::vector<TableRow> LPProblem::convertLimitation() {
    auto limitsMatrix = _getLimitsMatrix();
    auto free = getFree();

    struct check {
        bool isGoodColumn;
        int columnIndex, place;
    };

    std::vector<check> GoodColumn;
    for (int i = 0; i < _count_limitation; i++) {
        GoodColumn.push_back({ false, -1, -1 });
    }

    for (int i = 0; i < _count_limitation; i++) {
        for (int j = 0; j < _count_variables; j++) {
            bool checked = true;
            if (limitsMatrix[i][j] == 1) {
                for (int h = 0; h < _count_limitation; h++) {
                    if (h != i) {
                        if (limitsMatrix[h][j] != 0) {
                            checked = false;
                            break;
                        }
                    }
                }
                if (checked) {
                    GoodColumn[i].isGoodColumn = true;
                    GoodColumn[i].columnIndex = j;
                    GoodColumn[i].place = i;
                    break;
                }
            }
        }
    }

    //std::cout << "Good columns: ";
    //for (int i = 0; i < GoodColumn.size(); i++) {
    //    std::cout << GoodColumn[i].isGoodColumn << ' ' << GoodColumn[i].columnIndex << ' ' << GoodColumn[i].place << std::endl;
    //}
    //std::cout << std::endl;

    for (int i = 0; i < GoodColumn.size(); i++) {
        if (GoodColumn[i].isGoodColumn) {
            for (int j = 0; j < _count_limitation; j++) {
                std::swap(limitsMatrix[j][GoodColumn[i].columnIndex], limitsMatrix[j][GoodColumn[i].place]);
            }
            std::swap(_var_order[GoodColumn[i].columnIndex], _var_order[i]);
            std::swap(_func_coef[GoodColumn[i].columnIndex], _func_coef[i]);
        }
    }
    
    bool is_all_good_columns = false;
    for (int i = 0; i < GoodColumn.size(); i++) {
        if (!GoodColumn[i].isGoodColumn) {
            break;
        }
    }

    if (!is_all_good_columns) {
        for (int i = 0; i < limitsMatrix.size() - 1; i++) {
            if (limitsMatrix[i][i] != 0) {
                for (int j = i + 1; j < limitsMatrix.size(); j++)
                {
                    RationalNumber drib = -limitsMatrix[j][i] / limitsMatrix[i][i];
                    for (int k = 0; k < limitsMatrix[i].size(); k++)
                    {
                        RationalNumber koef = drib * limitsMatrix[i][k];
                        limitsMatrix[j][k] += koef;
                    }
                    free[j] += drib * free[i];
                }
            }
            else {
                for (int j = i + 1; j < limitsMatrix.size(); j++) {
                    if (limitsMatrix[i][j] != 0) {
                        for (int k = 0; k < limitsMatrix.size(); k++) {
                            std::swap(limitsMatrix[k][j], limitsMatrix[k][i]);
                        }
                    }
                    std::swap(_var_order[i], _var_order[j]);
                    break;
                }
                i--;
            }
        }
        for (int i = 0; i < limitsMatrix.size(); i++) {
            RationalNumber koef = limitsMatrix[i][i];
            for (int j = i; j < limitsMatrix[i].size(); j++) {
                limitsMatrix[i][j] /= koef;
            }
            free[i] /= koef;
        }
        for (int i = limitsMatrix.size() - 1; i > 0; i--) {
            for (int j = i - 1; j >= 0; j--)
            {
                RationalNumber drib = -limitsMatrix[j][i] / limitsMatrix[i][i];
                for (int k = 0; k < limitsMatrix[i].size(); k++)
                {
                    RationalNumber koef = drib * limitsMatrix[i][k];
                    limitsMatrix[j][k] += koef;
                }
                free[j] += drib * free[i];
            }
        }
    }
    for (int i = 0; i < _count_limitation; i++) {
        _basis.push_back(_var_order[i]);
    }
    for (int i = _basis.size(); i < _count_variables; i++) {
        _nonbasis.push_back(_var_order[i]);
    }

    for (int i = 0; i < _var_order.size(); i++) {
        std::cout << std::setw(4) << "x" << _var_order[i] << ' ';
    }
    std::cout << std::endl;
    for (int i = 0; i < limitsMatrix.size(); i++) {
        for (int j = 0; j < limitsMatrix[i].size(); j++) {
            std::cout << std::setw(4) << limitsMatrix[i][j] << ' ';
        }
        std::cout << std::setw(4) << ' ' << free[i] << std::endl;
    }

    std::vector<TableRow> middle;
    for (int i = 0; i < limitsMatrix.size(); i++) {
        TableRow row;
        for (int j = _basis.size(); j < limitsMatrix[i].size(); j++) {
            row.push_coef(limitsMatrix[i][j]);
        }
        row.push_free(free[i]);
        middle.push_back(row);
    }

    return middle;
}
std::ostream& operator<<(std::ostream& out, LPProblem& problem) {
    std::cout << "LP Problem\n";
    for (int i = 0; i < problem._count_variables; i++) {
        if (problem._func_coef[i] != 0) {
            if (i == 0) {
                if (problem._func_coef[i] == -1) std::cout << '-';
                else if (problem._func_coef[i] != 1) std::cout << problem._func_coef[i] << "*";
                std::cout << "x" << i + 1;
            }
            else {
                if (problem._func_coef[i] == -1) std::cout << '-';
                else if (problem._func_coef[i] != 1)
                    std::cout << std::showpos << problem._func_coef[i] << std::noshowpos << "*";
                else
                    std::cout << "+";
                std::cout << "x" << i + 1;
            }
        }
    }
    if (problem._func_coef[problem._func_coef.size() - 1] != 0) {
        if (problem._func_coef[problem._func_coef.size() - 1] > 0) {
            std::cout << '+';
        }
        std::cout << problem._func_coef[problem._func_coef.size() - 1] << std::endl;
    }
    std::cout << " \32 " << problem._extr << std::endl;
    std::cout << "Limitation:\n";
    for (int i = 0; i < problem._count_limitation; i++) {
        std::cout << *(problem._limitation[i]) << std::endl;
    }
    return out;
}
std::istream& operator>>(std::istream& in, LPProblem& problem) {
    std::cout << "Count variables: "; in >> problem._count_variables;
    problem._start_count_variables = problem._count_variables;
    for (int i = 0; i < problem._count_variables; i++) {
        problem._var_order.push_back(i + 1);
    }
    std::cout << "Function coef:\n";
    for (int i = 0; i < problem._count_variables + 1; i++) {
        double coef;
        in >> coef;
        problem._func_coef.push_back(RationalNumber(coef));
        problem._start_func_coef.push_back(RationalNumber(coef));
    }
    std::cout << "max or min: "; in >> problem._extr;
    //while (problem._extr != "max" || problem._extr != "min") {
    //    std::cout << "Error extr! Enter max or min: "; in >> problem._extr;
    //}
    std::cout << "Count limitation: "; in >> problem._count_limitation;
    std::cout << "Limitation coef:\n";
    for (int i = 0; i < problem._count_limitation; i++) {
        std::vector<double> left_coef;
        std::string type;
        double right;
        for (int j = 0; j < problem._count_variables; j++) {
            double coef; in >> coef;
            left_coef.push_back(coef);
        }
        in >> type >> right;
        std::shared_ptr<ILimitation> limit;
        if (type == "=") {
            limit = std::make_shared<Equation>(left_coef.begin(), left_coef.end(), RationalNumber(right));
            //limit = new Equation(left_coef.begin(), left_coef.end(), RationalNumber(right));
        }
        else if (type == "<=") {
            limit = std::make_shared<LessEqual>(left_coef.begin(), left_coef.end(), RationalNumber(right));
            //limit = new LessEqual(left_coef.begin(), left_coef.end(), right);
        }
        else if (type == ">=") {
            limit = std::make_shared<GreatEqual>(left_coef.begin(), left_coef.end(), RationalNumber(right));
            //limit = new GreatEqual(left_coef.begin(), left_coef.end(), right);
        }
        problem._limitation.push_back(limit);
    }
    return in;
}

std::vector<std::vector<RationalNumber>> LPProblem::_getLimitsMatrix() {
    std::vector<std::vector<RationalNumber>> matrix;
    for (int i = 0; i < _limitation.size(); i++) {
        matrix.push_back(_limitation[i]->getLeft());
    }
    return matrix;
}

void LPProblem::_convertToCanon() {
    struct info {
        int index;
        std::string type;
    };
    std::vector<info> check;
    for (int i = 0; i < _count_limitation; i++) {
        std::string type = _limitation[i]->getSign();
        if (type != "=") {
            check.push_back({ i, type });
        }
    }
    if (check.size() != 0) {
        std::vector<std::shared_ptr<ILimitation>> limit_copy;
        for (int i = 0; i < _count_limitation; i++) {
            limit_copy.push_back(_limitation[i]);
            //delete _limitation[i];
        }
        _limitation.clear();
        for (int i = 0, counter = 0; i < _count_limitation; i++) {
            std::string sign = limit_copy[i]->getSign();
            std::shared_ptr<ILimitation> limit;
            auto coefs = limit_copy[i]->getLeft();
            if (sign == "=") {
                for (int i = 0; i < check.size(); i++) {
                    coefs.push_back(0);
                }
                //_limitation.push_back(new Equation(coefs.begin(), coefs.end(), limit_copy[i]->getRight()));
            }
            else {
                for (int i = 0; i < check.size(); i++) {
                    if (i == counter) {
                        if (sign == "<=") {
                            coefs.push_back(1);
                        }
                        else if (sign == ">=") {
                            coefs.push_back(-1);
                        }
                    }
                    else {
                        coefs.push_back(0);
                    }
                }
                //_limitation.push_back(new Equation(coefs.begin(), coefs.end(), limit_copy[i]->getRight()));
                counter++;
            }
            limit = std::make_shared<Equation>(coefs.begin(), coefs.end(), limit_copy[i]->getRight());
            _limitation.push_back(limit);
        }
        _count_variables += check.size();
        for (int i = 0; i < check.size(); i++) {
            _func_coef.push_back(0);
            _var_order.push_back(_var_order.size() + 1);
        }
    }
}