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

void consumeToken();

void printError(char* expected, char* found);

void directiveList();

void language(list* tokenslist);

void function();

void type();

void argumentsList();

void argument();

void declaration();

void identifiersList();

void statement();

void stmFor();

void optionalExpression();

void stmWhile();

void stmDoWhile();

void elseFragment();

void stmReturn();

void stmScope();

void stmList();

void stmListLine();

void expression1();

void expression2();

void valueR();

void valueRLine();

void comparation();

void magnitude();

void term();

void magnitudeLine();

void termLine();

void factor();

void arithmetic();

void duplicatedOperatorArithmetic();

void composedOperatorArithmetic();

void stmIf();