#include "bib.h"

#define NOKEYWORD 1000

Token token1;
Token token2;

void cleanString(char* string){
	int size = strlen(string);
	/*llalalala*/
	for(int i = 0; i < size ; i++){
		string[i] = '\0';
	}
}

void lexicalAnalyzer(list* tokensList, char* line, int lineNumber){

	token1.line = lineNumber;
	token2.line = lineNumber;

	int size = strlen(line);
	int type;
	int i, j = 0;
	int aux = 0; //identifies if the last char is a reserved symbol (= 1) or part of a unidentified string (= 0)
	char* unidentifiedToken = (char*)malloc(size*sizeof(char));
	
	/* percorre a string até achar um separador ou operador */
	for(i = 0; i < size; i++){
		token2.column = i;

		/* if the current char is a comment symbol */
		if (isComment(line, &i, size, tokensList, lineNumber) != -1){ 

			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber);
				cleanString(unidentifiedToken);
				j = 0;

			}

			aux = 1;

		/* if the current char is a literal symbol */
		} else if(isLiteral(line, &i, tokensList, lineNumber) != -1){

			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber);
				cleanString(unidentifiedToken);
				j = 0;

			}

			insert(tokensList, token2);
			aux = 1;

		/* if the current char is a separator*/
		} else if(MatchSeparator(line[i], tokensList, i, lineNumber) != -1){
			
			// se o anterior é o final de um token
			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber);
				cleanString(unidentifiedToken);
				j = 0;

			}

			if(line[i]!=' ' && line[i]!=9) insert(tokensList, token2);
			aux = 1;

		/* if the current char is a operator*/
		} else if(MatchOperator(line[i], tokensList, line[i+1], &i, lineNumber) != -1){

			// se o anterior é o final de um token (se for separador n precisa)
			if(aux == 0){

				ChooseType(unidentifiedToken, tokensList, i, lineNumber);
				cleanString(unidentifiedToken);
			}

			insert(tokensList, token2);
			aux = 1;

		/* the current char isn't a reserved symbol */
		} else {
			aux = 0;
			unidentifiedToken[j] = line[i];
			j++;
		}

	}
}

int isLiteral(char* line, int *i, list* tokensList, int lineNumber){

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

		token2.group = literal;
		token2.type = include;
		token2.name = unidentifiedToken;

		//printf("Token: %s group: %d type: %d\n\n",token2.name, token2.group, token2.type);
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

		token2.group = literal;
		token2.type = literal;
		token2.name = unidentifiedToken;

		//printf("Token: %s group: %d type: %d\n\n",token2.name, token2.group, token2.type);
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

		token2.group = literal;
		token2.type = literal;
		token2.name = unidentifiedToken;
	 
		//printf("Token: %s group: %d type: %d\n\n",token2.name, token2.group, token2.type);
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

			token2.group = header;
			token2.type = header;
			token2.name = unidentifiedToken;

			//printf("Token: %s group: %d type: %d\n\n",token2.name, token2.group, token2.type);
			free(unidentifiedToken);

			return *i;
		}
	}

	free(unidentifiedToken);
	return -1;
}

int isComment(char* line, int *i, int size, list* tokensList, int lineNumber){

	char* unidentifiedToken = (char*)malloc(strlen(line)*sizeof(char));
	int j = 0, aux = *i;

	if(line[aux] == '/' && line[aux+1] == '/'){

		while(aux < size){

			unidentifiedToken[j] = line[aux];
			j++;
			aux++;
		}

		token2.group = comment;
		token2.type = comment;
		token2.name = unidentifiedToken;

		 
		//printf("Token: %s group: %d type: %d\n\n",token2.name, token2.group, token2.type);
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

		token2.group = comment;
		token2.type = comment;
		token2.name = unidentifiedToken;

		 
		*i = aux+1;

		//printf("Token: %s group: %d type: %d\n\n",token2.name, token2.group, token2.type);
		free(unidentifiedToken);

		return aux;
	}

	return -1;
}

