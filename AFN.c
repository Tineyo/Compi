#include "AFN.h"

void affichage (Automate A1)
{
	printf("\nEtat initial = %d \n",A1.entre);
	
	printf("\nLa liste des etats : \n");
	for(int i=0;i<A1.Tetat;i++)
	{
		printf("%d ",A1.etat[i]);
	}
	
	printf("\nAlphabet : \n");
	for(int i=0;i<A1.Talphabet;i++)
	{
		printf("%c ",A1.alphabet[i]);
	}
	
	printf("\nLa liste des états accepteurs : \n");
	for(int i=0;i<A1.Tsortie;i++)
	{
		printf("%d ",A1.sortie[i]);
	}
	
	printf("\nLa liste des transitions : \n");
	for(int i=0;i<A1.Ttransition;i++)
	{
		printf("Transition de %d avec %c vers %d \n",A1.transition[i].src,A1.transition[i].alpha,A1.transition[i].dest);
	}
	return;
}

Automate build_langage_vide()
{
	Automate a;
	a.etat=malloc(sizeof(int));
	
	//Initialisation des taille de tableau
	a.Tetat=1;
	a.Ttransition=0;
	a.Tsortie=0;
	a.Talphabet=0;

	//Initialisation des tableaux
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
	a.alphabet=malloc(sizeof(int));
	
	a.Tetat=1;
	a.Ttransition=0;
	a.Tsortie=1;
	a.Talphabet=1;	

	a.etat[0]=0;
	a.alphabet[0]='\\';
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
	
	//On incrémente tous les états	
	for (int i = 0 ; i < a.Tetat ; i++)
	{
		a.etat[i]+=inc;
	}
	//On incrémente l'entrée
	a.entre+=inc;
	
	//On incrémente la source et la destionation de chaque transition
	for ( int i = 0 ; i < a.Ttransition ; i++ )
	{
		a.transition[i].src+=inc;
		a.transition[i].dest+=inc;
	}
	
	//On incrémente toutes les sorties
	for ( int i = 0 ; i < a.Tsortie ; i++ )
	{
		a.sortie[i]+=inc;
	} 
	return a;
}

void retirer_doublon ( char *str )
{
	int tab[256]={0};
	int index1=0 , index2=0;
	char temp;
	while ( str[index1] )
	{
		temp = str[index1];
		if(tab[temp]==0)
		{
			tab[temp]=1;
			str[index2]=str[index1];
			index2++;	
		}
		index1++;
	}
	str[index2]='\0';
}

