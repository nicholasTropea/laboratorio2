#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isPrime(int);

int main(int argc, char ** argv) {
    // valgrind to test memory allocation
    // gcc -std=c11 -g -O -Wall -o ... ...

    int n;

    printf("Inserisci un numero intero: ");
    int e = scanf("%d", &n);

    printf("\n");

    if (e != 1) {
        puts("Errore: Inserimento non valido");
        exit(1);
    }

    int * a;
    int capacita = 10;
    int messi = 0;

    a = malloc(capacita * sizeof(int));

    if (a == NULL) {
        puts("Errore: Malloc fallita");
        exit(2);
    }

    for (int i = 2; i <= n; i++) {
        if (isPrime(i)) {
            if (messi == capacita) {
                capacita *= 2;
                a = realloc(a, capacita * sizeof(int));

                if (a == NULL) {
                    puts("Errore: Realloc fallita");
                    exit(3);
                }
            }

            a[messi] = i;
            messi += 1;
        }
    }    
    
    capacita = messi;
    a = realloc(a, capacita * sizeof(int));

    if (a == NULL) {
        puts("Errore: Realloc fallita");
        exit(3);
    }

    for (int i = 0; i < capacita; i++) {
        printf("%8d", a[i]); // %8d means use at least 8 spaces to print the number
    }

    printf("\n");

    free(a);

    return 0;
}


int isPrime(int N) {
    for (int i = 2; i < N; i++) {      
        if (N % i == 0) {
            return 0;
        }
    }

    return 1;
}