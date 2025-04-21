
/*
* Copyright (C) Mohsen Zohrevandi, 2017
* Rida Bazzi 2019
* Do not share this file with anyone
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "parser.h"
#include <stack>
#include "lexer.h"


using namespace std;





// read grammar
void ReadGrammar() {
	//LexicalAnalyzer lex;
	//Parser p(lex);


}


int main(int argc, char* argv[])
{

	Parser p;

	int task;
	if (argc < 2)
	{
		cout << "Error: missing argument\n";
		return 1;
	}

	/*
	Note that by convention argv[0] is the name of your executable,
	and the first argument to your program is stored in argv[1]
	*/
	task = atoi(argv[1]);
	ReadGrammar(); // Reads the input grammar from standard input
	// and represent it internally in data structures
	// ad described in project 2 presentation file

	system("pause");

	return 0;
}