int isKeywords(char* unidentifiedToken, list* tokensList, int column, int lineNumber){

	token1.group = keywords;

	if(strcmp(unidentifiedToken, "int") == 0){
		token1.type = intKey;
		token1.name = "int";
	}else if(strcmp(unidentifiedToken, "char") == 0){
		token1.type = charKey;
		token1.name = "char";
	}else if(strcmp(unidentifiedToken, "float") == 0){
		token1.type = floatKey;
		token1.name = "float";
	}else if(strcmp(unidentifiedToken, "double") == 0){
		token1.type = doubleKey;
		token1.name = "double";
	}else if(strcmp(unidentifiedToken, "unsigned") == 0){
		token1.type = unsignedKey;
		token1.name = "unsigned";
	}else if(strcmp(unidentifiedToken, "for") == 0){
		token1.type = forKey;
		token1.name = "for";
	}else if(strcmp(unidentifiedToken, "long") == 0){
		token1.type = longKey;
		token1.name = "long";
	}else if(strcmp(unidentifiedToken, "while") == 0){
		token1.type = whileKey;
		token1.name = "while";
	}else if(strcmp(unidentifiedToken, "do") == 0){
		token1.type = doKey;
		token1.name = "do";
	}else if(strcmp(unidentifiedToken, "if") == 0){
		token1.type = ifKey;
		token1.name = "if";
	}else if(strcmp(unidentifiedToken, "return") == 0){
		token1.type = returnKey;
		token1.name = "return";
	}else if(strcmp(unidentifiedToken, "continue") == 0){
		token1.type = continueKey;
		token1.name = "continue";
	}else if(strcmp(unidentifiedToken, "break") == 0){
		token1.type = breakKey;
		token1.name = "break";
	}else if(strcmp(unidentifiedToken, "case") == 0){
		token1.type = caseKey;
		token1.name = "case";
	}else if(strcmp(unidentifiedToken, "switch") == 0){
		token1.type = switchKey;
		token1.name = "switch";
	}else if(strcmp(unidentifiedToken, "else") == 0){
		token1.type = elseKey;
		token1.name = "else";
	}else if(strcmp(unidentifiedToken, "defautl") == 0){
		token1.type = defaultKey;
		token1.name = "default";
	}else if(strcmp(unidentifiedToken, "struct") == 0){
		token1.type = structKey;
		token1.name = "struct";
	}else if(strcmp(unidentifiedToken, "typedef") == 0){
		token1.type = typedefKey;
		token1.name = "typedrf";
	}else if(strcmp(unidentifiedToken, "const") == 0){
		token1.type = constantKey;
		token1.name = "const";
	}else if(strcmp(unidentifiedToken, "asm") == 0){
		token1.type = asmKey;
		token1.name = "asm";
	}else if(strcmp(unidentifiedToken, "auto") == 0){
		token1.type = autoKey;
		token1.name = "auto";
	}else if(strcmp(unidentifiedToken, "enum") == 0){
		token1.type = enumKey;
		token1.name = "enum";
	}else if(strcmp(unidentifiedToken, "extern") == 0){
		token1.type = externKey;
		token1.name = "extern";
	}else if(strcmp(unidentifiedToken, "goto") == 0){
		token1.type = gotoKey;
		token1.name = "goto";
	}else if(strcmp(unidentifiedToken, "register") == 0){
		token1.type = registerKey;
		token1.name = "register";
	}else if(strcmp(unidentifiedToken, "short") == 0){
		token1.type = shortKey;
		token1.name = "short";
	}else if(strcmp(unidentifiedToken, "signed") == 0){
		token1.type = signedKey;
		token1.name = "signed";
	}else if(strcmp(unidentifiedToken, "sizeof") == 0){
		token1.type = sizeofKey;
		token1.name = "sizeof";
	}else if(strcmp(unidentifiedToken, "static") == 0){
		token1.type = staticKey;
		token1.name = "static";
	}else if(strcmp(unidentifiedToken, "union") == 0){
		token1.type = unionKey;
		token1.name = "union";
	}else if(strcmp(unidentifiedToken, "volatile") == 0){
		token1.type = volatileKey;
		token1.name = "volatile";
	}else if(strcmp(unidentifiedToken, "malloc") == 0){
		token1.type = mallocKey;
		token1.name = "malloc";
	}else if(strcmp(unidentifiedToken, "calloc") == 0){
		token1.type = callocKey;
		token1.name = "calloc";
	}else if(strcmp(unidentifiedToken, "free") == 0){
		token1.type = freeKey;
		token1.name = "free";
	}else{
		return -1;
	}

	 
	//printf("Token: %s group: %d type: %d\n\n", token1.name, token1.group, token1.type);
	insert(tokensList, token1);
	return 0;
}

