#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main(int argc, char * argv[]) {
    int n;

    printf("Inserisci la grandezza: ");
    int e = scanf("%d", &n); // scanf restituisce quanti oggetti ha letto in maniera corretta

    // Controlli sulla lettura (necessari)
    if (e != 1) {
        puts("Errore: Valore non trovato");
        exit(1);
    }

    if (n <= 0) {
        puts("Errore: La grandezza deve essere positiva"); // Analogo a printf
        exit(2);
    }

    printf("\n");

    int * myArray = malloc(n * sizeof(int));

    if (myArray == NULL) {
        puts("Errore: Malloc Fallita");
        exit(3);
    }

    int somma = 0;

    for (int i = 0; i < n; i++) {
        printf("Inserisci il %d numero: ", i + 1); // puts non può contenere variabili
        e = scanf("%d", &myArray[i]);

        clear_input_buffer();

        if (e != 1) {
            puts("Errore: Valore non trovato");
            exit(1);
        }

        somma += myArray[i];
    }

    printf("\nLa somma dei numeri vale %d", somma);

    free(myArray);
    myArray = NULL;
    
    return 0;
}