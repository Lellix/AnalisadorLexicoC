#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bib.h"

#define TRUE  1
#define FALSE 0

typedef struct node{

	Token token;
	struct List* next;
	int empty;

} Node;

char* ID2Name(int ID);

void consumeToken(char* label);

void printError(char* expected, char* found);

void directiveList();

void language(list* tokenslist);

void function(char* label);

void type(char* label);

void argumentsList(char* label);

void argument(char* label);

void declaration(char* label);

void identifiersList(char* label);

void statement(char* label);

void stmFor();

void optionalExpression(char* label);

void stmWhile();

void stmDoWhile();

void elseFragment();

void stmReturn(char* label);

void stmScope(char* label);

void stmList(char* label);

void stmListLine(char* label);

void expression1(char* label);

void expression2(char* label);

void valueR(char* label);

void valueRLine(char* label);

void comparation(char* label);

void magnitude(char* label);

void term(char* label);

void magnitudeLine(char* label);

void termLine(char* label);

void factor(char* label);

void arithmetic(char* label);

void duplicatedOperatorArithmetic(char* label);

void composedOperatorArithmetic(char* label);

void stmIf();

void dataFile();

void codeGeneratorAtrib(char* label);