/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 16:01:37 by julian            #+#    #+#             */
/*   Updated: 2026/07/29 16:01:37 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <cstddef>

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
