#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

// Prototypes
void termina(const char*);


// Main
int main(int argc, char ** argv) {
    // Se il programma è chiamato in maniera invalida termino
    if (argc != 2) {
        fprintf(stderr, "Errore: L'utilizzo corretto è %s <fileName>\n", argv[0]);
        return 1;
    }

    // Apro il file di input
    FILE * file = fopen(argv[1], "rt");
    if (file == NULL) termina("Errore: Apertura del file fallita.\n");

    // Inizializzo le variabili per popolare l'array
    int size = 10;
    int counter = 0;

    // Creo l'array di stringhe
    char ** fileStrings = malloc(size * sizeof(char*));
    if (fileStrings == NULL) termina("Errore: Malloc fallita, memoria insufficiente.\n");

    // Creo la variabile che ottiene le stringhe
    char * current;
    int e = fscanf(file, "%ms", &current);

    // Popolo l'array con le stringhe nel file
    while (e != EOF) {
        // Aggiungo la stringa all'array
        fileStrings[counter] = strdup(current);
        if (fileStrings[counter] == NULL) termina("Errore: Memoria insufficiente.\n");
        counter++;

        // Rialloco memoria in caso l'array si sia riempito
        if (counter == size) {
            size *= 1.5;

            fileStrings = realloc(fileStrings, size * sizeof(char*));
            if (fileStrings == NULL) termina("Errore: Realloc fallita, memoria insufficiente.\n");
        }

        // Fetch della stringa successiva
        e = fscanf(file, "%s", current);
    }

    // Rialloco la memoria con quella necessaria
    if (size != counter) {
        size = counter;

        fileStrings = realloc(fileStrings, size * sizeof(char*));
        if (fileStrings == NULL) termina("Errore: Realloc fallita, memoria insufficiente.\n");
    }

    // Chiudo il file di input
    if (fclose(file) != 0) termina("Errore: Chiusura del file di input fallita.\n");
    
    // Creo il file di output con il nome giusto
    char * fileName;
    e = asprintf(&fileName, "%s.triangolo", argv[1]);
    if (e == -1) termina("Errore: Memoria Insufficiente.\n");

    file = fopen(fileName, "wt");
    if (file == NULL) termina("Errore: Creazione del file di output fallita.\n");

    // Dealloco la memoria usata per il nome del file
    free(fileName);

    // Popolo il file di output
    for (int i = 0; i < size; i++) {
        for (int j = i; j >= 0; j--) {
            e = fprintf(file, "%s ", fileStrings[j]);
            if (e < 0) termina("Errore: Scrittura del file fallita.\n");
        }

        e = fprintf(file, "\n");
        if (e < 0) termina("Errore: Scrittura del file fallita.\n");
    }

    // Chiudo il file di output
    if (fclose(file) != 0) termina("Errore: Chiusura del file di output fallita.\n");

    // Dealloco la memoria dell'array di stringhe
    for (int i = 0; i < size; i++) free(fileStrings[i]);

    free(fileStrings);

    return 0;
}


// Functions' signatures
void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}