#include "bib.h"

#define NOKEYWORD 1000

void cleanString(char* string){
	int size = strlen(string);

	for(int i = 0; i < size ; i++){
		string[i] = '\0';
	}

}

void lexicalAnalyzer(list* tokensList, char* line){

	int size = strlen(line);
	int type;
	int i, j = 0, aux = 0;
	char* unidentifiedToken = (char*)malloc(size*sizeof(char));
	char* unidentifiedSeparator;
	char* unidentifiedOperator;
	
	/* percorre a string até achar um separador ou operador */
	for(i = 0; i < size; i++){

		unidentifiedSeparator = (char*)malloc(size*sizeof(char));
		unidentifiedOperator = (char*)malloc(size*sizeof(char));

		/* if line[i] is a separator*/
		if(line[i] == ',' || line[i] == ';' || line[i] == '(' || line[i] == ')' || line[i] == '{' || line[i] == '}' || line[i] == '[' 
			|| line[i] == ']' || line[i] == ':' || line[i] == ' ' || line[i] == '	'){
			
			unidentifiedSeparator[0] = line[i];
			
			// se o anterior é o final de um token
			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList);
				cleanString(unidentifiedToken);
				j = 0;

			}

			MatchSeparator(unidentifiedSeparator, tokensList);
			cleanString(unidentifiedSeparator);

			aux = 1;

		/* if line[i] is a operator*/
		} else if(line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '>' || line[i] == '<' || line[i] == '=' 
			|| line[i] == '&' || line[i] == '|' || line[i] == '!'){

			unidentifiedOperator[0] = line[i];
			
			// se o anterior é o final de um token (se for separador n precisa)
			if(aux != 1){

				ChooseType(unidentifiedToken, tokensList);
				cleanString(unidentifiedToken);
				j = 0;

			}

			//se o próximo é operador (++, --, [...])
			if(line[i+1] == '+' || line[i+1] == '-' || line[i+1] == '*' || line[i+1] == '/' || line[i+1] == '>' || line[i+1] == '<' || line[i+1] == '=' 
			   || line[i+1] == '&' || line[i+1] == '|' || line[i+1] == '!'){

				unidentifiedOperator[1] = line[i+1];
				MatchOperator(unidentifiedOperator, tokensList);
				cleanString(unidentifiedOperator);
				i++;

			} else {

				MatchOperator(unidentifiedOperator, tokensList);
				cleanString(unidentifiedOperator);
			
			}//LIMPAR AS STRINGS


			aux = 2;
			
		} else if(line[i] == '\"'){ //literal
			int k = i+1;

			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList);
				cleanString(unidentifiedToken);
				j = 0;

			}

 			unidentifiedToken[j] = line[i];
 			j++;

			while(line[k] != '\"'){

				unidentifiedToken[j] = line[k];
				j++;
				k++;

			}

			i = k; //pula pro final do literal
			unidentifiedToken[j] = line[i];

			Token token;
			token.group = literal;
			token.type = literal;
			token.name = unidentifiedToken;

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedToken, token.group, token.type);
			aux = 1;

		} else if (line[i] == '/' && line[i+1] == '/'){ //comentário

			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList);
				cleanString(unidentifiedToken);
				j = 0;

			}

		}else {
			/* line[i] isn't a separator */
			aux = 0;
			unidentifiedToken[j] = line[i];
			j++;

		}

	}

}

