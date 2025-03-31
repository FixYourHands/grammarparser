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

	ret += "Nullable = {";
	int count = 0;
	for (int i = 0; i < nonTerminals.size(); i++)
	{
		string LHS = nonTerminals[i];
		if (nullableMap[LHS] == TRUE)
		{
			ret += LHS;
			count++;
			if (count == nullCount)
				break;

			ret += ", ";
		}
	}
	

	ret += "}";
	return ret;
}

string Parser::executeTaskThree()
{
	string ret = "";
	unordered_map<string, bool> seen;
	vector<string> temp;
	for (auto& n : nonTerminals)
	{
		string LHS = n;
		int size = firstSets[LHS].size();
		if (size == 0) //don't add to output if firstSet is empty
			continue;
		ret += "FIRST(" + LHS + ") = {";
		for (auto& t : terminals)
		{
			if (firstSets[LHS].find(t) != firstSets[LHS].end())
			{
				size--;
				ret += t;
				if (size > 0)
				{
					ret += ", ";
				}
			}
			
		}
		ret += "}\n";
	}
	


	return ret;
}

string Parser::executeTaskFour()
{
	string ret = "";
	for (auto& n : nonTerminals)
	{
		string LHS = n;
		int size = followSets[LHS].size();
		if (size == 0) //don't add to output if firstSet is empty
			continue;
		cout << "Follow(" +LHS + ") = {";
		ret += "Follow(" + LHS + ") = {";
		for (auto& s : followSets[LHS])
		{
			size--;
			cout << s;

			if (size > 0)
			{
				cout << ", ";
				ret += ", ";
			}
		}
		cout << "}\n";
		ret += "}\n";
	}
	if (!ret.empty() && ret.back() == '\n') {
		ret.pop_back();
	}

	return ret;
}

string Parser::executeTaskFive()
{
	string ret = "";
	vector<Rules> g = leftFactor();
	sort(g.begin(), g.end(), [](const Rules& a, const Rules& b)
		{
			return a.LHS < b.LHS;
		});

	for (auto& r : g)
	{
		for (int i = 0; i < r.RHS[0].size(); i++)
		{
			if (r.RHS[0].back() == "*")
				r.RHS[0].back() = "#";			
		}

	}

	for (auto& r : g)
	{
		for (auto& w : r.RHS[0])
		{
			if (r.RHS[0].back() != "#")
			{
				r.RHS[0].push_back("#");
				break;
			}
		}
	}

	for (auto& r : g)
	{
		//cout << r.LHS + "-> ";
		ret += (r.LHS + "-> ");
		for (int i = 0; i < r.RHS[0].size(); i++)
		{
			//cout << r.RHS[0][i] + " ";
			ret += r.RHS[0][i] + " ";
		}
		//cout << "\n";
		ret += "\n";
	}
	return ret;
}

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
		ret.push_back(string() + c); //convert back to vector
	}

	return ret;
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

vector<vector<string>> Parser::sortSetList(set<vector<string>>& set) //sort vectors by longest
{
	vector<vector<string>> list(set.begin(), set.end());
	sort(list.begin(), list.end(), [](const vector<std::string>& a, const vector<std::string>& b)
		{
			return a.size() > b.size();
		});

	return list;
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

vector<Rules> Parser::leftFactor()
{
	
	unordered_map<string, set<vector<string>>> commonPrefixes = getCommonPrefixes();
	vector<Rules> updatedGrammar;
	unordered_map<string, bool> passthrough; //checking if a rule has already been added to the list
	int longestMatch = 0;
	int variableCount = 1;
	set<string> seen;
	Rules rule2add;
	for (auto& pair : commonPrefixes)
	{
		vector<vector<string>> sorted = sortSetList(pair.second);
		for (auto& v : sorted) //iterate through each vector in set
		{
			for (int i = 0; i < singleLineRules.size(); i++)
			{
				string LHS = singleLineRules[i].LHS;
				Rules currRule = singleLineRules[i];
				vector<string> vec = v;
				if (vec.back() != "*")
				{
					vec.push_back("#"); //add * to end of each rule
				}
				if (commonPrefixes.find(LHS) == commonPrefixes.end()) //if rule does not have any rules that need fixing
				{
					if (!passthrough[LHS])
					{
						updatedGrammar.push_back(singleLineRules[i]);
						seen.insert(LHS);
					}
					

				}
				else
				{
					if (longestCommonPrefix(singleLineRules[i].RHS[0], vec) == v) //check if current rule contains prefix
					{
						int size = v.size();
						currRule.LHS = LHS + to_string(variableCount); //update rule name

						if (!passthrough[currRule.LHS])
						{
							Rules rule;
							rule.LHS = LHS, rule.RHS.push_back(v);
							rule.RHS[0].push_back(currRule.LHS);
							rule.RHS[0].push_back("#");

							rule2add = rule;
							passthrough[currRule.LHS] = true;

						}
						currRule.RHS[0].erase(currRule.RHS[0].begin(), currRule.RHS[0].begin() + size); //remove prefix
						
						updatedGrammar.push_back(currRule); //add updated rule to list
						singleLineRules[i].RHS[0].clear();
						
					}
					continue;
				}
			}

			singleLineRules.push_back(rule2add);
			for (auto& s : seen)
			{
				passthrough[s] = true;
			}
			variableCount++;
		}
		variableCount = 1;
	}
	updatedGrammar.push_back(rule2add);


	

	return updatedGrammar;
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
						if (contains(next,'t') || nullableMap[next] != TRUE)
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
	else if (currentToken.token_type == STAR)
	{
		match(STAR);
		return true;
	}
		

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