#include "parser.h"
#include <iostream>
#include <algorithm>
#include <ctype.h>

vector<string> Parser::longestCommonPrefix(vector<string>& v1, vector<string>& v2)
{
	string prefix = "";
	string string1 = "";
	string string2 = "";
	vector<string> ret;

	for (auto& s : v1) //convert to string
		string1 += s;
	for (auto& s : v2) //convert to string
		string2 += s;
	int minLength = min(string1.size(), string2.size());

	for (int i = 0; i < minLength - 1; i++)
	{
		if (string1[i] == string2[i])
			prefix += string1[i];
		else
			break;
	}

	for (auto& c : prefix)
	{
		ret.push_back(string() + c);
	}

	cout << "test";
	return ret;
}

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
	return;
}

Parser::Parser(LexicalAnalyzer& lex)
	:lexer(lex)
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
	parseGrammar();
	getNullable();
	storeFirstSets();
	storeFirstSets();
	storeFollowSets();
	executeTaskThree();
	executeTaskFour();
	leftFactor();


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

string Parser::executeTaskOne()
{
	string ret = "";
	for (auto w : terminals)
	{
		ret += (w + " ");
	}

	for (auto w : nonTerminals)
	{
		ret += (w + " ");
	}

	return ret;
}

string Parser::executeTaskTwo()
{
	int nullCount = 0;
	vector<string> nulls;
	string ret = "";
	for (auto& pair : nullableMap)
	{
		if (pair.second == TRUE)
		{
			nullCount++;
		}
	}

	for (auto& r : rules)
	{
		for (auto& rhs : r.RHS)
		{
			for (auto& word : rhs)
			{
				if (nullableMap[word] == TRUE)
				{
					if (find(nulls.begin(),nulls.end(),word) == nulls.end()) //only add if not already in vector
						nulls.push_back(word);
				}
			}
		}
	}

	ret += "Nullable = {";
	if (nullCount == 0)
	{
		ret += "}";
		
		return ret;
	}
	
	for (int i = 0; i < nulls.size(); i++)
	{
		ret += nulls[i];

		if (i == nulls.size() - 1)
			break;

		ret += ", ";
	}

	ret += "}";
	return ret;
}

string Parser::executeTaskThree()
{
	for (auto& pair : firstSets)
	{
		int size = pair.second.size();
		if (size == 0) //don't add to output if firstSet is empty
			continue;
		cout << "FIRST(" + pair.first + ") = {";
		for (auto& s : pair.second)
		{
			size--;
			cout << s;

			if (size > 0)
			{
				cout << ", ";
			}
		}
		cout << "}\n";
	}

	return "";
}

string Parser::executeTaskFour()
{
	for (auto& pair : followSets)
	{
		int size = pair.second.size();
		if (size == 0) //don't add to output if firstSet is empty
			continue;
		cout << "Follow(" + pair.first + ") = {";
		for (auto& s : pair.second)
		{
			size--;
			cout << s;

			if (size > 0)
			{
				cout << ", ";
			}
		}
		cout << "}\n";
	}

	return "";
}

vector<Rules> Parser::fixedRuleList()
{
	unordered_map<string, Rules> newGroup;

	for (auto& r : rules)
	{
		string LHS = r.LHS;
		for (auto& RHS : r.RHS)
		{
			newGroup[LHS].RHS.push_back(RHS);
		}
	}
	vector<Rules> originalGrammar = rules;
	vector<Rules> newGrammar;

	for (int i = 0; i < originalGrammar.size(); i++)
	{
		if (newGroup[originalGrammar[i].LHS].RHS.size() == 1)
		{
			newGrammar.push_back(originalGrammar[i]);
		}

		if (newGroup[originalGrammar[i].LHS].RHS.size() > 1)
		{
			for (auto& r : newGroup[originalGrammar[i].LHS].RHS)
			{
				Rules temp;
				temp.LHS = originalGrammar[i].LHS;
				temp.RHS.push_back(r);
				newGrammar.push_back(temp);

			}
			newGroup[originalGrammar[i].LHS].RHS.clear();
		}


	}
	return newGrammar;
}

unordered_map<string, set<vector<string>>> Parser::getCommonPrefixes()
{
	int index = 0;
	unordered_map<string, set<vector<string>>> commonPrefixes;
	singleLineRules = fixedRuleList();
	for (auto& r : singleLineRules)
	{
		string LHS = r.LHS;
		vector<string> v1 = r.RHS[0];
		for (int i = 0; i < singleLineRules.size(); i++)
		{
			if (i != index && singleLineRules[i].LHS == LHS) //dont check the same rule against itself
			{
				vector<string> v2 = singleLineRules[i].RHS[0];
				vector<string> temp = longestCommonPrefix(v1, v2);

				if (!temp.empty()) //only if a common prefix was found
					commonPrefixes[LHS].insert(temp);
			}
		}
		index++;
	}
	return commonPrefixes;
}

string Parser::leftFactor()
{
	
	unordered_map<string, set<vector<string>>> commonPrefixes = getCommonPrefixes();
	

	
	int longestPrefixCount = 1;



	

	return "";
}



vector<string> Parser::getNonTerminals()
{
	return nonTerminals;
}

vector<string> Parser::getTerminals()
{
	return terminals;
}

NULLABLE_STATUS Parser::getNullableStatus(string word)
{
	return nullableMap[word];
}


