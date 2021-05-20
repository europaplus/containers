//
// Created by Knot Fonda on 5/2/21.
//

#include "test.hpp"
#include <functional>

static void constructors(void)
{
	std::list<int> firstS;                                // empty list of ints
	std::list<int> secondS (4,100);                       // four ints with value 100
	std::list<int> thirdS (secondS.begin(),secondS.end());  // iterating through second
	std::list<int> fourthS (thirdS);                       // a copy of third
	// the iterator constructor can also be used to construct from arrays:
	int myintsS[] = {16,2,77,29};
	std::list<int> fifthS (myintsS, myintsS + sizeof(myintsS) / sizeof(int) );

	std::list<int> firstF;                                // empty list of ints
	std::list<int> secondF (4,100);                       // four ints with value 100
	std::list<int> thirdF (secondF.begin(),secondF.end());  // iterating through second
	std::list<int> fourthF (thirdF);                       // a copy of third
	// the iterator constructor can also be used to construct from arrays:
	int myintsF[] = {16,2,77,29};
	std::list<int> fifthF (myintsF, myintsF + sizeof(myintsF) / sizeof(int) );
	expect(firstF == firstS, "constructors");
	expect(secondF == secondS, "constructors");
	expect(thirdF == thirdS, "constructors");
	expect(fourthF == fourthS, "constructors");
	expect(fifthF == fifthS, "constructors");
};

static void size(void)
{
	std::list<int> vecS;
	ft::list<int> vecF;
	expect(vecF == vecS, "size");
	for (int i = 0; i < 10; i++) vecS.push_back(i);
	for (int i = 0; i < 10; i++) vecF.push_back(i);
	expect(vecF == vecS, "size");
	vecS.insert(vecS.begin(), 10, 100);
	vecF.insert(vecF.begin(), 10, 100);
	expect(vecF == vecS, "size");
	vecS.pop_back();
	vecF.pop_back();
	expect(vecF == vecS, "size");
};

static void frontBack(void)
{
	//front
	{
		std::list<int> vecS;
		vecS.push_back(77);
		vecS.push_back(22);
		std::list<int> vecF;
		vecF.push_back(77);
		vecF.push_back(22);
		// now front equals 77, and back 22
		vecS.front() -= vecS.back();
		vecF.front() -= vecF.back();
		expect(vecF == vecS, "front");
	}
	//back
	{
		std::list<int> vecS;
		vecS.push_back(10);
		while (vecS.back() != 0)
			vecS.push_back ( vecS.back() -1 );
		std::list<int> vecF;
		vecF.push_back(10);
		while (vecF.back() != 0)
			vecF.push_back ( vecF.back() -1 );
		expect(vecF == vecS, "back");
	}
};

static void assign(void)
{
	std::list<int> firstS;
	std::list<int> secondS;
	ft::list<int> firstF;
	ft::list<int> secondF;
	firstS.assign (7,100);
	firstF.assign (7,100);                      // 7 ints with value 100
	expect(firstF == firstS, "assign");
	secondS.assign (firstS.begin(),firstS.end()); // a copy of first
	secondF.assign (firstF.begin(),firstF.end()); // a copy of first
	expect(secondF == secondS, "assign");
	int myintsS[]={1776,7,4};
	firstS.assign (myintsS,myintsS+3);
	int myintsF[]={1776,7,4};
	firstF.assign (myintsF,myintsF+3);            // assigning from array
	expect(firstF == firstS, "assign");
};

static void pushFrontBack(void)
{
	//push_front
	{
		std::list<int> vecS;         // two ints with a value of 100
		ft::list<int> vecF;         // two ints with a value of 100
		vecS.push_front(200);
		vecF.push_front(200);
		expect(vecF == vecS, "pushFront");
		vecS.push_front(300);
		vecF.push_front(300);
		expect(vecF == vecS, "pushFront");
	}
	//push_back
	{
		std::list<int> vecS;
		ft::list<int> vecF;
		for (int i = 1; i < 3; ++i)
		{
			vecF.push_back (i);
			vecS.push_back (i);
			expect(vecF == vecS, "pushBack");
		}
	}
};

