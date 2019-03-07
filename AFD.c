#include "AFD.h"


void test_mot(struct Automate A1, char* mot)
{
	int etatactuel=A1.entre;
	int k;
	
	printf("\nTest du mot %s \n",mot);
	printf("Sur un automate supposé fini et déterministe avec comme caractères : ");
	for(int i=0; i<A1.Talphabet;i++)
	{
		printf("%c  ",A1.alphabet[i]);
	}
	printf("\nEtat de départ : %d \n",A1.entre);
	
	for(int i=0; i<strlen(mot);i++)		//on lits toutes les lettres du mots
	{
		for(k=0; k<A1.Ttransition;k++)	//on cherche la transition qui part de l etat actuel et qui consomme le caractere demande
		{
			if((A1.transition[k].src==etatactuel)&&(A1.transition[k].alpha==mot[i]))
			{
				printf("\nEtat actuel : %d avec %c comme caractère a consommé et destination : %d",etatactuel,mot[i],A1.transition[k].dest);
				etatactuel=A1.transition[k].dest;		//on modifie notre etat
				k=A1.Ttransition+2;		//on a trouve la transition voulu on sort de la boucle
			}	
		}
		if(k==A1.Ttransition)		//si on ne trouve pas de transition alors on ne peut pas executer ce mot
		{
			printf("\n\nDepuis l'état %d, on ne peut pas effectuer une transition en consommant %c : il est impossible d'exécuter %s sur cette automate.\n\n",etatactuel,mot[i],mot);
			return;
		}
	}
	
	printf("\nEtat final : %d avec les états de sorties qui sont : ",etatactuel);
	for(int i=0; i<A1.Tsortie;i++)
	{
		printf("%d  ",A1.sortie[i]);
	}
	
	int test=0;
	for(int i=0; i<A1.Tsortie;i++)
	{
		if(etatactuel==A1.sortie[i])
		{
			printf("\n\nIl est possible d'exécuter ce mot avec cet AFD car %d est un état accepteur.\n\n",A1.sortie[i]);
			test=1;
		}
	}
	if(test==0)
	{
		printf("\n\nIl est impossible d'exécuter ce mot avec cet AFD car l'état final ne correspond pas à un des états accepteurs.\n\n");
	}
	return;
}

