#include "RationalNumber.h"

RationalNumber operator+(const RationalNumber& first, const RationalNumber& second) {
	RationalNumber res;
	if (first._denominator != second._denominator) {
		int max_denom = std::max(first._denominator, second._denominator), min_denom = std::min(first._denominator, second._denominator);
		while (max_denom % min_denom != 0) {
			max_denom += max_denom;
		}
		res._numerator = first._numerator * (max_denom / first._denominator) + second._numerator * (max_denom / second._denominator);
		res._denominator = max_denom;
	}
	else {
		res._numerator = first._numerator + second._numerator;
		res._denominator = first._denominator;
	}
	res.cast();
	return res;
}
RationalNumber operator-(const RationalNumber& first, const RationalNumber& second) {
	RationalNumber res;
	if (first._denominator != second._denominator) {
		int max_denom = std::max(first._denominator, second._denominator), min_denom = std::min(first._denominator, second._denominator);
		while (max_denom % min_denom != 0) {
			max_denom += max_denom;
		}
		res._numerator = first._numerator * (max_denom / first._denominator) - second._numerator * (max_denom / second._denominator);
		res._denominator = max_denom;
	}
	else {
		res._numerator = first._numerator - second._numerator;
		res._denominator = first._denominator;
	}
	res.cast();
	return res;
}
RationalNumber operator*(const RationalNumber& first, const RationalNumber& second) {
	RationalNumber res(first._numerator * second._numerator, first._denominator * second._denominator);
	res.cast();
	return res;
}
RationalNumber operator/(const RationalNumber& first, const RationalNumber& second) {//ToDo: a/0 - no way
	RationalNumber res(first._numerator * second._denominator, first._denominator * second._numerator);
	res.cast();
	return res;
}

RationalNumber RationalNumber::operator+=(const RationalNumber& second) {
	if (*this == 0) {
		*this = second;
	}
	else if (second != 0) {
		if (this->_denominator != second._denominator) {
			int max_denom = std::max(this->_denominator, second._denominator), min_denom = std::min(this->_denominator, second._denominator);
			while (max_denom % min_denom != 0) {
				max_denom += max_denom;
			}
			this->_numerator = this->_numerator * (max_denom / this->_denominator) + second._numerator * (max_denom / second._denominator);
			this->_denominator = max_denom;
		}
		else {
			this->_numerator += second._numerator;
		}
	}
	this->cast();
	return *this;
}
RationalNumber RationalNumber::operator-=(const RationalNumber& second) {
	if (*this == 0) {
		*this = second;
	}
	else if (second != 0) {
		if (this->_denominator != second._denominator) {
			int max_denom = std::max(this->_denominator, second._denominator), min_denom = std::min(this->_denominator, second._denominator);
			while (max_denom % min_denom != 0) {
				max_denom += max_denom;
			}
			this->_numerator = this->_numerator * (max_denom / this->_denominator) - second._numerator * (max_denom / second._denominator);
			this->_denominator = max_denom;
		}
		else {
			this->_numerator -= second._numerator;
		}
	}
	this->cast();
	return *this;
}
RationalNumber RationalNumber::operator*=(const RationalNumber& second) {
	this->_numerator *= second._numerator;
	this->_denominator *= second._denominator;
	this->cast();
	return *this;
}
RationalNumber RationalNumber::operator/=(const RationalNumber& second) {
	this->_numerator *= second._denominator;
	this->_denominator *= second._numerator;
	this->cast();
	return *this;
}

RationalNumber RationalNumber::operator-() {
	RationalNumber res(-this->_numerator, this->_denominator);
	res.cast();
	return res;
}

std::ostream& operator<<(std::ostream& out, const RationalNumber& number) {
	out << number._numerator;
	if(abs(number._denominator) != 1 && number._numerator != 0) out << '/' << number._denominator;
	return out;
}
std::istream& operator>>(std::istream& in, RationalNumber& number) {
	in >> number._numerator;
	in >> number._denominator;
	while (number._denominator == 0) {
		std::cerr << "Bad denominator.\n";
		in >> number._denominator;
	}
	number.cast();
	return in;
}

