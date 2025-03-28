#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>


// %-16s stampa usando 16 caratteri e i non usati li mette a destra


// Prototypes and structs
void termina(const char*);

// Capitale struct
typedef struct capit {
  char * nome;
  double lat;
  double lon;
  struct capit * next;
} capitale;


// Capitale prototypes
void capitale_stampa(const capitale*, FILE*);
capitale* capitale_crea(char*, double, double);
void capitale_distruggi(capitale*);
void lista_capitale_stampa(const capitale*, FILE*);
void lista_capitale_distruggi(capitale*);
capitale* capitale_leggi(FILE*);


// Binary tree prototypes
capitale* crea_abr(FILE*);



// Main
int main(int argc, char ** argv) {
    if(argc != 2) {
        printf("Uso: %s nomefile\n", argv[0]);
        exit(1);
    }

    FILE * f = fopen(argv[1], "r");
    if(f == NULL) termina("Errore apertura file");

    // costruzione lista leggendo capitali dal file
    capitale * testa = crea_lista_testa(f);
    puts("--- inizio lista ---");

    // stampa lista capitali appena creata
    lista_capitale_stampa(testa, stdout);  
    puts("--- fine lista ---");
    lista_capitale_distruggi(testa);

    // costruzione lista inserendo in coda
    rewind(f); // riavvolge il file
    testa = crea_lista_coda(f);
    puts("--- inizio lista ---");

    // stampa lista capitali appena creata
    lista_capitale_stampa(testa, stdout);  
    puts("--- fine lista ---");


    if(fclose(f) == EOF) termina("Errore chiusura");

    // dealloca la memoria usata dalla lista 
    lista_capitale_distruggi(testa);

    return 0;
}


// Functions' signatures
void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}


void capitale_stampa(const capitale * a, FILE * f) {
    fprintf(f, "%20s (%f, %f)\n", a -> nome, a -> lat, a -> lon);
}


capitale* capitale_crea(char * s, double lat, double lon) {
    capitale * a  = malloc(sizeof(*a));

    a -> lat = lat;
    a -> lon = lon;
    a -> nome = strdup(s);
    a -> next = NULL;

    return a;
}


void capitale_distruggi(capitale * a) {
    free(a -> nome);
    free(a);
}


// stampa tutti gli elementi della lista lis
void lista_capitale_stampa(const capitale * lis, FILE * f) {
    while(lis != NULL) {
        capitale_stampa(lis, f);
        lis = lis -> next;
    }
}


// distrugge tutti gli elementi della lista lis
void lista_capitale_distruggi(capitale * lis) {
    while(lis != NULL) {
        capitale * tmp = lis -> next;
        capitale_distruggi(lis);
        lis = tmp;
    }
}


// legge un oggetto capitale dal file f
// restituisce il puntatore all'oggetto letto
// oppure NULL se non riesce a leggere dal file
capitale* capitale_leggi(FILE * f) {
    assert(f != NULL);

    char * s;
    double lat, lon;

    int e = fscanf(f, "%ms %lf %lf", &s, &lat, &lon);
    if(e != 3) {
        if(e == EOF) return NULL;
        else termina("Formato input file non valido");
    }
    
    capitale * c = capitale_crea(s, lat, lon);

    free(s);
    return c;
}


capitale* crea_abr(FILE * f) {
    capitale * radice = NULL;

    while(true) {
        capitale * b = capitale_leggi(f);
        if(b == NULL) break;

    
        b -> next = testa;
        testa = b;
    }

    return radice;
}



/// @brief //////////////////////////////////////////////////////////////
/// @param testa 
/// @param s 
/// @return /
capitale* cancella_nome(capitale * testa, char * s) {
    assert(s != NULL);

    if (testa == NULL) return NULL;

    if (strcmp(testa -> nome, s) == 0) {
        capitale * tmp = testa -> next;
        capitale_distruggi(testa);
        return tmp;
    }

    testa -> next = cancella_nome(testa -> next, s);
    return testa;
}




capitale* inserisci_lat_ric(capitale * testa, capitale * c) {
    if (testa == NULL || c -> lat < testa -> lat) {
        c -> next = testa;
        return c;
    }

    testa -> next = inserisci_lat_ric(testa -> next, c);
    return testa;
}