void Parser::storeFirstSets()
{
	
	set<string> set;
	

	for (auto& r : rules)
	{
		string curr = r.LHS;
		for (auto& vec : r.RHS)
		{
			bool completed = false;
			bool isFirst = true;
			for (auto& word : vec)
			{
				if (!completed)
				{
					if (isFirst && word == "*")
					{
						//firstSets[curr].insert("_");
						break;
					}
					if (contains(word, 't')) //if word is a terminal
					{
						firstSets[curr].insert(word); //add to first set
						break;
					}
					else if (contains(word, 'n')) //if word is a nonTerminal
					{
						if (nullableMap[word] == TRUE) //if nullable
						{
							merge(firstSets[curr].begin(), firstSets[curr].end(), firstSets[word].begin(), firstSets[word].end(), inserter(set, set.begin()));
							firstSets[curr] = set;
							//firstSets[curr].insert("_"); //epsilon
							isFirst = false;
							continue;
						}
						else if (nullableMap[word] == FALSE)
						{
							merge(firstSets[curr].begin(), firstSets[curr].end(), firstSets[word].begin(), firstSets[word].end(), inserter(set, set.begin()));
							firstSets[curr] = set;
							break;
						}
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
				if (currentIndex + 1 >= size) //if currentword is the last rule in a string
				{
					set<string> temp;
					merge(followSets[word].begin(), followSets[word].end(), followSets[LHS].begin(), followSets[LHS].end(), inserter(temp, temp.begin())); //add follow set of LHS
					followSets[word] = temp;
				}
				else
				{
					string next = RHS[currentIndex + 1];
					if (contains(next, 't')) //if follow word is a terminal
					{
						followSets[word].insert(next);
					}
					else if (contains(next,'n'))
					{
						set<string> temp;
						merge(followSets[word].begin(), followSets[word].end(), firstSets[next].begin(), firstSets[next].end(), inserter(temp, temp.begin())); //add first set of next
						followSets[word] = temp;
					}
					else if (next == "*")
					{
						set<string> temp;
						merge(followSets[word].begin(), followSets[word].end(), followSets[LHS].begin(), followSets[LHS].end(), inserter(temp, temp.begin())); //add follow set of LHS
						followSets[word] = temp;
					}
				}
				currentIndex++;
			}
		}
	}
	
	cout << "stop\n";
}
void Parser::getNullable()
{
	bool terminalEncountered = false;

	for (int i = 0; i < rules.size(); i++)
	{
		string& currentNonTerminal = rules[i].LHS;
		bool isFirstEncountered = true;
		bool nullable = true;
		bool waitingPotential = false;
		
		for (auto& vec : rules[i].RHS) //iterator for inner vector
		{
			NULLABLE_STATUS temp = POTENTIAL;
			int wordCount = 0;
			for (auto& firstIt : vec)
			{
				if (nullableMap[currentNonTerminal] == TRUE)
				{
					continue;
				}

				if (contains(firstIt,'n') && nullableMap[firstIt]== FALSE)
				{
					nullableMap[currentNonTerminal] = FALSE;
					break;
				}

				if (contains(firstIt, 't') || nullableMap[firstIt] == FALSE)
				{
					nullableMap[currentNonTerminal] = FALSE;
					terminalEncountered = true;
					nullable = false;
					wordCount++;
				}

				else if (terminalEncountered == false && firstIt == "*" && wordCount == 0)
				{
					nullableMap[currentNonTerminal] = TRUE;
				}

				else if (nullable && contains(firstIt, 'n') && isFirstEncountered)
				{
					if (nullableMap[firstIt] == TRUE)
					{
						isFirstEncountered = false;
						continue;
					}
					
					if (nullableMap[firstIt] == FALSE && nullableMap[currentNonTerminal] != TRUE)
					{
						nullableMap[currentNonTerminal] = FALSE;
						nullable = false;
						isFirstEncountered = true;
						continue;
					}
					else if (nullableMap[firstIt] == UNREAD)
					{

						nullableMap[firstIt] = POTENTIAL;
						nullableMap[currentNonTerminal] = POTENTIAL;
						wordCount++;
					}
					
					//terminalEncountered = true;
				}
				
			}
			
			//rules[i].nullable = true;
			isFirstEncountered = true;
			terminalEncountered = false;
			waitingPotential = false;
			nullable = true;
		}
	}

	for (auto& p : nullableMap)
	{
		if (p.second == POTENTIAL)
		{
			getNullable();
			break;
		}
	}

	
}

void Parser::match(TokenType expectedType)
{
	if (currentToken.token_type == expectedType)
		currentToken = lexer.GetToken();
	else
	{
		cout << "SYNTAX ERROR !!!!!!!!!!!!!!\n";
		exit(-1);
	}
}

bool Parser::parseGrammar()
{
	if (!parseRuleList())
		return false;
	if (currentToken.token_type == HASH)
	{
		match(HASH); //consume '#'
		return true;
	}
	return false;
}

bool Parser::parseRuleList()
{
	if (!parseRule())
		return false;
	if (!parseRuleList())
		return false;

	return false;
}

bool Parser::parseIDList()
{
	if (currentToken.token_type == ID)
	{
		match(ID);
		if (!parseIDList())
			return false;
	}
	else if (currentToken.token_type == OR)
	{
		match(OR);
		
		if (!parseRHS())
			return false;
	}
	else if (currentToken.lexeme == "")
		return true;

	return false;
}

bool Parser::parseRule()
{
	if (currentToken.token_type == ID)
	{
		match(ID); //consume ID
		match(ARROW); //consume '->'
		if (!parseRHS())
			return false;
		match(STAR);
		return true;
	}

	return false;
}

bool Parser::parseRHS()
{
	if (!parseIDList())
		return false;
	else
	{
		if (!parseIDList())
			return false;
		match(OR); // consume '|'
		if (!parseRHS())
			return false;
	}

	return true;
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