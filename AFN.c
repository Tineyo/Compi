#include "AFN.h"

void affichage (Automate A1)
{
	printf("\nA1.entre = %d \n",A1.entre);// on affiche l automate deterministe minimise
	
	printf("\nLa liste des %d etats : \n",A1.Tetat);
	for(int i=0;i<A1.Tetat;i++)
	{
		printf("A1.etat[%d] = %d \n",i,A1.etat[i]);
	}
	
	printf("\nLa liste des %d caracteres : \n",A1.Talphabet);
	for(int i=0;i<A1.Talphabet;i++)
	{
		printf("A1.alphabet[%d] = %c \n",i,A1.alphabet[i]);
	}
	
	printf("\nLa liste des %d sorties : \n",A1.Tsortie);
	for(int i=0;i<A1.Tsortie;i++)
	{
		printf("A1.sortie[%d] = %d \n",i,A1.sortie[i]);
	}
	
	printf("\nLa liste des %d transitions : \n",A1.Ttransition);
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
	char *str;
	str=malloc((a.Talphabet+b.Talphabet)*sizeof(char));
	
	strcpy(str,a.alphabet);
	strcat(str,b.alphabet);
	retirer_doublon(str);	

	res.Talphabet=strlen(str);
	res.Tetat=a.Tetat+b.Tetat-1;
	res.Ttransition=a.Ttransition+b.Ttransition;
	res.Tsortie=a.Tsortie+b.Tsortie;


	b=renommage_etat(b,a.Tetat-1);
	res.etat=malloc(res.Tetat*sizeof(int));
	res.transition=malloc(res.Tetat*sizeof(Transition));
	res.sortie=malloc(res.Tsortie*sizeof(int));
	res.alphabet=malloc(res.Talphabet*sizeof(char));	

	strcpy(res.alphabet,str);
	
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
		if ( res.transition[i].src == a.entre )
			res.transition[i].src = 0;
	}

	for ( int i = 0 ; i < b.Ttransition ; i++ )
	{
		res.transition[i+a.Ttransition]=b.transition[i];
		if ( res.transition[i+a.Ttransition].src == b.entre )
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

Automate concatenation_automate ( Automate a , Automate b )
{
	Automate res;
	int count=0;
	Transition *tmp; //stocke les transition de b pour lequels il faut remplacer l'etat initial de b par les etats terminaux de a
	char *str;
	
	str=malloc((a.Talphabet+b.Talphabet)*sizeof(char));
	
	strcpy(str,a.alphabet);
	strcat(str,b.alphabet);
	retirer_doublon(str);

	res.Talphabet=strlen(str);
	res.Tetat=a.Tetat+b.Tetat-1;
	res.Tetat=a.Tetat+b.Tetat-1;
	res.Tsortie=b.Tsortie;	

	
	b=renommage_etat(b,a.Tetat-1);
	//On compte le nombre transition sortant de l'état initial du second automate pour calculer le nombre total de transition après concaténation
	for (int i = 0 ; i < b.Ttransition ; i++ )  
	{
		if ( b.transition[i].src == b.entre )
			count++;
	}
	tmp = malloc(count*sizeof(Transition));
	for (int i = 0 ; i < b.Ttransition ; i++ )  
	{
		if ( b.transition[i].src == b.entre )
			tmp[i]=b.transition[i];
	}

	res.Ttransition=a.Ttransition+a.Tsortie*count-count+b.Ttransition;
	
	res.alphabet=malloc(res.Talphabet*sizeof(char));
	res.etat=malloc(res.Tetat*sizeof(int));
	res.transition=malloc(res.Tetat*sizeof(Transition));
	res.sortie=malloc(res.Tsortie*sizeof(int));
	
	strcpy(res.alphabet,str);
	
	
	for ( int i = 0  ; i < a.Tetat ; i++ )
	{
		res.etat[i]=a.etat[i];		
	}
		
	for ( int i = 1 ; i < b.Tetat ; i++ )
	{
		res.etat[i+a.Tetat-1]=b.etat[i];
	}

	for ( int i = 0 ; i < b.Tsortie ; i ++ )
	{
		res.sortie[i] = b.sortie[i];
	}
	
	for ( int i = 0 ; i < a.Ttransition ; i++ )
	{
		res.transition[i] = a.transition[i];
	}
	for ( int i = 0 ; i < b.Ttransition ; i++ )
	{
		if ( b.transition[i].dest != b.entre )
			res.transition[i+a.Ttransition]=b.transition[i];
	}
	
	int k=a.Ttransition+b.Ttransition-count;
	for ( int i = 0 ; i < a.Tsortie ; i++ )
	{
		for (int j = 0 ; j < count ; j++ )
		{
			res.transition[k+i*count+j]=tmp[j];
			res.transition[k+i*count+j].src=a.sortie[i];
		}
	}
	res.entre=a.entre;
	return res;
}

Automate kleen ( Automate a )
{
	Automate res;
	int count = 0 ;
	Transition *tmp;
	res=a;
	
	for (int i = 0 ; i < a.Ttransition ; i++ )  
	{
		if ( a.transition[i].src == a.entre )
			count++;
	}

	tmp = malloc(count*sizeof(Transition));
	for (int i = 0 ; i < a.Ttransition ; i++ )  
	{
		if ( a.transition[i].src == a.entre )
			tmp[i]=a.transition[i];
	}
	
	int k=res.Ttransition;
	res.Ttransition+=count*res.Tsortie;
	res.transition=malloc(res.Ttransition*sizeof(Transition));
	
	for( int i = 0 ; i < k ; i++)
	{
		res.transition[i]=a.transition[i];
	}
	
	for ( int i = 0 ; i < a.Tsortie ; i++ )
	{
		for (int j = 0 ; j < count ; j++ )
		{
			res.transition[k+i*count+j]=tmp[j];
			res.transition[k+i*count+j].src=a.sortie[i];
		}
	}
	
	return res;
}
