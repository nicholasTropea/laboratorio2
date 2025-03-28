// Algoritmo Mergesort che sfrutta la notazione &a[n1] per ottenere 
// il puntatore all'inizio dell'array contenente al seconda metà
// dell'input.

#define _GNU_SOURCE   // Avverte che usiamo le estensioni GNU 
#include <stdio.h>    // Permette di usare scanf printf etc ...
#include <stdlib.h>   // Conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // Gestisce tipo bool
#include <assert.h>   // Permette di usare la funzione assert
#include <string.h>   // Funzioni per stringhe
#include "xerrori.h"


// Dati dei thread
typedef struct {
  int *base;
  int size;

} dati;



// Prototipi delle funzioni che appaiono dopo il main()
void merge(int a[], int na, int c[], int nc, int b[]);
int *random_array(int n, int seed);
int intcmp(const void *a, const void *b);


// Ordina gli interi passati sulla linea di comando
int main(int argc, char *argv[]) {
  if(argc != 3) {
    fprintf(stderr, "Uso:\n\t   %s num_interi seed\n", argv[0]);
    exit(1);
  }

  int n = atoi(argv[1]);

  // Genera array random di n elementi
  int *a = random_array(n, atoi(argv[2]));

  // Calcola somma prima dell'ordinamento
  long somma = 0;
  for(int i = 0; i < n; i++) somma += a[i];
  











  int half = n / 2;

  // Creo array prima metà
  int *firstHalf = malloc(half * sizeof(int))

  for (int i = 0; i < half; i++) {
    firstHalf[i] = a[i];
  }

  // Creo array seconda metà
  int size;

  if (n % 2 == 0) size = half;

  // Esegue l'ordinamento
  qsort(a, n, sizeof(int), &intcmp);
  


























  // Controlla ordinamento
  long somma2 = a[0];

  for(int i = 1; i < n; i++) {
    somma2 += a[i];

    if(a[i - 1] > a[i]) {
      printf("Array non ordinato alla posizione %d (%d>%d)\n", i, a[i - 1], a[i]);
      exit(1);
    }
  }

  if(somma != somma2) {
    printf("Somma degli elementi non corrisponde: %ld vs %ld\n", somma, somma2);
    exit(1);
  }
  else 
    puts("Array ordinato e somma mantenuta, sorting probabilmente OK");
    
  // Dealloco l'array e termino
  free(a);   

  return 0;
}




// Funzione per il merge di due array in un terzo array già allocato
// merge di a[0...n1-1] e c[0... n2-1] dentro b[]
// Soluzione proposta da co-pilot apparentemente corretta
void merge(int a[], int na, int c[], int nc, int b[]) {  
  int i = 0; // Indice per a[]
  int j = 0; // Indice per c[]
  int k = 0; // Indice per b[]
  
  // Scorro a[] e c[] e copio il minore in b[]
  while(i < na && j < nc) {
    if(a[i] < c[j]) {
      b[k] = a[i];
      i++;
    }
    else {
      b[k] = c[j];
      j++;
    }

    k++;
  }
  
  // Copio il resto di a[] in b[]
  while(i < na) {
    b[k] = a[i];
    i++;
    k++;
  }
  
  // Copio il resto di c[] in b[]
  while(j < nc) {
    b[k] = c[j];
    j++;
    k++;
  } 
}

// Genera array di n elementi con interi random tra 0 e 999999 
int *random_array(int n, int seed) {
  srandom(seed);
  int *a = malloc(n * sizeof(int));

  for(int i = 0; i < n; i++)
    a[i] = (int) random() % 1000000;

  return a;
}

// Funzione di confronto tra interi passata a qsort
int intcmp(const void *a, const void *b) {
  return *((int *) a) - *((int *) b);
}


void *tbody(void *arg) {
  dati *a = (dati *) arg;
  
  qsort(a -> base, a -> size, sizeof(int), &intcmp);
}