static void swap(void)
{
	std::list<int> firstS (3,100);   // three ints with a value of 100
	std::list<int> secondS (5,200);  // five ints with a value of 200
	firstS.swap(secondS);
	ft::list<int> firstF (3,100);   // three ints with a value of 100
	ft::list<int> secondF (5,200);  // five ints with a value of 200
	firstF.swap(secondF);
	expect(firstF == firstS, "swap");
	expect(secondF == secondS, "swap");
};

static void splice(void)
{
	std::list<int> vecS1, vecS2;
	std::list<int>::iterator itS;
	// set some initial values:
	for (int i=1; i<=4; ++i)
		vecS1.push_back(i);      // vecS1: 1 2 3 4
	for (int i=1; i<=3; ++i)
		vecS2.push_back(i*10);   // vecS2: 10 20 30
	itS = vecS1.begin();
	++itS;                         // points to 2
	vecS1.splice (itS, vecS2); // vecS1: 1 10 20 30 2 3 4
	vecS2.splice (vecS2.begin(),vecS1, itS);
	itS = vecS1.begin();
	++itS;
	++itS;
	++itS;
	vecS1.splice ( vecS1.begin(), vecS1, itS, vecS1.end());
	ft::list<int> vecF1, vecF2;
	ft::list<int>::iterator itF;
	for (int i=1; i<=4; ++i)
		vecF1.push_back(i);
	for (int i=1; i<=3; ++i)
		vecF2.push_back(i*10);
	itF = vecF1.begin();
	++itF;
	vecF1.splice (itF, vecF2);
	vecF2.splice (vecF2.begin(),vecF1, itF);
	itF = vecF1.begin();
	++itF;
	++itF;
	++itF;
	vecF1.splice ( vecF1.begin(), vecF1, itF, vecF1.end());
	expect(vecF1 == vecS1, "splice");
	expect(vecF2 == vecS2, "splice");
};

static void remove(void)
{
	int myintsS[]= {17,89,7,14};
	std::list<int> vecS (myintsS,myintsS+4);
	vecS.remove(89);
	int myintsF[]= {17,89,7,14};
	ft::list<int> vecF (myintsF,myintsF+4);
	vecF.remove(89);
	expect(vecF == vecS, "remove");
};

static void erase(void)
{
	std::list<int> vecS;
	std::list<int>::iterator it1S,it2S;
	std::list<int> vecF;
	std::list<int>::iterator it1F,it2F;
	for (int i=1; i<10; ++i) vecS.push_back(i*10);
	it1S = it2S = vecS.begin(); // ^^
	advance (it2S,6);            // ^                 ^
	++it1S;
	for (int i=1; i<10; ++i) vecF.push_back(i*10);
	it1F = it2F = vecF.begin(); // ^^
	advance (it2F,6);            // ^                 ^
	++it1F;                      //    ^              ^
	it1S = vecS.erase (it1S);   // 10 30 40 50 60 70 80 90
	it1F = vecF.erase (it1F);   // 10 30 40 50 60 70 80 90
	expect(vecF == vecS, "erase");
	it2S = vecS.erase (it2S);   // 10 30 40 50 60 80 90
	it2F = vecF.erase (it2F);   // 10 30 40 50 60 80 90
	expect(vecF == vecS, "erase");
	++it1S;                      //       ^        ^
	--it2S;
	++it1F;                      //       ^        ^
	--it2F;                      //       ^     ^
	vecS.erase (it1S,it2S);     // 10 30 60 80 90
	vecF.erase (it1F,it2F);     // 10 30 60 80 90
	expect(vecF == vecS, "erase");
};

