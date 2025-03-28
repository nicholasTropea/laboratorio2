#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione ass
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // richiesto per usare errno
#include <math.h>
#include "terne.h"


terna * terna_crea(int a, int b, int c) {
  terna * t = malloc(sizeof(*t));
  if (t == NULL) termina("creazione terna");
  t -> x = a;
  t -> y = b;
  t -> z = c;
  t -> left = t -> right = NULL;
  return t;
}




void abr_stampa_pit_inorder(terna * root, FILE * f) {
  if (root != NULL) {
    if ((root -> x * root -> x) + (root -> y * root -> y) == (root -> z * root -> z)) {
      fprintf(f, "%d %d %d", root -> x, root -> y, root -> z);
    }
  }
}



int abr_conta_si(terna * root, int si) {
  if (root != NULL) {
    int res = 0;
    if (root -> x + root -> y + root -> z == si) res++;
    res += abr_conta_si(root -> left, si);
    res += abr_conta_si(root -> right, si);
    return res;
  }

  return 0;
}

int abr_conta(terna * root) {
  if (root != NULL) {
    return 1 + abr_conta(root -> left) +abr_conta(root -> right);
  }

  return 0;
}







terna * abr_inserisci(terna * root, terna * c) {
    c -> left = c -> right = NULL;

    if(root == NULL) return c;

    int ris = terna_confronta(c, root);

    if (ris == 0) {
      fprintf(stderr, "Nodo duplicato\n");
      return root;
    }
    if (ris < 0)
        root -> left = abr_inserisci(root -> left, c);
    else
        root -> right = abr_inserisci(root -> right, c);

        
    return root;
}





terna * abr_ricerca(terna * root, terna * c) {
    if(root==NULL) return NULL;

    int ris = terna_confronta(root, c);

    if(ris == 0) {
        return root;
    }
    else if(ris < 0) {
        return abr_ricerca(root -> left, c);
    }
    else {
        return abr_ricerca(root -> right, c);
    }
}












void terna_distruggi(terna * t) {
  free(t);
}

void terna_stampa(const terna * t, FILE * f) {
  fprintf(f, "%d %d %d\n", t -> x, t -> y, t -> z);
}

int terna_confronta(terna * t, terna * v) {
  if(t -> z != v -> z) return t -> z - v -> z;
  if(t -> y != v -> y) return t -> y - v -> y;
  return t -> x - v -> x;
}  

void termina(const char *messaggio)
{
  if(errno==0) 
     fprintf(stderr,"%s\n",messaggio);
  else 
    perror(messaggio);
  exit(1);
}
