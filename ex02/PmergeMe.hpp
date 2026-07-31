#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <cstddef>

/*
** PmergeMe implements the Ford-Johnson (merge-insert) sort algorithm.
**
** Two completely independent implementations are provided on purpose,
** one working on std::vector<int> and one working on std::deque<int>,
** so that two different containers are genuinely exercised instead of
** relying on a single generic/templated routine.
*/
class PmergeMe
{
	public:
		static std::vector<int> sortVector(std::vector<int> input);

		static std::deque<int> sortDeque(std::deque<int> input);

		static std::vector<std::size_t> jacobsthalInsertOrder(std::size_t pairCount);

	private:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();
};

#endif