static void unique(void)
{
	double mydoublesS[]={ 12.15,  2.72, 73.0,  12.77,  3.14,
						 12.77, 73.35, 72.25, 15.3,  72.25 };
	std::list<double> vecS (mydoublesS,mydoublesS+10);
	vecS.sort();             //  2.72,  3.14, 12.15, 12.77, 12.77,
	double mydoublesF[]={ 12.15,  2.72, 73.0,  12.77,  3.14,
						 12.77, 73.35, 72.25, 15.3,  72.25 };
	ft::list<double> vecF (mydoublesF,mydoublesF+10);
	vecF.sort();             //  2.72,  3.14, 12.15, 12.77, 12.77,
	// 15.3,  72.25, 72.25, 73.0,  73.35
	vecS.unique();           //  2.72,  3.14, 12.15, 12.77
	vecF.unique();           //  2.72,  3.14, 12.15, 12.77
	// 15.3,  72.25, 73.0,  73.35
	expect(vecF == vecS, "unique");
	vecS.unique (same_integral_part);  //  2.72,  3.14, 12.15
	vecF.unique (same_integral_part);  //  2.72,  3.14, 12.15
	// 15.3,  72.25, 73.0
	expect(vecF == vecS, "unique");
	// 15.3,  72.25, 73.0
	vecS.unique (is_near());
	vecF.unique (is_near());
	expect(vecF == vecS, "unique");
};

bool mycomparison (double first, double second)
{ return ( int(first)<int(second) ); }

static void merge(void)
{
	std::list<double> firstS, secondS;
	ft::list<double> firstF, secondF;

	firstS.push_back (3.1);
	firstS.push_back (2.2);
	firstS.push_back (2.9);

	firstF.push_back (3.1);
	firstF.push_back (2.2);
	firstF.push_back (2.9);

	secondS.push_back (3.7);
	secondS.push_back (7.1);
	secondS.push_back (1.4);

	secondF.push_back (3.7);
	secondF.push_back (7.1);
	secondF.push_back (1.4);

	firstS.sort();
	secondS.sort();

	firstF.sort();
	secondF.sort();

	firstS.merge(secondS);
	firstF.merge(secondF);
	expect(firstF == firstS, "merge");
	// (secondS is now empty)
	secondS.push_back (2.1);
	secondF.push_back (2.1);
	firstS.merge(secondS,mycomparison);
	firstF.merge(secondF,mycomparison);
	expect(firstF == firstS, "merge");
};

static void reverse(void)
{
	std::list<int> vecS;
	for (int i=1; i<10; ++i) vecS.push_back(i);
	vecS.reverse();
	ft::list<int> vecF;
	for (int i=1; i<10; ++i) vecF.push_back(i);
	vecF.reverse();
	expect(vecF == vecS, "reverse");
};

static void operators(void)
{
	std::list<int> aS;
	aS.push_back(10);
	aS.push_back(20);
	aS.push_back(30);
	std::list<int> bS;
	bS.push_back(10);
	bS.push_back(20);
	bS.push_back(30);
	std::list<int> cS;
	cS.push_back(30);
	cS.push_back(20);
	cS.push_back(10);

	ft::list<int> aF;
	aF.push_back(10);
	aF.push_back(20);
	aF.push_back(30);
	ft::list<int> bF;
	bF.push_back(10);
	bF.push_back(20);
	bF.push_back(30);
	ft::list<int> cF;
	cF.push_back(30);
	cF.push_back(20);
	cF.push_back(10);

	expect("operators", aF == bF, aF == bS);
	expect("operators", bF != cF, bS != cS);
	expect("operators", bF < cF, bS < cS);
	expect("operators", cF > bF, cS > bS);
	expect("operators", aF <= bF, aS <= bS);
	expect("operators", aF >= bF, aS >= bS);
};

void	testList(void)
{
	printHeader("List");
	constructors();
	size();
	frontBack();
	assign();
	pushFrontBack();
	swap();
	splice();
	remove();
	erase();
	unique();
	merge();
	reverse();
	operators();
	printHeader("END");
};