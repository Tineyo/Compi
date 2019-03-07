#include <stdio.h>
#include <stdlib.h>

#include "AFN.c"
#include "AFD.c"

int main(int argc, char *argv[])
{
	struct Automate A1=creation();
	
	printf("\nAutomate de depart :\n");
	affichage(A1);
	
	A1=determiniser(A1);	//on demande une determinisation de l automate
	affichage(A1);
	
	A1=minimiser(A1);	//on demande une minimisation de l automate
	affichage(A1);
	
	char mot[]={'b','a','a','b'};
	//char mot[]={'a'};
	
	test_mot(A1,mot);
	
	return 0;
}
