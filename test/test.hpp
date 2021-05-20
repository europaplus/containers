//
// Created by Knot Fonda on 5/3/21.
//

#ifndef FT_CONTAINERS_TEST_HPP
#define FT_CONTAINERS_TEST_HPP
# include <vector>
# include <list>
# include <map>
# include <stack>
# include <queue>
# include <iostream>
# include <iterator>

# include "../srcs/list.hpp"
# include "../srcs/vector.hpp"
# include "../srcs/map.hpp"
# include "../srcs/stack.hpp"
# include "../srcs/queue.hpp"

# define RESET "\e[0m"
# define GREEN "\e[92m"
# define RED "\e[91m"
# define BLUE "\e[94m"

# define OK "✓"
# define KO "x"

void	testVector(void);
void	testList(void);
void	testMap(void);
void	testStack(void);
void	testQueue(void);

template <class T>
bool operator==(ft::vector<T> &vecF, std::vector<T> &vecS)
{
	if (vecF.empty() != vecS.empty())
		return (false);
	if (vecF.size() != vecS.size())
		return (false);
	typename ft::vector<T>::iterator itF = vecF.begin();
	typename std::vector<T>::iterator itS = vecS.begin();
	for (; itS != vecS.end(); ++itS, ++itF)
	{
		if (*itS != *itF)
			return (false);
	}
	return (true);
};

//template <class T, class U, class Comp = std::less<T> >
template <class T, class U>
bool operator==(ft::map<T,U> &vecF, std::map<T,U> &vecS)
{
	if (vecF.empty() != vecS.empty())
		return (false);
	if (vecF.size() != vecS.size())
		return (false);
	typename ft::map<T,U>::iterator itF = vecF.begin();
	typename std::map<T,U>::iterator itS = vecS.begin();
	for (; itS != vecS.end(); ++itS, ++itF)
	{
		if (itS->first != itF->first || itS->second != itF->second)
			return (false);
	}
	return (true);
};

template <class T>
bool operator==(ft::list<T> &vecF, std::list<T> &vecS)
{
	if (vecF.empty() != vecS.empty())
		return (false);
	if (vecF.size() != vecS.size())
		return (false);
	typename ft::list<T>::iterator itF = vecF.begin();
	typename std::list<T>::iterator itS = vecS.begin();
	for (; itS != vecS.end(); ++itS, ++itF)
	{
		if (*itS != *itF)
			return (false);
	}
	return (true);
};

inline void expect(std::string name, bool a, bool b)
{
	if (a == b)
		std::cout << name << std::string(40 - name.length() - 1, ' ') << GREEN << OK << RESET << std::endl;
	else
		std::cout << name << std::string(40 - name.length() - 1, ' ') << RED << KO << RESET << std::endl;
};

inline void printHeader(std::string str)
{
	unsigned long width = (40 - str.length())/2;
	std::cout << BLUE;
	std::cout << std::string(40, '*') << std::endl;
	std::cout << '*' << std::string(width, ' ') << str << std::string(width - 2, ' ') << '*' << std::endl;
	std::cout << std::string(40, '*') << std::endl;
	std::cout << RESET;
}

inline void expect(bool b, std::string name)
{
	if (b)
		std::cout << name << std::string(40 - name.length() - 1, ' ') << GREEN << OK << RESET << std::endl;
	else
		std::cout << name << std::string(40 - name.length() - 1, ' ') << RED << KO << RESET << std::endl;
};

inline bool same_integral_part (double first, double second)
{ return ( int(first)==int(second) ); }

// a binary predicate implemented as a class:
struct is_near {
	bool operator() (double first, double second)
	{ return (first-second<5.0); }
};

#endif //FT_CONTAINERS_TEST_HPP
