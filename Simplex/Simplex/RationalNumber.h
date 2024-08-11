#ifndef _RATIONALNUMBER_H_
#define _RATIONALNUMBER_H_

#include <iostream>

class RationalNumber {
	int _numerator = 0; //чисельник
	int _denominator = 1; //знаменник
public:
	RationalNumber() = default;
	RationalNumber(int number) {
		this->_numerator = number;
	}
	//RationalNumber(double number) {
	//	//this->_numerator = number;
	//}
	RationalNumber(int numerator, int denominator) {
		this->_numerator = numerator;
		this->_denominator = denominator;
		this->cast();
	}
	RationalNumber(const RationalNumber& other) {
		this->_numerator = other._numerator;
		this->_denominator = other._denominator;
		this->cast();
	}
	virtual ~RationalNumber() = default;

	void cast();

	friend RationalNumber operator+(const RationalNumber& first, const RationalNumber& second);
	friend RationalNumber operator-(const RationalNumber& first, const RationalNumber& second);
	friend RationalNumber operator*(const RationalNumber& first, const RationalNumber& second);
	friend RationalNumber operator/(const RationalNumber& first, const RationalNumber& second);

	RationalNumber operator-();

	RationalNumber operator+=(const RationalNumber& second);
	RationalNumber operator-=(const RationalNumber& second);
	RationalNumber operator*=(const RationalNumber& second);
	RationalNumber operator/=(const RationalNumber& second);

	friend bool operator==(const RationalNumber& first, const RationalNumber& second);
	friend bool operator!=(const RationalNumber& first, const RationalNumber& second);
	friend bool operator>=(const RationalNumber& first, const RationalNumber& second);
	friend bool operator<=(const RationalNumber& first, const RationalNumber& second);
	friend bool operator>(const RationalNumber& first, const RationalNumber& second);
	friend bool operator<(const RationalNumber& first, const RationalNumber& second);

	friend std::ostream& operator<<(std::ostream& out, const RationalNumber& number);
	friend std::istream& operator>>(std::istream& in, RationalNumber& number);
};

#endif