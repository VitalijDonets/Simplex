#ifndef _LIMITATION_H_
#define _LIMITATION_H_

#include <iostream>
#include <vector>

#include "RationalNumber.h"

class ILimitation {
protected:
    std::vector<RationalNumber> _left;
    RationalNumber _right = 0;
public:
    std::vector<RationalNumber> getLeft() {
        return _left;
    }
    RationalNumber getRight() {
        return _right;
    }
    ILimitation() = default;
    template <class iter>
    ILimitation(iter left_begin, iter left_end, RationalNumber right) {
        while (left_begin != left_end) {
            _left.push_back(*left_begin);
            left_begin++;
        }
        _right = right;
    }
    ILimitation(const ILimitation& other) {
        for (int i = 0; i < other._left.size(); i++) {
            this->_left.push_back(other._left[i]);
        }
        this->_right = other._right;
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
    virtual std::shared_ptr<ILimitation> copy() = 0;
};

class Equation : public ILimitation {
public:
    Equation() = default;
    template <class iter>
    Equation(iter left_begin, iter left_end, RationalNumber right)
        : ILimitation(left_begin, left_end, right) {}
    virtual ~Equation() = default;
    const std::string getLimitationType() {
        return "Equation";
    }
    const std::string getSign() {
        return "=";
    }
    std::shared_ptr<ILimitation> copy() {
        return std::make_shared<Equation>(_left.begin(), _left.end(), _right);
        //return new Equation(_left.begin(), _left.end(), _right);
    }
};

class LessEqual : public ILimitation {
public:
    LessEqual() = default;
    template <class iter>
    LessEqual(iter left_begin, iter left_end, RationalNumber right)
        : ILimitation(left_begin, left_end, right) {}
    virtual ~LessEqual() = default;
    const std::string getLimitationType() {
        return "LessEqual";
    }
    const std::string getSign() {
        return "<=";
    }
    std::shared_ptr<ILimitation> copy() {
        return std::make_shared<LessEqual>(_left.begin(), _left.end(), _right);
        //return new LessEqual(_left.begin(), _left.end(), _right);
    }
};

class GreatEqual : public ILimitation {
public:
    GreatEqual() = default;
    template <class iter>
    GreatEqual(iter left_begin, iter left_end, RationalNumber right)
        : ILimitation(left_begin, left_end, right) {}
    virtual ~GreatEqual() = default;
    const std::string getLimitationType() {
        return "GreatEqual";
    }
    const std::string getSign() {
        return ">=";
    }
    std::shared_ptr<ILimitation> copy() {
        return std::make_shared<GreatEqual>(_left.begin(), _left.end(), _right);
        //return new GreatEqual(_left.begin(), _left.end(), _right);
    }
};

#endif