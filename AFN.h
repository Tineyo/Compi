#ifndef AFN_H
#define AFN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Transition
{
	int src ,dest;
	char alpha;
}Transition;

typedef struct Automate
{
	int *etat;
	char *alphabet;
	int entre;
	int *sortie;
	Transition *transition;

	int Tetat,Talphabet,Tsortie,Ttransition;
}Automate;

void affichage (Automate A1);
Automate build_langage_vide();
Automate build_mot_vide();
Automate build_mot(char mot);
Automate renommage_etat( Automate a , int inc );
Automate union_automate( Automate a  , Automate b );
Automate concatenation_automate( Automate a  , Automate b );
#endif
