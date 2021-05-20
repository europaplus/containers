//
// Created by Knot Fonda on 5/2/21.
//

#include "test.hpp"

int 	main(int argc, char **argv)
{
	if (argc != 2)
	{
		testVector();
		testMap();
		testList();
		testQueue();
		testStack();
//		std::cout << "Please, enter one of the options:\n";
//		std::cout << "	- vector;\n	- list;\n	- map;\n	- stack;\n	- queue.\n 	- all\n";
	}
	else
	{
		if (!strcmp(argv[1], "vector"))
			testVector();
		if (!strcmp(argv[1], "list"))
			testList();
		if (!strcmp(argv[1], "map"))
			testMap();
		if (!strcmp(argv[1], "stack"))
			testStack();
		if (!strcmp(argv[1], "queue"))
			testQueue();
	}
	return (0);
};