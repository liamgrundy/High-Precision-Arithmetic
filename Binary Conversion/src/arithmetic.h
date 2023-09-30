#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <iostream>

class uint_t
{
public:
	uint_t(std::string decimalNumber);
	uint_t(const uint_t& number);

	uint_t operator+(uint_t number) const;
	uint_t operator-(uint_t number) const;
	uint_t operator*(uint_t number) const;
	uint_t operator/(uint_t number) const;

	bool operator>=(uint_t number) const;
	bool operator>(uint_t number) const;

private:
	uint_t(size_t size, uint32_t digit=0u);

private:
	std::vector<uint32_t> data;

	friend std::ostream& operator<<(std::ostream& os, const uint_t& obj);
	
	friend inline void removeZeros(uint_t& number);
	friend inline void makeLengthEqual(uint_t& number1, uint_t& number2);
};