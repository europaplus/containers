//
// Created by Knot Fonda on 5/2/21.
//

#include "test.hpp"

bool fncomp (char lhs, char rhs) {return lhs<rhs;}

struct classcomp {
	bool operator() (const char& lhs, const char& rhs) const
	{return lhs<rhs;}
};

static void constructors()
{
	std::map<char,int> firstS;
	ft::map<char,int> firstF;
	firstS['a']=10;
	firstS['b']=30;
	firstS['c']=50;
	firstS['d']=70;
	firstF['a']=10;
	firstF['b']=30;
	firstF['c']=50;
	firstF['d']=70;
	std::map<char,int> secondS (firstS.begin(),firstS.end());
	ft::map<char,int> secondF (firstF.begin(),firstF.end());
	std::map<char,int> thirdS (secondS);
	ft::map<char,int> thirdF (secondF);
//	std::map<char,int,classcomp> fourthS;                 // class as Compare
//	ft::map<char,int,classcomp> fourthF;                 // class as Compare
//	bool(*fn_pt)(char,char) = fncomp;
//	std::map<char,int,bool(*)(char,char)> fifthS (fn_pt);
//	ft::map<char,int,bool(*)(char,char)> fifthF (fn_pt);
	expect(firstF == firstS, "constructors");
	expect(secondF == secondS, "constructors");
	expect(thirdF == thirdS, "constructors");
//	expect(fourthF == fourthS, "constructors");
//	expect(fifthF == fifthS, "constructors");
};

static void accessOperator()
{
	std::map<char,std::string> mapS;
	ft::map<char,std::string> mapF;

	mapS['a']="an element";
	mapS['b']="another element";
	mapS['c']=mapS['b'];

	mapF['a']="an element";
	mapF['b']="another element";
	mapF['c']=mapF['b'];
	expect(mapF == mapS, "accessOperator");
};

static void insert()
{
	std::map<char,int> mapS;
	ft::map<char,int> mapF;

	// first insert function version (single parameter):
	mapS.insert ( std::pair<char,int>('a',100) );
	mapS.insert ( std::pair<char,int>('z',200) );

	mapF.insert ( std::pair<char,int>('a',100) );
	mapF.insert ( std::pair<char,int>('z',200) );
	expect(mapF == mapS, "insert");
	// second insert function version (with hint position):
	std::map<char,int>::iterator itS = mapS.begin();
	mapS.insert (itS, std::pair<char,int>('b',300));  // max efficiency inserting
	mapS.insert (itS, std::pair<char,int>('c',400));  // no max efficiency inserting

	ft::map<char,int>::iterator itF = mapF.begin();
	mapF.insert (itF, std::pair<char,int>('b',300));  // max efficiency inserting
	mapF.insert (itF, std::pair<char,int>('c',400));  // no max efficiency inserting
	expect(mapF == mapS, "insert");
	// third insert function version (range insertion):
	std::map<char,int> mapS2;
	mapS2.insert(mapS.begin(),mapS.find('c'));

	ft::map<char,int> mapF2;
	mapF2.insert(mapF.begin(),mapF.find('c'));
	expect(mapF2 == mapS2, "insert");
};

static void erase()
{
	std::map<char,int> mapS;
	std::map<char,int>::iterator itS;

	ft::map<char,int> mapF;
	ft::map<char,int>::iterator itF;

	// insert some values:
	mapS['a']=10;
	mapS['b']=20;
	mapS['c']=30;
	mapS['d']=40;
	mapS['e']=50;
	mapS['f']=60;

	mapF['a']=10;
	mapF['b']=20;
	mapF['c']=30;
	mapF['d']=40;
	mapF['e']=50;
	mapF['f']=60;

	itS=mapS.find('b');
	mapS.erase (itS);                   // erasing by iterator

	itF=mapF.find('b');
	mapF.erase (itF);                   // erasing by iterator
	expect(mapF == mapS, "erase");
	mapS.erase ('c');                  // erasing by key
	mapF.erase ('c');                  // erasing by key
	expect(mapF == mapS, "erase");
	itS=mapS.find ('e');
	mapS.erase ( itS, mapS.end() );    // erasing by range

	itF=mapF.find ('e');
	mapF.erase ( itF, mapF.end() );    // erasing by range
	expect(mapF == mapS, "erase");
};

