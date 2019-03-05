#include <stdio.h>
#include <stdlib.h>

#include "AFN.c"


struct Automate determiniser(struct Automate A1)
{
	int Taille=A1.Tetat*5;		//la taille du tableau depend du nombre d etats de base
	
	//tab va contenir les transitions entre les nouveaux etats et -1 va marquer la fin d une colonne ou d une ligne
	int tab[Taille][A1.Talphabet+1];		
	//1 ere colonne: le nom des nouveaux etats, les colonnes suivantes sont les transitions depuis ce nouvel etat en utilisant 1 caractere de l'alphabet dans l'ordre
	
	int tab2[Taille][A1.Tetat+2];	
	//tab2 va indiquer de quoi sont constituer les nouveaux etats, par rapports aux anciens
	
	int inc=1;
	int test=-2;
	char car;
	int max=2;
	int nbetat=1;
	int tab3[A1.Tetat+2];		//tab3 contient les etats atteints par la transition etudiee
	int Ecloture[A1.Tetat][A1.Tetat+1];	//contient les Ecloture des transitions avec le mot vide, on a defini \ comme le mot vide
	
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
	
	for(int a=0;a<A1.Tetat;a++)		//on cherche tous les Eclotures, l existance de transitions mot vide pour tous les anciens etats
	{
		inc=1;	//permet de se positionner dans le tableau Ecloture
		for(int b=0;b<A1.Ttransition;b++)
		{
			//on regarde si l etat en question a une transition grace au mot vide
			if((A1.transition[b].alpha=='\\')&&(A1.transition[b].src==Ecloture[a][0]))
			{
				Ecloture[a][inc]=A1.transition[b].dest;
				inc++;
				Ecloture[a][inc]=-1;
			}
		}
	}
	
	tab[0][0]=1;		//initialisation des tableaux
	tab2[0][0]=tab[0][0];
	tab2[0][1]=A1.entre;
	tab2[1][0]=-1;	//indique la fin de l etat etudie
	tab[1][0]=-1;
	
	inc=2;
	
	//on verifie par rapport a Ecloture pour l entre si on dois prendre juste l etat d entre ou ceux toucher grace au mot vide
	for(int b=0;b<A1.Tetat;b++)	
	{
		if(A1.entre==Ecloture[b][0])
		{
			for(int z=1;Ecloture[b][z]!=-1;z++)	//on verifie par rapport a Ecloture
			{
				tab2[0][inc]=Ecloture[b][z];	//si l etat d entre va vers d autres etats grace au mot vide
				inc++;
			}
		}
	}
	tab2[0][inc]=-1;
	
	
	
