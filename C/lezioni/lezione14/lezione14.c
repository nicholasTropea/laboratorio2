#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


// Prototypes
void terminate(const char*);
bool primo(int);


// Main
int main(int argc, char ** argv) {
    // In formato binario per qualsiasi numero vengono usati 4 byte
    // In formato testo dipende dal numero di caratteri:
    // 1000 = '1' '0' '0' '0' '\n' (separatore) = 5 byte
    // 2 = '2' '\n' = 2 byte



    // od -Ad -td1 fileName ------ visualizza gli interi a 1 byte (quindi byte x byte) scritti in un file binario
    // od -Ad -td4 fileName ------ visualizza gli interi a 4 byte scritti in un file binario
    // od -Ad -td8 fileName ------ visualizza gli interi a 8 byte scritti in un file binario


    if(argc != 3) {
        printf("Uso: %s N nome_file\n", argv[0]);
        return 1;
    }

    // converte il primo parametro in un intero
    int n = atoi(argv[1]);
    if(n <= 0) terminate("Il parametro n deve essere positivo.\n");

    // copia il puntatore nella variabile nome_file
    char *nome_file = argv[2];

    // apro il file in scrittura
    FILE *f = fopen(nome_file, "wb");
    if(f == NULL) terminate("Apertura file fallita.\n");

    // cerca i primi da 2 a n e li scrive dentro il file
    for(int i=2;i<=n;i++) {
        if(primo(i)) {
            // scrittura dell'intero i in formato binario 
            int e = fwrite(&i, sizeof(i), 1, f);
            if(e != 1) terminate("Errore nella scrittura.\n");
        }
    }

    // chiudi il file e termina 
    if(fclose(f) == EOF) terminate("Errore chiusura file.\n");
    
    return 0;
}


// Functions' signatures
void terminate(const char * msg) {
    if (errno != 0) perror(msg);
    else fprintf(stderr, "%s\n", msg);
    
    exit(1);
}

bool primo(int k) {
    assert(k > 0);

    // se k è pari allora è primo se e solo se k==2
    if(k % 2 == 0) return k == 2;

    // mi occupo ora del caso k dispari
    assert(k % 2 != 0);

    for(int i = 3; i < k; i += 2) {
    // ho scoperto che il numero non è primo
    if(k % i == 0) return false;
    if(i * i > k) break;
    }
  
  return true;
}