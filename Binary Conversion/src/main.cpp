#include <future>
#include <iostream>
#include <string>

#include "arithmetic.h"
#include "binary.h"

void run()
{
	uint_t num1{ "2023" };
	uint_t num2{ "2023" };

	std::cout << num1 << " + " << num2 << " = " << num1 + num2 << std::endl;
	std::cout << num1 << " - " << num2 << " = " << num1 - num2 << std::endl;
	std::cout << num1 << " * " << num2 << " = " << num1 * num2 << std::endl;
	std::cout << num1 << " / " << num2 << " = " << num1 / num2 << std::endl;

	std::cout << "(" << num1 << " - " << num2 << ") * " << num2 << " = " << (num1 - num2) * num2 << std::endl;
}

int main()
{
	try
	{
		run();
	}
	catch (std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}

	return 0;
}