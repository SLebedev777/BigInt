#include "BigInt.h"
#include <iostream>
#include <cassert>


BigInt::BigInt() :
	m_str("0"),
	m_sign(false)
{}


BigInt::BigInt(const std::string& view_str)
{
	char c = view_str[0];
	if (c == '-' || c == '+')
	{
		m_sign = (c == '-' && view_str[1] != '0') ? true : false;
		m_str = ReverseStr(view_str.substr(1));
	}
	else
	{
		m_str = ReverseStr(view_str);
	}
}


BigInt::BigInt(const int& i) :
	BigInt(std::to_string(i))
{}


BigInt::BigInt(const BigInt& other) :
	m_str(other.GetStr()),
	m_sign(other.GetSign())
{}


BigInt& BigInt::operator=(const BigInt& other)
{
	if (this == &other)
		return *this;

	m_str = other.GetStr();
	m_sign = other.GetSign();
	return *this;
}


std::string BigInt::ReverseStr(const std::string& s) const
{
	std::string tmp(s.rbegin(), s.rend());
	return tmp;
}


std::string BigInt::GetViewStr() const
{
	std::string tmp;
	if (GetSign())
		tmp.push_back('-');
	tmp += ReverseStr(m_str);
	return tmp;
}


std::ostream& operator<<(std::ostream& stream, const BigInt& x)
{
	stream << "(" << x.GetViewStr() << ")";
	return stream;
}


BigInt& BigInt::operator+=(const BigInt& other)
{
	auto size1 = GetNumDigits();
	auto size2 = other.GetNumDigits();
	auto max_size = std::max(size1, size2);
	const std::string& str2 = other.GetStr();

	bool sign1 = GetSign();
	bool sign2 = other.GetSign();

	// a >= 0 && b >= 0
	// a < 0 && b < 0
	if (sign1 == sign2)
	{
		int carry = 0;
		for (int i = 0; i < max_size; ++i)
		{
			int d1 = (i < size1) ? (m_str[i] - '0') : 0;
			int d2 = (i < size2) ? (str2[i] - '0') : 0;
			int sum = d1 + d2 + carry;
			int digit = sum % 10;
			carry = sum / 10;

			char c = '0' + digit;
			if (i < size1)
			{
				m_str[i] = c;
			}
			else
			{
				m_str.push_back(c);
			}
			// early stopping
			if (carry == 0 && (size1 > size2 && i > size2))
			{
				break;
			}
		}
		if (carry > 0)
			m_str.push_back('0' + carry);
	}
	else
	{
		// a >= 0, b < 0 : a += -|b| <-> a -= |b|
		if (!sign1 && sign2)
		{
			*this -= -other;
		}
		// a < 0, b >= 0 : -|a| += b <-> a = b - |a|
		if (sign1 && !sign2)
		{
			BigInt tmp = *this;
			*this = other;
			*this -= -tmp;
		}
	}
	return *this;
}


BigInt& BigInt::operator-=(const BigInt& other)
{
	if (*this == other)
	{
		*this = BigInt("0");
		return *this;
	}

	auto size1 = GetNumDigits();
	auto size2 = other.GetNumDigits();
	auto max_size = std::max(size1, size2);
	const std::string& str2 = other.GetStr();

	bool sign1 = GetSign();
	bool sign2 = other.GetSign();

	// + +
	if (!(sign1 || sign2))
	{
		if (*this > other)
		{
			int borrow = 0;
			for (int i = 0; i < max_size; ++i)
			{
				int d1 = (i < size1) ? (m_str[i] - '0') : 0;
				int d2 = (i < size2) ? (str2[i] - '0') : 0;
				int sub = d1 - d2 - borrow;
				int digit = (sub < 0) ? sub + 10 : sub;
				borrow = (sub < 0) ? 1 : 0;
				char c = '0' + digit;
				m_str[i] = c;
			}
			RemoveHeadingZeroes();
		}
		else
		{
			// 12 - 12345 = -(12345 - 12)
			BigInt tmp = *this;
			*this = other;
			*this -= tmp;
			Negate();
		}
	}
	// - -
	else if (sign1 && sign2)
	{
		// -12 -= (-12345) <-> -12 += 12345  <-> 12345 -= 12
		if (*this > other)
		{
			Negate();
			BigInt tmp = *this;
			*this = other;
			Negate();
			*this -= tmp;
		}
		// -12345 -= (-12) <-> -12345 += 12
		else
		{
			Negate();
			*this -= -other;
			Negate();
		}
	}
	// + -
	else if (!sign1 && sign2)
	{
		*this += -other;
	}
	// - +
	else
	{
		// -12345 -= 12
		Negate();
		*this += other;
		Negate();
	}

	return *this;
}


