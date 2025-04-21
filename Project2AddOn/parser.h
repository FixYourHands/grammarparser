#ifndef PARSER
#define PARSER
#include "lexer.h"
#include "unordered_map"
#include <set>
#include <stack>


enum NULLABLE_STATUS
{
	UNREAD, FALSE, TRUE, POTENTIAL
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

	Parser();
	//Parser();



private:
	string inputString;
	Token currentToken;
	vector<string> nonTerminals;
	vector<string> terminals;
	vector<Rules> rules;
	vector<Rules> singleLineRules; // all rules listed line by line instead of separated by |
	LexicalAnalyzer lexer;
	unordered_map<string, unordered_map<string, vector<string>>> parseTable;
	unordered_map<string, set<string>> firstSets;
	unordered_map<string, set<string>> followSets;
	unordered_map<string, NULLABLE_STATUS> nullableMap;
	void storeRules();
	void storeNonTerminals();
	void storeTerminals();
	void getNullable();
	void storeFirstSets();
	void storeFollowSets();
	void buildParseTable();
	void parseString();
	bool match(string word);


	bool contains(const string& string, char key);

};
#endif