	for(int i=0;i<Taille;i++)	//tous les nouveaux etats
	{
		for(int a=0;a<A1.Talphabet;a++)		//on va tester tous les caracteres possibles
		{
			inc=1;
			test=-1;
			for(int k=1;tab2[i][k]!=-1;k++)		//on lis tous les etats inclus dans le nouvel etat
			{
				for(int j=0;j<A1.Ttransition;j++)		//on lis toutes les transitions qui partent de ce nouvel etat
				{
					if((A1.transition[j].src==tab2[i][k])&&(A1.transition[j].alpha==A1.alphabet[a]))		//la transition part d'un des etats etudie inclus dans le nouvel etat avec le caractere etudie
					{
						for(int b=1;b<inc;b++)	//on verifie qu on a pas deja enregistré cet etat
						{
							if(tab3[b]==A1.transition[j].dest)
							{
								test=1;
							}
						}
						if(test<0)		//si nouvel etat alors on l enregistre
						{
							tab3[inc]=A1.transition[j].dest;
							inc++;
							for(int b=0;b<A1.Tetat;b++)	//on verifie par rapport a Ecloture
							{
								if(A1.transition[j].dest==Ecloture[b][0])
								{
									for(int z=1;Ecloture[b][z]!=-1;z++)	//on verifie par rapport a Ecloture
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
				
			tab3[inc]=-1;	//on ferme le tableau
			
			for(int x=1;tab3[x]!=-1;x++)		//on trie les etats par ordre "croissant" car l etat 013 = 310: pour ne pas creer plusieurs fois le meme etat 
			{
				for(int y=x+1;tab3[y]!=-1;y++)
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
				for(int y=1;(tab3[y]!=-1)&&(tab2[x][y]!=-1);y++)		//on analyse les etats dans notre nouvel etat
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
				for(int x=1;tab3[x-1]!=-1;x++)
				{
					tab2[nbetat][x]=tab3[x];
				}
				max++;
				nbetat++;
				tab2[nbetat][0]=-1;
				tab[nbetat][0]=-1;
			}	
		}
		if(tab[i+1][0]==-1)
		{
			break;
		}
	}
	
	printf("\ntab\n  ");
	for(int a=0;a<A1.Talphabet;a++)
	{
		printf("%c ",A1.alphabet[a]);
	}
	printf("\n");
	for(int a=0;tab[a][0]!=-1;a++)
	{
		for(int b=0; b<A1.Talphabet+1;b++)
		{
			printf("%d ",tab[a][b]);
		}
		printf("\n");
	}
	
	printf("\ntab2\n");
	for(int a=0;tab2[a][0]!=-1;a++)
	{
		for(int b=0;tab2[a][b]!=-1;b++)
		{
			printf("%d ",tab2[a][b]);
		}
		printf("\n");
	}
	
	printf("\nEcloture\n");
	for(int a=0;a<A1.Tetat;a++)
	{
		printf("%d : ",A1.etat[a]);
		for(int b=0;Ecloture[a][b]!=-1;b++)
		{
			printf("%d ",Ecloture[a][b]);
		}
		printf("\n");
	}
	
	//on genere l automate depuis cette table
	A1.entre=1;
	
	int incsortie=0;
	for(int a=0;tab2[a][0]!=-1;a++)		//on regarde tous les nouveaux etats
	{
		for(int b=1;tab2[a][b]!=-1;b++)	//on regarde les anciens etats dans le nouvel etat
		{
			test=-2;
			for(int c=0;c<A1.Tsortie;c++)		//on regarde toutes les sorties existantes
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
	
	free(A1.transition);
	free(A1.etat);
	free(A1.sortie);
	
	A1.Tetat=nbetat;
	A1.Tsortie=incsortie;
	
	A1.etat=malloc(A1.Tetat*sizeof(int));
	A1.sortie=malloc(A1.Tsortie*sizeof(int));
	A1.Ttransition=(A1.Tetat)*(A1.Talphabet);
	A1.transition=malloc(A1.Ttransition*sizeof(Transition));

	for(int i=0;i<A1.Tetat;i++)	// les etats sont notes par ordre croissant
	{
		A1.etat[i]=i+1;
	}
	
	//initialisé A1.sortie
	for(int i=0;i<A1.Tsortie;i++)		//les sorties sont enregistres dans tab2
	{
		A1.sortie[i]=tab3[i];
	}
	
	inc=0;		//compte le nombre de case lu dans tab
	test=0;		//compte le numero de l etat et la colonne de tab
	for(int i=0;i<nbetat;i++)
	{
		for(int j=0;j<A1.Talphabet;j++)
		{
			A1.transition[inc].src=A1.etat[test];	//0 correspond a l etat de depart
			A1.transition[inc].alpha=A1.alphabet[j];	//1 correspond au caractere consomme
			A1.transition[inc].dest=tab[i][j+1];		//2 correspond a l arrive de l automate depuis 0 en prenant 1
			inc++;
		}
		test++;
	}
	
	return A1;
}



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
	
	A1.etat=malloc(A1.Tetat*sizeof(int));
	A1.alphabet=malloc(A1.Talphabet*sizeof(char));
	A1.sortie=malloc(A1.Tsortie*sizeof(char));
	A1.transition=malloc(A1.Ttransition*sizeof(Transition));
	
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
	
	A1=determiniser(A1);	//on demande une determinisation de l automate
	affichage(A1);
	
	return 0;
}
