#include "sintatico.h"

int line = 0;

list *createList(){

	list *tokensList = (list*)malloc(sizeof(list));

	if(tokensList == NULL){
		printf("Pointer error (tokensList is NULL): file.c line 7\n"); // MUDAR A LINHA //
		exit(1);
	}

	tokensList->next = NULL;
	tokensList->empty = 1;

	return tokensList;

}

void insert(list* tokensList, Token token){

	list *node = (list*)malloc(sizeof(list));
	node->tok.name = (char*)malloc(strlen(token.name)*sizeof(char));

	if(node == NULL){
		printf("Pointer error (node is NULL) : file.c line \n"); /*  MUDAR A LINHA */
		exit(1);
	}

	if(strlen(token.name) > 0){
		node->tok.group = token.group;
		node->tok.type = token.type;
		node->tok.line = token.line+1;
		node->tok.column = token.column+1;

		strcpy(node->tok.name, token.name);

		node->next = NULL;

		if(tokensList->empty){
			
			tokensList->next = node;
			tokensList->empty = 0;

		}else{

			list *aux = tokensList->next;
			while(aux->next != NULL) aux = aux->next;

			aux->next = node;
		}
	}

}

void readFile(char* fileName){

	FILE *file = fopen(fileName, "r");
	list *tokensList = createList();

	if(file == NULL){
		printf("Failed to open file %s\n", fileName);
		exit(1);
	}

	char *fileLine = (char*)malloc(250*sizeof(char));

	if(fileLine == NULL){
		printf("Pointer error (line is NULL): file.c line 30\n"); // MUDAR A LINHA //
		exit(1);
	}

	while(!feof(file)){
		fgets(fileLine, 250, file);
		lexicalAnalyzer(tokensList, fileLine, line);
		line++;
	}
	
	printLista(tokensList);
	language(tokensList);


}
