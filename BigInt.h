#pragma once

#include <string>

class BigInt
{
public:
	BigInt();
	explicit BigInt(const std::string& view_str);
	BigInt(const int& i);
	BigInt(const BigInt& other);

	BigInt& operator=(const BigInt& other);

	// getters
	const std::string& GetStr() const { return m_str; }
	bool GetSign() const { return m_sign; }
	size_t GetNumDigits() const { return m_str.size(); }

	// OPERATORS
	// compound arithmetic
	BigInt& operator+=(const BigInt& other);
	BigInt& operator-=(const BigInt& other);
	BigInt& operator*=(const BigInt& other);
	BigInt& operator/=(const BigInt& other);
	BigInt& operator%=(const BigInt& other);
	// unary arithmetic
	const BigInt operator-() const;
	// increments/decrements
	BigInt& operator++();  // prefix
	BigInt& operator--();
	BigInt operator++(int); // postfix
	BigInt operator--(int);

	// methods
	void Negate();  // change sign in-place
	const BigInt Abs() const;  // get copy of absolute value

	// viewing
	std::string GetViewStr() const;
	friend std::ostream& operator<<(std::ostream& stream, const BigInt& x);

private:
	std::string ReverseStr(const std::string& s) const;
	void RemoveHeadingZeroes(); // in-place
	BigInt DivideBy(const BigInt& b);
	BigInt& MulByOneDigitNumber(int num);  // num is [0; 9]
	BigInt& MulByTen(size_t k); // x *= (10 ** k)

private:
	std::string m_str;  // contains digits in reverse order
	bool m_sign = false;  // false means non-negative, true means negative
};


// OPERATORS
// binary arithmetic
const BigInt operator+(const BigInt& left, const BigInt& right);
const BigInt operator-(const BigInt& left, const BigInt& right);
const BigInt operator*(const BigInt& left, const BigInt& right);
const BigInt operator/(const BigInt& left, const BigInt& right);
const BigInt operator%(const BigInt& left, const BigInt& right);

// comparison
bool operator<(const BigInt& left, const BigInt& right);
bool operator>(const BigInt& left, const BigInt& right);
bool operator<=(const BigInt& left, const BigInt& right);
bool operator>=(const BigInt& left, const BigInt& right);
bool operator==(const BigInt& left, const BigInt& right);
bool operator!=(const BigInt& left, const BigInt& right);