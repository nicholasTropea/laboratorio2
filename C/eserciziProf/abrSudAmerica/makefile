# il funzionamento del makefile lo vedremo prossimamente

# definizione del compilatore e dei flag di compilazione
# che vengono usati dalle regole implicite
CC=gcc
CFLAGS=-std=c11 -Wall -O -g
LDLIBS=-lm

EXECS=result

all: $(EXECS)

result: main.o abr.o
	$(CC) main.o abr.o $(LDLIBS) -o result
	
abr.o: abr.c abr.h
	$(CC) abr.c $(CFLAGS) -c -o abr.o
	
clean:
		rm -f $(EXECS) *.o
		
		