/*ve qualé*/
int isKeywords(char* unidentifiedToken, list* tokensList){

	Token token;
	token.group = keywords;

	if(strcmp(unidentifiedToken, "int") == 0){
		token.type = intKey;
		token.name = "int";
	}else if(strcmp(unidentifiedToken, "char") == 0){
		token.type = charKey;
		token.name = "char";
	}else if(strcmp(unidentifiedToken, "float") == 0){
		token.type = floatKey;
		token.name = "float";
	}else if(strcmp(unidentifiedToken, "double") == 0){
		token.type = doubleKey;
		token.name = "double";
	}else if(strcmp(unidentifiedToken, "unsigned") == 0){
		token.type = unsignedKey;
		token.name = "unsigned";
	}else if(strcmp(unidentifiedToken, "for") == 0){
		token.type = forKey;
		token.name = "for";
	}else if(strcmp(unidentifiedToken, "long") == 0){
		token.type = longKey;
		token.name = "long";
	}else if(strcmp(unidentifiedToken, "while") == 0){
		token.type = whileKey;
		token.name = "while";
	}else if(strcmp(unidentifiedToken, "do") == 0){
		token.type = doKey;
		token.name = "do";
	}else if(strcmp(unidentifiedToken, "if") == 0){
		token.type = ifKey;
		token.name = "if";
	}else if(strcmp(unidentifiedToken, "return") == 0){
		token.type = returnKey;
		token.name = "return";
	}else if(strcmp(unidentifiedToken, "continue") == 0){
		token.type = continueKey;
		token.name = "continue";
	}else if(strcmp(unidentifiedToken, "break") == 0){
		token.type = breakKey;
		token.name = "break";
	}else if(strcmp(unidentifiedToken, "case") == 0){
		token.type = caseKey;
		token.name = "case";
	}else if(strcmp(unidentifiedToken, "switch") == 0){
		token.type = switchKey;
		token.name = "switch";
	}else if(strcmp(unidentifiedToken, "else") == 0){
		token.type = elseKey;
		token.name = "else";
	}else if(strcmp(unidentifiedToken, "defautl") == 0){
		token.type = defaultKey;
		token.name = "default";
	}else if(strcmp(unidentifiedToken, "struct") == 0){
		token.type = structKey;
		token.name = "struct";
	}else if(strcmp(unidentifiedToken, "typedef") == 0){
		token.type = typedefKey;
		token.name = "typedrf";
	}else if(strcmp(unidentifiedToken, "const") == 0){
		token.type = constantKey;
		token.name = "const";
	}else if(strcmp(unidentifiedToken, "asm") == 0){
		token.type = asmKey;
		token.name = "asm";
	}else if(strcmp(unidentifiedToken, "auto") == 0){
		token.type = autoKey;
		token.name = "auto";
	}else if(strcmp(unidentifiedToken, "enum") == 0){
		token.type = enumKey;
		token.name = "enum";
	}else if(strcmp(unidentifiedToken, "extern") == 0){
		token.type = externKey;
		token.name = "extern";
	}else if(strcmp(unidentifiedToken, "goto") == 0){
		token.type = gotoKey;
		token.name = "goto";
	}else if(strcmp(unidentifiedToken, "register") == 0){
		token.type = registerKey;
		token.name = "register";
	}else if(strcmp(unidentifiedToken, "short") == 0){
		token.type = shortKey;
		token.name = "short";
	}else if(strcmp(unidentifiedToken, "signed") == 0){
		token.type = signedKey;
		token.name = "signed";
	}else if(strcmp(unidentifiedToken, "sizeof") == 0){
		token.type = sizeofKey;
		token.name = "sizeof";
	}else if(strcmp(unidentifiedToken, "static") == 0){
		token.type = staticKey;
		token.name = "static";
	}else if(strcmp(unidentifiedToken, "union") == 0){
		token.type = unionKey;
		token.name = "union";
	}else if(strcmp(unidentifiedToken, "volatile") == 0){
		token.type = volatileKey;
		token.name = "volatile";
	}
	else{
		return -1;
	}

	insert(tokensList, token);
	printf("Token: %s group: %d type: %d\n\n",unidentifiedToken, token.group, token.type);
	return 0;

}

int isIdentifierOrNumber(char* unidentifiedToken, list* tokensList){

	Token token;
	int test = 0;

	if(unidentifiedToken[0] == '_' || unidentifiedToken[0] == '$' || (unidentifiedToken[0] >= 65 && unidentifiedToken[0] <= 90)||(unidentifiedToken[0] >= 97 && unidentifiedToken[0] <= 122)){
		
		token.group = identifiers;
		token.type = identifiers;
		token.name = unidentifiedToken;

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n",unidentifiedToken, token.group, token.type);
		return 0;

	}

	if(strcmp(unidentifiedToken, "#include") == 0){

		token.group = include;
		token.type = include;
		token.name = unidentifiedToken;

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n",unidentifiedToken, token.group, token.type);

		return 0;

	}

	for(int i = 0; i < strlen(unidentifiedToken); i++){

		if(isdigit(unidentifiedToken[i]) || unidentifiedToken[i] == '.'){
			test++;
		}

	}


	if(test == strlen(unidentifiedToken)){ 

		token.group = number;
		token.type = 000;
		token.name = unidentifiedToken;

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n",unidentifiedToken, token.group, token.type);

		return 0;

	}

	return -1;

}

