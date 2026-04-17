/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvenkata <jvenkata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 20:41:03 by jvenkata          #+#    #+#             */
/*   Updated: 2026/04/16 21:09:11 by jvenkata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "BitcoinExchange.hpp"


BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(BitcoinExchange const &other)
{
	if (this != &other)
		*this = other;
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &other)
{
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::check_DB()
{
    std::ifstream file("data.csv");
    if (!file.is_open())
        std::cerr << "Fail to open data.csv" << std::endl;

    std::string line;
    std::getline(file, line);
    if (line != "date,exchange_rate")
            std::cerr << "First line has to be 'date,exchange_rate'." << std::endl;
    std::stringstream ss(line);
}