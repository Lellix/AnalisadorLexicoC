#include "sintatico.h"

int verification = FALSE;

list* tokensList = NULL;
list* currentToken = NULL;

/* int to char */
char* ID2Name(int ID){

	char* name;

	if(ID == 0){
		name = "keyword";
	} else if(ID == 1){
		name = "identifier";
	} else if(ID == 2){
		name = "arithmeticOperator";
	} else if(ID == 3){
		name = "logicalOperatos";
	} else if(ID == 4){
		name = "separator";
	} else if(ID == 5){
		name = "comment";
	} else if(ID == 6){
		name = "bitwise";
	} else if(ID == 7){
		name = "number";
	} else if(ID == 8){
		name = "header";
	} else if(ID == 101){
		name = "include";
	} else if(ID == 102){
		name = "literal";
	} else if(ID == 103){
		name = "define";
	} 

	return name;
}

void consumeToken(){

	currentToken = currentToken->next;

}

void printError(char* expected, char* found){

	printf("\n");
	printf("x ----------------------------------------- x\n");
	printf("| Token: %s\n",currentToken->tok.name);
	printf("| Expected: %s\n", expected);
	printf("| Found: %s\n", found);
	printf("| Position: [%d, %d] \n",currentToken->tok.line, currentToken->tok.column);
	printf("x ----------------------------------------- x\n");
	printf("\n");

}

void directiveList(){}

void language(list* tokenslist){

	tokensList = tokenslist;
	currentToken = tokensList->next;

	while(currentToken != NULL){

		function();

	}

}

void function(){

	// <type>
	type();

	if(verification == TRUE){
		// identifier
		if(currentToken->tok.group == identifiers){

			consumeToken();

			if(currentToken->tok.group == 4 && currentToken->tok.type == 2){ // (

				consumeToken();
				argumentsList(); // <argumentsList>

				if(verification == TRUE){

					if(currentToken->tok.group == 4 && currentToken->tok.type == 3){

						consumeToken();

					} else {

						verification = FALSE;
						printError(")", currentToken->tok.name);

					}

					//<statementEscope>
					stmScope();

				}

			} else {

				printError("(", currentToken->tok.name);
				verification = FALSE;

			}

		} else {

			printError("identifier", ID2Name(currentToken->tok.group));
			verification = FALSE;

		}
	}
}

void type(){

	if((currentToken->tok.group == keywords) && 
		((currentToken->tok.type >=0 && currentToken->tok.type <=3) ||
		currentToken->tok.type == voidKey)){

		consumeToken();
		verification = TRUE;
		return;

	} else {

		printError("type", currentToken->tok.name);
		verification = FALSE;
		return;

	}

}

void argumentsList(){

	argument();

	if(currentToken->tok.name == ","){

		consumeToken();
		argumentsList();

	}else{

		return;	

	}

}

void argument(){

	type();

	if(verification == TRUE){
		if(currentToken->tok.group = identifiers){

			consumeToken();

		} else {

			printError("identifier", ID2Name(currentToken->tok.group));
		}
	}

}

void declaration(){

	type();

	if(verification == TRUE){
		identifiersList();
	}

}

void identifiersList(){

	if(currentToken->tok.group == identifiers){

		consumeToken();
		if(currentToken->tok.name == ","){
			consumeToken();
			identifiersList();
		}

	} else {

		printError("identifier", ID2Name(currentToken->tok.group));
	}

}

void statement(){

	stmFor();

	if(verification == FALSE){
		stmWhile();
	}

	if(verification == FALSE){
		expression1();
		if(verification == TRUE){
			if(currentToken->tok.name == ";"){
				consumeToken();
			} else {
				printError(";", currentToken->tok.name);
			}
		}
	}

	if(verification == FALSE){
		stmIf();
	}

	if(verification == FALSE){
		stmScope();
	}

	if(verification == FALSE){
		declaration();
	}

	if(verification == FALSE){
		stmDoWhile();
	}

	if(verification == FALSE){
		if(currentToken->tok.group == 0 &&currentToken->tok.group == 11){
			stmReturn();
			if(verification == TRUE){
				if(currentToken->tok.name == ";"){
					consumeToken();
				} else {
					printError(";", currentToken->tok.name);
				}
			}
		}
	}

	if(verification == FALSE){
		if(currentToken->tok.group == 0 &&currentToken->tok.group == 13){
			consumeToken();
			verification = TRUE;	
			if(currentToken->tok.name == ";"){
				consumeToken();
			} else {
				printError(";", currentToken->tok.name);
			}
		}
	}

	if(verification == FALSE){
		switch1();
	}

	if(verification == FALSE){
		if(currentToken->tok.name == ";"){
			consumeToken();
			verification = TRUE;
		}
	}

}

void stmFor(){

	if(currentToken->tok.group == 0 &&currentToken->tok.group == 6){
		consumeToken();
		if(currentToken->tok.name == "("){
			consumeToken();
			expression1();
			if(verification == TRUE){
				if(currentToken->tok.name == ";"){
					optionalExpression();
					if(verification == TRUE){
						if(currentToken->tok.name == ";"){
							optionalExpression();
							if(verification == TRUE){
								if(currentToken->tok.name == ")"){
									statement();
								} else { 
									verification = FALSE;
									printError(")", currentToken->tok.name);
								}
							}
						} else {
							verification = FALSE; 
							printError(";", currentToken->tok.name);
						}
					}
				} else {
					verification = FALSE;
					printError(";", currentToken->tok.name);
				}
			} 
		} else {
			verification = FALSE;
			printError("(", currentToken->tok.name);
		}
	} else {
		verification = FALSE; 
	}

}

void optionalExpression(){

	expression1();

	if (verification == FALSE){ //empty
		verification = TRUE;
	}

}

void stmWhile(){

	if(currentToken->tok.group == 0 &&currentToken->tok.group == 7){
		consumeToken();
		if(currentToken->tok.name == "("){
			consumeToken();
			expression1();
			if(verification == TRUE){
				if(currentToken->tok.name == ")"){
					consumeToken();
					statement();
				} else {
					verification = FALSE;
					printError(")", currentToken->tok.name);
				}
			}
		} else {
			verification = FALSE;
			printError("(", currentToken->tok.name);
		}
	} else {
		verification = FALSE;
	}

}

void stmDoWhile(){}

void elseFragment(){}

void stmReturn(){}

void stmScope(){}

void stmList(){}

void stmListLine(){}

void expression1(){}

void expression2(){}

void valueR(){}

void valueRLine(){}

void comparation(){}

void magnitude(){}

void term(){}

void magnitudeLine(){}

void termLine(){}

void factor(){}

void arithmetic(){}

void duplicatedOperatorArithmetic(){}

void composedOperatorArithmetic(){}

void switch1(){}

void switch2(){}

void caseList(){}

void caseListLine(){}

void functionCall(){}

void functionCallArgumentsList(){}

void functionCallArgumentsListLine(){}

void stmIf(){}