Automate union_automate( Automate a  , Automate b ) 
{
	Automate res;
	char *str; //Chaine qui va contenir la concaténation des deux alphabet temporairement
		
	str=malloc((a.Talphabet+b.Talphabet)*sizeof(char)); 
	
	strcpy(str,a.alphabet); //On copie a dans str
	strcat(str,b.alphabet); //On concatène b à str
	retirer_doublon(str); //On retire les caractères en plusieurs fois dans str

	//Initialisation de la taille des tableaux
	res.Talphabet=strlen(str); 
	res.Tetat=a.Tetat+b.Tetat-1; 
	res.Ttransition=a.Ttransition+b.Ttransition;
	res.Tsortie=a.Tsortie+b.Tsortie;

	//Renommge des états de b
	b=renommage_etat(b,a.Tetat-1);

	//Allocation des tableaux de res
	
	res.etat=malloc(res.Tetat*sizeof(int));
	res.sortie=malloc(res.Tsortie*sizeof(int));
	res.alphabet=malloc(res.Talphabet*sizeof(char));	
	res.transition=malloc(res.Ttransition*sizeof(Transition));
	//Remplissage du tableau contenant l'aphabet
	strcpy(res.alphabet,str);

	//Initialisation de l'état initial
	res.etat[0]=0; 
	res.entre=0;
	
	//Remplissage du tableau contenant les états
	for ( int i = 1  ; i < a.Tetat ; i++ )
	{
		res.etat[i]=a.etat[i];		
	}
		
	for ( int i = 1 ; i < b.Tetat ; i++ )
	{
		res.etat[i+a.Tetat-1]=b.etat[i];
	}
	
	//Remplissage du tableau contenant les transitions
	for ( int i = 0 ; i < a.Ttransition ; i++ )
	{
		res.transition[i]=a.transition[i];
		if ( res.transition[i].src == a.entre ) //On modifie la source de la transition si la source est l'état initial de l'automate est l'entrée
			res.transition[i].src = 0; //Par le nouvel état initial
	}

	for ( int i = 0 ; i < b.Ttransition ; i++ )
	{
		res.transition[i+a.Ttransition]=b.transition[i];
		if ( res.transition[i+a.Ttransition].src == b.entre )
			res.transition[i+a.Ttransition].src = 0;
	}
	
	//Remplissage du tableau contenant les états terminaux
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

Automate concatenation_automate ( Automate a , Automate b )
{
	Automate res;
	int count=0,inc=0;
	Transition *tmp; //stocke les transition de b pour lequels il faut remplacer l'etat initial de b par les etats terminaux de a
	char *str;
	b=renommage_etat(b,a.Tetat-1);
	str=malloc((a.Talphabet+b.Talphabet)*sizeof(char));
	strcpy(str,a.alphabet);
	strcat(str,b.alphabet);
		
	retirer_doublon(str);

	res.Talphabet=strlen(str);
	res.Tetat=a.Tetat+b.Tetat-1;
	if ( b.entre == b.sortie[0] ) //Si l'état initial de b est accepteur, les états accepteur de la concaténation sont ceux de a et de b	
		res.Tsortie=a.Tsortie+b.Tsortie-1;	
	else //Sinon ce sont uniquement ceux de b
		res.Tsortie=b.Tsortie;

	
	
	//On compte le nombre transition sortant de l'état initial du second automate pour calculer le nombre total de transition après concaténation
	for (int i = 0 ; i < b.Ttransition ; i++ )  
	{
		if ( b.transition[i].src == b.entre )
			count++;
	}
	tmp = malloc(count*sizeof(Transition));
	
	res.Ttransition=a.Ttransition+a.Tsortie*count-count+b.Ttransition;
	
	//Allocation des tableaux de res

	res.alphabet=malloc(res.Talphabet*sizeof(char));
	res.etat=malloc(res.Tetat*sizeof(int));
	res.sortie=malloc(res.Tsortie*sizeof(int));
	res.transition=malloc(res.Ttransition*sizeof(Transition));
	

	//On remplit tmp avec les transitions voulues
	for (int i = 0 ; i < b.Ttransition ; i++ )  
	{
		if ( b.transition[i].src == b.entre )
			tmp[i]=b.transition[i];
	}
	
	strcpy(res.alphabet,str);
	
	//Remplissage du tableau contenant les états
	for ( int i = 0  ; i < a.Tetat ; i++ )
	{
		res.etat[i]=a.etat[i];		
	}
		
	for ( int i = 1 ; i < b.Tetat ; i++ )
	{
		res.etat[i+a.Tetat-1]=b.etat[i];
	}
	
	//Remplissage du tableau contant les états terminaux
	if ( b.entre == b.sortie[0] )
	{
		for ( int i= 0 ; i < a.Tsortie ; i++ )
		{
			res.sortie[i] = a.sortie[i];
		}

		inc=a.Tsortie;
		for ( int i = 1 ; i < b.Tsortie ; i ++ )
		{
			res.sortie[i+inc-1] = b.sortie[i];
		}
	}
	else
	{			
		for ( int i = 0 ; i < b.Tsortie ; i ++ )
		{
			res.sortie[i] = b.sortie[i];
		}
	}
	
	
	//Remplissage du tableau contenant les transitions
	int k=0	;
	for ( int i = 0 ; i < a.Ttransition ; i++ )
	{
		res.transition[i] = a.transition[i];
		k++;
	}
	for ( int i = 0 ; i < b.Ttransition ; i++ )
	{
		if ( b.transition[i].src != b.entre )
			{
				res.transition[k]=b.transition[i];
				k++;			
			}		
	}
	for ( int i = 0 ; i < a.Tsortie ; i++ ) //Pour chaque état terminal
	{
		for (int j = 0 ; j < count ; j++ ) //Pour chaque transition ayant pour origine l'état initial de b
		{
			res.transition[k+i*count+j]=tmp[j]; //On ajoute la transition 
			res.transition[k+i*count+j].src=a.sortie[i]; //On modifie la source
		}
	}
	
	//Définition de l'état d'entrée de res
	res.entre=a.entre;
	return res;
}

Automate kleen ( Automate a )
{
	Automate res;
	int count = 0 ;
	Transition *tmp; //Même chose que pour la concaténation
	
	res.Talphabet=a.Talphabet;
	res.Tetat=a.Tetat;


	res.alphabet=malloc(res.Talphabet*sizeof(char));
	res.etat=malloc(res.Tetat*sizeof(int));
	res.entre=a.entre;

	for ( int i = 0 ; i < res.Tetat ; i++ )
	{
		res.etat[i]=a.etat[i];	
	}
	strcpy(res.alphabet,a.alphabet);
	
	//On compte le nombre de transtion ayant pour origine l'état initial de a
	for (int i = 0 ; i < a.Ttransition ; i++ )  
	{
		if ( a.transition[i].src == a.entre )
			count++;
	}
	
	tmp = malloc(count*sizeof(Transition));
	//On les stocke dans tmp
	for (int i = 0 ; i < a.Ttransition ; i++ )  
	{
		if ( a.transition[i].src == a.entre )
			tmp[i]=a.transition[i];
	}
	
	res.Ttransition=a.Ttransition+count*a.Tsortie; //Modification du nombre de transition
	res.transition=malloc(res.Ttransition*sizeof(Transition));
	
	//Remplissage du tableau de transition
	for( int i = 0 ; i < a.Ttransition ; i++) 
	{
		res.transition[i]=a.transition[i];
	}
	
	for ( int i = 0 ; i < a.Tsortie ; i++ ) //Pour chaque état terminal
	{
		for (int j = 0 ; j < count ; j++ ) //Pour chaque transition ayant pour source l'état initial
		{
			res.transition[a.Ttransition+i*count+j]=tmp[j]; //On ajoute la transition
			res.transition[a.Ttransition+i*count+j].src=a.sortie[i];//On modifie la source
		}
	}
	
	//Ajout de l'état initial comme terminal
	res.Tsortie=a.Tsortie+1;
	res.sortie=malloc(res.Tsortie*sizeof(int));
	res.sortie[0]=0;
	for ( int i = 1 ; i < res.Tsortie ; i++ )
		res.sortie[i]=a.sortie[i-1];
	return res;
}
