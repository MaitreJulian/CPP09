#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <climits>
#include <cctype>
#include <chrono>
#include <iomanip>
#include <algorithm>

/*
** Parses one command line token into a positive int.
** Returns true on success and stores the value in "out".
** Rejects: empty tokens, non-digit characters (except a leading '+'),
** values <= 0, and values that overflow an int.
*/
static bool parsePositiveInt(const std::string &token, int &out)
{
	if (token.empty())
		return false;

	std::size_t i = 0;
	if (token[0] == '+')
		i = 1;
	if (i == token.size())
		return false;

	for (std::size_t j = i; j < token.size(); ++j)
	{
		if (!std::isdigit(static_cast<unsigned char>(token[j])))
			return false;
	}

	// Parse manually to detect overflow reliably instead of relying on
	// exceptions from std::stoi/std::stol.
	long value = 0;
	for (std::size_t j = i; j < token.size(); ++j)
	{
		value = value * 10 + (token[j] - '0');
		if (value > INT_MAX)
			return false;
	}

	if (value <= 0)
		return false;

	out = static_cast<int>(value);
	return true;
}

template <typename Container>
static void printContainer(const std::string &label, const Container &c)
{
	std::cout << label;
	for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}

	std::vector<int> numbers;
	numbers.reserve(static_cast<std::size_t>(argc - 1));

	for (int i = 1; i < argc; ++i)
	{
		int value;
		if (!parsePositiveInt(argv[i], value))
		{
			std::cerr << "Error: invalid or non positive integer sequence" << std::endl;
			return 1;
		}
		numbers.push_back(value);
	}

	// Duplicate handling: rejected as invalid input (see subject: "left to
	// your discretion").
	std::vector<int> uniquenessCheck = numbers;
	std::sort(uniquenessCheck.begin(), uniquenessCheck.end());
	if (std::adjacent_find(uniquenessCheck.begin(), uniquenessCheck.end()) != uniquenessCheck.end())
	{
		std::cerr << "Error: duplicate values are not allowed" << std::endl;
		return 1;
	}

	printContainer("Before: ", numbers);

	std::deque<int> numbersDeque(numbers.begin(), numbers.end());

	std::chrono::high_resolution_clock::time_point vecStart = std::chrono::high_resolution_clock::now();
	std::vector<int> vecCopy(numbers.begin(), numbers.end());
	std::vector<int> sortedVector = PmergeMe::sortVector(vecCopy);
	std::chrono::high_resolution_clock::time_point vecEnd = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point deqStart = std::chrono::high_resolution_clock::now();
	std::deque<int> deqCopy(numbersDeque.begin(), numbersDeque.end());
	std::deque<int> sortedDeque = PmergeMe::sortDeque(deqCopy);
	std::chrono::high_resolution_clock::time_point deqEnd = std::chrono::high_resolution_clock::now();

	printContainer("After: ", sortedVector);

	double vecMicro = std::chrono::duration<double, std::micro>(vecEnd - vecStart).count();
	double deqMicro = std::chrono::duration<double, std::micro>(deqEnd - deqStart).count();

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << numbers.size()
		<< " elements with std::vector : " << vecMicro << " us" << std::endl;
	std::cout << "Time to process a range of " << numbers.size()
		<< " elements with std::deque : " << deqMicro << " us" << std::endl;

	return 0;
}
