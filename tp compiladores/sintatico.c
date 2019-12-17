#include "sintatico.h"

int verification = FALSE;
int cont = 0;
int contTable = 0;
int typeAux;

char* last;

list* tokensList = NULL;
list* currentToken = NULL;

symbol* table;

symbol* CreateTable(){

	last = (char*) malloc(20*sizeof(char));

	symbol* table = (symbol*) malloc(250*sizeof(symbol));
	int i;

	for(i = 0; i < 250; i++){
		table[i].name = (char*) malloc(20*sizeof(char));
		table[i].type = -1;
		table[i].column = -1;
		table[i].line = -1;
	}

	return table;
}

int varAlreadyExists(char* symbName){

	int i;

	if(contTable > 0){
		for(i = 0; i < contTable; i++){
			if(strcmp(table[i].name, symbName) == 0){
				return 1;
			}
		}

	}

	return 0;
}

int returnType(char* symbName){

	int i;
	
	for(i = 0; i < contTable; i++){
		if(strcmp(table[i].name, symbName) == 0){
			return table[i].type;
		}
	}

	return -1;
}

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

	if(currentToken->next == NULL) {
		if(cont == 0){
			printf("\n\t FIM DA EXECUCAO -- NENHUM ERRO ENCONTRADO.\n\n");
		} else {
			printf("\n\t FIM DA EXECUCAO -- %d ERRO(S) ENCONTRADO(S).\n\n", cont);
		}
		exit(1);
	}

	//printf(" last token: %s - ", currentToken->tok.name);
	currentToken = currentToken->next;
	//printf("current token: %s . . . . . %s  . . . . .\n", currentToken->tok.name, funcao);
}

