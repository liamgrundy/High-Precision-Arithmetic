#pragma once

#include <algorithm>
#include <exception>
#include <execution>
#include <iostream>
#include <string>

// ---------- Type Conversion Functions ----------

std::string convertToBinary(std::string decimalNumber);
std::string convertToDecimal(std::string binaryNumber);

// ---------- Binary Arithmetic Functions ----------

std::string addBinary(std::string binaryNumber1, std::string binaryNumber2);

// ---------- Decimal Arithmetic Functions ----------

namespace decimal
{
	std::string add(std::string decimalNumber1, std::string decimalNumber2);
	std::string subtract(std::string decimalNumber1, std::string decimalNumber2);
	std::string multiply(std::string decimalNumber1, std::string decimalNumber2);
	std::string divide(std::string decimalNumber1, std::string decimalNumber2);
}