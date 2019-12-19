#include "sintatico.h"

int verification = FALSE;
int cont = 0;
int contTable = 0;
int typeAux;
int recursao = 0;

char pilha[50][50];
int contPilha = 0; 

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
		table[i].value = 0;
		table[i].word = "0";
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
		dataFile();
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

		function(" ");

	}

}

void function(char* label){

	int j;

	type(label);

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

	argumentsList(label); // <argumentsList>

	if(verification == TRUE){

		if(!(currentToken->tok.group == separators && currentToken->tok.type == rightParenthesis)){ // )
			printError(")", currentToken->tok.name);
		} else {
			consumeToken("funcion3");
		} 

		stmScope(label);

		return;

	}
}

void type(char* label){

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

void argumentsList(char* label){

	argument(label);

	if(verification = TRUE){
		if(currentToken->tok.group == 4 && currentToken->tok.type == comma){
			consumeToken("argumentsList");
			recursao++;
			argumentsList(label);
			recursao--;

		}else{

			return;	

		}
	}  else {

		verification = FALSE;
		return;
	}

	if(recursao == 0){
		codeGeneratorAtrib(label);
		contPilha = 0;
	}

}

void argument(char* label){
	int j;
	type(label);

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

			strcpy(pilha[contPilha], currentToken->tok.name);
			contPilha++;

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

void declaration(char* label){

	type(label);

	if(verification == TRUE){
		identifiersList(label);
	}
}

void identifiersList(char* label){
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

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("identifiersList");

		if(currentToken->tok.group == separators && currentToken->tok.type == comma){
			consumeToken("identifiersList");
			identifiersList(label);
		}

	} else {

		printError("identifier", ID2Name(currentToken->tok.group));
	}
}

void statement(char* label){

	verification = FALSE;


	stmFor(label);

	if(verification == FALSE){
		stmWhile(label);
	}

	if(verification == FALSE){
		declaration(label);
		if(verification == FALSE){
			expression1(label);
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
		stmIf(label);
	}

	if(verification == FALSE){
		if(currentToken->tok.group == 4 && currentToken->tok.type == 4){
			stmScope(label);
		}
	}

	if(verification == FALSE){
		stmDoWhile(label);
	}

	if(verification == FALSE){
		if(currentToken->tok.group == 0 && currentToken->tok.type == 11){
			stmReturn(label);
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
		printf("if: ");
		if(currentToken->tok.group == separators && currentToken->tok.type == leftParenthesis){
			consumeToken("stmIf");
			expression1("if");

			if(currentToken->tok.group == separators && currentToken->tok.type == rightParenthesis){
				consumeToken("stmIf");
				statement("if");
				if(verification == TRUE) {
					elseFragment("if");
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

		printf("for: ");
		consumeToken("stmFor");
		if(currentToken->tok.group == 4 &&currentToken->tok.type == leftParenthesis)
		{
			consumeToken("stmFor");
			expression1("for");
			if(verification == TRUE){
				if(!(currentToken->tok.group == 4 &&currentToken->tok.type == semicolon)){
					printError(";", currentToken->tok.name);
				}

				consumeToken("stmFor");
				optionalExpression("for");
				if(verification == TRUE){
					if(!(currentToken->tok.group == 4 &&currentToken->tok.type == semicolon)){
						printError(";", currentToken->tok.name);
					}

					consumeToken("stmFor");
					optionalExpression("for");
					if(verification == TRUE){

						if(currentToken->tok.group == 4 &&currentToken->tok.type == rightParenthesis){

							consumeToken("stmFor");
							statement("for");
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

void optionalExpression(char* label){

	expression1(label);

	if (verification == FALSE){ //empty
		verification = TRUE;
	}
}

void stmWhile(){

	if(currentToken->tok.group == 0 && currentToken->tok.type == 7){
		printf("while: ");
		consumeToken("stmWhile");
		if(currentToken->tok.group == 4 && currentToken->tok.type == leftParenthesis){
			consumeToken("stmWhile");
			expression1("while");
			if(verification == TRUE){
				if(currentToken->tok.group == 4 &&currentToken->tok.type == rightParenthesis){
					consumeToken("stmWhile");
					statement("while");
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
		stmScope("stmDoWhile");
		if(verification == TRUE){
			if(strcmp(currentToken->tok.name, "while")){
				consumeToken("stmDoWhile");
				if(currentToken->tok.group == 4 && currentToken->tok.type == leftParenthesis){
					consumeToken("stmDoWhile");
					expression1("stmDoWhile");
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
		printf("else: ");
		consumeToken("elseFragment");
		statement("else");
		verification = TRUE;
		return;
	} else {
		verification = FALSE;
		return;
	}
}

void stmReturn(char* label){
	
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

void stmScope(char* label){

	if(currentToken->tok.group == separators && currentToken->tok.type == leftBrace){
		consumeToken("stmScope");
		stmList(label);

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

void stmList(char* label){

	statement(label);

	if(verification == TRUE){
		//consumeToken("stmList 1");
		stmListLine(label);
	} else {
		verification = TRUE;
		consumeToken("stmList 2");
		stmListLine(label);
	}
}

void stmListLine(char* label){

	statement(label);

	if(verification == TRUE){
		stmListLine(label);
		return;
	} else {
		verification = TRUE;
		//consumeToken("stmListLine");
		return;
	}
}

void expression1(char* label){

	int aux = 0, j;

	type(label);

	int teste = varAlreadyExists(currentToken->tok.name);

	/*  */
	if(currentToken->tok.group == identifiers){

		/* add to symbols table */
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

		if(verification == FALSE &&(varAlreadyExists(currentToken->tok.name) == 0)) printf("Var %s doesn't exists. Position: [%d:%d]\n", currentToken->tok.name, currentToken->tok.line, currentToken->tok.column);

		if(aux>0 && currentToken->tok.type != typeAux){
			printf("Warning: different type comparison. Position: [%d,%d]\n",currentToken->tok.line, currentToken->tok.column);
		}

		aux++;
		typeAux = currentToken->tok.type;
		strcpy(last, currentToken->tok.name);

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("expression1");

		expression2(label);
		//verification = TRUE;
	} else {
		valueR(label);
	}

	aux = 0;
	if(recursao == 0){
		codeGeneratorAtrib(label);
		contPilha = 0;
	}

}

void expression2(char* label){

	if(currentToken->tok.group == logicalOperators && currentToken->tok.type == atrib){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("expression2");
		expression1(label);
		return;
	} else {

		comparation(label);

		if(verification == TRUE){
			recursao++;
			expression1(label);
			recursao--;
		}

		if(verification == FALSE){
			arithmetic(label);
			if(verification == TRUE){
				recursao++;
				expression1(label);
				recursao--;
			}
		}

		if(verification == FALSE){
			duplicatedOperatorArithmetic(label);
			if(verification == TRUE){
				recursao++;
				expression1(label);
				recursao--;
			}
		}

		if(verification == FALSE){
			composedOperatorArithmetic(label);
			if(verification == TRUE){
				recursao++;
				expression1(label);
				recursao--;
			}
		} 

	} 
}

void valueR(char* label){

	comparation(label);
	if(verification == TRUE){
		magnitude(label);
		if(verification == TRUE){
			valueRLine(label);
		}
	} else {
		magnitude(label);
	}
}

void valueRLine(char* label){

	comparation(label);
	if(verification == TRUE){
		magnitude(label);
		if(verification == TRUE){
			valueRLine(label);
		}
	}
}

void comparation(char* label){

	if(currentToken->tok.group == logicalOperators){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("comparation");
		verification = TRUE;
	}
}

void magnitude(char* label){

	term(label);
	if(verification == TRUE){
		magnitudeLine(label);
	}
}

void term(char* label){

	factor(label);
	if(verification == TRUE){
		termLine(label);
	}
}

void magnitudeLine(char* label){

	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == plus){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("magnitudeLine");
		term(label);
		if(verification == TRUE){
			magnitudeLine(label);
		}
	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == minus){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("magnitudeLine");
		term(label);
		if(verification == TRUE){
			magnitudeLine(label);
		}
	} else {
		verification = TRUE;
		return;
	}
}

void termLine(char* label){

	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == times){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("termLine");
		factor(label);
		if(verification == TRUE){
			termLine(label);
		}

	} else 	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == division){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("termLine");
		factor(label);
		if(verification == TRUE){
			termLine(label);
		}

	} else {

		verification =  TRUE;
		return;
	}
}

void factor(char* label){

	if(currentToken->tok.group == 4 && currentToken->tok.type == leftParenthesis){

		consumeToken("factor");
		expression1(label);

		if(currentToken->tok.group == 4 && currentToken->tok.type == rightParenthesis){

			consumeToken("factor");
			verification = TRUE;

		} else {
			printError(")", currentToken->tok.name);
		}
	} else if(currentToken->tok.group == 4 && currentToken->tok.type == plus){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("factor");
		factor(label);

	} else if(currentToken->tok.group == 4 && currentToken->tok.type == minus){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("factor");
		factor(label);

	} else if(currentToken->tok.group == identifiers){
		if(varAlreadyExists(currentToken->tok.name) == 0) printf("Var %s doesn't exists. Position: [%d:%d]", currentToken->tok.name, currentToken->tok.line, currentToken->tok.column);

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("factor");
		verification = TRUE;

	} else if(currentToken->tok.group == number){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

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

void arithmetic(char* label){

	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == plus){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("arithmetic");
		verification = TRUE;

	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == minus){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("arithmetic");
		verification = TRUE;

	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == times){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("arithmetic");
		verification = TRUE;

	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == division){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("arithmetic");
		verification = TRUE;

	} else {
		verification = FALSE;
	}
}

void duplicatedOperatorArithmetic(char* label){

	if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == increment){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("duplicatedOperatorArithmetic");
		verification = TRUE;

	} else if(currentToken->tok.group == arithmeticOperators && currentToken->tok.type == decrease){

		strcpy(pilha[contPilha], currentToken->tok.name);
		contPilha++;

		consumeToken("duplicatedOperatorArithmetic");
		verification = TRUE;

	} else {
		verification = FALSE;
	}
}

void composedOperatorArithmetic(char* label){

	duplicatedOperatorArithmetic(label);
}

void dataFile(){

	int i;

	FILE *data;
	data = fopen("data.txt", "w");

	fprintf(data, ".data\n\n");
	//printf(".data\n\n");

	for(i = 0; i < contTable; i++){
		
		if(table[i].type == charKey){
			fprintf(data, "%s: .byte '%s'\n", table[i].name, table[i].word);
			//printf("%s: .word '%s'\n", table[i].name, table[i].word);
		} else {
			fprintf(data, "%s: .byte %d\n", table[i].name, table[i].value);
			//printf("%s: .byte %f\n", table[i].name, table[i].value);
		}
	}

	fclose(data);
}

void codeGeneratorAtrib(char* label){

	int i = 0;
	int reg = 0;

	char* var1 = (char*)malloc(10*sizeof(char));
	char* var2 = (char*)malloc(10*sizeof(char));
	char oper1;
	char oper2;
	int add = 0, sub = 0, mul = 0, div = 0, atri = 0, decr = 0, incre = 0;
	int menor = 0, maior = 0, maiorIgual = 0, menorIgual = 0, igual = 0, neg = 0;
	int aux = 0;

	for(i = contPilha-1 ; i >= 0 ; i--){

		if(strcmp(pilha[i], ";") == 0) continue;
		
		if(varAlreadyExists(pilha[i])){

			if(aux == 0){
				strcpy(var2, pilha[i]);
				printf("load $t%d, %s\n", reg, pilha[i]);
				reg++;
				aux++;

			}else{
				strcpy(var1, pilha[i]);
				printf("load $t%d, %s\n", reg, pilha[i]);
				reg++;
			}

			

		} else if (strcmp(pilha[i], "+") == 0){

			oper1 = '+';
			add = 1;

			if (strcmp(pilha[i-1], "+") == 0){
				add = 0;
				incre = 1;
				oper2 = '+';
				i --;
			}


		} else if (strcmp(pilha[i], "-") == 0){

			oper1 = '-';
			sub = 1;

		} else if (strcmp(pilha[i], "*") == 0){

			oper1 = '*';
			mul = 1;

		} else if (strcmp(pilha[i], "/") == 0){

			oper1 = '/';
			div = 1;

		} else if (strcmp(pilha[i], "=") == 0){

			oper1 = '=';
			atri = 1;

		}else if (strcmp(pilha[i], "--") == 0){
			sub = 0;
			decr = 1;
			//oper2 = '--';
		}else if (strcmp(pilha[i], "++") == 0){
			add = 0;
			incre = 1;
			//oper2 = '++';
		} else if (strcmp(pilha[i], "<") == 0){

			menor = 1;

		} else if (strcmp(pilha[i], ">=") == 0){

			maiorIgual = 1;

		} else if (strcmp(pilha[i], "<=") == 0){

			menorIgual = 1;

		} else if (strcmp(pilha[i], "==") == 0){

			igual = 1;

		} else if (strcmp(pilha[i], "!=") == 0){

			neg = 1;

		}else if(strcmp(pilha[i], ">") == 0){
			maior = 1; 
		}else{
			strcpy(var2, pilha[i]);
			printf("load $t%d, %s\n", reg, pilha[i]);
			reg++;
		}

	}

	if(add){

		printf("add $t2, $t0, $t1\n");

	}else if(sub){

		printf("sub $t2, $t0, $t1\n");

	}else if(mul){

		printf("mul $t2, $t0, $t1\n");

	}else if(div){

		printf("div $t2, $t0, $t1\n");

	}else if(atri){

		printf("sw $t0, $t1\n");

	}else if(decr){

		printf("add $t2, $t1, 1\n");

	}else if(incre){

		printf("sub $t2, $t1, 1\n");

	} else if(maior){

		printf("slt $t2, $t1, $t0\n");
		printf("beq $t2, $zero, %s\n", label);

	}else if(menor){

		printf("slt $t2, $t0, $t1\n");
		printf("beq $t0, $t1, %s\n", label);

	}else if(maiorIgual){

		printf("slt $t2, $t1, $t0\n");
		printf("beq $t0, $t1, %s\n", label);

	}else if(menorIgual){

		printf("slt $t2, $t0, $t1\n");
		printf("beq $t0, $t1, %s\n", label);

	}else if(igual){

		printf("beq $t0, $t1, %s\n", label);

	}else if(neg){

		printf("bne $t0, $t1, %s\n", label);

	}

}