void printError(char* expected, char* found){
	cont++;

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

void directiveList(){

	if(currentToken->tok.group == include || currentToken->tok.group == define){
		consumeToken("diretivas");
		if(!(currentToken->tok.group == header)){
			printError("header", ID2Name(currentToken->tok.group));
			consumeToken("diretivas");
		} 
	}

}

void language(list* tokenslist){

	tokensList = tokenslist;
	currentToken = tokensList->next;

	table = CreateTable();

	directiveList();

	while(currentToken != NULL){

		function();

	}

}

void function(){

	int j;

	type();

	if(verification == FALSE){
		printError("type", ID2Name(currentToken->tok.group));
		consumeToken("function1");
	}

	if(currentToken->tok.group == identifiers){

		consumeToken("function");
	} else {
		printError("identifier", ID2Name(currentToken->tok.group));
		consumeToken("function1");
	}

	if(!(currentToken->tok.group == 4 && currentToken->tok.type == 2)){ // (
		printError("(", currentToken->tok.name);
	} else {
		consumeToken("funcion2");
	}

	argumentsList(); // <argumentsList>

	if(verification == TRUE){

		if(!(currentToken->tok.group == separators && currentToken->tok.type == rightParenthesis)){ // )
			printError(")", currentToken->tok.name);
		} else {
			consumeToken("funcion3");
		} 

		stmScope();

		return;

	}

}

void type(){

	if((currentToken->tok.group == keywords) && 
		((currentToken->tok.type >=0 && currentToken->tok.type <=3) ||
		currentToken->tok.type == voidKey)){

		typeAux = currentToken->tok.type;
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
	int j;
	type();

	if(verification == TRUE){
		if(currentToken->tok.group = identifiers){
			int teste = varAlreadyExists(currentToken->tok.name);
		if(teste == 0){

			table[contTable].type = typeAux;

			for(j = 0; j < strlen(currentToken->tok.name); j++){
				table[contTable].name[j] = currentToken->tok.name[j];
			}

			table[contTable].line = currentToken->tok.line;
			table[contTable].column = currentToken->tok.column;
			contTable++;

		} else {
			printf("Var [%s] already exists! Position[%d, %d]\n", currentToken->tok.name, currentToken->tok.line, currentToken->tok.column);
		}
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
	int j;

	if(currentToken->tok.group == identifiers){

		int teste = varAlreadyExists(currentToken->tok.name);
		if(teste == 0){

			table[contTable].type = typeAux;

			for(j = 0; j < strlen(currentToken->tok.name); j++){
				table[contTable].name[j] = currentToken->tok.name[j];
			}

			table[contTable].line = currentToken->tok.line;
			table[contTable].column = currentToken->tok.column;
			contTable++;

		} else {
			printf("Var [%s] already exists! Position[%d, %d]\n", currentToken->tok.name, currentToken->tok.line, currentToken->tok.column);
		}

		consumeToken("identifiersList");

		if(currentToken->tok.group == separators && currentToken->tok.type == comma){
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

	stmFor();

	if(verification == FALSE){
		stmWhile();
	}

	if(verification == FALSE){
		declaration();
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

	if(verification == FALSE){
		stmIf();
	}

	if(verification == FALSE){
		if(currentToken->tok.group == 4 && currentToken->tok.type == 4){
			stmScope();
		}
	}

	if(verification == FALSE){
		stmDoWhile();
	}

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

		consumeToken("stmFor");
		if(currentToken->tok.group == 4 &&currentToken->tok.type == leftParenthesis)
		{
			consumeToken("stmFor");
			expression1();
			if(verification == TRUE){
				if(!(currentToken->tok.group == 4 &&currentToken->tok.type == semicolon)){
					printError(";", currentToken->tok.name);
				}

				consumeToken("stmFor");
				optionalExpression();
				if(verification == TRUE){
					if(!(currentToken->tok.group == 4 &&currentToken->tok.type == semicolon)){
						printError(";", currentToken->tok.name);
					}

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

	int aux = 0, j;

	type();

	int teste = varAlreadyExists(currentToken->tok.name);
		if(teste == 0 && verification == TRUE){

			table[contTable].type = typeAux;

			for(j = 0; j < strlen(currentToken->tok.name); j++){
				table[contTable].name[j] = currentToken->tok.name[j];
			}

			table[contTable].line = currentToken->tok.line;
			table[contTable].column = currentToken->tok.column;
			contTable++;

		} else if(currentToken->tok.group == identifiers && verification == TRUE) {
			printf("Var [%s] already exists! Position[%d, %d]\n", currentToken->tok.name, currentToken->tok.line, currentToken->tok.column);
		}

	if(currentToken->tok.group == identifiers){
		if(verification == FALSE &&(varAlreadyExists(currentToken->tok.name) == 0)) printf("Var %s doesn't exists. Position: [%d:%d]\n", currentToken->tok.name, currentToken->tok.line, currentToken->tok.column);

		if(aux>0 && currentToken->tok.type != typeAux){
			printf("Warning: different type comparison. Position: [%d,%d]\n",currentToken->tok.line, currentToken->tok.column);
		}

		aux++;
		typeAux = currentToken->tok.type;
		strcpy(last, currentToken->tok.name);

		consumeToken("expression1");
		expression2();
		//verification = TRUE;
	} else {
		valueR();
	}

	aux = 0;

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
		if(varAlreadyExists(currentToken->tok.name) == 0) printf("Var %s doesn't exists. Position: [%d:%d]", currentToken->tok.name, currentToken->tok.line, currentToken->tok.column);

		consumeToken("factor");
		verification = TRUE;

	} else if(currentToken->tok.group == number){

		int aux = 0, i;

		for(i=0; i < strlen(currentToken->tok.name); i++){
			if(currentToken->tok.name[i] == '.'){
				aux = 1;
			}
		}

		if((returnType(last) == intKey && aux == 1) || ((returnType(last) == floatKey || returnType(last) == doubleKey) && aux == 0)){
			printf("Warning: different type comparison. Position: [%d,%d]\n",currentToken->tok.line, currentToken->tok.column);
		}

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

