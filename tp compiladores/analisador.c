#include "bib.h"

#define NOKEYWORD 1000

void cleanString(char* String){
	int size = strlen(String);

	for(int i = 0; i < size ; i++){
		String[i] = '\0';
	}
}

void lexicalAnalyzer(list* tokensList, char* line, int lineNumber){

	int size = strlen(line);
	int type;
	int i, j = 0;
	int aux = 0; //identifies if the last char is a reserved symbol (= 1) or part of a unidentified string (= 0)
	char* unidentifiedToken = (char*)malloc(size*sizeof(char));
	
	/* percorre a string até achar um separador ou operador */
	for(i = 0; i < size; i++){
		/* if line[*i] is a separator*/
		if(MatchSeparator(line[i], tokensList) != -1){
			
			// se o anterior é o final de um token
			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber);
				cleanString(unidentifiedToken);
				j = 0;

			}

			aux = 1;

		/* if line[*i] is a operator*/
		} else if(MatchOperator(line[i], tokensList, line[i+1], &i) != -1){

			// se o anterior é o final de um token (se for separador n precisa)
			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber);
				cleanString(unidentifiedToken);
			}

			aux = 1;

		/* if line[*i] is a literal symbol */
		} else if(isLiteral(line, &i, tokensList) != -1){

			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber);
				cleanString(unidentifiedToken);
				j = 0;

			}

			aux = 1;

		/* if line[*i] is a comment symbol */
		} else if (isComment(line, &i, size, tokensList) != -1){ 

			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber);
				cleanString(unidentifiedToken);
				j = 0;

			}
			aux = 1;

		/* line[*i] isn't a reserved symbol */
		} else {
			aux = 0;
			unidentifiedToken[j] = line[i];
			printf("TOKEN: %s\n",unidentifiedToken);
			j++;

		}

	}
}

int isLiteral(char* line, int *i, list* tokensList){

	Token token;
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

		insert(tokensList, token);
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

		insert(tokensList, token);
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

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		return *i;
	} else if (line[*i] == '<'){

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

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		return *i;
	}

	free(unidentifiedToken);
	return -1;
}

int isComment(char* line, int *i, int size, list* tokensList){

	Token token;
	char* unidentifiedToken = (char*)malloc(strlen(line)*sizeof(char));
	int j = 0;


	if(line[*i] == '/' && line[*i+1] == '/'){

		while(*i < size){

			unidentifiedToken[j] = line[*i];
			*i++;
		}

		token.group = comment;
		token.type = comment;
		token.name = unidentifiedToken;

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		return *i;
	} else if (line[*i] == '/' && line[*i+1] == '*') {


		while(line[*i] != '*' && line[*i+1] != '/'){ //nao vai pegar o ultimo?

			unidentifiedToken[j] = line[*i];
			j++;
			i++;
		}

		unidentifiedToken[j+1] = line[*i+1];

		token.group = comment;
		token.type = comment;
		token.name = unidentifiedToken;

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
		free(unidentifiedToken);

		return *i;
	}

	return -1;
}

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

	insert(tokensList, token);
	printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
	return 0;
}

int isIdentifierOrNumber(char* unidentifiedToken, list* tokensList){

	Token token;
	int test = 0, dotCont = 0, isVariable = 0;

	/* identifier */
	if(unidentifiedToken[0] == '_' || (unidentifiedToken[0] >= 65 && unidentifiedToken[0] <= 90) || (unidentifiedToken[0] >= 97 && unidentifiedToken[0] <= 122)){
		
		for(int i = 0; i < strlen(unidentifiedToken); i++){

			if(unidentifiedToken[i] != '_' && 
				(unidentifiedToken[i] < 65 || unidentifiedToken[i] > 90) && (unidentifiedToken[i] < 97 || unidentifiedToken[i] > 122) &&
				!isdigit(unidentifiedToken[i])){
				
				isVariable = 1;
				break;
			}

		}

		if(isVariable == 0){
			token.group = identifiers;
			token.type = identifiers;
			token.name = unidentifiedToken;

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);
			return 0;
		}

	}

	/* include */
	if(strcmp(unidentifiedToken, "#include") == 0){

		token.group = include;
		token.type = include;
		token.name = unidentifiedToken;

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n",token.name, token.group, token.type);

		return 0;

	}

	/* define */
	if(strcmp(unidentifiedToken, "#define") == 0){

		token.group = define;
		token.type = define;
		token.name = unidentifiedToken;

		insert(tokensList, token);
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

		insert(tokensList, token);
		printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

		return 0;

	}

	return -1;
}

