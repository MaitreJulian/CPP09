#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <cctype>
#include <cstdlib>

static bool is_number(const std::string &token)
{
    return (token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0])));
}

static bool is_operator(const std::string &token)
{
    return (token.size() == 1 &&
            (token[0] == '+' || token[0] == '-' ||
             token[0] == '*' || token[0] == '/'));
}

static bool apply_operator(std::stack<long> &s, char op)
{
    if (s.size() < 2)
        return (false); /* not enough operands */

    long b = s.top(); s.pop();
    long a = s.top(); s.pop();
    long res = 0;

    if (op == '+')
        res = a + b;
    else if (op == '-')
        res = a - b;
    else if (op == '*')
        res = a * b;
    else /* '/' */
    {
        if (b == 0)
            return (false);
        res = a / b;
    }
    s.push(res);
    return (true);
}

int main(int argc, char **argv)
{
    if (argc != 2 || argv[1][0] == '\0')
    {
        std::cerr << "Error" << std::endl;
        return (EXIT_FAILURE);
    }

    std::stack<long> s;
    std::istringstream iss(argv[1]);
    std::string token;
    bool ok = true;

    while (ok && (iss >> token))
    {
        if (is_number(token))
            s.push(token[0] - '0');
        else if (is_operator(token))
        {
            if (!apply_operator(s, token[0]))
                ok = false;
        }
        else
            ok = false;
    }

    if (ok && s.size() == 1)
    {
        std::cout << s.top() << std::endl;
        return (EXIT_SUCCESS);
    }

    std::cerr << "Error" << std::endl;
    return (EXIT_FAILURE);
}
