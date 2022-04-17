#include <iostream>
#include <map>
#include "BigInt.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

enum class OPERATORS_COMPOUND
{
	PLUS_EQUAL,
	MINUS_EQUAL,
	MUL_EQUAL,
	DIV_EQUAL,
	MOD_EQUAL
};

enum class OPERATORS_BINARY
{
	PLUS,
	MINUS,
	MUL,
	DIV,
	MOD
};


enum class OPERATORS_COMPARISON
{
	LESS,
	MORE,
	EQUAL
};


template<typename T>
void print_test_result(const T& result, const T& expected)
{
	cout << "; result = " << result << ", expected = " << expected;
	std::string passed = (result == expected) ? "PASSED" : "FAILED";
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int color = (result == expected) ? 2 : 4;  // Windows console foreground color codes: 2 - green (passed), 4 - red (failed)
	SetConsoleTextAttribute(hConsole, color);
	cout << " " << passed << endl;
	SetConsoleTextAttribute(hConsole, 15);
#else
	cout << " " << passed << endl;
#endif
}


void test_compound_arithmetic(BigInt& left, const BigInt& right, const std::string& op, const BigInt& expected)
{
	static std::map<std::string, OPERATORS_COMPOUND> map_op = { 
		{"+=", OPERATORS_COMPOUND::PLUS_EQUAL}, 
		{"-=", OPERATORS_COMPOUND::MINUS_EQUAL},
		{"*=", OPERATORS_COMPOUND::MUL_EQUAL},
		{"/=", OPERATORS_COMPOUND::DIV_EQUAL},
		{"%=", OPERATORS_COMPOUND::MOD_EQUAL}
	};

	cout << left << " " << op << " " << right;

	BigInt result;	
	switch (map_op[op])
	{
	case OPERATORS_COMPOUND::PLUS_EQUAL: {left += right; result = left; break; }
	case OPERATORS_COMPOUND::MINUS_EQUAL: {left -= right; result = left;  break; }
	case OPERATORS_COMPOUND::MUL_EQUAL: {left *= right; result = left;  break; }
	case OPERATORS_COMPOUND::DIV_EQUAL: {left /= right; result = left;  break; }
	case OPERATORS_COMPOUND::MOD_EQUAL: {left %= right; result = left;  break; }
	default: {throw std::invalid_argument("unknown compound operator"); break; }
	}

	print_test_result<BigInt>(result, expected);
}


void test_binary_arithmetic(const BigInt& left, const BigInt& right, const std::string& op, const BigInt& expected)
{
	static std::map<std::string, OPERATORS_BINARY> map_op = {
		{"+", OPERATORS_BINARY::PLUS},
		{"-", OPERATORS_BINARY::MINUS},
		{"*", OPERATORS_BINARY::MUL},
		{"/", OPERATORS_BINARY::DIV},
		{"%", OPERATORS_BINARY::MOD}
	};

	cout << left << " " << op << " " << right;

	BigInt result;
	switch (map_op[op])
	{
	case OPERATORS_BINARY::PLUS: {result = left + right; break; }
	case OPERATORS_BINARY::MINUS: {result = left - right; break; }
	case OPERATORS_BINARY::MUL: {result = left * right; break; }
	case OPERATORS_BINARY::DIV: {result = left / right; break; }
	case OPERATORS_BINARY::MOD: {result = left % right; break; }
	default: {throw std::invalid_argument("unknown binary operator"); break; }
	}

	print_test_result<BigInt>(result, expected);
}


void test_comparison(const BigInt& left, const BigInt& right, const std::string& op, bool expected)
{
	static std::map<std::string, OPERATORS_COMPARISON> map_op = {
		{"<", OPERATORS_COMPARISON::LESS},
		{">", OPERATORS_COMPARISON::MORE},
		{"==", OPERATORS_COMPARISON::EQUAL}
	};

	cout << left << " " << op << " " << right;

	bool result;
	switch (map_op[op])
	{
	case OPERATORS_COMPARISON::LESS: result = (left < right); break;
	case OPERATORS_COMPARISON::MORE: result = (left > right); break;
	case OPERATORS_COMPARISON::EQUAL: result = (left == right); break;
	default: throw std::invalid_argument("unknown comparison operator"); break;
	}

	print_test_result<bool>(result, expected);
}


