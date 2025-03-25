#include <stdio.h>
#include <stdlib.h>

int incrementa1(int z) {
    printf(
        "incrementa 1) La variabile z si trova nella posizione %ld e vale %ld\n",
        (long) &z, (long) z
    );

    z += 1;

    return 0;
}

int incrementa1p(int * p) {
    printf(
        "incrementa 1p) La variabile p si trova nella posizione %ld e vale %ld\n",
        (long) &p, (long) p
    );

    *p += 1;

    return 0;
}

int sommaprod(int n, int m, int * p) {
    *p = n * m;
    return n + m;
}

int main(int argc, char ** argv) {
    // Introduzione ai puntatori
    // Puntatori occupano 8 byte di solito (indirizzo di memoria 64bit)
    // a[3] = a[indMem + 3 * sizeof(tipo)]

    // int -> 32bit -> 4byte
    // long -> interi 64bit -> 8byte

    // Puntatori utili per restituire più valori da una funzione

    int n = 5;

    printf(
        "1) La variabile n si trova nella posizione %ld e vale %ld\n",
        (long) &n, (long) n
    );

    incrementa1(n);

    printf(
        "2) La variabile n si trova nella posizione %ld e vale %ld\n",
        (long) &n, (long) n
    );

    incrementa1p(&n);

    printf(
        "3) La variabile n si trova nella posizione %ld e vale %ld\n",
        (long) &n, (long) n
    );

    // Calcolo di somma e prodotto (funzione che "restituisce" più valori)
    int m = 11;
    int somma, prodotto;

    somma = sommaprod(n, m, &prodotto);

    printf(
        "La somma è %d\n"
        "Il prodotto è %d\n",
        somma, prodotto
    );

    return 0;
}