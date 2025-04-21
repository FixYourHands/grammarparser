#include "parser.h"
#include <iostream>
#include <algorithm>
#include <ctype.h>


void Parser::storeRules() //store left and right hand side in an array
{
	Token temp;
	temp.token_type = ERROR;
	vector<int> set;
	int index = 1;

	while (temp.token_type != END_OF_FILE)
	{
		temp = lexer.peek(index);

		if (temp.token_type == ARROW)
		{
			set.push_back(index - 1);
		}

		if (temp.token_type == HASH)
		{
			set.push_back(index);
		}
		index++;
	}
	index = 1;
	int i = 0;

	temp.token_type = ERROR;
	while (temp.token_type != END_OF_FILE)
	{
		int ruleCounter = 0;
		Rules r;
		r.RHS.push_back({}); //initalize first vector for RHS
		temp = lexer.peek(index);

		for (int j = 0; j < ruleCounter; j++)
			r.RHS.push_back({});



		if (i < set.size() && index == set[i])
		{
			r.LHS = temp.lexeme; //store left hand nonTerminal

			index++; //skip arrow
			i++;
			if (i >= set.size())
				break;
			//populate RHS
			while (index < set[i])
			{

				if (lexer.peek(index).token_type == ID)
					r.RHS[ruleCounter].push_back(lexer.peek(index).lexeme);
				else if (lexer.peek(index).token_type == OR)
				{
					r.RHS.push_back({}); //add another vector for additional set of rules
					ruleCounter++;
				}

				else if (lexer.peek(index).token_type == STAR)
					r.RHS[ruleCounter].push_back("*");

				index++;

			}

		}

		rules.push_back(r); //add to list of rules
	}
	inputString = lexer.peek(index + 1).lexeme; //get input string at end of file
	inputString.erase(remove(inputString.begin(),inputString.end(),'"')); //remove quotes from string
	return;
}

Parser::Parser()
{
	vector<int> arrows;
	storeNonTerminals();
	storeTerminals();
	for (auto& e : nonTerminals) //initialze firstset map
	{
		firstSets[e] = {};
		followSets[e] = {};
	}

	storeRules();
	currentToken = lexer.GetToken();
	//parseGrammar();
	getNullable();
	storeFirstSets();
	storeFirstSets();
	storeFollowSets();
	buildParseTable();
	parseString();




}


void Parser::storeNonTerminals()
{
	Token t;
	t.token_type = ERROR;
	int index = 1;
	while (t.token_type != END_OF_FILE)
	{
		t = lexer.peek(index);

		if (t.token_type == ARROW) //if current token is an ARROW
		{
			Token temp = lexer.peek(index - 1);

			if (find(nonTerminals.begin(), nonTerminals.end(), temp.lexeme) == nonTerminals.end()) //if character is not already in the non terminal vector...
			{
				nonTerminals.push_back(temp.lexeme);
			}

		}
		index++;
	}


	//ensure the non terminals are stored in order of appearance
	t.token_type = ERROR;
	index = 1;
	int counter = 0;
	vector<string> temp;

	while (t.token_type != END_OF_FILE)
	{
		if (counter == nonTerminals.size())
			break;

		t = lexer.peek(index);

		if (find(nonTerminals.begin(), nonTerminals.end(), t.lexeme) != nonTerminals.end()) //check if token is a non terminals by comparing to non terminal vector
		{
			if (find(temp.begin(), temp.end(), t.lexeme) == temp.end()) //if character is not already in the temp vector...
			{
				temp.push_back(t.lexeme);
				counter++;
			}

		}
		index++;
	}


	nonTerminals = temp;

}

void Parser::storeTerminals()
{
	Token t;
	t.token_type = ERROR;
	int index = 1;

	while (t.token_type != END_OF_FILE)
	{
		t = lexer.peek(index);

		//check if current lexeme is a part of non terminals
		if (find(nonTerminals.begin(), nonTerminals.end(), t.lexeme) == nonTerminals.end()) //check if token is a non terminals by comparing to non terminal vector
		{
			if (t.lexeme != "*" && t.lexeme != "|" && t.lexeme != "#" && t.lexeme != "")
			{
				//make sure character is not already inside the terminals vector
				if (find(terminals.begin(), terminals.end(), t.lexeme) == terminals.end())
				{
					terminals.push_back(t.lexeme);
				}
			}
		}
		index++;
	}

}

