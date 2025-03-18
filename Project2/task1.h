#ifndef TASK1
#define TASK1
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
class TaskOne
{
public:
	TaskOne(ifstream& input);
	void printNonTerminals();
	void printTerminals();


	void executeTask(); //completes task asked for in assignment
	

private:
	vector<string> buffer; //keep track of every word read so that we can make sure each terminal/nonterminal is in order later
	vector<string> nonTerminals;
	vector<string> terminals;
	void orderNonTerminals(vector<string> vec); //ensure nonTerminals are in order of appearance
	void storeTerminals();
	
};
#endif
