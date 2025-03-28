
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
#include "task1.h"
using namespace std;

struct Tasks {
	string task1;
	string task2;
	string task3;
	string task4;
	string task5;
	string task6;
};

Tasks execution;

// read grammar
void ReadGrammar() {
	LexicalAnalyzer lex;
	Parser p(lex);
	execution.task1 = p.executeTaskOne();
	execution.task2 = p.executeTaskTwo();
}
/*
* Task 1:
* Printing the terminals, then nonterminals of grammar in appearing order
* output is one line, and all names are space delineated
*/
void Task1()
{
	cout << execution.task1;
}
/*
* Task 2:
* Print out nullable set of the grammar in specified format.
*/
void Task2()
{
	cout << execution.task2;
}
// Task 3: FIRST sets
void Task3()
{
}
// Task 4: FOLLOW sets
void Task4()
{
}
// Task 5: left factoring
void Task5()
{
}
// Task 6: eliminate left recursion
void Task6()
{
}

void resetStream(ifstream& stream)
{
	stream.clear();
	stream.seekg(0, ios::beg);
}

int main(int argc, char* argv[])
{
	


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
	switch (task) {
	case 1: Task1();
		break;
	case 2: Task2();
		break;
	case 3: Task3();
		break;
	case 4: Task4();
		break;
	case 5: Task5();
		break;
	case 6: Task6();
		break;
	default:
		cout << "Error: unrecognized task number " << task << "\n";
		break;
	}
	system("pause");
	
	return 0;
}
