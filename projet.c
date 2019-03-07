#include <stdio.h>
#include <stdlib.h>

struct Automate
{
	char* expr;
	char* etat;
	char* alphabet;
	char entre;
	char* sortie;
	char** transition;
};

void affichage (struct Automate A1, int Tetat, int Talphabet, int Ttransition, int Tsortie)
{
	printf("\nA1.entre = %c \n",A1.entre);		// on affiche l automate deterministe minimise
	printf("\nLa liste des etats : \n");
	for(int i=0;i<Tetat;i++)
	{
		printf("A1.etat[%d] = %c \n",i,A1.etat[i]);
	}
	printf("\nLa liste des caracteres : \n");
	for(int i=0;i<Talphabet;i++)
	{
		printf("A1.alphabet[%d] = %c \n",i,A1.alphabet[i]);
	}
	printf("\nLa liste des sorties : \n");
	for(int i=0;i<Tsortie;i++)
	{
		printf("A1.sortie[%d] = %c \n",i,A1.sortie[i]);
	}
	printf("\nLa liste des transistions : \n");
	for(int i=0;i<Ttransition;i++)
	{
		printf("transition de %c avec %c vers %c \n",A1.transition[i][0],A1.transition[i][1],A1.transition[i][2]);
	}
	return;
}

struct Automate determiniser(struct Automate A1, int *Tetat, int *Talphabet, int *Ttransition, int *Tsortie)
{
	int Taille=*Tetat*5;		//la taille du tableau depend du nombre d etats de base
	//tab va contenir les transitions entre les nouveaux etats et -1 va marquer la fin d une colonne ou d une ligne
	char tab[Taille][*Talphabet+1];		//1 ere colonne: le nom des nouveaux etats, les colonnes suivantes sont les transitions depuis ce nouvel etat en utilisant 1 caractere de l'alphabet dans l'ordre
	char tab2[Taille][*Tetat+2];	//tab2 va indiquer de quoi sont constituer les nouveaux etats, par rapports aux anciens
	int inc=1;
	int test=-2;
	char car;
	char max=66;
	int nbetat=1;
	char tab3[*Tetat+2];		//tab3 contient les etats atteints par la transition etudiee
	char Ecloture[*Tetat][*Tetat+1];	//contient les Ecloture des transitions E
	
	printf("\nDeterminisation de l automate\n");
	
	for(int a=0;a<Taille;a++)		//on mets tous les tableaux a 0 ( pas de valeurs indeterminees )
	{
		for(int b=0;b<*Talphabet+1;b++)
		{
			tab[a][b]='0';
		}
		for(int b=0;b<*Tetat+2;b++)
		{
			tab2[a][b]='0';
		}
	}
	for(int b=0;b<*Tetat+2;b++)
	{
		tab3[b]='0';
	}
	for(int a=0;a<*Tetat;a++)
	{
		Ecloture[a][0]=A1.etat[a];
		Ecloture[a][1]='x';
	}
	for(int a=0;a<*Tetat;a++)
	{
		for(int b=2;b<*Tetat+1;b++)
		{
			Ecloture[a][b]='0';
		}
	}
	
