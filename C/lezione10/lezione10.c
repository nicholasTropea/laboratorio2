#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


void termina(const char*);
int somma(int, int);
int prodotto(int, int);
int applica(int, int, int (*)(int, int));


int main(int argc, char ** argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <intero1> <intero2>", argv[0]);
        return 1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    // Quando si passa una funzione come parametro, è obbligatorio passare il puntatore alla funzione
    // In caso di omissione dell'&, il compilatore lo fa in automatico (come per prodotto nella printf)
    printf("Somma: %d, Prodotto: %d\n", applica(a, b, &somma), applica(a, b, prodotto));
    
    return 0;
}


int somma(int a, int b) {
    return a + b;
}


int prodotto(int a, int b) {
    return a * b;
}


// Il tipo della funzione è: returnType (*) (par1Type, par2Type, ..., parNType)
int applica(int x, int y, int (*f)(int, int)) { 
    int z = f(x, y);
    return z;
}


void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}