//
// Created by Knot Fonda on 5/2/21.
//

#include "test.hpp"

static void constructor()
{
	std::stack<int,std::vector<int> > firstS;  // empty stack using vector
	std::stack<int,std::vector<int> > secondS (firstS);

	ft::stack<int,ft::vector<int> > firstF;  // empty stack using vector
	ft::stack<int,ft::vector<int> > secondF (firstF);

	expect(firstF.size() == firstS.size(), "constructor");
	expect(secondF.size() == secondS.size(), "constructor");
};

static void test()
{
	ft::stack<int> stackS;
	std::stack<int> stackF;
	stackS.push(5);
	stackS.push(4);
	stackF.push(5);
	stackF.push(5);
	expect(stackF.size() == stackS.size(), "size");
	expect(stackF.empty() == stackS.empty(), "empty");
	stackS.pop();
	stackF.pop();
	expect(stackF.size() == stackS.size(), "pop");
	expect(stackF.top() == stackS.top(), "top");
};


void testStack(void)
{
	printHeader("Stack");
	constructor();
	test();
	printHeader("END");
};