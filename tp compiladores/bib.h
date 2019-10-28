#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* GROUPS */

#define keywords             0
#define identifiers          1
#define arithmeticOperators  2
#define logicalOperators     3
#define separators           4
#define comment              5
#define bitwise 			 6
#define number               7
#define header				 8
#define include              101
#define literal              102
#define define               103
#define error				 404

/* ---------------------------------------------------------------------- */

/* KEYS */

/* keywords */
#define intKey              0
#define charKey    			1
#define floatKey    		2
#define doubleKey   		3
#define longKey    			4
#define unsignedKey    		5
#define forKey    			6
#define whileKey    		7
#define doKey    			8
#define ifKey    			9
#define elseKey    			10
#define returnKey    		11
#define continueKey    		12
#define breakKey    		13
#define caseKey    			14
#define switchKey   		15
#define defaultKey    		16
#define typedefKey    		17
#define structKey    		18
#define includeKey    		19
#define defineKey    		20
#define constantKey    		21
#define asmKey    			22
#define autoKey    			23
#define enumKey    			24
#define externKey    		25
#define gotoKey    			26
#define registerKey    		27
#define shortKey    		28
#define signedKey    		29
#define sizeofKey    		30
#define staticKey    		31
#define unionKey    		32
#define volatileKey    		33
#define mallocKey			34
#define callocKey    		35
#define freeKey	    		36

/* arithmetic operators */
#define plus    			0 // +
#define minus    			1 // -
#define times    			2 // *
#define division    		3 // /
#define increment    		4 // ++ 
#define decrease    		5 // --

/* logical and relacional operators */
#define isMoreThan    		0 // >
#define isLessThan    		1 // <
#define atrib    			2 // =
#define andOperator   		3 // &&
#define orOperator    		4 // ||
#define notOperator    		5 // !=
#define equals    			6 // ==
#define isMoreOrEqual       7 // >=
#define isLessOrEqual       8 // <=
#define not 				9 // !
#define ternary				10// ?:


/* separators */
#define comma    			0 // ,
#define semicolon   		1 // ;
#define leftParenthesis    	2 // (
#define rightParenthesis    3 // )
#define leftBrace    		4 // {
#define rightBrace    		5 // }
#define leftBracket    		6 // [
#define rightBracket    	7 // ]
#define colon    			8 // :

/* bitwise */
#define comercial			0 // &
#define bwor				1 // |
#define bwxor				2 // ^
#define bwnot				3 // ~
#define shiftRight			4 // >>
#define shiftLeft			5 // <<

/* ---------------------------------------------------------------------- */

/* STRUCTURES */

typedef struct token{

	int group; // 0 - keyword | 1 - identifier | 2 - arithmetic operator | 3 - logical operator | 4 - separator | 5 - comment
	int type; //
	int position[2];
	char* name;

}Token;

typedef struct List{

	Token tok;
	struct List* next;
	int empty;

}list;

void cleanString(char* string);
void lexicalAnalyzer(list* tokensList, char* line, int lineNumber);
int isLiteral(char* line, int *i, list* tokensList, int lineNumber, Token token);
int isComment(char* line, int *i, int size, list* tokensList, int lineNumber, Token token);
int isKeywords(char* unidentifiedToken, list* tokensList, int column, int lineNumber, Token token);
int isIdentifierOrNumber(char* unidentifiedToken, list* tokensList, int column, int lineNumber, Token token);
int MatchOperator(char unidentifiedOperator, list* tokensList, char next, int *i, int lineNumber, Token token);
int MatchSeparator(char unidentifiedSeparator, list* tokensList, int column, int lineNumber, Token token);
void ChooseType(char* unidentifiedToken, list* tokensList, int i, int lineNumber, Token token);

list *createList();
void insert(list* tokensList, Token token);
void readFile(char* fileName);