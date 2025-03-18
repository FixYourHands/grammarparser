#include "task1.h"
#include <algorithm>

TaskOne::TaskOne(ifstream& inputFile)
{
	string line;
	string prev = "";
	vector<string> vec;

	while (inputFile >> line)
	{
		if (line == "->")
		{
			vec.push_back(prev); //get nonTerminals
		}
		else
		{
			buffer.push_back(line); //add words to buffer
		}
		prev = line;
	}
	orderNonTerminals(vec);
	storeTerminals();
}

void TaskOne::orderNonTerminals(vector<string> vec)
{
	for (auto i : buffer)
	{
		if (nonTerminals.size() == vec.size())
		{
			break;
		}

		if (find(vec.begin(), vec.end(), i) != vec.end()) //if word is in a nonTerminal 
		{
			if (find(nonTerminals.begin(), nonTerminals.end(), i) == nonTerminals.end()) //if word is not found in nonTerminals vector, add it
			{
				nonTerminals.push_back(i);
			}
		}
	}
}

void TaskOne::storeTerminals()
{
	//iterate through buffer and add all words that aren't already stored as nonTerminals
	for (auto i : buffer)
	{
		if (i == "|" || i == "*" || i == "#")
			continue;

		if (find(nonTerminals.begin(), nonTerminals.end(), i) != nonTerminals.end()) //if word is in nonTerminals vector..
		{

		}
		else
		{
			//check if value is already in terminals vector
			if (find(terminals.begin(), terminals.end(), i) == terminals.end()) //if word is not found, add it
			{
				terminals.push_back(i);
			}
		}
	}

}

void TaskOne::printNonTerminals()
{
	for (auto i : nonTerminals)
	{
		cout << "NonTerminal: " << i << '\n';
	}
}

void TaskOne::printTerminals()
{
	for (auto i : terminals)
	{
		cout << "Terminal: " << i << '\n';
	}
}

void TaskOne::executeTask() //print terminals followed by nonterminals
{
	for (auto i : terminals)
	{
		cout << i << " ";
	}

	for (auto i : nonTerminals)
	{
		cout << i << " ";
	}
	
}