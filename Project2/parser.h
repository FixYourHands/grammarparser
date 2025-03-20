#ifndef PARSER
#define PARSER
#include "lexer.h"
#include "unordered_map"
#include <set>


using namespace std;
struct Rules
{
	string LHS;
	vector<vector<string>> RHS;
	bool nullable;
};

class Parser
{
public:
	Parser(LexicalAnalyzer& lex);
	LexicalAnalyzer& lexer;
	void executeTaskOne();
	vector<string> getTerminals();
	vector<string> getNonTerminals();

	
private:
	Token currentToken;
	vector<string> nonTerminals;
	vector<string> terminals;
	vector<Rules> rules;
	unordered_map<string, set<string>> firstSets;
	unordered_map<string, set<string>> followSets;
	void storeRules();
	void storeNonTerminals();
	void storeTerminals();
	void match(TokenType token);
	bool parseGrammar();
	bool parseRuleList();
	bool parseIDList();
	bool parseRule();
	bool parseRHS();
	bool contains(const string& string);
	
};
#endif