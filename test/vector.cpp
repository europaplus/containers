//
// Created by Knot Fonda on 5/3/21.
//

#include "test.hpp"

static void constructor()
{
	std::vector<int> vecS;
	ft::vector<int> vecF;
	expect(vecF == vecS, "constructor");
	for (int i = 1; i <= 10; ++i)
		vecS.push_back(i);
	for (int i = 1; i <= 10; ++i)
		vecF.push_back(i);
	expect(vecF == vecS, "constructor");
};

static void copyConstructor()
{
	ft::vector<int> vecFF;
	ft::vector<int> vecF;
	vecF = vecFF;
	expect(vecF == vecFF, "copy_constructor");
	for (int i = 1; i <= 10; ++i)
		vecF.push_back(i);
	vecFF = vecF;
	expect(vecF == vecFF, "copy_constructor");
};

static void size()
{
	std::vector<int> vecS;
	ft::vector<int> vecF;
	expect(vecF == vecS, "size()");
	for (int i=0; i<10; i++) vecS.push_back(i);
	for (int i=0; i<10; i++) vecF.push_back(i);
	expect(vecF == vecS, "size()");
	vecS.insert (vecS.end(),10,100);
	vecF.insert (vecF.end(),10,100);
	expect(vecF == vecS, "size()");
	vecS.pop_back();
	vecF.pop_back();
	expect(vecF == vecS, "size()");
};

static void resize()
{
	std::vector<int> vecS;
	ft::vector<int> vecF;
	for (int i=1;i<10;i++) vecS.push_back(i);
	for (int i=1;i<10;i++) vecF.push_back(i);
	vecS.resize(5);
	vecF.resize(5);
	expect(vecF == vecS, "resize()");
	vecS.resize(8,100);
	vecF.resize(8,100);
	expect(vecF == vecS, "resize()");
	vecS.resize(12);
	vecF.resize(12);
	expect(vecF == vecS, "resize()");
};

static void accessOperator()
{
	std::vector<unsigned> vecS (10);
	ft::vector<unsigned> vecF (10);
	expect(vecF == vecS, "accessOperator()");
	ft::vector<int>::size_type szF = vecF.size();
	for (unsigned i=0; i<szF; i++) vecF[i] = i;
	std::vector<int>::size_type szS = vecS.size();
	for (unsigned i=0; i<szS; i++) vecS[i] = i;
	expect(vecF == vecS, "accessOperator()");
	for (unsigned i=0; i<szS/2; i++)
	{
		unsigned temp;
		temp = vecS[szS-1-i];
		vecS[szS-1-i]=vecS[i];
		vecS[i]=temp;
	}
	for (unsigned i=0; i<szF/2; i++)
	{
		unsigned temp;
		temp = vecF[szF-1-i];
		vecF[szF-1-i]=vecF[i];
		vecF[i]=temp;
	}
	expect(vecF == vecS, "accessOperator()");
};

static void frontBack()
{
	//front
	{
		std::vector<int> vecS;
		ft::vector<int> vecF;
		vecS.push_back(78);
		vecF.push_back(78);
		vecS.push_back(16);
		vecF.push_back(16);
		vecS.front() -= vecS.back();
		vecF.front() -= vecS.back();
		expect(vecF.front() == vecS.front(), "front()");
	}

	//back
	{
		std::vector<int> vecS;
		ft::vector<int> vecF;
		vecS.push_back(10);
		vecF.push_back(10);
		while (vecS.back() != 0)
			vecS.push_back ( vecS.back() - 1 );
		while (vecF.back() != 0)
			vecF.push_back ( vecF.back() - 1 );
		expect(vecF.back() == vecS.back(), "back()");
	}
};

