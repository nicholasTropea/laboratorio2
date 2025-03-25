#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


void termina(const char*);



int main(int argc, char ** argv) {
    if (argc != 3) {
        printf("Utilizzo: %s <inputFile> <outputFile>\n", argv[0]);
        return 1;
    }

    FILE * file = fopen(argv[1], "r");
    if (file == NULL) termina("Errore durante l'apertura del file di input.");
    

    int size = 10;
    int messi = 0;
    char ** nameArr = malloc(size * sizeof(char*));
    if (nameArr == NULL) termina("Errore: Memoria Insufficiente.");


    while (1) {
        char * name;
        int e = fscanf(file, "%ms", &name);

        if (e == EOF) {
            if (ferror(file)) termina("Input Error occurred."); // Input Error
            break; // End of file
        }

        if (e != 1) continue; // Matching failure

        if (name == NULL) termina("Errore: Memoria Insufficiente.");

        nameArr[messi++] = name;

        if (messi == size) {
            size *= 1.5;
            nameArr = realloc(nameArr, size * sizeof(char*));

            if (nameArr == NULL) termina("Errore: Memoria Insufficiente.");
        }
    }

    size = messi;
    nameArr = realloc(nameArr, size * sizeof(char*));

    if (fclose(file) != 0) termina("Chiusura del file di input fallita.");

    file = fopen(argv[2], "wt");
    if (file == NULL) termina("Errore durante l'apertura del file di output.");

    for (int i = size - 1; i >= 0; i--) {
        int e = fprintf(file, "%s\n", nameArr[i]);

        free(nameArr[i]);
        if (e < 0) termina("Scrittura del file fallita.");
    }

    if (fclose(file) != 0) termina("Chiusura del file di output fallita.");

    free(nameArr);

    return 0;
}



void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}