#include "arithmetic.h"

#define IS_ZERO(x) (x.data.size() == 1 && x.data.front() == 0)

constexpr uint64_t uint32Max{ static_cast<uint64_t>(UINT32_MAX) };

inline void removeZeros(uint_t& number)
{
	while (!number.data.empty() && number.data.back() == 0)
	{
		number.data.pop_back();
	}
}

inline void makeLengthEqual(uint_t& number1, uint_t& number2)
{
	size_t number1Size{ number1.data.size() };
	size_t number2Size{ number2.data.size() };

	if (number1Size > number2Size)
	{
		number2.data.insert(number2.data.end(), number1Size - number2Size, 0u);
	}

	if (number2Size > number1Size)
	{
		number1.data.insert(number1.data.end(), number2Size - number1Size, 0u);
	}
}

uint_t::uint_t(size_t size, uint32_t digit)
{
	data = std::vector<uint32_t>(size, digit);
}

uint_t::uint_t(const uint_t& number)
{
	data = number.data;
}

uint_t::uint_t(std::string decimalNumber)
{
	// Remove sign from front
	if (decimalNumber.front() == '-' || decimalNumber.front() == '+')
	{
		decimalNumber.erase(decimalNumber.begin());
	}

	for (char c : decimalNumber)
	{
		uint32_t remainder{ static_cast<uint32_t>(c - '0') };

		for (auto it{ data.begin() }; it != data.end(); it++)
		{
			uint64_t digit = static_cast<uint64_t>(*it) * 10ull + static_cast<uint64_t>(remainder);
			remainder = digit >> 32;
			*it = static_cast<uint32_t>(digit);
		}

		if (remainder != 0)
		{
			data.push_back(remainder);
		}
	}

	if (data.empty()) data.push_back(0);
}

std::ostream& operator<<(std::ostream& os, const uint_t& obj)
{
	std::string decimalNumber{ "0" };

	for (auto c{ obj.data.rbegin() }; c != obj.data.rend(); c++)
	{
		uint32_t remainder{ *c };

		for (auto it{ decimalNumber.rbegin() }; it != decimalNumber.rend(); it++)
		{
			uint64_t digit = (static_cast<uint64_t>(*it - '0') << 32) + static_cast<uint64_t>(remainder);
			remainder = static_cast<uint32_t>(digit / 10ull);
			*it = static_cast<char>(digit % 10ull) + '0';
		}

		while (remainder != 0)
		{
			decimalNumber.insert(decimalNumber.begin(), static_cast<char>(remainder % 10u) + '0');
			remainder /= 10u;
		}
	}

	os << decimalNumber;

	return os;
}

uint_t uint_t::operator+(uint_t number) const
{
	uint_t result{ *this };
	size_t decimalSize{ std::max(data.size(), number.data.size()) };

	if (decimalSize == 0) return uint_t("0");

	makeLengthEqual(result, number);

	uint64_t carry{ 0 };
	for (auto digit1{ result.data.begin() }, digit2{ number.data.begin() };
		digit1 != result.data.end() && digit2 != number.data.end(); digit1++, digit2++)
	{
		carry += static_cast<uint64_t>(*digit1) + static_cast<uint64_t>(*digit2);
		*digit1 = static_cast<uint32_t>(carry);
		carry = carry >> 32;
	}

	if (carry != 0) result.data.insert(result.data.begin(), static_cast<uint32_t>(carry));

	return result;
}

uint_t uint_t::operator-(uint_t number) const
{
	uint_t result{ *this };
	size_t decimalSize{ std::max(data.size(), number.data.size()) };

	if (decimalSize == 0 || decimalSize < number.data.size()) return uint_t("0");

	makeLengthEqual(result, number);

	// Perform subtraction using method of complements
	uint64_t carry{ 1 };
	for (auto digit1{ result.data.begin() }, digit2{ number.data.begin() };
		digit1 != result.data.end() && digit2 != number.data.end(); digit1++, digit2++)
	{
		carry += static_cast<uint64_t>(*digit1) - static_cast<uint64_t>(*digit2) + uint32Max;
		*digit1 = static_cast<uint32_t>(carry);
		carry = carry >> 32;
	}

	// Handles case of negative numbers
	if (carry != 1) return uint_t("0");

	removeZeros(result);

	return result;
}

uint_t uint_t::operator*(uint_t number) const
{
	uint_t result(data.size() + number.data.size());
	auto product{ result.data.begin() };

	// Performs long multiplication
	for (auto num1{ data.begin() }; num1 != data.end(); num1++)
	{
		uint64_t carry{};

		for (auto num2{ number.data.begin() }; num2 != number.data.end(); num2++, product++)
		{
			uint64_t value = static_cast<uint64_t>(*product);
			value += carry + static_cast<uint64_t>(*num1) * static_cast<uint64_t>(*num2);
			carry = value >> 32;
			*product = static_cast<uint32_t>(value);
		}

		*product = static_cast<uint32_t>(carry);
		product -= number.data.size() - 1;
	}

	removeZeros(result);

	return result;
}

uint_t uint_t::operator/(uint_t number) const
{
	uint_t number1{ *this };

	if (IS_ZERO(number)) throw std::runtime_error("division by zero");
	if (number.data.size() > number1.data.size()) return uint_t("0");

	uint_t result(number1.data.size());

	makeLengthEqual(number1, number);
	number.data.insert(number.data.begin(), number1.data.size(), 0u);
	number1.data.insert(number1.data.end(), number1.data.size(), 0u);

	for (size_t n{}; n < result.data.size() * 32; n++)
	{
		uint32_t carry{}, add{};
		for (auto it{ number.data.rbegin() }; it != number.data.rend(); it++)
		{
			carry = *it & 1;
			*it = (*it >> 1) + add;
			add = carry << 31;
		}

		if (number > number1)
		{
			add = 0;
		}
		else
		{
			add = 1;

			// Subtract divisor
			uint64_t sub_carry{ 1 };
			for (auto digit1{ number1.data.begin() }, digit2{ number.data.begin() };
				digit1 != number1.data.end() && digit2 != number.data.end(); digit1++, digit2++)
			{
				sub_carry += static_cast<uint64_t>(*digit1) - static_cast<uint64_t>(*digit2) + uint32Max;
				*digit1 = static_cast<uint32_t>(sub_carry);
				sub_carry = sub_carry >> 32;
			}
		}

		for (auto it{ result.data.begin() }; it != result.data.end(); it++)
		{
			carry = *it >> 31;
			*it = (*it << 1) + add;
			add = carry;
		}
	}

	removeZeros(result);

	return result;
}

bool uint_t::operator>=(uint_t number) const
{
	if (data.size() > number.data.size())
	{
		return true;
	}
	else if (data.size() < number.data.size())
	{
		return false;
	}
	else
	{
		for (auto value1{ data.crbegin() }, value2{ number.data.crbegin() };
			value1 != data.crend() && value2 != number.data.crend(); value1++, value2++)
		{
			if (*value1 < *value2) return false;
			else if (*value1 > *value2) return true;
		}

		return true;
	}
}

bool uint_t::operator>(uint_t number) const
{
	if (data.size() > number.data.size())
	{
		return true;
	}
	else if (data.size() < number.data.size())
	{
		return false;
	}
	else
	{
		for (auto value1{ data.crbegin() }, value2{ number.data.crbegin() };
			value1 != data.crend() && value2 != number.data.crend(); value1++, value2++)
		{
			if (*value1 < *value2) return false;
			else if (*value1 > *value2) return true;
		}

		return false;
	}
}