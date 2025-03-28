#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    PROTOTIPO DEL MAIN (int main(int argc, char ** argv))
    argc quante stringhe sono, argv che stringhe sono
    (ciò che scriviamo sulla riga di comando quando eseguiamo il programma)

    se non specifico -o nome a gcc, l'eseguibile avrà nome a.out
    se chiamo a.out in questo modo: a.out ciao 52, argc sarà 3, argv sarà ["a.out", "ciao", "52"]
*/


int main(int argc, char ** argv) {
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    printf("\n");

    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %ld  %s \n", i, (long) argv[i], argv[i]);
    }

    // Notando gli indirizzi delle varie stringhe, si nota che sono blocchi uno dopo l'altro

    printf("\n");

    // Copiare le stringhe
    char * copia = argv[0];

    printf(
        "argv[0]: %s\n"
        "copia: %s\n\n"
        , argv[0], copia
    );

    copia[0] = 'B';

    printf(
        "argv[0]: %s\n"
        "copia: %s\n\n"
        , argv[0], copia
    );
    
    /*
        Per effettivamente copiare una stringa (array in generale), devo copiare carattere per
        carattere gli elementi in un nuovo blocco di memoria.

        Per le stringhe si può utilizzare strdup(). (#define _GNU_SOURCE necessario!)

        Ricevuta una stringa, conta quanti caratteri ha (\0) incluso,
        chiama malloc per ricevere un blocco libero dell'heap e restituisce il puntatore
        all'indirizzo del primo elemento del nuovo blocco.

        NOTA BENE !!!!!!!!!!!!!!!!!!
        Ricordarsi di liberare la memoria quando non serve più la copia della stringa.
    */

    char * effettiva = strdup(argv[1]);

    effettiva[0] = 'B';

    printf(
        "argv[1]: %s\n"
        "effettiva: %s\n\n"
        , argv[1], effettiva
    );

    free(effettiva);

    return 0;
}