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
    // Se non è stato inserito nessun intero non faccio nulla
    if (argc == 1) return 0;

    // Per ogni intero creo il corrispondente file
    for (int i = 1; i < argc; i++) {
        // Converto la stringa a int
        int n = atoi(argv[i]);

        // Creo il nome del file da generare
        char * fileName;
        int e = asprintf(&fileName, "%d.tab", n);
        if (e == -1) termina("Errore: Memoria Insufficiente.\n");

        // Creo il file
        FILE * file = fopen(fileName, "wt");
        if (file == NULL) termina("Errore durante la creazione del file.\n");

        // Popolo il file con la tabellina
        for (int i = 1; i <= 10; i++) {
            e = fprintf(file, "%d\n", i * n);
            if (e < 0) termina("Errore: Scrittura del file fallita.\n");
        }

        // Dealloco la memoria utilizzata e chiudo il file
        free(fileName);

        if (fclose(file) != 0) termina("Errore: Chiusura del file di input fallita.\n");
    }

    return 0;
}


// Functions' signatures
void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}