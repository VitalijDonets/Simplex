#ifndef _LIMITATION_H_
#define _LIMITATION_H_

#include <iostream>
#include <vector>

class ILimitation {
protected:
    std::vector<double> _left;
    double _right = 0;
public:
    std::vector<double> getLeft() {
        return _left;
    }
    double getRight() {
        return _right;
    }
    ILimitation() = default;
    template <class iter>
    ILimitation(iter left_begin, iter left_end, double right) {
        while (left_begin != left_end) {
            _left.push_back(*left_begin);
            left_begin++;
        }
        _right = right;
    }
    virtual ~ILimitation() = default;
    virtual const std::string getLimitationType() = 0;
    virtual const std::string getSign() = 0;
    friend std::ostream& operator<<(std::ostream& out, ILimitation& limit) {
        for (int i = 0; i < limit._left.size(); i++) {
            if (limit._left[i] != 0) {
                if (i == 0) {
                    if (limit._left[i] == -1) std::cout << '-';
                    else if (limit._left[i] != 1) std::cout << limit._left[i] << "*";
                    std::cout << 'x' << i + 1;
                }
                else {
                    if (limit._left[i] == -1) std::cout << '-';
                    else if (limit._left[i] != 1)
                        std::cout << std::showpos << limit._left[i] << std::noshowpos << "*";
                    else {
                        std::cout << "+";
                    }
                    std::cout << 'x' << i + 1;
                }
            }
        }
        std::cout << ' ' << limit.getSign() << ' ' << limit._right;
        return out;
    }
    virtual ILimitation* copy() = 0;
};

class Equation : public ILimitation {
public:
    Equation() = default;
    template <class iter>
    Equation(iter left_begin, iter left_end, double right)
        : ILimitation(left_begin, left_end, right) {}
    virtual ~Equation() = default;
    const std::string getLimitationType() {
        return "Equation";
    }
    const std::string getSign() {
        return "=";
    }
    ILimitation* copy() {
        return new Equation(_left.begin(), _left.end(), _right);
    }
};

class LessEqual : public ILimitation {
public:
    LessEqual() = default;
    template <class iter>
    LessEqual(iter left_begin, iter left_end, double right)
        : ILimitation(left_begin, left_end, right) {}
    virtual ~LessEqual() = default;
    const std::string getLimitationType() {
        return "LessEqual";
    }
    const std::string getSign() {
        return "<=";
    }
    ILimitation* copy() {
        return new LessEqual(_left.begin(), _left.end(), _right);
    }
};

class GreatEqual : public ILimitation {
public:
    GreatEqual() = default;
    template <class iter>
    GreatEqual(iter left_begin, iter left_end, double right)
        : ILimitation(left_begin, left_end, right) {}
    virtual ~GreatEqual() = default;
    const std::string getLimitationType() {
        return "GreatEqual";
    }
    const std::string getSign() {
        return ">=";
    }
    ILimitation* copy() {
        return new GreatEqual(_left.begin(), _left.end(), _right);
    }
};

#endif