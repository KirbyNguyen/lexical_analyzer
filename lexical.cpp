/**********

Lexical Analyzer

Group Members: Duc Nguyen, Wayne Lin

**********/

#include <unistd.h>
#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <list>
#include <set>
#include <unordered_set>
#include <map>

using namespace std;

/*
    Enumerator to get lexeme name and to transist the FSM
*/
enum TOKEN_TYPES
{
	REJECT = 0,
	INTEGER,
	REAL,
	SEPERATOR,
	OPERATOR,
	STRING,
	UNKNOWN,
	SPACE
};

/*
    A struct to hold a token information
*/
struct Token
{
	string token;
	int lexemeNum;
	string lexemeName;
};

/*
*	FUNCTION: getLexemeName
* USE: takes a number and return the corresponding string to the token type.
* @param lexemeNum - integer that corresponds to the state
* @return - a string of the state
*/
string getLexemeName(int lexemeNum)
{
	switch (lexemeNum)
	{
	case INTEGER:
		return "INTEGER";
		break;
	case REAL:
		return "REAL";
		break;
	case SEPERATOR:
		return "SEPERATOR";
		break;
	case OPERATOR:
		return "OPERATOR";
		break;
	case STRING:
		return "STRING";
		break;
	case UNKNOWN:
		return "UNKNOWN";
		break;
	case SPACE:
		return "SPACE";
		break;
	default:
		return "ERROR";
		break;
	};
};

int main()
{
	// Declare variables for reading file
	ifstream infile;
	string fileName;
	string line;

	// Data holder for seperators
	static const unordered_set<char> seperators({ '{', '}', '[', ']', '(', ')', ',', '.', ';', ':' });

	// Read the file
	cout << "\nPlease enter the name of the file: ";
	getline(cin, fileName);

	// Open the file
	infile.open(fileName.c_str());

	// Error shooting if the file cannot be opened
	if (infile.fail())
	{
		cout << "\n** ERROR - the file \"" << fileName << "\" cannot be found!\n\n";
		exit(1);
	}

	getline(infile, line);
	// Get each line in the text file
	//while (getline(infile, line))
	//{
		for (int i = 0; i < line.size(); i++)
		{
			cout << "This is a character in the file: " << line[i] << endl;
		}
	//}

	// Close the file
	infile.close();
	infile.clear();

	return 0;
}

void lexer(char ch){
	
};
