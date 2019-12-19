int main(int a, char b)	{

	int cont, i;

	if(i < cont){
		cont = 0;
	} else {
		cont = 1;
	}

	for(int j = 0; j < 3; j++){

		if(j == cont){
			cont = 0;
		} else {
			cont = 1;
		}

	}

	while(cont < 10){

		cont = cont++;

	}

}