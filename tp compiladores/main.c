#include "bib.h"

int main(int argc, char const *argv[]){

	char *fileName = (char*)malloc(250*sizeof(char));
	strcpy(fileName, argv[1]);
	readFile(fileName);

	return 0;
}