static void swap()
{
	std::map<char,int> fooS,barS;
	ft::map<char,int> fooF,barF;

	fooS['x']=100;
	fooS['y']=200;

	barS['a']=11;
	barS['b']=22;
	barS['c']=33;

	fooF['x']=100;
	fooF['y']=200;

	barF['a']=11;
	barF['b']=22;
	barF['c']=33;

	fooS.swap(barS);
	fooF.swap(barF);
	expect(fooF == fooS, "swap");
	expect(barF == barS, "swap");
};

static void clear()
{
	std::map<char,int> mapS;
	ft::map<char,int> mapF;

	mapS['x']=100;
	mapS['y']=200;
	mapS['z']=300;

	mapF['x']=100;
	mapF['y']=200;
	mapF['z']=300;

	mapS.clear();
	mapF.clear();
	expect(mapF == mapS, "clear");
	mapS['a']=1101;
	mapS['b']=2202;

	mapF['a']=1101;
	mapF['b']=2202;
	expect(mapF == mapS, "clear");
};

static void find()
{
	std::map<char,int> mapS;
	std::map<char,int>::iterator itS;

	ft::map<char,int> mapF;
	ft::map<char,int>::iterator itF;

	mapS['a']=50;
	mapS['b']=100;
	mapS['c']=150;
	mapS['d']=200;

	mapF['a']=50;
	mapF['b']=100;
	mapF['c']=150;
	mapF['d']=200;

	itS = mapS.find('b');
	if (itS != mapS.end())
		mapS.erase (itS);

	itF = mapF.find('b');
	if (itF != mapF.end())
		mapF.erase (itF);
	expect(mapF == mapS, "find");
};

static void bounds()
{
	std::map<char,int> mapS;
	std::map<char,int>::iterator itlowS,itupS;

	ft::map<char,int> mapF;
	ft::map<char,int>::iterator itlowF,itupF;

	mapS['a']=20;
	mapS['b']=40;
	mapS['c']=60;
	mapS['d']=80;
	mapS['e']=100;

	mapF['a']=20;
	mapF['b']=40;
	mapF['c']=60;
	mapF['d']=80;
	mapF['e']=100;

	itlowS=mapS.lower_bound ('b');  // itlow points to b
	itupS=mapS.upper_bound ('d');   // itup points to e (not d!)

	mapS.erase(itlowS,itupS);        // erases [itlow,itup)

	itlowF=mapF.lower_bound ('b');  // itlow points to b
	itupF=mapF.upper_bound ('d');   // itup points to e (not d!)

	mapF.erase(itlowF,itupF);        // erases [itlow,itup)
	expect(mapF == mapS, "bounds and range");
};

static void operatorsComp()
{
	std::map<char, int> fooS;   // three ints with a value of 100
	std::map<char, int> barS;   // two ints with a value of 200
	fooS['a']=20;
	fooS['b']=40;

	barS['c']=60;
	barS['d']=80;
	barS['e']=100;

	ft::map<char, int> fooF;   // three ints with a value of 100
	ft::map<char, int> barF;   // two ints with a value of 200

	fooF['a']=20;
	fooF['b']=40;

	barF['c']=60;
	barF['d']=80;
	barF['e']=100;

	expect("operators", fooS == barS, fooF == barF);
	expect("operators", fooS != barS, fooF != barF);
	expect("operators", fooS < barS, fooF < barF);
	expect("operators", fooS > barS, fooF > barF);
	expect("operators", fooS <= barS, fooF <= barF);
	expect("operators", fooS >= barS, fooF >= barF);
};

void	testMap(void)
{
	printHeader("Map");
	constructors();
	accessOperator();
	insert();
	erase();
	swap();
	clear();
	find();
	bounds();
	operatorsComp();
	printHeader("END");
};