/**********

Lexical Analyzer

Group Members: Duc Nguyen, Wayne Lin

**********/

#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

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
	COMMENT,
	SPACE
};

// State table for transvering through the lexeme
// A string can either be a KEYWORD or an IDENTIFIER
int stateTable[][9] = {
	{0, INTEGER, REAL, SEPERATOR, OPERATOR, STRING, UNKNOWN, COMMENT, SPACE},
	{INTEGER, INTEGER, REAL, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
	{REAL, REAL, UNKNOWN, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
	{SEPERATOR, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
	{OPERATOR, REJECT, REJECT, REJECT, OPERATOR, STRING, REJECT, REJECT, REJECT},
	{STRING, STRING, STRING, REJECT, STRING, STRING, REJECT, REJECT, REJECT},
	{UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, REJECT, UNKNOWN},
	{SPACE, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT}};

/*
    A struct to hold a token information
*/
struct Token
{
	int lexemeNum = 0;
	string token = "";
	string lexemeName = "";
};

// Set to hold seperators
static const unordered_set<char> SEPERATORS({'{', '}', '[', ']', '(', ')', ',', '.', ';', ':'});
// Set to hold operators
static const unordered_set<char> OPERATORS({'+', '-', '*', '/', '=', '<', '>', '%'});
// Set to hold special characters
// static const unordered_set<char> SPECIALS({'!', '_'});
// Set to hold keywords
static const unordered_set<string> KEYWORDS({"int", "float", "bool", "True", "False",
											 "if", "else", "then", "endif", "endelse", "while", "whileend", "do",
											 "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"});

/* PROTOTYPES FOR THE FUNCTIONS */
vector<Token> lexer(string);
int getCharState(char);
string getLexemeName(int, string);

int main()
{
	// Declare variables for reading file
	ifstream infile;
	string fileName;
	string line;

	// A vector hold the tokens
	vector<Token> tokens;

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

	// Get each line in the text file
	while (getline(infile, line))
	{
		tokens = lexer(line);

		// Display the tokens to the screen
		for (unsigned x = 0; x < tokens.size(); ++x)
		{
			if (tokens[x].lexemeNum != COMMENT)
			{
				cout << tokens[x].lexemeName << "  \t"
					 << tokens[x].token << endl;
			}
		}
	}

	// Close the file
	infile.close();
	infile.clear();

	return 0;
}

/*
*	FUNCTION: lexer
* USE: go through the string to return a list of tokens.
* @param expression - the code line
* @return - a list of tokens
*/
vector<Token> lexer(string expression)
{
	// Variable for the state machine
	Token access;
	vector<Token> tokens;
	string currentToken = "";
	char currentChar = ' ';
	int charState = REJECT;
	int currentState = REJECT;
	int prevState = REJECT;

	// Go through each character
	for (unsigned x = 0; x < expression.length();)
	{

		// Check the current state of the character
		currentChar = expression[x];
		charState = getCharState(currentChar);

		// Get the current state
		currentState = stateTable[currentState][charState];

		// If the current state is REJECT, we have started a new parse,
		// and the old one is succesfull.
		// If not, continue finding a token
		if (currentState == REJECT)
		{
			if (prevState != SPACE) // Ignore whitespace
			{
				access.token = currentToken;
				access.lexemeNum = prevState;
				access.lexemeName = getLexemeName(access.lexemeNum, access.token);
				tokens.push_back(access);
			}
			currentToken = "";
		}
		else
		{
			currentToken += currentChar;
			++x;
		}

		// Set the previous state before going to a new state
		prevState = currentState;

		// Make sure the last token is included
		if (currentState != SPACE && currentToken != "")
		{
			access.token = currentToken;
			access.lexemeNum = currentState;
			access.lexemeName = getLexemeName(access.lexemeNum, access.token);
			tokens.push_back(access);
		}

		// Return the list of tokens
		return tokens;
	}
};

/*
* FUNCTION: getCharState
* USE: get the corresponding state based on the current character
* @param currentChar - the current char
* @return int - the current state of the character
*/
int getCharState(char currentChar)
{
	// Check for whitespace
	if (isspace(currentChar))
	{
		return SPACE;
	}

	// Check for integer numbers
	else if (isdigit(currentChar))
	{
		return INTEGER;
	}

	// Check for real numbers
	else if (currentChar == '.')
	{
		return REAL;
	}

	// Check for comments
	else if (currentChar == '!')
	{
		return COMMENT;
	}

	// Check for characters
	else if (isalpha(currentChar) || currentChar == '_')
	{
		return STRING;
	}

	// Check for seperators
	else if (SEPERATORS.find(currentChar) != SEPERATORS.end())
	{
		return SEPERATOR;
	}

	// Check for operators
	else if (OPERATORS.find(currentChar) != OPERATORS.end())
	{
		return OPERATOR;
	}

	return UNKNOWN;
}

/*
*	FUNCTION: getLexemeName
* USE: takes a number and return the corresponding string to the token type.
* @param lexemeNum - integer that corresponds to the state
* @return - a string of the state
*/
string getLexemeName(int lexemeNum, string token)
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
		return (KEYWORDS.find(token) != KEYWORDS.end() ? "KEYWORD" : "IDENTIFER");
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