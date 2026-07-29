/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 16:01:34 by julian            #+#    #+#             */
/*   Updated: 2026/07/29 16:01:34 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <algorithm>
#include <unordered_map>
#include <utility>

std::vector<std::size_t> PmergeMe::jacobsthalInsertOrder(std::size_t pairCount)
{
	std::vector<std::size_t> order;

	if (pairCount < 2)
		return order;

	std::vector<std::size_t> jacobsthal;
	jacobsthal.push_back(0);
	jacobsthal.push_back(1);
	while (jacobsthal.back() < pairCount)
	{
		std::size_t n = jacobsthal.size();
		std::size_t next = jacobsthal[n - 1] + 2 * jacobsthal[n - 2];
		jacobsthal.push_back(next);
	}

	std::size_t previous = 1;
	for (std::size_t idx = 2; idx < jacobsthal.size(); ++idx)
	{
		std::size_t current = std::min(jacobsthal[idx], pairCount);
		if (previous >= pairCount)
			break;
		std::size_t low = previous + 1;
		std::size_t high = current;
		if (low <= high)
		{
			std::size_t v = high;
			while (true)
			{
				order.push_back(v);
				if (v == low)
					break;
				--v;
			}
		}
		previous = current;
	}
	return order;
}

/*
** ==========================================================================
** VECTOR IMPLEMENTATION
** ==========================================================================
*/
namespace vecimpl
{
	static void binaryInsert(std::vector<int> &chain, int value, std::size_t upperBoundIdx)
	{
		std::vector<int>::iterator begin = chain.begin();
		std::vector<int>::iterator boundIt = begin + static_cast<long>(upperBoundIdx);
		std::vector<int>::iterator pos = std::lower_bound(begin, boundIt, value);
		chain.insert(pos, value);
	}

	static std::vector<int> fordJohnson(std::vector<int> input)
	{
		std::size_t n = input.size();
		if (n <= 1)
			return input;

		bool hasStray = (n % 2 != 0);
		int stray = 0;
		std::size_t pairCount = n / 2;

		std::vector<int> bigs;
		std::vector<int> smallOf; 
		bigs.reserve(pairCount);
		smallOf.reserve(pairCount);

		for (std::size_t i = 0; i < pairCount; ++i)
		{
			int a = input[2 * i];
			int b = input[2 * i + 1];
			if (a < b)
				std::swap(a, b);
			bigs.push_back(a);
			smallOf.push_back(b);
		}
		if (hasStray)
			stray = input[n - 1];

		std::unordered_map<int, std::size_t> pairIndexOf;
		pairIndexOf.reserve(pairCount * 2);
		for (std::size_t i = 0; i < pairCount; ++i)
			pairIndexOf[bigs[i]] = i;
		std::vector<int> sortedBigs = fordJohnson(bigs);
		std::vector<int> chain = sortedBigs;
		if (pairCount >= 1)
		{
			int firstSmall = smallOf[pairIndexOf[chain[0]]];
			chain.insert(chain.begin(), firstSmall);
		}
		std::vector<std::size_t> order = PmergeMe::jacobsthalInsertOrder(pairCount);
		for (std::size_t i = 0; i < order.size(); ++i)
		{
			std::size_t rank = order[i];
			int bigValue = sortedBigs[rank - 1];
			int smallValue = smallOf[pairIndexOf[bigValue]];

			std::vector<int>::iterator itBig = std::lower_bound(chain.begin(), chain.end(), bigValue);
			std::size_t boundIdx = static_cast<std::size_t>(itBig - chain.begin());
			binaryInsert(chain, smallValue, boundIdx);
		}

		if (hasStray)
		{
			std::vector<int>::iterator pos = std::lower_bound(chain.begin(), chain.end(), stray);
			chain.insert(pos, stray);
		}

		return chain;
	}
}

/*
** ==========================================================================
** DEQUE IMPLEMENTATION (independent from the vector one on purpose)
** ==========================================================================
*/
namespace dequeimpl
{
	static void binaryInsert(std::deque<int> &chain, int value, std::size_t upperBoundIdx)
	{
		std::deque<int>::iterator begin = chain.begin();
		std::deque<int>::iterator boundIt = begin + static_cast<long>(upperBoundIdx);
		std::deque<int>::iterator pos = std::lower_bound(begin, boundIt, value);
		chain.insert(pos, value);
	}

	static std::deque<int> fordJohnson(std::deque<int> input)
	{
		std::size_t n = input.size();
		if (n <= 1)
			return input;

		bool hasStray = (n % 2 != 0);
		int stray = 0;
		std::size_t pairCount = n / 2;

		std::deque<int> bigs;
		std::deque<int> smallOf;

		for (std::size_t i = 0; i < pairCount; ++i)
		{
			int a = input[2 * i];
			int b = input[2 * i + 1];
			if (a < b)
				std::swap(a, b);
			bigs.push_back(a);
			smallOf.push_back(b);
		}
		if (hasStray)
			stray = input[n - 1];

		std::unordered_map<int, std::size_t> pairIndexOf;
		pairIndexOf.reserve(pairCount * 2);
		for (std::size_t i = 0; i < pairCount; ++i)
			pairIndexOf[bigs[i]] = i;

		std::deque<int> sortedBigs = fordJohnson(bigs);
		std::deque<int> chain = sortedBigs;

		if (pairCount >= 1)
		{
			int firstSmall = smallOf[pairIndexOf[chain[0]]];
			chain.insert(chain.begin(), firstSmall);
		}

		std::vector<std::size_t> order = PmergeMe::jacobsthalInsertOrder(pairCount);
		for (std::size_t i = 0; i < order.size(); ++i)
		{
			std::size_t rank = order[i];
			int bigValue = sortedBigs[rank - 1];
			int smallValue = smallOf[pairIndexOf[bigValue]];

			std::deque<int>::iterator itBig = std::lower_bound(chain.begin(), chain.end(), bigValue);
			std::size_t boundIdx = static_cast<std::size_t>(itBig - chain.begin());
			binaryInsert(chain, smallValue, boundIdx);
		}

		if (hasStray)
		{
			std::deque<int>::iterator pos = std::lower_bound(chain.begin(), chain.end(), stray);
			chain.insert(pos, stray);
		}

		return chain;
	}
}

std::vector<int> PmergeMe::sortVector(std::vector<int> input)
{
	return vecimpl::fordJohnson(input);
}

std::deque<int> PmergeMe::sortDeque(std::deque<int> input)
{
	return dequeimpl::fordJohnson(input);
}
