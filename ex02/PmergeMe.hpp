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
		// Sorts a copy of "input" using std::vector<int> and returns it.
		static std::vector<int> sortVector(std::vector<int> input);

		// Sorts a copy of "input" using std::deque<int> and returns it.
		static std::deque<int> sortDeque(std::deque<int> input);

		// Shared helper: builds the Jacobsthal-based insertion order used
		// by both implementations. It only produces plain indices, it does
		// not sort or store the actual sequence, so it does not count as
		// one of the "containers" used to hold/sort the integer sequence.
		static std::vector<std::size_t> jacobsthalInsertOrder(std::size_t pairCount);

	private:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();
};

#endif
