int main(int a, char b)	{

	int cont;

	if(i < cont){
		cont = 0;
	} else {
		cont = 1;
	}

	for(int i = 0; i < 3; i++){

		if(i == cont){
			cont = 0;
		} else {
			cont = 1;
		}

	}

	while(cont < 10){

		cont++;

	}

	do{

		cont--;

	} while(cont > 0);

}