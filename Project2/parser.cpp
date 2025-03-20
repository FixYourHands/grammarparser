#include "parser.h"
#include <iostream>
#include <algorithm>


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

	cout << "test\n";
	return;
}

Parser::Parser(LexicalAnalyzer& lex)
	:lexer(lex)
{
	vector<int> arrows;
	storeNonTerminals();
	storeTerminals();

	storeRules();
	currentToken = lexer.GetToken();
	parseGrammar();


	cout << "passed\n";
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

void Parser::executeTaskOne()
{
	for (auto w : terminals)
	{
		cout << w << " ";
	}

	for (auto w : nonTerminals)
	{
		cout << w << " ";
	}

	cout << endl;
}

vector<string> Parser::getNonTerminals()
{
	return nonTerminals;
}

vector<string> Parser::getTerminals()
{
	return terminals;
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

bool Parser::contains(const string& string)
{
	if (find(nonTerminals.begin(), nonTerminals.end(), string) != nonTerminals.end())
	{
		return true;
	}

	return false;
}