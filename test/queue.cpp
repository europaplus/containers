#include "test.hpp"

static void constructor()
{
	std::queue<int,std::list<int> > firstS;  // empty stack using vector
	std::queue<int,std::list<int> > secondS (firstS);

	ft::queue<int,ft::list<int> > firstF;  // empty stack using vector
	ft::queue<int,ft::list<int> > secondF (firstF);

	expect(firstF.size() == firstS.size(), "constructor");
	expect(secondF.size() == secondS.size(), "constructor");
};

static void test()
{
	ft::stack<int> queueS;
	std::stack<int> queueF;
	queueS.push(5);
	queueS.push(4);
	queueF.push(5);
	queueF.push(5);
	expect(queueF.size() == queueS.size(), "size");
	expect(queueF.empty() == queueS.empty(), "empty");
	queueS.pop();
	queueF.pop();
	expect(queueF.size() == queueS.size(), "pop");
	expect(queueF.top() == queueS.top(), "top");
};

void testQueue(void)
{
	printHeader("Queue");
	constructor();
	test();
	printHeader("END");
};