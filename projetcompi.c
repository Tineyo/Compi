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
	
	/*Automate a,b,c;
	a=build_mot('a');
	a=kleen(a);
	b=build_mot('b');
	a=concatenation_automate(a,b);
	c=build_mot('a');
	c=kleen(c);
	b=concatenation_automate(b,c);
	a=union_automate(a,b);
	affichage(a);
	*/
	
	return 0;
}
