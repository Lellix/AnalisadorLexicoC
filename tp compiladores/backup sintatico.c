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

void consumeToken(char* funcao){

	if(currentToken->next == NULL) exit(1);

	printf(". . . . . %s  . . . . . last token:%s - ", funcao, currentToken->tok.name);
	currentToken = currentToken->next;
	printf("current token:%s \n", currentToken->tok.name);
}

void printError(char* expected, char* found){

	printf("\n");
	printf("x ----------------------------------------- x\n");
	foreground(RED);
	printf(" \t\tERROR DETECTED ");
	foreground(WHITE);
	printf("\n \n  Token: ");
	foreground(GREEN);
	printf("%s\n",currentToken->tok.name);
	foreground(WHITE);
	printf("  Expected: ");
	foreground(GREEN);
	printf("%s\n", expected);
	foreground(WHITE);
	printf("  Found: ");
	foreground(RED);
	printf("%s\n", found);
	foreground(WHITE);
	printf("  Position: [%d, %d] \n",currentToken->tok.line, currentToken->tok.column);
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

			consumeToken("function");

			if(currentToken->tok.group == 4 && currentToken->tok.type == 2){ // (

				consumeToken("function");
				argumentsList(); // <argumentsList>

				if(verification == TRUE){

					if(currentToken->tok.group == 4 && currentToken->tok.type == 3){ // )

						consumeToken("function");
						printf("%s\n", currentToken->tok.name);
						stmScope();

						return;

					} else {

						verification = FALSE;
						printError(")", currentToken->tok.name);

					}

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

		consumeToken("type");
		verification = TRUE;
		return;

	} else {

		verification = FALSE;
		return;

	}

}

void argumentsList(){

	argument();

	if(verification = TRUE){
		if(currentToken->tok.group == 4 && currentToken->tok.type == comma){
			consumeToken("argumentsList");
			argumentsList();

		}else{

			return;	

		}
	}  else {

		verification = FALSE;
		return;
	}

}

void argument(){

	type();

	if(verification == TRUE){
		if(currentToken->tok.group = identifiers){
			consumeToken("argument");
			verification = TRUE;
			return;

		} else {

			printError("identifier", ID2Name(currentToken->tok.group));
		}
	} else {
		verification = FALSE;
		return;
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

		consumeToken("identifiersList");
		if(currentToken->tok.name == ","){
			consumeToken("identifiersList");
			identifiersList();
		}

	} else {

		printError("identifier", ID2Name(currentToken->tok.group));
	}

}

void statement(){

	verification = FALSE;

	//printf("aaa %d %d -------------- \n",currentToken->tok.group, currentToken->tok.type);

	printf(" - for %d\n",verification);
	stmFor();

	printf(" -- while %d\n",verification);
	if(verification == FALSE){
		stmWhile();
	}

	printf(" --- declaracao %d\n",verification);
	if(verification == FALSE){
		declaration();
		printf(" ---- expressao %d\n",verification);
		if(verification == FALSE){
			expression1();
			if(verification == TRUE){
				if(currentToken->tok.group == separators && currentToken->tok.type == semicolon){
					consumeToken("statement - expressao");;
					return;
				} else {
					printError(";", currentToken->tok.name);
				}
			}
		}
	}

	printf(" ----- if %d\n",verification);
	if(verification == FALSE){
		stmIf();
	}

	printf(" ------ escopo %d\n",verification);
	if(verification == FALSE){
		if(currentToken->tok.group == 4 && currentToken->tok.type == 4){
			stmScope();
		}
	}

	printf(" ------- do while %d\n",verification);
	if(verification == FALSE){
		stmDoWhile();
	}

	printf(" -------- return %d\n",verification);
	if(verification == FALSE){
		if(currentToken->tok.group == 0 && currentToken->tok.type == 11){
			stmReturn();
			if(verification == TRUE){
				if(currentToken->tok.name == ";"){
					consumeToken("statement - return");
				} else {
					printError(";", currentToken->tok.name);
				}
			}
		}
	}

	printf(" --------- break %d\n",verification);
	if(verification == FALSE){
		if(currentToken->tok.group == 0 && currentToken->tok.type == 13){
			consumeToken("statement = break");
			verification = TRUE;	
			if(currentToken->tok.name == ";"){
				consumeToken("statement = break");
			} else {
				printError(";", currentToken->tok.name);
			}
		}
	}

	printf(" ----------- ; %d\n",verification);
	if(verification == FALSE){
		if(currentToken->tok.group == 4 && currentToken->tok.type == semicolon){
			consumeToken("statement - ;");
			verification = TRUE;
		}
	}

	return;
}

void stmIf(){

	if(strcmp(currentToken->tok.name, "if") == 0){
		consumeToken("stmIf");
		if(currentToken->tok.group == separators && currentToken->tok.type == leftParenthesis){
			consumeToken("stmIf");
			expression1();

			if(currentToken->tok.group == separators && currentToken->tok.type == rightParenthesis){
				consumeToken("stmIf");
				statement();
				if(verification == TRUE) {
					printf("saiu do IF\n");
					elseFragment();
				}
			}  else {
				printError(")", currentToken->tok.name);
			}

		} else {
			printError("(", currentToken->tok.name);
		}
	}

}

void stmFor(){

	if(currentToken->tok.group == keywords && currentToken->tok.type == forKey){

		printf(" ------------ FOR ---------------- \n");
		consumeToken("stmFor");
		if(currentToken->tok.group == 4 &&currentToken->tok.type == leftParenthesis)
		{
			consumeToken("stmFor");
			expression1();
			if(verification == TRUE){
				if(currentToken->tok.group == 4 &&currentToken->tok.type == semicolon){

					consumeToken("stmFor");
					optionalExpression();
					if(verification == TRUE){
						if(currentToken->tok.group == 4 &&currentToken->tok.type == semicolon){

							consumeToken("stmFor");
							optionalExpression();
							if(verification == TRUE){

								if(currentToken->tok.group == 4 &&currentToken->tok.type == rightParenthesis){

									consumeToken("stmFor");
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

	if(currentToken->tok.group == 0 && currentToken->tok.type == 7){
		consumeToken("stmWhile");
		if(currentToken->tok.group == 4 && currentToken->tok.type == leftParenthesis){
			consumeToken("stmWhile");
			expression1();
			if(verification == TRUE){
				if(currentToken->tok.group == 4 &&currentToken->tok.type == rightParenthesis){
					consumeToken("stmWhile");
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

void stmDoWhile(){

	if(strcmp(currentToken->tok.name, "do") == 0){

		consumeToken("stmDoWhile");
		stmScope();
		if(verification == TRUE){
			if(strcmp(currentToken->tok.name, "while")){
				consumeToken("stmDoWhile");
				if(currentToken->tok.group == 4 && currentToken->tok.type == leftParenthesis){
					consumeToken("stmDoWhile");
					expression1();
					if(verification == TRUE){
						if(currentToken->tok.group == 4 && currentToken->tok.type == rightParenthesis){
							consumeToken("stmDoWhile");
							return;
						} else {
							verification = FALSE;
							return;
						}
					}
				} else {
					verification = FALSE;
				}
			} else {
				verification = FALSE;
			}
		}
	} else {
		verification = FALSE;
		return;
	}

}

void elseFragment(){

	if(currentToken->tok.group == keywords && currentToken->tok.type == elseKey){
		consumeToken("elseFragment");
		statement();
		verification = TRUE;
		return;
	} else {
		verification = FALSE;
		return;
	}
}

void stmReturn(){
	
	if((currentToken->tok.group == 1) ||
		(currentToken->tok.group == 102) ||
		(currentToken->tok.group == 7)){
		consumeToken("stmReturn");
		verification = TRUE;
		return;
	} else {
		return;
	}

}

void stmScope(){

	if(currentToken->tok.group == separators && currentToken->tok.type == leftBrace){
		consumeToken("stmScope");
		stmList();

		if(verification == TRUE){

			if(currentToken->tok.group == separators && currentToken->tok.type == rightBrace){

				consumeToken("stmScope");
				
				verification = TRUE;
				return;
			} else {
				printError("}", currentToken->tok.name);
				verification = FALSE;
				return;
			}
		}
	} else {
		verification = FALSE;
		return;
	}

}

void stmList(){

	statement();

	if(verification == TRUE){
		//consumeToken("stmList 1");
		stmListLine();
	} else {
		verification = TRUE;
		consumeToken("stmList 2");
		stmListLine();
	}
}

void stmListLine(){

	statement();

	if(verification == TRUE){
		stmListLine();
		return;
	} else {
		verification = TRUE;
		//consumeToken("stmListLine");
		return;
	}

}

void expression1(){

	type();

	if(currentToken->tok.group == identifiers){

		consumeToken("expression1");
		expression2();
		//verification = TRUE;
	} else {
		valueR();
	}

}

void expression2(){

	if(currentToken->tok.group == logicalOperators && currentToken->tok.type == atrib){

		consumeToken("expression2");
		expression1();
		return;
	} else {

		comparation();

		if(verification == TRUE){
			expression1();
		}

		if(verification == FALSE){
			arithmetic();
			if(verification == TRUE){
				expression1();
			}
		}

		if(verification == FALSE){
			duplicatedOperatorArithmetic();
			if(verification == TRUE){
				expression1();
			}
		}

		if(verification == FALSE){
			composedOperatorArithmetic();
			if(verification == TRUE){
				expression1();
			}
		} 

	} 
	
}

void valueR(){

	comparation();
	if(verification == TRUE){
		magnitude();
		if(verification == TRUE){
			valueRLine();
		}
	} else {
		magnitude();
	}
}

void valueRLine(){

	comparation();
	if(verification == TRUE){
		magnitude();
		if(verification == TRUE){
			valueRLine();
		}
	}

}

void comparation(){

	if(currentToken->tok.group == logicalOperators){
		consumeToken("comparation");
		verification = TRUE;
	}
}

void magnitude(){

	term();
	if(verification == TRUE){
		magnitudeLine();
	}

}

void term(){

	factor();
	if(verification == TRUE){
		termLine();
	}

}

void magnitudeLine(){

	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == plus){
		consumeToken("magnitudeLine");
		term();
		if(verification == TRUE){
			magnitudeLine();
		}
	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == minus){
		consumeToken("magnitudeLine");
		term();
		if(verification == TRUE){
			magnitudeLine();
		}
	} else {
		verification = TRUE;
		return;
	}

}

void termLine(){

	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == times){

		consumeToken("termLine");
		factor();
		if(verification == TRUE){
			termLine();
		}

	} else 	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == division){

		consumeToken("termLine");
		factor();
		if(verification == TRUE){
			termLine();
		}

	} else {

		verification =  TRUE;
		return;
	}

}

void factor(){

	if(currentToken->tok.group == 4 && currentToken->tok.type == leftParenthesis){

		consumeToken("factor");
		expression1();

		if(currentToken->tok.group == 4 && currentToken->tok.type == rightParenthesis){

			consumeToken("factor");
			verification = TRUE;

		} else {
			printError(")", currentToken->tok.name);
		}
	} else if(currentToken->tok.group == 4 && currentToken->tok.type == plus){

		consumeToken("factor");
		factor();

	} else if(currentToken->tok.group == 4 && currentToken->tok.type == minus){

		consumeToken("factor");
		factor();

	} else if(currentToken->tok.group == identifiers){

		consumeToken("factor");
		verification = TRUE;

	} else if(currentToken->tok.group == number){

		consumeToken("factor");
		verification = TRUE;

	} else {
		verification = FALSE;
	}

}

void arithmetic(){

	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == plus){

		consumeToken("arithmetic");
		verification = TRUE;

	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == minus){

		consumeToken("arithmetic");
		verification = TRUE;

	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == times){

		consumeToken("arithmetic");
		verification = TRUE;

	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == division){

		consumeToken("arithmetic");
		verification = TRUE;

	} else {
		verification = FALSE;
	}

}

void duplicatedOperatorArithmetic(){

	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == increment){

		consumeToken("duplicatedOperatorArithmetic");
		verification = TRUE;

	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == decrease){

		consumeToken("duplicatedOperatorArithmetic");
		verification = TRUE;

	} else {
		verification = FALSE;
	}

}

void composedOperatorArithmetic(){

	duplicatedOperatorArithmetic();

}

