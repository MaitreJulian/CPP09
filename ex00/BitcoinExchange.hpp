#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <exception>
# include <string>

class BitcoinExchange
{
	private:
	
	public:
		BitcoinExchange();
		BitcoinExchange(BitcoinExchange const &other);
		BitcoinExchange &operator=(BitcoinExchange const &other);
		~BitcoinExchange();

		std::map<std::string, double> const &getQuotes() const;
		void check_DB();
		void execute(char const *fileName);
};

#endif