bool operator==(const RationalNumber& first, const RationalNumber& second) {
	bool res;
	if (first._denominator != second._denominator) {
		int max_denom = std::max(first._denominator, second._denominator), min_denom = std::min(first._denominator, second._denominator);
		while (max_denom % min_denom != 0) {
			max_denom += max_denom;
		}
		int f_numerator = first._numerator * (max_denom / first._denominator),
			s_numerator = second._numerator * (max_denom / second._denominator);
		res = (f_numerator == s_numerator);
	}
	else {
		res = (first._numerator == second._numerator);
	}
	return res;
}

bool operator!=(const RationalNumber& first, const RationalNumber& second) {
	bool res;
	if (first._denominator != second._denominator) {
		int max_denom = std::max(first._denominator, second._denominator), min_denom = std::min(first._denominator, second._denominator);
		while (max_denom % min_denom != 0) {
			max_denom += max_denom;
		}
		int f_numerator = first._numerator * (max_denom / first._denominator),
			s_numerator = second._numerator * (max_denom / second._denominator);
		res = (f_numerator != s_numerator);
	}
	else {
		res = (first._numerator != second._numerator);
	}
	return res;
}

bool operator>=(const RationalNumber& first, const RationalNumber& second) {
	bool res;
	if (first._denominator != second._denominator) {
		int max_denom = std::max(first._denominator, second._denominator), min_denom = std::min(first._denominator, second._denominator);
		while (max_denom % min_denom != 0) {
			max_denom += max_denom;
		}
		int f_numerator = first._numerator * (max_denom / first._denominator), 
			s_numerator = second._numerator * (max_denom / second._denominator);
		res = (f_numerator >= s_numerator);
	}
	else {
		res = (first._numerator >= second._numerator);
	}
	return res;
}

bool operator<=(const RationalNumber& first, const RationalNumber& second) {
	bool res;
	if (first._denominator != second._denominator) {
		int max_denom = std::max(first._denominator, second._denominator), min_denom = std::min(first._denominator, second._denominator);
		while (max_denom % min_denom != 0) {
			max_denom += max_denom;
		}
		int f_numerator = first._numerator * (max_denom / first._denominator),
			s_numerator = second._numerator * (max_denom / second._denominator);
		res = (f_numerator <= s_numerator);
	}
	else {
		res = (first._numerator <= second._numerator);
	}
	return res;
}

bool operator>(const RationalNumber& first, const RationalNumber& second) {
	bool res;
	if (first._denominator != second._denominator) {
		int max_denom = std::max(first._denominator, second._denominator), min_denom = std::min(first._denominator, second._denominator);
		int t = max_denom;
		while (t % min_denom != 0) {
			t += max_denom;
		}
		int f_numerator = first._numerator * (max_denom / first._denominator),
			s_numerator = second._numerator * (max_denom / second._denominator);
		res = (f_numerator > s_numerator);
	}
	else {
		res = (first._numerator > second._numerator);
	}
	return res;
}

bool operator<(const RationalNumber& first, const RationalNumber& second) {
	bool res;
	if (first._denominator != second._denominator) {
		int max_denom = std::max(first._denominator, second._denominator), min_denom = std::min(first._denominator, second._denominator);
		while (max_denom % min_denom != 0) {
			max_denom += max_denom;
		}
		int f_numerator = first._numerator * (max_denom / first._denominator),
			s_numerator = second._numerator * (max_denom / second._denominator);
		res = (f_numerator < s_numerator);
	}
	else {
		res = (first._numerator < second._numerator);
	}
	return res;
}

void RationalNumber::cast() {
	if (this->_numerator != 0) {
		if (this->_denominator < 0) {
			this->_numerator *= (-1);
			this->_denominator *= (-1);
		}
		int a = std::max(abs(this->_numerator), abs(this->_denominator));
		int b = std::min(abs(this->_numerator), abs(this->_denominator));
		while (a % b != 0) {
			int t2 = a % b;
			a = b;
			b = t2;
		}
		this->_numerator /= b;
		this->_denominator /= b;
	}
}

RationalNumber RationalNumber::operator=(const RationalNumber& other) {
	if (this == &other) {
		return *this;
	}

	this->_numerator = other._numerator;
	this->_denominator = other._denominator;
	return *this;
}