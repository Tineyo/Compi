#include "AFN.h"

void affichage (Automate A1)
{
	printf("\nA1.entre = %d \n",A1.entre);		// on affiche l automate deterministe minimise
	printf("\nLa liste des etats : \n");
	for(int i=0;i<A1.Tetat;i++)
	{
		printf("A1.etat[%d] = %d \n",i,A1.etat[i]);
	}
	/*printf("\nLa liste des caracteres : \n");
	for(int i=0;i<A1.Talphabet;i++)
	{
		printf("A1.alphabet[%d] = %c \n",i,A1.alphabet[i]);
	}*/
	printf("\nLa liste des sorties : \n");
	for(int i=0;i<A1.Tsortie;i++)
	{
		printf("A1.sortie[%d] = %d \n",i,A1.sortie[i]);
	}
	printf("\nLa liste des transistions : \n");
	for(int i=0;i<A1.Ttransition;i++)
	{
		printf("transition de %d avec %c vers %d \n",A1.transition[i].src,A1.transition[i].alpha,A1.transition[i].dest);
	}
	return;
}

Automate build_langage_vide()
{
	Automate a;
	a.etat=malloc(sizeof(int));

	a.Tetat=1;
	a.Ttransition=0;
	a.Tsortie=0;
	a.Talphabet=0;

	a.etat[0]=0;
	a.alphabet=NULL;
	a.entre=0;
	a.sortie=NULL;

	return a;
}

Automate build_mot_vide()
{
	Automate a;
	a.etat=malloc(sizeof(int));
	a.sortie=malloc(sizeof(int));

	a.Tetat=1;
	a.Ttransition=0;
	a.Tsortie=1;
	a.Talphabet=0;	

	a.etat[0]=0;
	a.alphabet=NULL;
	a.entre=0;
	a.sortie[0]=0;

	return a;
}

Automate build_mot(char mot)
{
	Automate a;
	
	a.Tetat=2;
	a.Ttransition=1;
	a.Tsortie=1;
	a.Talphabet=1;	

	a.etat=malloc(a.Tetat*sizeof(int));
	a.sortie=malloc(a.Tsortie*sizeof(int));
	a.alphabet=malloc(a.Talphabet*sizeof(char));
	a.transition=malloc(a.Ttransition*sizeof(Transition));		
	
	a.etat[0]=0;
	a.etat[1]=1;

	a.alphabet[0]=mot;
	a.entre=0;
	a.transition[0].src=a.etat[0];
	a.transition[0].alpha=mot;
	a.transition[0].dest=a.etat[1];
	a.sortie[0]=1;

	return a;
}

Automate renommage_etat ( Automate a , int inc )
{
	
	for (int i = 0 ; i < a.Tetat ; i++)
	{
		a.etat[i]+=inc;
	}
	a.entre+=inc;
	
	for ( int i = 0 ; i < a.Ttransition ; i++ )
	{
		a.transition[i].src+=inc;
		a.transition[i].dest+=inc;
	}
	
	for ( int i = 0 ; i < a.Tsortie ; i++ )
	{
		a.sortie[i]+=inc;
	} 
	return a;
}

Automate union_automate( Automate a  , Automate b ) 
{
	Automate res;
	res.Tetat=a.Tetat+b.Tetat-1;
	res.Ttransition=a.Ttransition+b.Ttransition;
	res.Tsortie=a.Tsortie+b.Tsortie;
	//res.Talphabet;
	b=renommage_etat(b,a.Tetat);
	res.etat=malloc(res.Tetat*sizeof(int));
	res.transition=malloc(res.Tetat*sizeof(Transition));
	res.sortie=malloc(res.Tsortie*sizeof(int));

	res.etat[0]=0;
	res.entre=0;
	
	for ( int i = 1  ; i < a.Tetat ; i++ )
	{
		res.etat[i]=a.etat[i];		
	}
		
	for ( int i = 1 ; i < b.Tetat ; i++ )
	{
		res.etat[i+a.Tetat-1]=b.etat[i];
	}
	
	for ( int i = 0 ; i < a.Ttransition ; i++ )
	{
		res.transition[i]=a.transition[i];
		if ( res.transition[i].src = a.entre )
			res.transition[i].src = 0;
	}

	for ( int i = 0 ; i < b.Ttransition ; i++ )
	{
		res.transition[i+a.Ttransition]=b.transition[i];
		if ( res.transition[i+a.Ttransition].src = b.entre )
			res.transition[i+a.Ttransition].src = 0;
	}
	
	for ( int i = 0 ; i < a.Tsortie ; i++ )
	{
		res.sortie[i]=a.sortie[i];
	}
	for ( int i = 0 ; i < b.Tsortie ; i++ )
	{
		res.sortie[i+a.Tsortie]=b.sortie[i];
	}
	return res;
}