Automate determiniser(struct Automate A1)
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
		for(int b=0;b<A1.Ttransition;b++)	//on lit toutes les transitions
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
	
	tab[0][0]=1;		//initialisation des tableaux on considere que le premier etat sera 1 et les autres seront juste une incrementation
	tab2[0][0]=tab[0][0];
	tab2[0][1]=A1.entre;
	tab2[1][0]=-1;	//indique la fin de l etat etudie
	tab[1][0]=-1;
	
	inc=2;
	
	//on verifie par rapport a Ecloture pour l entre si on dois prendre juste l etat de depart ou ceux toucher grace au mot vide
	for(int b=0;b<A1.Tetat;b++)	
	{
		if(A1.entre==Ecloture[b][0])
		{
			for(int z=1;Ecloture[b][z]!=-1;z++)	//on verifie par rapport a Ecloture
			{
				tab2[0][inc]=Ecloture[b][z];	//si l etat de depart va vers d autres etats grace au mot vide
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
				tab[i][a+1]=max;	//le nom du nouvel etat vaux celui du precedent etat creee +1
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
		if(tab[i+1][0]==-1)	//si pas de nouvel etat a analyser alors on a atteind la fin de l analyse
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
	
	//on cherche les nouveaux etats terminaux qu on enregistre dans tab3 car on a plus besoin des informations fournies par tab3
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
	
	free(A1.transition);	//on libere tous les tableaux de l ancien automate
	free(A1.etat);
	free(A1.sortie);
	
	A1.Tetat=nbetat;		//nouvelles tailles
	A1.Tsortie=incsortie;
	
	A1.etat=malloc(A1.Tetat*sizeof(int));		//on definie les nouvelles tailles
	A1.sortie=malloc(A1.Tsortie*sizeof(int));
	A1.Ttransition=(A1.Tetat)*(A1.Talphabet);
	A1.transition=malloc(A1.Ttransition*sizeof(Transition));

	for(int i=0;i<A1.Tetat;i++)	// les etats sont notes par ordre croissant
	{
		A1.etat[i]=i+1;
	}
	
	//initialisé A1.sortie
	for(int i=0;i<A1.Tsortie;i++)		//les sorties sont enregistres dans tab3
	{
		A1.sortie[i]=tab3[i];
	}
	
	inc=0;		//compte le nombre de case lu dans tab
	test=0;		//compte le numero de l etat et la colonne de tab
	for(int i=0;i<nbetat;i++)
	{
		for(int j=0;j<A1.Talphabet;j++)
		{
			//toutes les informations sont enregistrees dans tab
			A1.transition[inc].src=A1.etat[test];
			A1.transition[inc].alpha=A1.alphabet[j];
			A1.transition[inc].dest=tab[i][j+1];
			inc++;
		}
		test++;
	}
	//nous avons maintenant une version deterministe de l automate de depart
	return A1;
}

Automate minimiser(struct Automate A1)
{
	//technique de minimisation fait en td en partant d un automate deterministe généré depuis la fonction determinisation
	int test,boucle=0,inc;
	int tab[A1.Talphabet+1][A1.Tetat];		//tab va contenir les valeurs de l analyse de la minimisation
	int tab2[A1.Tetat];		//tab2 va permettre de tester si l automate generer pas tab est minimiser entierement ou non
	
	printf("\nMinimisation de l automate\n");
	
	for(int a=0;a<A1.Tetat;a++)		//initialisation on regarde qui est un etat accepteur ou non =0 si non et =1 si oui
	{
		test=0;
		for(int b=0;b<A1.Tsortie;b++)
		{
			if(A1.etat[a]==A1.sortie[b])
			{
				test=1;
			}
		}
		if(test==1)
		{
			tab[0][a]=1;
		}
		else
		{
			tab[0][a]=0;
		}
	}
	
	while(boucle==0)		//on fais forcement au moins un tour de boucle
	{		// si les vecteurs tab[0] = tab2 alors on a la version la plus minimaliste
		inc=0;		//chaque etat a nb alphabet transition 
		for(int j=0;j<A1.Tetat;j++)
		{
			for(int i=1;i<A1.Talphabet+1;i++)
			{
			//depuis tab[0] et A1.transition on en deduit chaque case qui correspond a un etat et un caractere ou on arrive
				tab[i][j]=tab[0][A1.transition[inc].dest-1];
				inc++;
			}
		}
		
		tab2[0]=1;	//le premier etat de tab2 = 1
		inc=1;		// la valeur ascii de 0
		for(int j=1;j<A1.Tetat;j++)
		{
			for(int k=0;k<j;k++)	//on test chaque colonne de tab avec celle deja vu pour obtenir le nombre de colonnes differentes
			{
				test=-2;	//pas d erreur
				for(int i=0;i<A1.Talphabet+1;i++)
				{
					if((tab[i][j]!=tab[i][k])&&(test<0))
					{
						test=-1;	//si une difference alors nouvel etat
					}
				}
				if(test==-2)		// si a la fin d une comparaison pas de differences alors etat deja etudie
				{
					test=tab2[k];		// on enregistre le fait que cette colonne existe deja et inutile de comparer avec les suivantes
					break;
				}
			}
			if(test>0)		// a la fin si test>0 ( valeur ascii d un etat) alors etat deja existant
			{
				tab2[j]=test;
			}
			else 		// si non alors nouvel etat on prend la valeur max des etats existants +1
			{
				inc++;
				tab2[j]=inc;
			}
		}
		
		printf("\ntab\n  ");		//affichage de tab obtenu sur ce tour de boucle
		for(int k=0;k<A1.Tetat;k++)
		{
			printf("%d ",A1.etat[k]);
		}
		printf("\n");
		for(int i=0;i<A1.Talphabet+1;i++)
		{
			if(i==0)
			{
				printf("E ");
			}else
			{
				printf("%c ",A1.alphabet[i-1]);
			}
			for(int j=0;j<A1.Tetat;j++)
			{
				printf("%d ",tab[i][j]);
			}
			printf("\n");
		}
		
		printf("\ntab2\n  ");		//affichage de tab2 obtenu
		for(int i=0;i<A1.Tetat;i++)
		{
			printf("%d ",tab2[i]);
		}
		printf("\n");
	
		for(int j=0;j<A1.Tetat;j++)		//on test si tab[0] = tab2
		{
			if(tab[0][j]!=tab2[j])	//si non on refais un tour sur while car on peut encore minimiser
			{
				boucle=-1;
			}
		}
		
		if(boucle==-1)
		{
			for(int j=0;j<A1.Tetat;j++)		//on va encore minimiser donc on prend la valeur de tab2
			{
				tab[0][j]=tab2[j];
			}
			boucle=0;
		}
		else
		{
			boucle=1;		//on a obtenu la version la plus minimaliste on arrete
		}
	}
	
	printf("\ntab minimiser\n  ");		//on affiche le tab generant l automate le plus minimaliste
	for(int k=0;k<A1.Tetat;k++)
	{
		printf("%d ",A1.etat[k]);
	}
	printf("\n");
	for(int i=0;i<A1.Talphabet+1;i++)
	{
		if(i==0)
		{
			printf("E ");
		}else
		{
			printf("%c ",A1.alphabet[i-1]);
		}
		for(int j=0;j<A1.Tetat;j++)
		{
			printf("%d ",tab[i][j]);
		}
		printf("\n");
	}
	
	//on genere l automate depuis cette table
	A1.entre=tab[0][A1.entre-1];
	
	int incsortie=0;		//on calcul le nombre d etat de sortie différents et on note les etats de sorties differents
	if(A1.Tsortie!=0)
	{
		tab2[0]=tab[0][A1.sortie[0]-1];		// on re utilise tab2 car anciennes valeurs inutiles et on enregistre les nouvelles sorties
		test=0;
		incsortie++;
		for(int a=1;a<A1.Tsortie;a++)
		{
			for(int b=0;b<a;b++)
			{
				if(tab[0][A1.sortie[a]-1]==tab[0][A1.sortie[b]-1])
				{
					test=-1;
				}
			}
			if(test==0)
			{
				tab2[incsortie]=tab[0][A1.sortie[a]-1];
				incsortie++;
			}
		}
	}
	
	free(A1.transition);
	free(A1.etat);
	free(A1.sortie);
	
	A1.Tetat=inc;
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
		A1.sortie[i]=tab2[i];
	}
	
	inc=0;		//compte le nombre de case lu dans tab
	test=1;		//compte le numero de l etat et la colonne de tab
	for(int i=0;i<sizeof(tab2)/sizeof(tab2[0]);i++)
	{
		if(tab[0][i]==test)
		{
			for(int j=0;j<A1.Talphabet;j++)
			{
				A1.transition[inc].src=A1.etat[test-1];
				A1.transition[inc].alpha=A1.alphabet[j];
				A1.transition[inc].dest=tab[j+1][i];
				inc++;
			}
			test++;
		}
	}
	return A1;		//on return l automate minimiser
}