int isIdentifierOrNumber(char* unidentifiedToken, list* tokensList, int column, int lineNumber){

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
			token1.group = identifiers;
			token1.type = identifiers;
			token1.name = unidentifiedToken;

			 
			//printf("Token: %s group: %d type: %d\n\n",token1.name, token1.group, token1.type);
			insert(tokensList, token1);
			return 0;
		}

	}

	/* include */
	if(strcmp(unidentifiedToken, "#include") == 0){

		token1.group = include;
		token1.type = include;
		token1.name = unidentifiedToken;

		 
		//printf("Token: %s group: %d type: %d\n\n",token1.name, token1.group, token1.type);
		insert(tokensList, token1);
		return 0;

	}

	/* define */
	if(strcmp(unidentifiedToken, "#define") == 0){

		token1.group = define;
		token1.type = define;
		token1.name = unidentifiedToken;

		 
		//printf("Token: %s group: %d type: %d\n\n",token1.name, token1.group, token1.type);
		insert(tokensList, token1);
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

		token1.group = number;
		token1.type = 000;
		token1.name = unidentifiedToken;

		 
		//printf("Token: %s group: %d type: %d\n\n", token1.name, token1.group, token1.type);
		insert(tokensList, token1);
		return 0;

	}

	return -1;
}

int MatchOperator(char unidentifiedOperator, list* tokensList, char next, int *i, int lineNumber){

	switch (unidentifiedOperator){

			case '+':

				if(next == '='){

					token2.group = arithmeticOperators;
					token2.type = increment;
					token2.name = "+=";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				} else if(next == '+'){

					token2.group = arithmeticOperators;
					token2.type = increment;
					token2.name = "++";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				} else {

					token2.group = arithmeticOperators;
					token2.type = plus;
					token2.name = "+";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}

				return *i;
			break;

			case '-':

				if(next == '='){

					token2.group = arithmeticOperators;
					token2.type = decrease;
					token2.name = "-=";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}else if(next == '-'){

					token2.group = arithmeticOperators;
					token2.type = decrease;
					token2.name = "--";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				} else {

					token2.group = arithmeticOperators;
					token2.type = minus;
					token2.name = "-";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '*':

				if(next == '='){

					token2.group = arithmeticOperators;
					token2.type = times;
					token2.name = "*=";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);

				} else {	

					token2.group = arithmeticOperators;
					token2.type = times;
					token2.name = "*";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '/':

				if(next == '='){

					token2.group = arithmeticOperators;
					token2.type = division;
					token2.name = "/=";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);

				} else {

					token2.group = arithmeticOperators;
					token2.type = division;
					token2.name = "/";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '>':

				if(next == '>'){

					token2.group = bitwise;
					token2.type = shiftRight;
					token2.name = ">>";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				} else if(next == '='){

					token2.group = logicalOperators;
					token2.type = isMoreOrEqual;
					token2.name = ">=";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				} else {
					token2.group = logicalOperators;
					token2.type = isMoreThan;
					token2.name = ">";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '<':

				if(next == '<'){

					token2.group = bitwise;
					token2.type = shiftLeft;
					token2.name = "<<";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				} else if(next == '='){

					token2.group = logicalOperators;
					token2.type = isLessOrEqual;
					token2.name = "<=";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				} else {
					token2.group = arithmeticOperators;
					token2.type = isLessThan;
					token2.name = "<";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '=':

				if(next == '='){

					token2.group = logicalOperators;
					token2.type = equals;
					token2.name = "==";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);	
				} else {
					token2.group = logicalOperators;
					token2.type = atrib;
					token2.name = "=";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '&':

				if(next == '&'){

					token2.group = logicalOperators;
					token2.type = andOperator;
					token2.name = "&&";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				} else {

					token2.group = bitwise;
					token2.type = comercial;
					token2.name = "&";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '|':

				if(next == '|'){

					token2.group = logicalOperators;
					token2.type = orOperator;
					token2.name = "||";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);

				} else {

					token2.group = bitwise;
					token2.type = bwor;
					token2.name = "|";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '!':

				if(next == '!'){

					token2.group = logicalOperators;
					token2.type = notOperator;
					token2.name = "!=";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);

				} else {

					token2.group = logicalOperators;
					token2.type = not;
					token2.name = "!";

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
				}
				return *i;
			break;

			case '?':

				if(next == ':'){

					token2.group = logicalOperators;
					token2.type = ternary;
					token2.name = "?:";

					*i+=1;

					 
					//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);

				}
			break;
		}
		return -1;
}

int MatchSeparator(char unidentifiedSeparator, list* tokensList, int column, int lineNumber){

	switch (unidentifiedSeparator){

		case ',':

			token2.group = separators;
			token2.type = comma;
			token2.name = ",";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
			return 0;

		break;

		case ';':

			token2.group = separators;
			token2.type = semicolon;
			token2.name = ";";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
			return 0;

		break;

		case '(':

			token2.group = separators;
			token2.type = leftParenthesis;
			token2.name = "(";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
			return 0;

		break;

		case ')':

			token2.group = separators;
			token2.type = rightParenthesis;
			token2.name = ")";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
			return 0;

		break;

		case '{':

			token2.group = separators;
			token2.type = leftBrace;
			token2.name = "{";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
			return 0;

		break;

		case '}':

			token2.group = separators;
			token2.type = rightBrace;
			token2.name = "}";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
			return 0;

		break;

		case '[':

			token2.group = separators;
			token2.type = leftBracket;
			token2.name = "[";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
			return 0;

		break;

		case ']':

			token2.group = separators;
			token2.type = rightBracket;
			token2.name = "]";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
			return 0;

		break;

		case ':':

			token2.group = separators;
			token2.type = colon;
			token2.name = ":";

			 
			//printf("Token: %s group: %d type: %d\n\n", token2.name, token2.group, token2.type);
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

void ChooseType(char* unidentifiedToken, list* tokensList, int i, int lineNumber){

	int column = i-strlen(unidentifiedToken);

	token1.column = column;

	int aux = isKeywords(unidentifiedToken, tokensList, i, lineNumber);


	if(aux == -1){

		aux = isIdentifierOrNumber(unidentifiedToken, tokensList, i, lineNumber);

		if(aux == -1){
			//printf("\n-------------------------------\n");
			//printf("Error 404: type not found\n");
			//printf("token: %s\n", unidentifiedToken);
			//printf("line: %d\n", lineNumber+1);
			//printf("column: %d\n", column);
			//printf("\n-------------------------------\n");
		}

		cleanString(unidentifiedToken);

	} else cleanString(unidentifiedToken);
}

void printLista(list* tokensList){

	list* aux = tokensList;
	printf("____________________________________________________________________________________\n\n");


	do{

		aux = aux->next;
		background(WHITE);
		foreground(BLACK);
		printf("Token:");
		background(BLACK);
		foreground(GREEN);
		printf("  %s \n",  aux->tok.name);
		foreground(WHITE);
		printf("\tgroup: %d type: %d \n\tposition: [%d:%d] \n\n",aux->tok.group, aux->tok.type, aux->tok.line, aux->tok.column);

	}while(aux->next != NULL);

	printf("____________________________________________________________________________________\n");

}