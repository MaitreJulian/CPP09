#include "BitcoinExchange.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: ./bitcoin [file]" << std::endl;
		return (1);
	}

	try
	{
		BitcoinExchange exchange;
		exchange.execute(argv[1]);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}