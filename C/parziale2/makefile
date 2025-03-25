# Per maggiori informazioni sul comando make:
#     http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# Sulle variabili automatiche vedere:
#     https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
 
# definizione del compilatore e dei flag di compilazione
# che vengono usati dalle regole implicite
CC=gcc
CFLAGS=-std=c11 -Wall -O -g
LDLIBS=-lm

pitagora: pitagora.o terne.o
	$(CC) pitagora.o terne.o $(LDLIBS) -o pitagora
	
pitagora.o: pitagora.c terne.h
	$(CC) pitagora.c $(CFLAGS) -c -o pitagora.o
	
terne.o: terne.c terne.h
	$(CC) terne.c $(CFLAGS) -c -o terne.o

		
		