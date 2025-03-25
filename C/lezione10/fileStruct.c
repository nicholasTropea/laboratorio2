#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


typedef struct {
    char * nome;
    long lunghezza;
} file;


void termina(const char*);


int main(int argc, char ** argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <fileName>\n", argv[0]);
        return 1;
    }


    return 0;
}


file * fileCrea(char * s) {
    assert(s != NULL);

    // Calcolare la grandezza del file
    // Sposto il puntatore del file alla fine e gli chiedo a quale byte è
    FILE * f = fopen(s, "r");

    if (f == NULL) {
        fprintf(stderr, "Non è stato possibile aprire il file %s\n.", s);
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) { // Setta il puntatore alla fine del file
        fprintf(stderr, "Non è stato possibile eseguire fseek sul file %s\n.", s);
        fclose(f);
        return NULL;
    } 

    long pos = ftell(f); // Restituisce il byte corrente del file

    if (pos < 0) {
        fprintf(stderr, "Non è stato possibile eseguire ftell sul file %s\n", s);
        fclose(f);
        return NULL;
    }

    fclose(f);

    file * a = malloc(sizeof(*a));
    if (a == NULL) termina("Errore: Memoria Insufficiente.");

    a -> nome = s;
    a -> lunghezza = pos;

    return a;
}


void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}