	for(int a=0;a<*Tetat;a++)		//on cherche tous les Eclotures, l existance de transitions E pour tous les anciens etats
	{
		inc=1;
		for(int b=0;b<*Ttransition;b++)
		{
			if((A1.transition[b][1]=='E')&&(A1.transition[b][0]==Ecloture[a][0]))	//on regarde si l etat en question a une transition E
			{
				Ecloture[a][inc]=A1.transition[b][2];
				inc++;
				Ecloture[a][inc]='x';
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

struct Automate minimiser(struct Automate A1, int *Tetat, int *Talphabet, int *Ttransition, int *Tsortie)
{
	//technique de minimisation fait en td en partant d un automate deterministe
	int test,boucle=0,inc;
	char tab[*Talphabet+1][*Tetat];		//tab va contenir les valeurs de l analyse de la minimisation
	char tab2[*Tetat];		//tab2 va permettre de tester si l automate generer pas tab est minimiser entierement ou non
	
	printf("\nMinimisation de l automate\n");
	
	for(int a=0;a<*Tetat;a++)		//initialisation on regarde qui est un etat acceptant ou non =0 si non et =1 si oui
	{
		test=0;
		for(int b=0;b<*Tsortie;b++)
		{
			if(A1.etat[a]==A1.sortie[b])
			{
				test=1;
			}
		}
		if(test==1)
		{
			tab[0][a]='1';
		}
		else
		{
			tab[0][a]='0';
		}
	}
	
	while(boucle==0)		//on fais forcement au moins un tour de boucle
	{		// si tab[0] = tab2 alors on a la version la plus minimaliste
		inc=0;		//chaque etat a nb alphabet transition 
		for(int j=0;j<*Tetat;j++)
		{
			for(int i=1;i<*Talphabet+1;i++)
			{
			//depuis tab[0] et A1.transition on en deduit depuis chaque case qui correspond a un etat et un caractere ou on arrive
				tab[i][j]=tab[0][A1.transition[inc][2]-65];		
				inc++;
			}
		}
		
		tab2[0]='0';	//le premier etat de tab2 = 0
		inc=48;		// la valeur ascii de 0
		for(int j=1;j<*Tetat;j++)
		{
			for(int k=0;k<j;k++)	//on test chaque colonne de tab avec celle deja vu pour obtenir le nombre de colonnes differentes
			{
				test=-2;	//pas d erreur
				for(int i=0;i<*Talphabet+1;i++)
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
		
		printf("\ntab\n  ");		//affichage de tab obtenu
		for(int k=0;k<*Tetat;k++)
		{
			printf("%c ",A1.etat[k]);
		}
		printf("\n");
		for(int i=0;i<*Talphabet+1;i++)
		{
			if(i==0)
			{
				printf("E ");
			}else
			{
				printf("%c ",A1.alphabet[i-1]);
			}
			for(int j=0;j<*Tetat;j++)
			{
				printf("%c ",tab[i][j]);
			}
			printf("\n");
		}
		
		printf("\ntab2\n  ");		//affichage de tab2 obtenu
		for(int i=0;i<*Tetat;i++)
		{
			printf("%c ",tab2[i]);
		}
		printf("\n");
	
		for(int j=0;j<*Tetat;j++)		//on test si tab[0] = tab2
		{
			if(tab[0][j]!=tab2[j])	//si non on refais un tour sur while car on peut encore minimiser
			{
				boucle=-1;
			}
		}
		
		if(boucle==-1)
		{
			for(int j=0;j<*Tetat;j++)		//on va encore minimiser donc on prend la valeur de tab2
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
	
	printf("\ntab minimiser\n  ");		//on affiche le tab le plus minimaliste
	for(int k=0;k<*Tetat;k++)
	{
		printf("%c ",A1.etat[k]);
	}
	printf("\n");
	for(int i=0;i<*Talphabet+1;i++)
	{
		if(i==0)
		{
			printf("E ");
		}else
		{
			printf("%c ",A1.alphabet[i-1]);
		}
		for(int j=0;j<*Tetat;j++)
		{
			printf("%c ",tab[i][j]);
		}
		printf("\n");
	}
	
	//on genere l automate depuis cette table
	A1.entre=tab[0][A1.entre-65];
	
	tab2[0]=tab[0][A1.sortie[0]-65];		// on re utilise tab2 car anciennes valeurs inutiles et on enregistre les nouvelles sorties
	int incsortie=1;		//on calcul le nombre d etat de sortie différents et on note les etats de sorties differents
	test=0;
	for(int a=1;a<*Tsortie;a++)
	{
		for(int b=0;b<a;b++)
		{
			if(tab[0][A1.sortie[a]-65]==tab[0][A1.sortie[b]-65])
			{
				test=-1;
			}
		}
		if(test==0)
		{
			tab2[incsortie]=tab[0][A1.sortie[a]-65];
			incsortie++;
		}
	}
	
	for(int i=0;i<*Ttransition;i++)		// la taille des etats, des sorties et des transitions a changer 
	{
		free(A1.transition[i]);
	}
	free(A1.transition);
	free(A1.etat);
	free(A1.sortie);
	
	*Tetat=inc+1-48;
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
		A1.etat[i]=48+i;
	}
	
	//initialisé A1.sortie
	for(int i=0;i<*Tsortie;i++)		//les sorties sont enregistres dans tab2
	{
		A1.sortie[i]=tab2[i];
	}
	
	inc=0;		//compte le nombre de case lu dans tab
	test=0;		//compte le numero de l etat et la colonne de tab
	for(int i=0;i<sizeof(tab2);i++)
	{
		if(tab[0][i]==test+48)
		{
			for(int j=0;j<*Talphabet;j++)
			{
				A1.transition[inc][0]=A1.etat[test];	//0 correspond a l etat de depart
				A1.transition[inc][1]=A1.alphabet[j];	//1 correspond au caractere consomme
				A1.transition[inc][2]=tab[j+1][i];		//2 correspond a l arrive de l automate depuis 0 en prenant 1
				inc++;
			}
			test++;
		}
	}
	return A1;		//on return l automate minimiser
}

int main(int argc, char *argv[])
{
	struct Automate A1;
	
	/*
	//les valeurs pour tester la minimisation
	char etat[8]={'A','B','C','D','E','F','G','H'};
	char alphabet[2]={'a','b'};
	char sortie[4]={'E','F','G','H'};
	char entre='A';
	char transition[sizeof(etat)*sizeof(alphabet)][3]={{'A','a','B'},{'A','b','A'},{'B','a','C'},{'B','b','D'},{'C','a','E'},{'C','b','D'},{'D','a','F'},{'D','b','A'},{'E','a','E'},{'E','b','G'},{'F','a','E'},{'F','b','G'},{'G','a','E'},{'G','b','H'},{'H','a','F'},{'H','b','H'}};
	*/
	/*
	//les valeurs pour tester la determinisation qui donneront les valeurs de la minimisation
	char etat[4]={'0','1','2','3'};
	char alphabet[2]={'a','b'};
	char sortie[1]={'3'};
	char entre='0';
	char transition[8][3]={{'0','a','0'},{'0','b','0'},{'0','a','1'},{'1','a','2'},{'1','b','2'},{'2','a','3'},{'3','a','3'},{'3','b','3'}};
	*/
	/*
	//nouvel exemple plus complexe avec un etat mort
	char etat[10]={'0','1','2','3','4','5','6','7','8','9'};
	char alphabet[6]={'p','i','m','a','o','t',};
	char sortie[3]={'3','6','9'};
	char entre='0';
	char transition[9][3]={{'0','p','1'},{'0','p','4'},{'0','t','7'},{'1','i','2'},{'2','m','3'},{'4','a','5'},{'5','m','6'},{'7','o','8'},{'8','m','9'}};
	*/
	
	char etat[]={'1','2','3','4'};
	char alphabet[]={'a','b'};
	char sortie[]={'3','4'};
	char entre='1';
	char transition[][3]={{'1','E','2'},{'1','b','4'},{'2','a','2'},{'2','b','3'},{'4','a','4'}};
	
	int Tetat=sizeof(etat);		// les tailles qui caracterisent l automate
	int Talphabet=sizeof(alphabet);
	int Tsortie=sizeof(sortie);
	int Ttransition=sizeof(transition)/3;
	
	A1.etat=malloc(Tetat*sizeof(char));
	A1.alphabet=malloc(Talphabet*sizeof(char));
	A1.sortie=malloc(Tsortie*sizeof(char));
	A1.transition=malloc(Ttransition*sizeof(char*));
	for(int i=0;i<Ttransition;i++)
	{
		A1.transition[i]=malloc(3*sizeof(char));
	}
	
	A1.entre=entre;
	for(int i=0;i<Tetat;i++)
	{
		A1.etat[i]=etat[i];
	}
	for(int i=0;i<Talphabet;i++)
	{
		A1.alphabet[i]=alphabet[i];
	}
	for(int i=0;i<Tsortie;i++)
	{
		A1.sortie[i]=sortie[i];
	}
	for(int i=0;i<Ttransition;i++)
	{
		for(int j=0;j<3;j++)
		{
			A1.transition[i][j]=transition[i][j];
		}
	}
	
	printf("\nAutomate de depart :\n");
	affichage(A1, Tetat, Talphabet, Ttransition, Tsortie);
	
	A1=determiniser(A1,&Tetat,&Talphabet,&Ttransition,&Tsortie);	//on demande une determinisation de l automate
	
	printf("\nAutomate deterministe :\n");
	affichage(A1, Tetat, Talphabet, Ttransition, Tsortie);
	
	A1=minimiser(A1,&Tetat,&Talphabet,&Ttransition,&Tsortie);	// on demande une minimisation de cette automate deterministe
	
	printf("\nAutomate minimise :\n");
	affichage(A1, Tetat, Talphabet, Ttransition, Tsortie);
	
	for(int i=0;i<Ttransition;i++)
	{
		free(A1.transition[i]);
	}
	free(A1.transition);
	free(A1.etat);
	free(A1.sortie);
	free(A1.alphabet);
	
	return 0;
}
