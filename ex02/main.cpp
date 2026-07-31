#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <climits>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <sys/time.h>

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

static double nowMicroseconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return static_cast<double>(tv.tv_sec) * 1000000.0 + static_cast<double>(tv.tv_usec);
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



	std::vector<int> uniquenessCheck = numbers;
	std::sort(uniquenessCheck.begin(), uniquenessCheck.end());
	if (std::adjacent_find(uniquenessCheck.begin(), uniquenessCheck.end()) != uniquenessCheck.end())
	{
		std::cerr << "Error: duplicate values are not allowed" << std::endl;
		return 1;
	}

	printContainer("Before: ", numbers);

	std::deque<int> numbersDeque(numbers.begin(), numbers.end());

	double vecStart = nowMicroseconds();
	std::vector<int> vecCopy(numbers.begin(), numbers.end());
	std::vector<int> sortedVector = PmergeMe::sortVector(vecCopy);
	double vecEnd = nowMicroseconds();

	double deqStart = nowMicroseconds();
	std::deque<int> deqCopy(numbersDeque.begin(), numbersDeque.end());
	std::deque<int> sortedDeque = PmergeMe::sortDeque(deqCopy);
	double deqEnd = nowMicroseconds();

	printContainer("After: ", sortedVector);

	double vecMicro = vecEnd - vecStart;
	double deqMicro = deqEnd - deqStart;

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << numbers.size()
		<< " elements with std::vector : " << vecMicro << " us" << std::endl;
	std::cout << "Time to process a range of " << numbers.size()
		<< " elements with std::deque : " << deqMicro << " us" << std::endl;

	return 0;
}