int main()
{
	BigInt x("12345");
	cout << x << endl;
	cout << BigInt("-666") << endl;
	cout << BigInt("-0") << endl;

	cout << "testing compound arithmetic operators" << endl;
	test_compound_arithmetic(x, BigInt("17"), "+=", BigInt("12362"));
	BigInt y("1119999");
	BigInt z("1");
	test_compound_arithmetic(z, y, "+=", BigInt("1120000"));
	test_compound_arithmetic(y, 23, "+=", 1120022);  // implicit conversion from int to BigInt
	x = BigInt(12345);
	test_compound_arithmetic(x, BigInt(-12), "+=", BigInt("12333"));
	x = BigInt(12);
	test_compound_arithmetic(x, BigInt(-12345), "+=", BigInt("-12333"));
	x = BigInt(-12);
	test_compound_arithmetic(x, BigInt(12345), "+=", BigInt("12333"));
	x = BigInt(-12);
	test_compound_arithmetic(x, BigInt(-12345), "+=", BigInt("-12357"));
	x = BigInt(-12345);
	test_compound_arithmetic(x, BigInt(-12), "+=", BigInt("-12357"));
	x = BigInt("392992536972");
	test_compound_arithmetic(x, BigInt("654987561620"), "+=", BigInt("1047980098592"));

	// testing -=
	BigInt a(12345);
	test_compound_arithmetic(a, 12, "-=", BigInt(12333));
	a = BigInt(12345);
	test_compound_arithmetic(a, 12344, "-=", BigInt(1));
	a = BigInt(12345);
	test_compound_arithmetic(a, a, "-=", BigInt(0));
	a = BigInt(12);
	test_compound_arithmetic(a, 12345, "-=", BigInt("-12333"));
	a = BigInt(1000);
	test_compound_arithmetic(a, 1001, "-=", BigInt("-1"));
	a = BigInt(12);
	test_compound_arithmetic(a, -12345, "-=", BigInt("12357"));
	a = BigInt(-12);
	test_compound_arithmetic(a, 12345, "-=", BigInt("-12357"));
	a = BigInt(-12345);
	test_compound_arithmetic(a, 12, "-=", BigInt("-12357"));
	a = BigInt(-12);
	test_compound_arithmetic(a, -12345, "-=", BigInt("12333"));
	a = BigInt(2274);
	test_compound_arithmetic(a, 331, "-=", BigInt(2274-331));
	a = BigInt(46386);
	test_compound_arithmetic(a, 2708, "-=", BigInt(46386 - 2708));
	a = BigInt(-976286);
	test_compound_arithmetic(a, 22773, "-=", BigInt(-976286 - 22773));


	/*
	cout << "testing multiply" << endl;
	a = BigInt(438);
	a.MulByOneDigitNumber(2);
	print_test_result<BigInt>(a, BigInt(876));

	a = BigInt(438);
	a.MulByOneDigitNumber(0);
	print_test_result<BigInt>(a, BigInt(0));

	a = BigInt(438);
	a.MulByOneDigitNumber(1);
	print_test_result<BigInt>(a, a);

	a = BigInt(1234);
	a.MulByOneDigitNumber(5);
	print_test_result<BigInt>(a, BigInt(1234 * 5));

	a = BigInt(9999);
	a.MulByOneDigitNumber(9);
	print_test_result<BigInt>(a, BigInt(9999 * 9));

	a = BigInt("45758749445748340984845756848753");
	a.MulByOneDigitNumber(7);
	print_test_result<BigInt>(a, BigInt("320311246120238386893920297941271"));
	*/

	a = BigInt(12345);
	test_compound_arithmetic(a, 12, "*=", BigInt(12345*12));
	a = BigInt(12);
	test_compound_arithmetic(a, 12345, "*=", BigInt(12345 * 12));
	a = BigInt(34);
	test_compound_arithmetic(a, 12345, "*=", BigInt(12345 * 34));
	a = BigInt("259785462579854532");
	test_compound_arithmetic(a, BigInt("-456852197845"), "*=", BigInt("-118683559547786546764583883540"));
	a = BigInt("65498756162");
	test_compound_arithmetic(a, BigInt("654846512324598753216"), "*=", BigInt("42891632034285021531383997316992"));
	a = BigInt("654846512324598753216");
	test_compound_arithmetic(a, BigInt("65498756162"), "*=", BigInt("42891632034285021531383997316992"));
	a = BigInt("898756213212987956216245987562162654956865168765651");
	test_compound_arithmetic(a, -8, "*=", BigInt("-7190049705703903649729967900497301239654921350125208"));


	cout << "testing divide and modulo" << endl;
	a = BigInt(227492);
	test_compound_arithmetic(a, 331, "/=", BigInt(687));
	a = BigInt(227492);
	test_compound_arithmetic(a, 331, "%=", BigInt(95));
	a = BigInt(39160);
	test_compound_arithmetic(a, 8, "/=", BigInt(4895));
	a = BigInt(39160);
	test_compound_arithmetic(a, 8, "%=", BigInt(0));
	a = BigInt(88);
	test_compound_arithmetic(a, 12, "/=", BigInt(88 / 12));
	a = BigInt(88);
	test_compound_arithmetic(a, 12, "%=", BigInt(88 % 12));
	a = BigInt(1300);
	test_compound_arithmetic(a, 144, "/=", BigInt(1300 / 144));
	a = BigInt(1300);
	test_compound_arithmetic(a, 144, "%=", BigInt(1300 % 144));
	a = BigInt(12345);
	test_compound_arithmetic(a, a, "/=", BigInt(1));
	a = BigInt(12);
	test_compound_arithmetic(a, 12345, "/=", BigInt(12 / 12345));
	a = BigInt(12);
	test_compound_arithmetic(a, 12345, "%=", BigInt(12 % 12345));
	a = BigInt(23);
	test_compound_arithmetic(a, 100, "/=", BigInt(23 / 100));
	a = BigInt(23);
	test_compound_arithmetic(a, 100, "%=", BigInt(23 % 100));
	a = BigInt(23);
	test_compound_arithmetic(a, -100, "/=", BigInt(23 / -100));
	a = BigInt(23);
	test_compound_arithmetic(a, -100, "%=", BigInt(23 % -100));
	a = BigInt(-23);
	test_compound_arithmetic(a, 100, "/=", BigInt(-23 / 100));
	a = BigInt(-23);
	test_compound_arithmetic(a, 100, "%=", BigInt(-23 % 100));
	a = BigInt(-23);
	test_compound_arithmetic(a, -100, "/=", BigInt(-23 / -100));
	a = BigInt(-23);
	test_compound_arithmetic(a, -100, "%=", BigInt(-23 % -100));
	a = BigInt(549875);
	test_compound_arithmetic(a, 6277, "/=", BigInt(549875 / 6277));
	a = BigInt(549875);
	test_compound_arithmetic(a, 6277, "%=", BigInt(549875 % 6277));
	a = BigInt(-549875);
	test_compound_arithmetic(a, 6277, "/=", BigInt(-549875 / 6277));
	a = BigInt(-549875);
	test_compound_arithmetic(a, 6277, "%=", BigInt(-549875 % 6277));
	a = BigInt(549875);
	test_compound_arithmetic(a, -6277, "/=", BigInt(549875 / -6277));
	a = BigInt(549875);
	test_compound_arithmetic(a, -6277, "%=", BigInt(549875 % -6277));
	a = BigInt(-549875);
	test_compound_arithmetic(a, -6277, "/=", BigInt(-549875 / -6277));
	a = BigInt(-549875);
	test_compound_arithmetic(a, -6277, "%=", BigInt(-549875 % -6277));


	cout << "testing binary arithmetic" << endl;
	test_binary_arithmetic(12345, 12, "+", BigInt(12357));
	test_binary_arithmetic(12, 12345, "+", BigInt(12357));
	test_binary_arithmetic(12345, -12, "+", BigInt(12333));
	test_binary_arithmetic(-12345, 12, "+", BigInt(-12333));
	test_binary_arithmetic(-12345, -12, "+", BigInt(-12357));
	test_binary_arithmetic(12345, 0, "+", BigInt(12345));
	test_binary_arithmetic(12345, 12, "-", BigInt(12333));
	test_binary_arithmetic(12345, -12, "-", BigInt(12357));
	test_binary_arithmetic(-12345, 12, "-", BigInt(-12357));
	test_binary_arithmetic(-12345, -12, "-", BigInt(-12333));
	test_binary_arithmetic(12345, 12, "*", BigInt(12345*12));
	test_binary_arithmetic(-12345, 12, "*", BigInt(-12345 * 12));
	test_binary_arithmetic(12, 1234567, "*", BigInt(12 * 1234567));
	test_binary_arithmetic(BigInt("-5642321987565423217"), BigInt("-685421578954621"), "*", BigInt("3867369245687467817725668302835757"));
	test_binary_arithmetic(12345, 12, "/", BigInt(12345 / 12));
	test_binary_arithmetic(-12345, 12, "/", BigInt(-12345 / 12));
	test_binary_arithmetic(12345, 12, "%", BigInt(12345 % 12));
	test_binary_arithmetic(-12345, 12, "%", BigInt(-12345 % 12));


	cout << "testing unary minus" << endl;
	cout << -(-BigInt("5")) << endl;
	cout << BigInt(-5).Abs() << BigInt(5).Abs() << endl;

	cout << "testing comparison" << endl;
	test_comparison(12345, 12, "<", false);
	test_comparison(12, 12345, "<", true);
	test_comparison(12345, 12345, "<", false);
	test_comparison(12, -12, "<", false);
	test_comparison(-12345, 12, "<", true);
	test_comparison(-12345, -12, "<", true);
	test_comparison(-12, -12, "<", false);
	test_comparison(12345, 67890, "<", true);
	test_comparison(1123, 6123, "<", true);
	test_comparison(BigInt("56473947575069476370556383057489"), BigInt("56474957463057503"), "<", false);

	test_comparison(12345, 12, ">", true);
	test_comparison(12, 12345, ">", false);
	test_comparison(12345, 12345, ">", false);
	test_comparison(12, -12, ">", true);
	test_comparison(-12345, 12, ">", false);
	test_comparison(-12345, -12, ">", false);
	test_comparison(-12, -12, ">", false);
	test_comparison(12345, 67890, ">", false);
	test_comparison(1123, 6123, ">", false);

	test_comparison(12345, 12, "==", false);
	test_comparison(12, 12345, "==", false);
	test_comparison(12345, 12345, "==", true);
	test_comparison(12, -12, "==", false);
	test_comparison(-12345, 12, "==", false);
	test_comparison(-12345, -12, "==", false);
	test_comparison(-12, -12, "==", true);
	test_comparison(12345, 67890, "==", false);
	test_comparison(1123, 6123, "==", false);
	test_comparison(0, 0, "==", true);
	test_comparison(-0, 0, "==", true);
	test_comparison(-BigInt(0), 0, "==", true);
	test_comparison(BigInt("56473947575069476370556383057489"), BigInt("56473947575069476370556383057489"), "==", true);

	return 0;
}