#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


// ##################################################################################################
// scanf("%9s", s);      PRENDE AL MASSIMO 9 CARATTERI ##############################################
// ##################################################################################################


void termina(char*);
char * copiaRibalta(const char *);

int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("Utilizzo: %s <stringa1> <stringa2> ... <stringaN>\n", argv[0]);
        return 1;
    }

    // Creo array di stringhe ribaltate
    char ** arr;
    int n = argc - 1;

    arr = malloc(n * sizeof(char*));
    if (arr == NULL) termina("Errore: Memoria Esaurita.");

    for (int i = 0; i < n; i++) {
        arr[i] = copiaRibalta(argv[i + 1]);
        puts(arr[i]);
        free(arr[i]);
    }

    free(arr);

    return 0;
}


void termina(char * messaggio) {
    perror(messaggio);
    exit(1);
}


char * copiaRibalta(const char * s) {
    char * t = strdup(s);

    for (int i = 0; i < strlen(t); i++) t[i] = s[strlen(s) - i - 1];

    return t;
}