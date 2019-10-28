#include "bib.h"

#define NOKEYWORD 1000

void cleanString(char* string){
	int size = strlen(string);
	/*llalalala*/
	for(int i = 0; i < size ; i++){
		string[i] = '\0';
	}
}

void lexicalAnalyzer(list* tokensList, char* line, int lineNumber){

	Token token;
	int size = strlen(line);
	int type;
	int i, j = 0;
	int aux = 0; //identifies if the last char is a reserved symbol (= 1) or part of a unidentified string (= 0)
	char* unidentifiedToken = (char*)malloc(size*sizeof(char));
	
	/* percorre a string até achar um separador ou operador */
	for(i = 0; i < size; i++){

		/* if the current char is a comment symbol */
		if (isComment(line, &i, size, tokensList, lineNumber, token) != -1){ 

			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber, token);
				cleanString(unidentifiedToken);
				j = 0;

			}

			insert(tokensList, token);
			aux = 1;

		/* if the current char is a literal symbol */
		} else if(isLiteral(line, &i, tokensList, lineNumber, token) != -1){

			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber, token);
				cleanString(unidentifiedToken);
				j = 0;

			}

			insert(tokensList, token);
			aux = 1;

		/* if the current char is a separator*/
		} else if(MatchSeparator(line[i], tokensList, i, lineNumber, token) != -1){
			
			// se o anterior é o final de um token
			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber, token);
				cleanString(unidentifiedToken);
				j = 0;

			}

			if(line[i]!=' ' && line[i]!='	') insert(tokensList, token);
			aux = 1;

		/* if the current char is a operator*/
		} else if(MatchOperator(line[i], tokensList, line[i+1], &i, lineNumber, token) != -1){

			// se o anterior é o final de um token (se for separador n precisa)
			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber, token);
				cleanString(unidentifiedToken);
			}

			insert(tokensList, token);
			aux = 1;

		/* the current char isn't a reserved symbol */
		} else {
			aux = 0;
			unidentifiedToken[j] = line[i];
			j++;

		}

	}
}

int isLiteral(char* line, int *i, list* tokensList, int lineNumber, Token token){

	char* unidentifiedToken = (char*)malloc(strlen(line)*sizeof(char));
	int k, j = 0;

	if(line[*i] == '\"'){

		k = *i+1;

 		unidentifiedToken[j] = line[*i];
 		j++;

		while(line[k] != '\"'){

			unidentifiedToken[j] = line[k];
			j++;
			k++;

		}

		*i = k; //pula pro final do literal

		unidentifiedToken[j] = line[*i];

		token.group = literal;
		token.type = literal;
		token.name = unidentifiedToken;

		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		return *i;
	} else if (line[*i] == '\''){

		k = *i+1;

 		unidentifiedToken[j] = line[*i];
 		j++;

		while(line[k] != '\''){

			unidentifiedToken[j] = line[k];
			j++;
			k++;

		}

		*i = k; //pula pro final do literal
		unidentifiedToken[j] = line[*i];

		token.group = literal;
		token.type = literal;
		token.name = unidentifiedToken;

		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		return *i;
	} else if (line[*i] == '\\'){

		k = *i+1;

 		unidentifiedToken[j] = line[*i];
 		j++;

		while(line[k] != '\\'){

			unidentifiedToken[j] = line[k];
			j++;
			k++;

		}

		*i = k; //pula pro final do literal
		unidentifiedToken[j] = line[*i];

		token.group = literal;
		token.type = literal;
		token.name = unidentifiedToken;
	 
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		return *i;
	} else if (line[*i] == '<'){

		int x = *i+1;
		int isHeader = -1;

		while(x<strlen(line)){

			if(line[x] == '>') isHeader = 0;

			x++;

		}

		if(isHeader == 0){

			k = *i+1;

	 		unidentifiedToken[j] = line[*i];
	 		j++;

			while(line[k] != '>'){

				unidentifiedToken[j] = line[k];
				j++;
				k++;

			}

			*i = k; //pula pro final do literal
			unidentifiedToken[j] = line[*i];

			token.group = header;
			token.type = header;
			token.name = unidentifiedToken;

			printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
			free(unidentifiedToken);

			return *i;
		}
	}

	free(unidentifiedToken);
	return -1;
}

