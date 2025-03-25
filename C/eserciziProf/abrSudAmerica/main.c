#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "abr.h" // Capitale abr library


// Prototypes
void terminate(const char*);


// Main
int main(int argc, char ** argv) {
    FILE * f = fopen("sudAmerica", "rt");
    if (f == NULL) terminate("Error: fopen() failed.\n");

    capitale * tree = crea_abr(f);

    FILE * output = fopen("output", "w");

    if (tree != NULL) abr_capitale_stampa(tree, output);

    abr_capitale_distruggi(tree);
    fclose(f);
    fclose(output);
    
    return 0;
}


// Functions' signatures
void terminate(const char * msg) {
    if (errno != 0) perror(msg);
    else fprintf(stderr, "%s\n", msg);
    
    exit(1);
}