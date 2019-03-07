#include "AFN.h"
#include "AFD.h"

int main(void) 
{	
	//Exemple pour l'expression régulière  b*a|a*b	
	Automate c,d,e;	
	c=concatenation_automate(kleen(build_mot('a')),build_mot('b'));	
	d=concatenation_automate(build_mot('b'),kleen(build_mot('a')));
	e=union_automate(c,d);	
	affichage(e);
	e=determiniser(e);
	e=minimiser(e);
	affichage(e);
	test_mot(e,"aaaab");
	
	return 0;
}