void Parser::storeFirstSets()
{


	//initalization
	for (auto& r : rules)
	{
		string LHS = r.LHS;

		for (auto& RHS : r.RHS)
		{
			if (!RHS.empty())
			{
				if (contains(RHS[0], 't')) //check if first word is a terminal
				{
					firstSets[LHS].insert(RHS[0]);
				}
			}

		}
	}

	bool changed = true;
	int count = 0;
	while (changed)
	{
		count++;
		changed = false;
		for (auto& r : rules)
		{
			string LHS = r.LHS;

			for (auto& RHS : r.RHS)
			{
				for (auto& w : RHS)
				{
					if (contains(w, 'n')) //if we run into a nonterminal
					{

						if (nullableMap[w] == TRUE)
						{
							set<string> set;

							if (includes(firstSets[LHS].begin(), firstSets[LHS].end(), firstSets[w].begin(), firstSets[w].end())) //if we already have this set
								continue;
							merge(firstSets[LHS].begin(), firstSets[LHS].end(), firstSets[w].begin(), firstSets[w].end(), inserter(set, set.begin()));
							firstSets[LHS] = set;
							changed = true;
						}
						else
						{
							set<string> set;
							if (includes(firstSets[LHS].begin(), firstSets[LHS].end(), firstSets[w].begin(), firstSets[w].end())) //if we already have this set
								break;

							merge(firstSets[LHS].begin(), firstSets[LHS].end(), firstSets[w].begin(), firstSets[w].end(), inserter(set, set.begin()));
							firstSets[LHS] = set;
							changed = true;
							break;
						}
					}
					if (contains(w, 't')) //if we run into terminal
					{
						if (firstSets[LHS].find(w) != firstSets[LHS].end()) //if set already contains value
							break;
						firstSets[LHS].insert(w);
						changed = true;
						break;
					}
				}
			}
		}
	}

}

void Parser::storeFollowSets()
{

	bool startVariableFound = false;
	string startingVariable = rules[0].LHS;
	followSets[startingVariable].insert("$");


	//first pass rules 3 and 4
	for (auto& r : rules)
	{
		string LHS = r.LHS;
		for (auto& RHS : r.RHS)
		{
			int size = RHS.size();
			int currentIndex = 0;
			int currentSet = 1;
			for (auto& word : RHS)
			{
				if (!contains(word, 'n')) //if current word is not a nonTerminal, then skip to next word
				{
					currentIndex++;
					continue;
				}
				int nextIndex = currentIndex + 1;
				string next;
				if (nextIndex < RHS.size())
				{
					while (nextIndex < RHS.size())
					{
						next = RHS[nextIndex];
						if (contains(next, 't')) //if following word is a terminal
						{
							followSets[word].insert(next); //add terminal to follow set of Non Terminal
							break;
						}
						else if (contains(next, 'n')) //if follow word is a non terminal
						{
							if (nullableMap[next] != TRUE) //if not nullable
							{
								set<string> temp;
								merge(followSets[word].begin(), followSets[word].end(), firstSets[next].begin(), firstSets[next].end(), inserter(temp, temp.begin())); //add first set of next word to follow set of original non terminal
								followSets[word] = temp;
								break;
							}
							else
							{
								set<string> temp;
								merge(followSets[word].begin(), followSets[word].end(), firstSets[next].begin(), firstSets[next].end(), inserter(temp, temp.begin())); //add first set of next word to follow set of original non terminal
								followSets[word] = temp;
							}
						}
						nextIndex++;
					}
				}



				currentIndex++;
			}
		}
	}

	bool changed = true;
	while (changed)
	{
		changed = false;

		for (auto& r : rules)
		{
			string LHS = r.LHS;
			for (auto& RHS : r.RHS)
			{
				NULLABLE_STATUS status = TRUE;
				int currentIndex = 0;
				for (auto& word : RHS)
				{
					if (word == "*")
					{
						break;
					}

					if (!contains(word, 'n')) //if not a non terminal, skip
					{
						continue;
					}

					string next;
					int nextIndex = currentIndex + 1;
					while (nextIndex < RHS.size())
					{
						next = RHS[nextIndex];

						if (next == "*")
							break;
						if (contains(next, 't') || nullableMap[next] != TRUE)
						{
							status = FALSE;
							break;
						}
						nextIndex++;
					}
					if (status == TRUE) //if all non terminals were nullable add follow set of left hand side
					{
						set<string> temp;
						merge(followSets[word].begin(), followSets[word].end(), followSets[LHS].begin(), followSets[LHS].end(), inserter(temp, temp.begin()));

						if (followSets[word] != temp) //check if a change would occur to the follow set
						{
							changed = true;
							followSets[word] = temp;
						}

					}
					status = TRUE;
					currentIndex++;
				}
			}
		}
	}

}
void Parser::getNullable()
{
	bool terminalEncountered = false;
	bool changed = true;
	bool nullable = true;
	while (changed)
	{
		changed = false;
		for (auto& r : rules)
		{
			string LHS = r.LHS;
			NULLABLE_STATUS status = FALSE;
			for (auto& RHS : r.RHS)
			{
				if (RHS.size() == 1 && RHS[0] == "*" || RHS.size() == 0)
				{
					if (nullableMap[LHS] == TRUE)
						continue;
					nullableMap[LHS] = TRUE;
					changed = true;
				}

				for (auto& word : RHS)
				{
					if (word == "*")
						continue;
					if (nullableMap[word] == TRUE)
					{
						nullable = true;
						continue;
					}
					else
					{
						nullable = false;
						break;
					}
				}
				if (nullable)
				{
					if (nullableMap[LHS] == TRUE)
						continue;
					nullableMap[LHS] = TRUE;
					changed = true;
				}

			}

		}
	}

	for (auto it = nullableMap.begin(); it != nullableMap.end();) //fix map
	{
		if (it->second != TRUE)
		{
			it = nullableMap.erase(it);
		}
		else
		{
			it++;
		}
	}



	return;
}