BigInt& BigInt::operator*=(const BigInt& other)
{
	auto size1 = GetNumDigits();
	auto size2 = other.GetNumDigits();
	auto max_size = std::max(size1, size2);
	const std::string& str2 = other.GetStr();

	bool sign1 = GetSign();
	bool sign2 = other.GetSign();

	if (size2 == 1)
	{
		MulByOneDigitNumber(other.GetStr()[0] - '0');
		m_sign = (sign1 != sign2);
		return *this;
	}
	// calc partial multiplies for every digit in "other" and sum them
	BigInt sum(0);
	for (int i = 0; i < size2; ++i)
	{
		BigInt tmp = *this;
		tmp.MulByOneDigitNumber(str2[i] - '0');
		tmp.MulByTen(i);
		sum += tmp;
	}
	*this = sum;
	m_sign = (sign1 != sign2);  // logical XOR

	return *this;
}


BigInt& BigInt::operator/=(const BigInt& other)
{
	DivideBy(other);
	return *this;
}


BigInt& BigInt::operator%=(const BigInt& other)
{
	*this = DivideBy(other);
	return *this;
}


void BigInt::Negate()
{
	m_sign = !m_sign;
}


const BigInt BigInt::operator-() const
{
	BigInt tmp(*this);
	tmp.Negate();
	return std::move(tmp);
}

const BigInt BigInt::Abs() const
{
	if (GetSign())
		return -(*this);
	return *this;
}


bool operator<(const BigInt& left, const BigInt& right)
{
	bool sign1 = left.GetSign();
	bool sign2 = right.GetSign();
	// left < 0, right > 0
	if (sign1 && !sign2)
		return true;
	// left >= 0, right < 0
	if (!sign1 && sign2)
		return false;
	// signs are equal - compare abs(left) VS abs(right)
	// first check if number of digits differ
	auto size1 = left.GetNumDigits();
	auto size2 = right.GetNumDigits();
	if (size1 > size2)
		return sign1;
	else if (size1 < size2)
		return !sign1;
	else
	{
		// signs are equal and number of digits is equal
		const auto& str1 = left.GetStr();
		const auto& str2 = right.GetStr();
		for (int i = size1 - 1; i >= 0; --i)
		{
			int d1 = str1[i] - '0';
			int d2 = str2[i] - '0';
			if (d1 == d2)
				continue;
			return ((d1 < d2) && (!sign1));
		}
		// left and right are completely equal
		return false;
	}
}


void BigInt::RemoveHeadingZeroes()
{
	auto size = GetNumDigits();
	if (size < 2)
		return;
	auto new_size = size;
	for (int i = size - 1; i >= 1; --i)
	{
		if (m_str[i] != '0')
			break;
		--new_size;
	}
	if (new_size != size)
	{
		m_str.resize(new_size);
	}
}