void MatchOperator(char* unidentifiedOperator, list* tokensList){

	Token token;

	if(strlen(unidentifiedOperator) == 1){

		switch (unidentifiedOperator[0]){

			case '+':

				token.group = arithmeticOperators;
				token.type = plus;
				token.name = "+";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '-':

				token.group = arithmeticOperators;
				token.type = minus;
				token.name = "-";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '*':

				token.group = arithmeticOperators;
				token.type = times;
				token.name = "*";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '/':

				token.group = arithmeticOperators;
				token.type = division;
				token.name = "/";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '>':

				token.group = logicalOperators;
				token.type = isMoreThan;
				token.name = ">";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '<':

				token.group = arithmeticOperators;
				token.type = isLessThan;
				token.name = "<";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '=':

				token.group = arithmeticOperators;
				token.type = equals;
				token.name = "=";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '&':

				token.group = binaries;
				token.type = comercial;
				token.name = "&";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '|':

				token.group = binaries;
				token.type = orB;
				token.name = "|";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

			case '!':

				token.group = logicalOperators;
				token.type = not;
				token.name = "!";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			break;

		}

	} else { // ++, --, [...]

		if(unidentifiedOperator[0] == '+' ){

			if(unidentifiedOperator[1] == '='){

				token.group = arithmeticOperators;
				token.type = increment;
				token.name = "+=";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			}else if(unidentifiedOperator[1] == '+'){

				token.group = arithmeticOperators;
				token.type = not;
				token.name = "++";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);
			}

		} else if (unidentifiedOperator[0] == '-'){

			if(unidentifiedOperator[1] == '='){

				token.group = arithmeticOperators;
				token.type = decrease;
				token.name = "-=";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			}else if(unidentifiedOperator[1] == '-'){

				token.group = arithmeticOperators;
				token.type = decrease;
				token.name = "-=";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

			}

		} else if (unidentifiedOperator[0] == '>' && unidentifiedOperator[1] == '='){

				token.group = logicalOperators;
				token.type = isMoreOrEqual;
				token.name = ">=";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

		} else if (unidentifiedOperator[0] == '<' && unidentifiedOperator[1] == '='){

				token.group = logicalOperators;
				token.type = isLessOrEqual;
				token.name = "<=";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

		} else if (unidentifiedOperator[0] == '&' && unidentifiedOperator[1] == '&'){

				token.group = logicalOperators;
				token.type = andOperator;
				token.name = "&&";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

		} else if (unidentifiedOperator[0] == '|' && unidentifiedOperator[1] == '|'){

				token.group = logicalOperators;
				token.type = orOperator;
				token.name = "||";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

		} else if (unidentifiedOperator[0] == '!' && unidentifiedOperator[1] == '='){

				token.group = logicalOperators;
				token.type = notOperator;
				token.name = "!=";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

		} else if (unidentifiedOperator[0] == '=' && unidentifiedOperator[1] == '='){

				token.group = logicalOperators;
				token.type = equals;
				token.name = "==";

				insert(tokensList, token);
				printf("Token: %s group: %d type: %d\n\n",unidentifiedOperator, token.group, token.type);

		}

	}

}

void MatchSeparator(char* unidentifiedSeparator, list* tokensList){

	Token token;

	switch (unidentifiedSeparator[0]){

		case ',':

			token.group = separators;
			token.type = comma;
			token.name = ",";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case ';':

			token.group = separators;
			token.type = semicolon;
			token.name = ";";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case '(':

			token.group = separators;
			token.type = leftParenthesis;
			token.name = "(";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case ')':

			token.group = separators;
			token.type = rightParenthesis;
			token.name = ")";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case '{':

			token.group = separators;
			token.type = leftBrace;
			token.name = "{";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case '}':

			token.group = separators;
			token.type = rightBrace;
			token.name = "}";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case '[':

			token.group = separators;
			token.type = leftBracket;
			token.name = "[";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case ']':

			token.group = separators;
			token.type = rightBracket;
			token.name = "]";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case ':':

			token.group = separators;
			token.type = colon;
			token.name = ":";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case ' ':

			token.group = separators;
			token.type = space;
			token.name = " ";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

		case '	':

			token.group = separators;
			token.type = tab;
			token.name = "	";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",unidentifiedSeparator, token.group, token.type);

		break;

	}

}

void ChooseType(char* unidentifiedToken, list* tokensList){
	//chama todas as funções de identificar
	int aux = isKeywords(unidentifiedToken, tokensList);

	if(aux == -1){

		aux = isIdentifierOrNumber(unidentifiedToken, tokensList);

		if(aux == -1){
			printf("\n-------------------------------\n");
			printf("Error 404: type not found\n");
			printf("token: %s\n", unidentifiedToken);
			printf("\n-------------------------------\n");
		}

	}
}