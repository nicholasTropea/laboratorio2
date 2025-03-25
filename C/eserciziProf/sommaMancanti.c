#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>



void termina(const char*);



int main(int argc, char ** argv) {
    if (argc != 2) {
        printf("Utilizzo: %s <fileName>\n", argv[0]);
        return 1;
    }

    FILE * f = fopen(argv[1], "r");
    if (f == NULL) termina("Errore durante l'apertura del file.");

    int * hashSet = calloc(1000, sizeof(int));
    if (hashSet == NULL) termina("Errore durante la Malloc.");

    int cur;
    int sum = 500500;

    while (1) {
        int e = fscanf(f, "%d", &cur);

        if (e == EOF) {
            if (ferror(f)) termina("Input Error occurred."); // Input Error
            break; // End of file
        }

        // Matching failure
        if (e != 1) {
            fscanf(f, "%*s"); // Consumes the invalid input
            continue;
        }

        if (cur < 1 || cur > 1000) continue; // Bound failure

        // Number not seen before
        if (hashSet[cur - 1] == 0) {
            hashSet[cur - 1]++;
            sum -= cur;
        }
    }

    printf("Sum: %d\n", sum);

    free(hashSet);

    if (fclose(f) != 0) termina("Chiusura del file fallita.");

    return 0;
}



void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}