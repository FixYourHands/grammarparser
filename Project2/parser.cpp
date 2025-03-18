#include "parser.h"

Parser::Parser(LexicalAnalyzer& lex)
{
	lexer = lex;
	storeNonTerminals();
	storeTerminals();
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
			temp.push_back(t.lexeme);
			counter++;
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
			if (t.lexeme != "*" && t.lexeme != "|" || t.lexeme != "#")
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