int isComment(char* line, int *i, int size, list* tokensList, int lineNumber, Token token){

	char* unidentifiedToken = (char*)malloc(strlen(line)*sizeof(char));
	int j = 0, aux = *i;

	if(line[aux] == '/' && line[aux+1] == '/'){

		while(aux < size){

			unidentifiedToken[j] = line[aux];
			j++;
			aux++;
		}

		token.group = comment;
		token.type = comment;
		token.name = unidentifiedToken;

		 
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		*i = aux;

		return aux;
	} else if (line[aux] == '/' && line[aux+1] == '*') {

		unidentifiedToken[j] = line[aux];
		unidentifiedToken[j+1] = line[aux+1];

		j+=2;
		aux+=2;

		while(line[aux] != '*' && line[aux+1] != '/'){

			unidentifiedToken[j] = line[aux];
			j++;
			aux++;
		}

		unidentifiedToken[j] = line[aux];
		unidentifiedToken[j+1] = line[aux+1];

		token.group = comment;
		token.type = comment;
		token.name = unidentifiedToken;

		 
		*i = aux+1;

		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		return aux;
	}

	return -1;
}

int isKeywords(char* unidentifiedToken, list* tokensList, int column, int lineNumber, Token token){

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
	}else if(strcmp(unidentifiedToken, "malloc") == 0){
		token.type = mallocKey;
		token.name = "malloc";
	}else if(strcmp(unidentifiedToken, "calloc") == 0){
		token.type = callocKey;
		token.name = "calloc";
	}else if(strcmp(unidentifiedToken, "free") == 0){
		token.type = freeKey;
		token.name = "free";
	}else{
		return -1;
	}

	 
	printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
	return 0;
}

int isIdentifierOrNumber(char* unidentifiedToken, list* tokensList, int column, int lineNumber, Token token){

	int test = 0, dotCont = 0, isVariable = 0;

	/* identifier */
	if(unidentifiedToken[0] == '_' || (unidentifiedToken[0] >= 65 && unidentifiedToken[0] <= 90) || (unidentifiedToken[0] >= 97 && unidentifiedToken[0] <= 122)){
		
		for(int i = 0; i < strlen(unidentifiedToken); i++){

			if(unidentifiedToken[i] != '_' && unidentifiedToken[i] != '.' &&
				(unidentifiedToken[i] < 65 || unidentifiedToken[i] > 90) && (unidentifiedToken[i] < 97 || unidentifiedToken[i] > 122) &&
				!isdigit(unidentifiedToken[i])){
				
				isVariable = -1;
				break;
			}

		}

		if(isVariable == 0){
			token.group = identifiers;
			token.type = identifiers;
			token.name = unidentifiedToken;

			 
			printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
			return 0;
		}

	}

	/* include */
	if(strcmp(unidentifiedToken, "#include") == 0){

		token.group = include;
		token.type = include;
		token.name = unidentifiedToken;

		 
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);

		return 0;

	}

	/* define */
	if(strcmp(unidentifiedToken, "#define") == 0){

		token.group = define;
		token.type = define;
		token.name = unidentifiedToken;

		 
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);

		return 0;

	}

	/* number */
	for(int i = 0; i < strlen(unidentifiedToken); i++){

		if((unidentifiedToken[i] >= 48 && unidentifiedToken[i] <= 57) || unidentifiedToken[i] == '.'){

			if(unidentifiedToken[i] == '.'){

				if(dotCont == 0){

					test++;
					dotCont++;
				} else {

					break;
				}
			} else {

				test++;
			}
		} else {

			break;
		}

	}


	if(test == strlen(unidentifiedToken)){ 

		token.group = number;
		token.type = 000;
		token.name = unidentifiedToken;

		 
		printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

		return 0;

	}

	return -1;
}