static void assign()
{
	std::vector<int> firstS;
	std::vector<int> secondS;
	std::vector<int> thirdS;
	ft::vector<int> firstF;
	ft::vector<int> secondF;
	ft::vector<int> thirdF;

	firstS.assign (7,100);             // 7 ints with a value of 100
	std::vector<int>::iterator itS;
	itS=firstS.begin() + 1;
	secondS.assign (itS,firstS.end()-1); // the 5 central values of first
	int myintsS[] = {1776,7,4};
	thirdS.assign (myintsS,myintsS+3);   // assigning from array

	firstF.assign (7,100);             // 7 ints with a value of 100
	ft::vector<int>::iterator itF;
	itF = ++firstF.begin();
	secondF.assign (itF,firstF.end()-1); // the 5 central values of first
	int myintsF[] = {1776,7,4};
	thirdF.assign (myintsF,myintsF+3);   // assigning from array.
	expect(firstF == firstS, "assign()");
	expect(secondF == secondS, "assign()");
	expect(thirdF == thirdS, "assign()");
};

static void insert()
{
	std::vector<int> vecS (3,100);
	std::vector<int>::iterator itS;
	itS = vecS.begin();
	itS = vecS.insert ( itS , 200 );
	vecS.insert (itS,2,300);
	// "it" no longer valid, get a new one:
	itS = vecS.begin();
	std::vector<int> vecS2 (2,400);
	vecS.insert (itS+2,vecS2.begin(),vecS2.end());
	int myarrayS [] = { 501,502,503 };
	vecS.insert (vecS.begin(), myarrayS, myarrayS+3);

	ft::vector<int> vecF (3,100);
	ft::vector<int>::iterator itF;
	itF = vecF.begin();
	itF = vecF.insert ( itF , 200 );
	vecF.insert (itF,2,300);
	// "it" no longer valid, get a new one:
	itF = vecF.begin();
	ft::vector<int> vecF2 (2,400);
	vecF.insert (itF+2,vecF2.begin(),vecF2.end());
	int myarrayF [] = { 501,502,503 };
	vecF.insert (vecF.begin(), myarrayF, myarrayF+3);
	expect(vecF == vecS, "insert()");
	expect(vecF2 == vecS2, "insert()");
};

static void erase()
{
	std::vector<int> vecS;
	ft::vector<int> vecF;
	// set some values (from 1 to 10)
	for (int i=1; i<=10; i++) vecS.push_back(i);
	for (int i=1; i<=10; i++) vecF.push_back(i);
	// erase the 6th element
	vecS.erase (vecS.begin()+5);
	vecF.erase (vecF.begin()+5);
	expect(vecF == vecS, "erase()");
	// erase the first 3 elements:
	vecS.erase (vecS.begin(),vecS.begin()+3);
	vecF.erase (vecF.begin(),vecF.begin()+3);
	expect(vecF == vecS, "erase()");
};

static void swap()
{
	std::vector<int> fooS (3,100);   // three ints with a value of 100
	std::vector<int> barS (5,200);   // five ints with a value of 200
	fooS.swap(barS);
	ft::vector<int> fooF (3,100);   // three ints with a value of 100
	ft::vector<int> barF (5,200);   // five ints with a value of 200
	fooF.swap(barF);
	expect(fooF == fooS, "swap()");
	expect(barF == barS, "swap()");
};

static void operators()
{
	std::vector<int> fooS (3,100);   // three ints with a value of 100
	std::vector<int> barS (2,200);   // two ints with a value of 200
	ft::vector<int> fooF (3,100);   // three ints with a value of 100
	ft::vector<int> barF (2,200);   // two ints with a value of 200

	expect("operators", fooS == barS, fooF == barF);
	expect("operators", fooS != barS, fooF != barF);
	expect("operators", fooS < barS, fooF < barF);
	expect("operators", fooS > barS, fooF > barF);
	expect("operators", fooS <= barS, fooF <= barF);
	expect("operators", fooS >= barS, fooF >= barF);
};

void	testVector(void)
{
	printHeader("Vector");
	constructor();
	copyConstructor();
	size();
	resize();
	accessOperator();
	frontBack();
	assign();
	insert();
	erase();
	swap();
	operators();
	printHeader("END");
};
