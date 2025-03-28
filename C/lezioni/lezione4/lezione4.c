#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {
    // char è una variabile intera rappresentata da un singolo byte (-128 , 127)
    // se il valore è intero, rappresenta il carattere ASCII in quella posizione
    // se formattato con %c restituisce il carattere, se si usa %d restituisce il codice ASCII
    // NOTA BENE, char contiene un INTERO

    char a, b;

    a = 70;
    b = 'x';

    printf("a = %c %d, b = %c % d\n", a, a, b, b);
    
    // Una stringa è un array di caratteri
    // Le stringhe terminano quando si incontra il byte 0 (\0)
    // char * s = "ciao" in memoria è c, i, a, o, \0 (in ASCII ovviamente)

    char * z = strdup("the answer is 42"); // Stringa non modificabile senza strdup, allocata staticamente
    // strdup crea una seconda stringa in un'altra zona di memoria dove si può modificare
    // NECESSARIO #define _GNU_SOURCE
    printf("Originale: %s\n", z);

    z[1] = 70;
    printf("Cambiato z[1]: %s\n", z);

    z[5] = 0;
    printf("Cambiato anche z[5]: %s\n", z);

    z[16] = 70; // Sovrascrivo lo \0 finale
    printf("Cambiato anche z[5]: %s\n", z);

    // strlen() prende un puntatore, guarda quanti byte sono nella zona di memoria e si ferma quando trova \0 [O(n)!!!!]

    // size_t = long unsigned integer (intero positivo a 64bit %zd è il formato di size_t

    return 0;
}