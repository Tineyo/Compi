/*

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    etat *dest; //Pointeur sur l'etat d'arrivée
    char alpha; //caractère à consommer pour réaliser la transition
}transition;

typedef struct {
    transition* transitions; //Tab des transitions partant de l'état
    int nb_transition; //Nombre de transitions (pour les boucles ) 
    int accepteur; // 0 si non accepteur , 1 si accepteur
}etat; 

typedef struct maill {
    	transition *data;
	maill *suivant;
}maillon_transition;

typedef struct maill { 
	etat *data;
	maill *suivant;
}maillon_etat;

typedef struct {
        
    char *alphabet; //Tableau composé des caractères de l'alphabet
    int nb_caractère //Nb de caractères de l'alphabet    
    int nb_etats; //Nb d'états
    etat *initial; //Pointeur sur l'état initial 
    maillon etats; //Liste des etats composant l'afn
}AFN;   

/*
typedef struct {
    etat *dest; //Pointeur sur l'etat d'arrivée
    char alpha; //caractère à consommer pour réaliser la transition
}transition;

typedef struct {
    transition* transitions; //Tab des transitions partant de l'état
    int nb_transition; //Nombre de transitions (pour les boucles ) 
    int accepteur; // 0 si non accepteur , 1 si accepteur
}etat; 

typedef struct {
        
    char *alphabet; //Tableau composé des caractères de l'alphabet
    int nb_caractere //Nb de caractères de l'alphabet    
    int nb_etats; //Nb d'états
    etat *initial; //Pointeur sur l'état initial 
    etat *etats; //Tableau des etats composant l'afn
}AFN;   


void print_automate(AFN automate)
{

	printf("Alphabet:\n")
	for ( int i = 0 ; i < nb_caractere ; i++)
		printf("%c ",automate.alphabet[i] );

	printf("Liste des états:\n");
	for ( int i = 0 ; i < nb_etats ; i++ )
		printf("%s ",automate.etats->nom); //a modifier 
	
	 printf("Etat initial: %s\n" , automate.initial->nom ) ; 
	
	printf (" Transitions \n ");

	for ( int i = 0 ; i < nb_etats ; i ++ )
		for ( int j = 0 ; j < nb_transitions ; j++ )
			printf(" (%s,%c,%s) " ,automate.etats[i] , automate.etats[i].transition.alpha , automate.etats[i].transition->dest.nom )

	for ( int i = 0 ; i < nb_etats ; i ++ )
		{
			if ( automate.etats[i].accepteur )
				printf( "%s" , automate.etats[i].nom ) ;
		}
}
*\
*/ 
