all: projet	


projet:	AFN.o AFD.o exemple.o 
	gcc exemple.o AFN.o AFD.o -o exemple

exemple.o:	exemple.c	
		gcc -c exemple.c 

AFD.o:	AFD.c
	gcc -c AFD.c

AFN.o:	AFN.c
	gcc -c AFN.C