int MatchOperator(char unidentifiedOperator, list* tokensList, char next, int *i, int lineNumber, Token token){

	switch (unidentifiedOperator){

			case '+':

				if(next == '='){

					token.group = arithmeticOperators;
					token.type = increment;
					token.name = "+=";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else if(next == '+'){

					token.group = arithmeticOperators;
					token.type = not;
					token.name = "++";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {

					token.group = arithmeticOperators;
					token.type = plus;
					token.name = "+";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}

				return *i;
			break;

			case '-':

				if(next == '='){

					token.group = arithmeticOperators;
					token.type = decrease;
					token.name = "-=";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}else if(next == '-'){

					token.group = arithmeticOperators;
					token.type = decrease;
					token.name = "--";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {

					token.group = arithmeticOperators;
					token.type = minus;
					token.name = "-";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '*':

				if(next == '='){

					token.group = arithmeticOperators;
					token.type = times;
					token.name = "*=";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				} else {	

					token.group = arithmeticOperators;
					token.type = times;
					token.name = "*";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '/':

				if(next == '='){

					token.group = arithmeticOperators;
					token.type = division;
					token.name = "/=";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				} else {

					token.group = arithmeticOperators;
					token.type = division;
					token.name = "/";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '>':

				if(next == '>'){

					token.group = bitwise;
					token.type = shiftRight;
					token.name = ">>";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else if(next == '='){

					token.group = logicalOperators;
					token.type = isMoreOrEqual;
					token.name = ">=";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {
					token.group = logicalOperators;
					token.type = isMoreThan;
					token.name = ">";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '<':

				if(next == '<'){

					token.group = bitwise;
					token.type = shiftLeft;
					token.name = "<<";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else if(next == '='){

					token.group = logicalOperators;
					token.type = isLessOrEqual;
					token.name = "<=";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {
					token.group = arithmeticOperators;
					token.type = isLessThan;
					token.name = "<";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '=':

				if(next == '='){

					token.group = logicalOperators;
					token.type = andOperator;
					token.name = "&&";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);	
				} else {
					token.group = arithmeticOperators;
					token.type = equals;
					token.name = "=";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '&':

				if(next == '&'){

					token.group = logicalOperators;
					token.type = andOperator;
					token.name = "&&";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {

					token.group = bitwise;
					token.type = comercial;
					token.name = "&";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '|':

				if(next == '|'){

					token.group = logicalOperators;
					token.type = orOperator;
					token.name = "||";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				} else {

					token.group = bitwise;
					token.type = bwor;
					token.name = "|";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '!':

				if(next == '!'){

					token.group = logicalOperators;
					token.type = notOperator;
					token.name = "!=";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				} else {

					token.group = logicalOperators;
					token.type = not;
					token.name = "!";

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}
				return *i;
			break;

			case '?':

				if(next == ':'){

					token.group = logicalOperators;
					token.type = ternary;
					token.name = "?:";

					*i++;

					 
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				}
			break;
		}
		return -1;
}

int MatchSeparator(char unidentifiedSeparator, list* tokensList, int column, int lineNumber, Token token){

	switch (unidentifiedSeparator){

		case ',':

			token.group = separators;
			token.type = comma;
			token.name = ",";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ';':

			token.group = separators;
			token.type = semicolon;
			token.name = ";";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '(':

			token.group = separators;
			token.type = leftParenthesis;
			token.name = "(";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ')':

			token.group = separators;
			token.type = rightParenthesis;
			token.name = ")";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '{':

			token.group = separators;
			token.type = leftBrace;
			token.name = "{";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '}':

			token.group = separators;
			token.type = rightBrace;
			token.name = "}";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '[':

			token.group = separators;
			token.type = leftBracket;
			token.name = "[";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ']':

			token.group = separators;
			token.type = rightBracket;
			token.name = "]";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ':':

			token.group = separators;
			token.type = colon;
			token.name = ":";

			 
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ' ':

			return 0;

		break;

		case '	':

			return 0;

		break;

	}

	return -1;
}

void ChooseType(char* unidentifiedToken, list* tokensList, int i, int lineNumber, Token token){

	int aux = isKeywords(unidentifiedToken, tokensList, i, lineNumber, token);
	int column = i-strlen(unidentifiedToken);

	if(aux == -1){

		aux = isIdentifierOrNumber(unidentifiedToken, tokensList, i, lineNumber, token);

		if(aux == -1){
			printf("\n-------------------------------\n");
			printf("Error 404: type not found\n");
			printf("token: %s\n", unidentifiedToken);
			printf("line: %d\n", lineNumber+1);
			printf("column: %d\n", column);
			printf("\n-------------------------------\n");
		}

		cleanString(unidentifiedToken);

	} else cleanString(unidentifiedToken);
}