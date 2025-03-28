#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>



void termina(const char*);



typedef struct {
    char * nome; // nome[20] funziona ma scomodo per l'array statico che viene creato
    double lat;
    double lon;
} capitale;

capitale* capitale_crea(char*, double, double);
void capitale_distruggi(capitale*);

void capitale_stampa(const capitale*, FILE*);
capitale* capitale_leggi(FILE*);
capitale** capitale_leggi_file(FILE *, int *);



void merge(capitale **, int, capitale **, int, capitale **);
void mergesort(capitale **, int);

int confronta_nomi(capitale *, capitale *);
int confronta_longi(capitale *, capitale *);



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
int main(int argc, char ** argv) {
    if (argc != 2) {
        printf("Utilizzo: %s <nomeFile>\n", argv[0]);
        exit(1);
    }

    FILE * f = fopen(argv[1], "r");
    int n;

    capitale ** a = capitale_leggi_file(f, &n);

    fclose(f);

    mergesort(a, n);
    
    for (int i = 0; i < n; i++) {
        capitale_stampa(a[i], stdout);
        capitale_distruggi(a[i]);
    };

    free(a);

    return 0;
}



capitale* capitale_crea(char * s, double lat, double lon) {
    assert(s != NULL);
    assert(lat >= -90 && lat <= 90);
    assert(lon >= -180 && lon <= 180);

    capitale * a = malloc(sizeof(*a));
    if (a == NULL) termina("Memoria Insufficiente");

    a -> nome = strdup(s);
    a -> lat = lat;
    a -> lon = lon;

    return a;
}



void capitale_distruggi(capitale * c) {
    free(c -> nome);
    free(c);
}



void capitale_stampa(const capitale * c, FILE * f) {
    fprintf(f, "%20s (%f, %f)\n", c -> nome, c -> lat, c -> lon);
}



capitale* capitale_leggi(FILE * f) {
    assert(f != NULL);

    char * string;
    double lat, lon;


    int e = fscanf(f, "%ms %lf %lf", &string, &lat, &lon);

    if (e != 3) {
        if (e == EOF) return NULL;
        termina("Formato input file non valido.");
    }

    capitale * c = capitale_crea(string, lat, lon);

    free(string);

    return c;
}



capitale** capitale_leggi_file(FILE * f, int * n) {
    assert(f != NULL);

    int size = 10;
    int messi = 0;

    capitale ** a = malloc(size * sizeof(capitale*));

    if (a == NULL) termina("Memoria Insufficiente");

    while (1) {
        capitale * b = capitale_leggi(f);

        if (b == NULL) break;

        if (messi == size) {
            size *= 2;
            a = realloc(a, size * sizeof(capitale*));

            if (a == NULL ) termina("Realloc Fallita");
        }

        assert(messi < size);

        a[messi++] = b;
    }

    size = messi;
    a = realloc(a, size * sizeof(capitale*));
    if (a == NULL) termina("Realloc Fallita");

    *n = messi;
    return a;
}



void merge(capitale ** a, int na, capitale ** c, int nc, capitale ** b) {
  assert(a != NULL);
  assert(c != NULL);
  assert(b != NULL);
  assert(na > 0);
  assert(nc > 0);
  
  int i = 0; // indice per a[]
  int j = 0; // indice per c[]
  int k = 0; // indice per b[]
  
  while(i < na && j < nc) {
    if(strcmp(a[i] -> nome, c[j] -> nome) < 0) b[k] = a[i++];
    else b[k] = c[j++];

    k++;
  }
  
  while(i < na) {
    b[k] = a[i++];
    k++;
  }
  
  while(j < nc) {
    b[k] = c[j++];
    k++;
  }

  assert(i == na);
  assert(j == nc);
  assert(k == na + nc); 
}



void mergesort(capitale ** a, int n) {
  assert(a != NULL);
  assert(n > 0);
  
  if(n == 1) return;
  
  int n1 = n / 2;
  int n2 = n - n1;
  
  mergesort(a, n1);
  mergesort(&a[n1], n2);
  
  capitale ** b = malloc(n * sizeof(*b));
  if(b == NULL) termina("Malloc fallita nel merge");

  merge(a, n1, &a[n1], n2, b);  

  for(int i = 0; i < n; i++) a[i] = b[i]; 
  
  free(b);
}



int confronta_nomi(capitale * a, capitale * b) {
  return strcmp(a -> nome, b -> nome);
}



int confronta_longi(capitale * a, capitale * b) {
  if (a -> lon < b -> lon) return -1;
  else if (a -> lon > b -> lon) return 1;
  return 0; 
}



void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}