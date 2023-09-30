#include "binary.h"

std::string convertToBinary(std::string decimalNumber)
{
	std::string binaryNumber{};

	// Remove sign from front
	if (decimalNumber.front() == '-' || decimalNumber.front() == '+')
	{
		decimalNumber.erase(decimalNumber.begin());
	}

	if (decimalNumber == "0") return "0";

	while (!decimalNumber.empty())
	{
		// Remove zeros at front of decimal
		if (decimalNumber.at(0) == '0')
		{
			decimalNumber.erase(decimalNumber.begin());
			continue;
		}

		binaryNumber.insert(binaryNumber.begin(), (decimalNumber.back() - '0') % 2 + '0');

		int remainder{ 0 };
		for (char& c : decimalNumber)
		{
			if (c < '0' || c > '9') throw std::invalid_argument("A decimal number should only contain digits");

			int digit = c - '0';

			int result = (digit + 10 * remainder) >> 1;
			remainder = (digit + 10 * remainder) & 1;

			c = result + '0';
		}
	}
	
	return binaryNumber;
}

std::string convertToDecimal(std::string binaryNumber)
{
	std::string decimalNumber{ "0" };

	for (char c : binaryNumber)
	{
		if (c != '0' && c != '1') throw std::invalid_argument("A binary number should only contain 0 or 1");

		int remainder{ c - '0' };
		
		for (auto it{ decimalNumber.rbegin() }; it != decimalNumber.rend(); it++)
		{
			int digit = (*it - '0') * 2 + remainder;
			remainder = digit / 10;
			*it = (digit % 10) + '0';
		}

		if (remainder != 0)
		{
			decimalNumber.insert(decimalNumber.begin(), remainder + '0');
		}
	}

	return decimalNumber;
}

std::string addBinary(std::string binaryNumber1, std::string binaryNumber2)
{
	size_t binarySize{ std::max(binaryNumber1.size(), binaryNumber2.size()) };

	if (binarySize == 0) return "0";

	binaryNumber1.insert(binaryNumber1.begin(), binarySize - binaryNumber1.size(), '0');
	binaryNumber2.insert(binaryNumber2.begin(), binarySize - binaryNumber2.size(), '0');

	std::string result{};
	result.reserve(binarySize);

	int carry{ 0 };
	for (auto digit1{ binaryNumber1.rbegin() }, digit2{ binaryNumber2.rbegin() };
		digit1 != binaryNumber1.rend() && digit2 != binaryNumber2.rend(); digit1++, digit2++)
	{
		carry += (*digit1 == '1') + (*digit2 == '1');
		result.insert(result.begin(), (carry & 1) + '0');
		carry = carry >> 1;
	}

	if (carry != 0) result.insert(result.begin(), carry + '0');

	return result;
}

std::string decimal::add(std::string decimalNumber1, std::string decimalNumber2)
{
	size_t decimalSize{ std::max(decimalNumber1.size(), decimalNumber2.size()) };

	if (decimalSize == 0) return "0";

	decimalNumber1.insert(decimalNumber1.begin(), decimalSize - decimalNumber1.size(), '0');
	decimalNumber2.insert(decimalNumber2.begin(), decimalSize - decimalNumber2.size(), '0');

	std::string result(decimalSize, '0');

	int carry{ 0 };
	for (auto digit1{ decimalNumber1.rbegin() }, digit2{ decimalNumber2.rbegin() }, value{ result.rbegin() };
		digit1 != decimalNumber1.rend() && digit2 != decimalNumber2.rend(); digit1++, digit2++, value++)
	{
		carry += *digit1 - '0' + *digit2 - '0';
		*value += carry % 10;
		carry /= 10;
	}

	if (carry != 0) result.insert(result.begin(), carry + '0');

	return result;
}

std::string decimal::subtract(std::string decimalNumber1, std::string decimalNumber2)
{
	size_t decimalSize{ decimalNumber1.size() };

	if (decimalSize == 0 || decimalSize < decimalNumber2.size()) return "0";

	decimalNumber2.insert(decimalNumber2.begin(), decimalSize - decimalNumber2.size(), '0');

	std::string result(decimalSize, '0');

	// Perform subtraction using method of complements
	int carry{ 1 };
	for (auto digit1{ decimalNumber1.rbegin() }, digit2{ decimalNumber2.rbegin() }, value{ result.rbegin() };
		digit1 != decimalNumber1.rend() && digit2 != decimalNumber2.rend(); digit1++, digit2++, value++)
	{
		carry += *digit1 - *digit2 + 9;
		*value += carry % 10;
		carry /= 10;
	}

	// Handles case of negative numbers
	if (carry != 1) return "0";

	size_t pos = result.find_first_not_of('0');
	if (pos != std::string::npos)
	{
		return result.substr(pos);
	}

	return "0";
}

std::string decimal::multiply(std::string decimalNumber1, std::string decimalNumber2)
{
	std::string result(decimalNumber1.size() + decimalNumber2.size(), '0');
	auto product{ result.rbegin() };

	// Performs long multiplication
	for (auto num1{ decimalNumber1.rbegin() }; num1 != decimalNumber1.rend(); num1++)
	{
		int carry{};

		for (auto num2{ decimalNumber2.rbegin() }; num2 != decimalNumber2.rend(); num2++, product++)
		{
			int value = *product - '0';
			value += carry + (*num1 - '0') * (*num2 - '0');
			carry = value / 10;
			*product = (value % 10) +'0';
		}

		*product = carry + '0';
		product -= decimalNumber2.size() - 1;
	}

	return result.substr(result.find_first_not_of('0'));
}

static inline bool moreEqual(std::string num1, std::string num2)
{
	if (num1.size() > num2.size())
	{
		return true;
	}
	else if (num1.size() < num2.size())
	{
		return false;
	}
	else
	{
		for (auto value1{ num1.begin() }, value2{ num2.begin() };
			value1 != num1.end() && value2 != num2.end(); value1++, value2++)
		{
			if (*value1 < *value2) return false;
			else if (*value1 > *value2) return true;
		}

		return true;
	}
}

std::string decimal::divide(std::string decimalNumber1, std::string decimalNumber2)
{
	if (decimalNumber2 == "0" || decimalNumber2.empty()) throw std::runtime_error("Division by zero error");

	std::string result(decimalNumber1.size(), '0');
	std::string remainder{};

	for (auto num1{ decimalNumber1.begin() }, res{ result.begin() }; num1 < decimalNumber1.end(); num1++, res++)
	{
		if (remainder != "0") remainder.push_back('0');
		remainder.back() = *num1;

		int quotient{};
		while (moreEqual(remainder, decimalNumber2))
		{
			remainder = subtract(remainder, decimalNumber2);
			quotient++;
		}

		*res += quotient;
	}

	// Remove zeros in front
	size_t pos = result.find_first_not_of('0');
	if (pos != std::string::npos)
	{
		return result.substr(pos);
	}

	return "0";
}