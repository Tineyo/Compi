#include <stdio.h>
#include <stdlib.h>

#include "AFN.c"

/*
struct Automate determiniser(struct Automate A1)
{
	int Taille=A1.Tetat*5;		//la taille du tableau depend du nombre d etats de base
	
	//tab va contenir les transitions entre les nouveaux etats  et -1 va marquer la fin d une colonne ou d une ligne
	int tab[Taille][A1.Talphabet+1];		
	//1 ere colonne: le nom des nouveaux etats, les colonnes suivantes sont les transitions depuis ce nouvel etat en utilisant 1 caractere de l'alphabet dans l'ordre
	
	int tab2[Taille][A1.Tetat+2];	
	//tab2 va indiquer de quoi sont constituer les nouveaux etats, par rapports aux anciens
	
	int inc=1;
	int test=-2;
	char car;
	char max=66;
	int nbetat=1;
	int tab3[A1.Tetat+2];		//tab3 contient les etats atteints par la transition etudiee
	int Ecloture[A1.Tetat][A1.Tetat+1];	//contient les Ecloture des transitions avec le mot vide
	
	printf("\nDeterminisation de l automate\n");
	
	for(int a=0;a<Taille;a++)		//on mets tous les tableaux a 0 ( pas de valeurs indeterminees )
	{
		for(int b=0;b<A1.Talphabet+1;b++)
		{
			tab[a][b]=0;
		}
		for(int b=0;b<A1.Tetat+2;b++)
		{
			tab2[a][b]=0;
		}
	}
	for(int b=0;b<A1.Tetat+2;b++)
	{
		tab3[b]=0;
	}
	for(int a=0;a<A1.Tetat;a++)
	{
		Ecloture[a][0]=A1.etat[a];
		Ecloture[a][1]=-1;
	}
	for(int a=0;a<A1.Tetat;a++)
	{
		for(int b=2;b<A1.Tetat+1;b++)
		{
			Ecloture[a][b]=0;
		}
	}
	
	for(int a=0;a<A1.Tetat;a++)		//on cherche tous les Eclotures, l existance de transitions E pour tous les anciens etats
	{
		inc=1;	//permet de se positionner dans le tableau Ecloture
		for(int b=0;b<A1.Ttransition;b++)
		{
			if((A1.transition[b].alpha=='\\')&&(A1.transition[b].src==Ecloture[a][0]))	//on regarde si l etat en question a une transition E
			{
				Ecloture[a][inc]=A1.transition[b].dest;
				inc++;
				Ecloture[a][inc]=-1;
			}
		}
	}
	
	tab[0][0]='A';		//initialisation des tableaux
	tab2[0][0]=tab[0][0];
	tab2[0][1]=A1.entre;
	tab2[1][0]='x';	//indique la fin de l etat etudie
	tab[1][0]='x';
	
	inc=2;
	for(int b=0;b<*Tetat;b++)	//on verifie par rapport a Ecloture pour l entre
	{
		if(A1.entre==Ecloture[b][0])
		{
			for(int z=1;Ecloture[b][z]!='x';z++)	//on verifie par rapport a Ecloture
			{
				tab2[0][inc]=Ecloture[b][z];
				inc++;
			}
		}
	}
	tab2[0][inc]='x';
	
	for(int i=0;i<Taille;i++)	//tous les nouveaux etats
	{
		for(int a=0;a<*Talphabet;a++)		//on va tester tous les caracteres possibles
		{
			inc=1;
			test=-1;
			for(int k=1;tab2[i][k]!='x';k++)		//on lis tous les etats inclus dans le nouvel etat
			{
				for(int j=0;j<*Ttransition;j++)		//on lis toutes les transitions qui partent de ce nouvel etat
				{
					if((A1.transition[j][0]==tab2[i][k])&&(A1.transition[j][1]==A1.alphabet[a]))		//la transition part d'un des etats etudie inclus dans le nouvel etat avec le caractere etudie
					{
						for(int b=1;b<inc;b++)	//on verifie qu on a pas deja enregistré cet etat
						{
							if(tab3[b]==A1.transition[j][2])
							{
								test=1;
							}
						}
						if(test<0)		//si nouvel etat alors on l enregistre
						{
							tab3[inc]=A1.transition[j][2];
							inc++;
							for(int b=0;b<*Tetat;b++)	//on verifie par rapport a Ecloture
							{
								if(A1.transition[j][2]==Ecloture[b][0])
								{
									for(int z=1;Ecloture[b][z]!='x';z++)	//on verifie par rapport a Ecloture
									{
										test=-1;
										for(int b=1;b<inc;b++)	//on verifie qu on a pas deja enregistré cet etat
										{
											if(tab3[b]==Ecloture[b][z])
											{
												test=1;
											}
										}
										if(test<0)
										{
											tab3[inc]=Ecloture[b][z];
											inc++;
										}
									}
								}
							}
						}
					}
				}
			}
				
			tab3[inc]='x';	//on ferme le tableau
			
			for(int x=1;tab3[x]!='x';x++)		//on trie les etats par ordre "croissant" car l etat 013 = 310: pour ne pas creer plusieurs fois le meme etat 
			{
				for(int y=x+1;tab3[y]!='x';y++)
				{
					if (tab3[x]>tab3[y]) 
					{
						car = tab3[x];
						tab3[x] = tab3[y];
						tab3[y] = car;
					}
				}
			}
				
			for(int x=0;x<nbetat;x++)		//on analyse tous les nouveaux etats existants
			{
				test=-2;
				for(int y=1;(tab3[y]!='x')&&(tab2[x][y]!='x');y++)		//on analyse les etats dans notre nouvel etat
				{
					if(tab3[y]!=tab2[x][y])		//2 elements qui ne correspondent pas : nouvel etat
					{
						test=1;
					}
				}
					
				if(tab3[inc]!=tab2[x][inc])	//pas la meme taille donc different
				{
					test=1;
				}
				
				if(test<0)		//etat deja existant, on ne creer pas cet etat dans tab2 mais on lui donne son nom dans tab
				{
					tab[i][a+1]=tab2[x][0];
					break;
				}
			}
			if(test>0)		//aucun etat equivalent donc creation nouvel etat
			{
				tab[i][a+1]=max;
				tab2[nbetat][0]=max;
				tab[nbetat][0]=max;
				for(int x=1;tab3[x-1]!='x';x++)
				{
					tab2[nbetat][x]=tab3[x];
				}
				max++;
				nbetat++;
				tab2[nbetat][0]='x';
				tab[nbetat][0]='x';
			}	
		}
		if(tab[i+1][0]=='x')
		{
			break;
		}
	}
	
	printf("\ntab\n  ");
	for(int a=0;a<*Talphabet;a++)
	{
		printf("%c ",A1.alphabet[a]);
	}
	printf("\n");
	for(int a=0;tab[a][0]!='x';a++)
	{
		for(int b=0; b<*Talphabet+1;b++)
		{
			printf("%c ",tab[a][b]);
		}
		printf("\n");
	}
	
	printf("\ntab2\n");
	for(int a=0;tab2[a][0]!='x';a++)
	{
		for(int b=0;tab2[a][b]!='x';b++)
		{
			printf("%c ",tab2[a][b]);
		}
		printf("\n");
	}
	
	printf("\nEcloture\n");
	for(int a=0;a<*Tetat;a++)
	{
		printf("%c : ",A1.etat[a]);
		for(int b=0;Ecloture[a][b]!='x';b++)
		{
			printf("%c ",Ecloture[a][b]);
		}
		printf("\n");
	}
	
	//on genere l automate depuis cette table
	A1.entre='A';
	
	int incsortie=0;
	for(int a=0;tab2[a][0]!='x';a++)		//on regarde tous les nouveaux etats
	{
		for(int b=1;tab2[a][b]!='x';b++)	//on regarde les anciens etats dans le nouvel etat
		{
			test=-2;
			for(int c=0;c<*Tsortie;c++)		//on regarde toutes les sorties existantes
			{
				if(tab2[a][b]==A1.sortie[c])
				{
					for(int d=0;d<incsortie;d++)	//on verifie elle est pas deja mise
					{
						if(tab3[d]==tab2[a][0])
						{
							test=1;
						}
					}
					if(test<0)
					{
						tab3[incsortie]=tab2[a][0];
						incsortie++;
					}
				}
			}	
		}
	}
	
	for(int i=0;i<*Ttransition;i++)		// la taille des etats, des sorties et des transitions a changer 
	{
		free(A1.transition[i]);
	}
	free(A1.transition);
	free(A1.etat);
	free(A1.sortie);
	
	*Tetat=nbetat;
	*Tsortie=incsortie;
	
	A1.etat=malloc(*Tetat*sizeof(char));
	A1.sortie=malloc(*Tsortie*sizeof(char));
	*Ttransition=(*Tetat)*(*Talphabet);
	A1.transition=malloc(*Ttransition*sizeof(char*));
	for(int i=0;i<*Ttransition;i++)
	{
		A1.transition[i]=malloc(3*sizeof(char));
	}

	for(int i=0;i<*Tetat;i++)	// les etats sont notes par ordre croissant
	{
		A1.etat[i]=65+i;
	}
	
	//initialisé A1.sortie
	for(int i=0;i<*Tsortie;i++)		//les sorties sont enregistres dans tab2
	{
		A1.sortie[i]=tab3[i];
	}
	
	inc=0;		//compte le nombre de case lu dans tab
	test=0;		//compte le numero de l etat et la colonne de tab
	for(int i=0;i<nbetat;i++)
	{
		for(int j=0;j<*Talphabet;j++)
		{
			A1.transition[inc][0]=A1.etat[test];	//0 correspond a l etat de depart
			A1.transition[inc][1]=A1.alphabet[j];	//1 correspond au caractere consomme
			A1.transition[inc][2]=tab[i][j+1];		//2 correspond a l arrive de l automate depuis 0 en prenant 1
			inc++;
		}
		test++;
	}
	
	return A1;
}

*/

