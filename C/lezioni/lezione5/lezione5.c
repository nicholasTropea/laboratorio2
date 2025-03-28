#include <stdio.h>
#include <stdlib.h>

// Uso di file (di testo)
// Sequenze di byte interpretati come caratteri (non come numeri binari, convenzione)
// L'estenzione di un file non ha nessuna importanza, è un suggerimento su cosa c'è dentro

// Compilazione con make (necessario il makefile)

void termina(char*);

int isPrime(int);

int main(int argc, char ** argv) {
    if (argc != 3) {
        printf("Uso: %s intero_positivo nome_file\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]); // Converte argv[i] in intero (ascii to int) restituisce 0 se errore

    if (n < 0) termina("E' richiesto un intero positivo.\n");

    int * a;
    int capacita = 10;
    int messi = 0;

    a = malloc(capacita * sizeof(int));

    if (a == NULL) termina("Malloc fallita.");

    for (int i = 2; i <= n; i++) {
        if (isPrime(i)) {
            if (messi == capacita) {
                capacita *= 2;
                a = realloc(a, capacita * sizeof(int));

                if (a == NULL) termina("Realloc fallita.");
            }

            a[messi] = i;
            messi += 1;
        }
    }    
    
    capacita = messi;
    a = realloc(a, capacita * sizeof(int));

    if (a == NULL) termina("Realloc fallita.");

    // Scrivere i numeri in un file
    FILE * f = fopen(argv[2], "wt"); // fopen(filename, modalità) (wt, scrivere in un (t) file di testo, t non necessaria in linux)
    // Restituisce un FILE * (puntatore a file), NULL in caso di errore
    // Aprendo in scrittura così un file esistente, il contenuto viene sovrascritto

    if (f == NULL) termina("Apertura del file fallita.");

    for (int i = 0; i < messi; i++) {
        int e = fprintf(f, "%d\n", a[i]); // Scrive nel file il contenuto dell'array

        if (e < 0) termina("Scrittura del file fallita.");
    }

    if (fclose(f) != 0) termina("Chiusura del file fallita.");

    free(a);

    return 0;
}


void termina(char * message) {
    printf("Errore: %s\n", message);
    perror("");
    exit(1);
}


int isPrime(int N) {
    for (int i = 2; i < N; i++) {      
        if (N % i == 0) {
            return 0;
        }
    }

    return 1;
}