BigInt& BigInt::MulByOneDigitNumber(int num)
{
	assert(num >= 0 && num <= 9);
	if (num == 0)
	{
		m_str = "0";
		m_sign = false;
		return *this;
	}
	if (num == 1)
	{
		return *this;
	}
	int carry = 0;
	auto size = GetNumDigits();
	for (int i = 0; i < size; ++i)
	{
		int d1 = m_str[i] - '0';
		int result = d1 * num + carry;
		int digit = result % 10;
		carry = result / 10;
		char c = '0' + digit;
		m_str[i] = c;
	}
	if (carry > 0)
		m_str.push_back('0' + carry);
	
	return *this;
}


BigInt& BigInt::MulByTen(size_t k)
{
	if (k == 0)
		return *this;
	std::string zeroes(k, '0');
	zeroes.append(m_str);
	m_str = zeroes;
	return *this;
}


BigInt BigInt::DivideBy(const BigInt& other)
{
	auto size1 = GetNumDigits();
	auto size2 = other.GetNumDigits();
	auto max_size = std::max(size1, size2);
	const std::string& str2 = other.GetStr();

	bool sign1 = GetSign();
	bool sign2 = other.GetSign();

	BigInt b = other.Abs();

	 /*TODO:
	 beware of signs to get correct remainder!!!
	 Python: 
	 23 // 100 = 0
	 23 // -100 = -1
	 -23 // 100 = -1
	 -23 // -100 = 0
	 23 % 100 = 23
	 -23 % 100 = 77
	 23 % -100 = -77
	 -23 % -100 = -23
	 
	 C++:
	 23 / 100 = 0
	 23 / -100 = 0
	 -23 / 100 = 0
	 -23 / -100 = 0
	 23 % 100 = 23
	 -23 % 100 = -23
	 23 % -100 = 23
	 -23 % -100 = -23
	 */

	if (Abs() < b)
	{
		BigInt tmp(*this);
		m_str = "0";
		m_sign = false;
		return tmp;  // remainder
	}
	if (Abs() == b)
	{
		m_str = "1";
		m_sign = (sign1 != sign2);
		return BigInt(0);  // remainder
	}
	std::string divisor;
	int j = size1 - size2;
	j = (j < 0) ? 0 : j;
	BigInt r(ReverseStr(m_str.substr(j, size2)));
	while (j >= 0)
	{
		if (r < b)
		{
			j--;
			if (j < 0)
				break;
			r = r * 10 + BigInt(m_str[j] - '0');
		}
		int d = 0;
		while (r >= b)
		{
			r -= b;
			d++;
		}
		divisor.push_back(d + '0');
	}
	m_str = ReverseStr(divisor);
	m_sign = (sign1 != sign2);
	if (sign1)
		r.Negate();  // in C++, sign(remainder) == sign(*this)
	return r;
}


bool operator>(const BigInt& left, const BigInt& right)
{
	return right < left;
}


bool operator==(const BigInt& left, const BigInt& right)
{
	// patch for (-0) == (0)
	if (left.GetNumDigits() == 1 && right.GetNumDigits() == 1 && left.GetStr()[0] == '0' && right.GetStr()[0] == '0')
		return true;
	return !((left > right) || (left < right));
}


bool operator<=(const BigInt& left, const BigInt& right)
{
	return !(left > right);
}


bool operator>=(const BigInt& left, const BigInt& right)
{
	return !(left < right);
}


bool operator!=(const BigInt& left, const BigInt& right)
{
	return !(left == right);
}


const BigInt operator+(const BigInt& left, const BigInt& right)
{
	BigInt tmp = left;
	tmp += right;
	return tmp;
}


const BigInt operator-(const BigInt& left, const BigInt& right)
{
	BigInt tmp = left;
	tmp -= right;
	return tmp;
}


const BigInt operator*(const BigInt& left, const BigInt& right)
{
	BigInt tmp = left;
	tmp *= right;
	return tmp;
}


const BigInt operator/(const BigInt& left, const BigInt& right)
{
	BigInt tmp = left;
	tmp /= right;
	return tmp;
}


const BigInt operator%(const BigInt& left, const BigInt& right)
{
	BigInt tmp = left;
	tmp %= right;
	return tmp;
}
