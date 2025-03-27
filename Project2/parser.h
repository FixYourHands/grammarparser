#ifndef PARSER
#define PARSER
#include "lexer.h"
#include "unordered_map"
#include <set>
#include <stack>


enum NULLABLE_STATUS
{
	UNREAD,FALSE, TRUE,POTENTIAL
};
using namespace std;
struct Rules
{
	string LHS;
	vector<vector<string>> RHS;
	NULLABLE_STATUS nullable = FALSE;
};

class Parser
{
public:
	
	Parser(LexicalAnalyzer& lex);
	string executeTaskOne();
	string executeTaskTwo();
	string executeTaskThree();
	string executeTaskFour();
	string executeTaskFive();
	string executeTaskSix();
	vector<string> getTerminals();
	vector<string> getNonTerminals();

	
private:
	Token currentToken;
	vector<string> nonTerminals;
	vector<string> terminals;
	vector<Rules> rules;
	vector<Rules> singleLineRules; // all rules listed line by line instead of separated by |
	LexicalAnalyzer& lexer;
	unordered_map<string, set<string>> firstSets;
	unordered_map<string, set<string>> followSets;
	unordered_map<string, NULLABLE_STATUS> nullableMap;
	NULLABLE_STATUS getNullableStatus(string word);
	void storeRules();
	void storeNonTerminals();
	void storeTerminals();
	vector<Rules> fixedRuleList();
	void match(TokenType token);
	string leftFactor();
	void getNullable();
	void storeFirstSets();
	void storeFollowSets();
	bool parseGrammar();
	bool parseRuleList();
	bool parseIDList();
	bool parseRule();
	bool parseRHS();
	bool contains(const string& string,char key);
	vector<string> longestCommonPrefix(vector<string>& v1, vector<string>& v2);
	unordered_map<string, set<vector<string>>> getCommonPrefixes();
	
};
#endif