#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

void termina(const char*);
int isPrime(int);


int main(int argc, char ** argv) {
    if (argc != 3) {
        printf("Utilizzo: %s <stringa> <interoPositivo>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[2]);

    if (n < 3) {
        printf("Non esistono numeri primi minori di %d.\n", n);
        return 0;
    }

    // Creo array di file
    FILE ** files = malloc(10 * sizeof(FILE*));
    if (files == NULL) termina("Errore: Memoria Insufficiente.");

    // Inizializzo i file come NULL
    for (int i = 0; i < 10; i++) files[i] = NULL;

    // Itero per ogni numero < n
    for (int i = 2; i < n; i++) {
        // Se il numero è primo, creo e\o scrivo sul file
        if (isPrime(i)) {
            int indice = i % 10;

            // File già creato
            if (files[indice] != NULL) {
                int e = fprintf(files[indice], "%d\n", i);
                if (e < 0) termina("Errore durante la scrittura del file.");
            }
            // File da creare
            else {
                char * fileName;
                int e = asprintf(&fileName, "%s.%d.primi", argv[1], indice);

                if (e == -1) termina("Errore: Memoria Insufficiente.");

                files[indice] = fopen(fileName, "wt");
                if (files[indice] == NULL) termina("Errore durante la creazione del file.");

                e = fprintf(files[indice], "%d\n", i);
                if (e < 0) termina("Errore durante la scrittura del file.");

                free(fileName);
            } 
        }
    }

    for (int i = 0; i < 10; i++) {
        if (files[i] != NULL) {
            if (fclose(files[i]) != 0) termina("Chiusura del file di input fallita.");
        }
    }

    free(files);
    
    return 0;
}



int isPrime(int n) {
    if (n <= 1) return 0; // 0 and 1 are not prime numbers
    if (n <= 3) return 1; // 2 and 3 are prime numbers

    // Eliminate multiples of 2 and 3
    if (n % 2 == 0 || n % 3 == 0) return 0;

    // Check divisibility using 6k ± 1 rule
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }

    return 1;
}



void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}