void Parser::buildParseTable()
{
	vector<string> currRule;
	for (auto& r : rules)
	{
		string LHS = r.LHS;
		
		for (auto& RHS : r.RHS)
		{
			NULLABLE_STATUS status = TRUE;
			currRule = RHS;
			if (RHS[0] == "*" && RHS.size() == 1) //if rule leads to empty
			{
				for (auto& e : followSets[LHS])
				{
					parseTable[LHS][e] = RHS; //add epislon production to columns of follow set elements
				}
				continue;
			}
			for (auto& word : RHS)
			{
				vector<string> temp = RHS;
				if (temp.back() == "*")
					temp.pop_back();
				if (contains(word, 't')) //if char is a terminal
				{
					parseTable[LHS][word] = temp;
					status = FALSE;
					break;
				}

				if (contains(word, 'n')) //if char is a non terminal
				{
					for (auto& e : firstSets[word])
					{
						parseTable[LHS][e] = temp; //add first sets of non terminal to table
					}

					if (nullableMap[word] != TRUE)
					{
						status = FALSE;
						break;
					}
				}
			}
			if (status == TRUE) //if the entire rule is nullable
			{
				for (auto& e : followSets[LHS])
				{
					parseTable[LHS][e] = currRule; //add epislon production to columns of follow set elements
				}
			}
		}

		
	}
}

void Parser::parseString()
{
	bool successful = true;
	int size = inputString.size();
	int index = 0;
	stack<string> stack;
	string lookahead;
	string startingVariable = rules[0].LHS;
	stack.push("$");
	stack.push(startingVariable);

	string X = stack.top();
	while (X != "$")
	{
		if (index < size)
			lookahead = string() + inputString[index];

		if (contains(X, 't')) //if X is a terminal
		{
			if (X == lookahead)
			{
				stack.pop();
				successful = match(lookahead);
				if (!successful)
					break;
			}
			else
			{
				successful = false;
				break;
			}
		}
		else
		{
			if (!parseTable[X][lookahead].empty()) //if not empty
			{
				vector<string> rev(parseTable[X][lookahead].rbegin(), parseTable[X][lookahead].rend()); //reverse the rules of production for pushing in stack
				stack.pop();

				for (auto& e : rev)
				{
					stack.push(e);
				}
			}
			else
			{
				successful = false;
				break;
			}
		}
		X = stack.top();
	}

	cout << "output: ";

	if (successful)
	{
		cout << "y\n";
	}
	else
	{
		cout << "n\n";
	}
}

bool Parser::match(string word)
{
	char c = word[0];
	if (!inputString.empty())
	{
		if (c == inputString[0])
		{
			inputString.erase(0, 1);
			return true;
		}
			
		else
			return false;
	}

	return false;
}
bool Parser::contains(const string& string, char key) // 'n' to search nonTerminal vector, 't' to search terminal vector
{
	switch (key)
	{
	case 'n':
		if (find(nonTerminals.begin(), nonTerminals.end(), string) != nonTerminals.end())
		{
			return true;
		}
		break;
	case 't':
		if (find(terminals.begin(), terminals.end(), string) != terminals.end())
		{
			return true;
		}
		break;
	}


	return false;
}