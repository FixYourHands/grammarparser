#ifndef PARSER
#define PARSER
#include "lexer.h"

using namespace std;
class Parser
{
public:
	Parser(LexicalAnalyzer& lex);
	LexicalAnalyzer lexer;	
	
private:
	vector<string> nonTerminals;
	vector<string> terminals;
	void storeNonTerminals();
	void storeTerminals();
};
#endif