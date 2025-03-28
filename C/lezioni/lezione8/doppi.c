#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

// Definisce il "sinonimo" di un tipo
typedef int intero; // int -> intero
typedef int * puntatore_a_int; // int * -> puntatore_a_int



// Object oriented
typedef struct duetto { // "Tipo" composto da più campi
    intero primo;
    intero secondo;
} coppia; // struct duetto myCoppia -> coppia myCoppia



// Prototipi
coppia scambia(coppia);
void printCoppia(const coppia*);



int main(int argc, char ** argv) {
    coppia coppia1;
    coppia1.primo = 10;
    coppia1.secondo = 20;

    coppia coppia2 = scambia(coppia1);

    printCoppia(&coppia1);
    printCoppia(&coppia2);

    return 0;
}

// Passando come parametro una struct, ne viene creata una nuova all'interno della funzione
// con gli stessi campi-valori di quella passata (pass-by-value)
coppia scambia(coppia d) {
    intero tmp = d.primo;
    d.primo = d.secondo;
    d.secondo = tmp;
    return d;
}


// Passo la struct per riferimento attraverso un puntatore
void printCoppia(const coppia * d) {
    printf("Valore: (%d, %d)\n", (*d).primo, d -> secondo);
}

// NOTA: (*a).primo = a -> primo (solamente se a è un puntatore a struct)

// NOTA: Passare una struct per riferimento ha un costo decisamente inferiore che farne creare una copia
// e usare quella per poi eliminarla, passare per valore solamente se necessario