struct Automate creation()
{
	Automate A1;
	
	int etat[]={1,2,3,4};
	char alphabet[]={'a','b'};
	int sortie[]={3,4};
	int entre=1;
	int transition[][3]={{1,0,2},{1,98,4},{2,97,2},{2,98,3},{4,97,4}};	// valeur ASCII du caractere consomme
	
	A1.Tetat=sizeof(etat)/sizeof(etat[0]);		// les tailles qui caracterisent l automate
	A1.Talphabet=sizeof(alphabet);
	A1.Tsortie=sizeof(sortie)/sizeof(sortie[0]);
	A1.Ttransition=sizeof(transition)/(3*sizeof(transition[0][0]));
	printf("Ttransition = %d \n",A1.Ttransition);
	
	A1.etat=malloc(A1.Tetat*sizeof(int));
	A1.alphabet=malloc(A1.Talphabet*sizeof(char));
	A1.sortie=malloc(A1.Tsortie*sizeof(char));
	A1.transition=malloc(A1.Ttransition*sizeof(char*));
	
	A1.entre=entre;
	for(int i=0;i<A1.Tetat;i++)
	{
		A1.etat[i]=etat[i];
	}
	for(int i=0;i<A1.Talphabet;i++)
	{
		A1.alphabet[i]=alphabet[i];
	}
	for(int i=0;i<A1.Tsortie;i++)
	{
		A1.sortie[i]=sortie[i];
	}
	for(int i=0;i<A1.Ttransition;i++)
	{
		A1.transition[i].src=transition[i][0];
		if(transition[i][1]==0)
		{
			A1.transition[i].alpha='\\';
		}else
		{
			A1.transition[i].alpha=transition[i][1];
		}
		A1.transition[i].dest=transition[i][2];
	}
	
	return A1;
}

int main(int argc, char *argv[])
{
	struct Automate A1=creation();
	
	printf("\nAutomate de depart :\n");
	affichage(A1);
	
	//A1=determiniser(A1,&Tetat,&Talphabet,&Ttransition,&Tsortie);	//on demande une determinisation de l automate
	
	return 0;
}