int MatchOperator(char unidentifiedOperator, list* tokensList, char next, int *i){

	Token token;

	switch (unidentifiedOperator){

			case '+':

				if(next == '='){

					token.group = arithmeticOperators;
					token.type = increment;
					token.name = "+=";

					*i++;

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else if(next == '+'){

					token.group = arithmeticOperators;
					token.type = not;
					token.name = "++";

					*i++;

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {

					token.group = arithmeticOperators;
					token.type = plus;
					token.name = "+";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				}else if(next == '-'){

					token.group = arithmeticOperators;
					token.type = decrease;
					token.name = "--";

					*i++;

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {

					token.group = arithmeticOperators;
					token.type = minus;
					token.name = "-";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				} else {	

					token.group = arithmeticOperators;
					token.type = times;
					token.name = "*";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				} else {

					token.group = arithmeticOperators;
					token.type = division;
					token.name = "/";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else if(next == '='){

					token.group = logicalOperators;
					token.type = isMoreOrEqual;
					token.name = ">=";

					*i++;

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {
					token.group = logicalOperators;
					token.type = isMoreThan;
					token.name = ">";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else if(next == '='){

					token.group = logicalOperators;
					token.type = isLessOrEqual;
					token.name = "<=";

					*i++;

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {
					token.group = arithmeticOperators;
					token.type = isLessThan;
					token.name = "<";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);	
				} else {
					token.group = arithmeticOperators;
					token.type = equals;
					token.name = "=";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
				} else {

					token.group = bitwise;
					token.type = comercial;
					token.name = "&";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				} else {

					token.group = bitwise;
					token.type = bwor;
					token.name = "|";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				} else {

					token.group = logicalOperators;
					token.type = not;
					token.name = "!";

					insert(tokensList, token);
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

					insert(tokensList, token);
					printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);

				}
			break;
		}
		return -1;
}

int MatchSeparator(char unidentifiedSeparator, list* tokensList){

	Token token;

	switch (unidentifiedSeparator){

		case ',':

			token.group = separators;
			token.type = comma;
			token.name = ",";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ';':

			token.group = separators;
			token.type = semicolon;
			token.name = ";";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '(':

			token.group = separators;
			token.type = leftParenthesis;
			token.name = "(";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ')':

			token.group = separators;
			token.type = rightParenthesis;
			token.name = ")";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '{':

			token.group = separators;
			token.type = leftBrace;
			token.name = "{";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '}':

			token.group = separators;
			token.type = rightBrace;
			token.name = "}";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '[':

			token.group = separators;
			token.type = leftBracket;
			token.name = "[";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ']':

			token.group = separators;
			token.type = rightBracket;
			token.name = "]";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ':':

			token.group = separators;
			token.type = colon;
			token.name = ":";

			insert(tokensList, token);
			printf("Token: %s group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case ' ':

			token.group = separators;
			token.type = space;
			token.name = " ";

			insert(tokensList, token);
			printf("Token: %s(space) group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

		case '	':

			token.group = separators;
			token.type = tab;
			token.name = "	";

			insert(tokensList, token);
			printf("Token: %s(tab) group: %d type: %d\n\n", token.name, token.group, token.type);
			return 0;

		break;

	}

	return -1;
}

void ChooseType(char* unidentifiedToken, list* tokensList, int i, int lineNumber){

	int aux = isKeywords(unidentifiedToken, tokensList);
	int column = i-strlen(unidentifiedToken);

	if(aux == -1){

		aux = isIdentifierOrNumber(unidentifiedToken, tokensList);

		if(aux == -1){
			printf("\n-------------------------------\n");
			printf("Error 404: type not found\n");
			printf("token: %s\n", unidentifiedToken);
			printf("line: %d\n", lineNumber+1);
			printf("column: %d\n", column);
			printf("\n-------------------------------\n");
		}

		cleanString(unidentifiedToken);

	}
}