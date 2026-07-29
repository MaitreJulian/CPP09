/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 20:41:03 by jvenkata          #+#    #+#             */
/*   Updated: 2026/07/29 15:57:13 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
	_readDatabase();
}

BitcoinExchange::BitcoinExchange(BitcoinExchange const &other)
{
	if (this != &other)
		*this = other;
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &other)
{
	if (this != &other)
		_DB = other._DB;
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

std::map<std::string, double> const &BitcoinExchange::getQuotes() const
{
	return (_DB);
}

void BitcoinExchange::_readDatabase()
{
       std::ifstream file("data.csv");
        if (!file.is_open())
            throw CouldNotOpenFile();

        std::string line;

        std::getline(file, line);
        if (line != "date,exchange_rate")
            throw InvalidColumnFormat();

        while (std::getline(file, line))
		{
            std::string date, price;
            std::istringstream ss(line);
            std::getline(ss, date, ',');
            std::getline(ss, price, ',');

            double priceValue;
            std::istringstream priceStream(price);
            if (!(priceStream >> priceValue))
				throw InvalidPriceFormat();
            _DB[date] = priceValue;
        }

        file.close();
}

void BitcoinExchange::execute(char const *fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
		throw CouldNotOpenFile();

	std::string line;
	std::getline(file, line);
	
	if (line != "date | value")
		throw InvalidColumnFormat();

	while (std::getline(file, line))
	{
		std::string date;
		std::string valueStr;
		double priceValue;
		std::istringstream ss(line);
		std::getline(ss, date, '|');
		std::getline(ss, valueStr, '|');

		if (!date.empty())
			date = date.erase(date.length() -1);
		if (_validateDate(date) == false)
		{
			std::cout << "Error: bad input => " << date << std::endl;
			continue;
		}

		if(!valueStr.empty())
			valueStr = valueStr.erase(0, 1);
		priceValue = _validatePrice(valueStr);
		if (priceValue != -1)
			_multiplyWithQuote(date, priceValue);
		
	}

	file.close();
}

bool BitcoinExchange::_validateDate(std::string const &date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (isdigit(date[i]) == 0)
			return (false);
	}

	if (date[5] == '0' && date[6] == '0') 
		return (false);
	if ((date[5] == '1' && date[6] > '2') || date[5] > '1')
		return (false);

	if (date[8] == '0' && date[9] == '0')
		return (false);
	if ((date[8] == '3' && date[9] > '1') || date[8] > '3')
		return (false);
	
	return (true);
}

double BitcoinExchange::_validatePrice(std::string const &valueStr)
{
	double priceValue;
	std::istringstream priceStream(valueStr);

	if (!(priceStream >> priceValue))
	{
		std::cout << "Error: bad value input => " << priceValue << std::endl;
		return (-1);
	}

	if (priceValue < 0)
	{
		std::cout << "Error: not a positive number => " << priceValue << std::endl;
		return (-1);
	}
	else if (priceValue > 1000)
	{
		std::cout << "Error: too large number => " << priceValue << std::endl;
		return (-1);
	}
	
	return (priceValue);
}


void BitcoinExchange::_multiplyWithQuote(std::string const &date, double price)
{
	std::map<std::string, double>::iterator it = _DB.find(date);
	if (it != _DB.end())
		std::cout << date << " | " << price << " | " << price * it->second << std::endl;
	else
	{
		std::map<std::string, double>::iterator it2 = _DB.lower_bound(date);
		if (it2 == _DB.begin())
			std::cout << date << " | " << price << " | " << price * it2->second << std::endl;
		else
		{
			it2--;
			std::cout << date << " | " << price << " | " << price * it2->second << std::endl;
		}
	}
}
