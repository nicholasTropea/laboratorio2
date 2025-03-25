#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <errno.h>


// Capitale struct
typedef struct capit {
  char *nome;
  char *nazione;
  struct capit *left, *right;
} capitale;


// Functions

// Stampa la capitale a sulla prossima riga nel file f
void capitale_stampa(const capitale * a, FILE * f) {
    fprintf(f, "(%s, %s)\n", a -> nome, a -> nazione);
}

// Crea un nodo capitale con nome n e nazione na
capitale * capitale_crea(char * n, char * na) {
    // Alloca memoria e crea campi testuali
    capitale * a  = malloc(sizeof(capitale));
    a -> nome = strdup(n);
    a -> nazione = strdup(na);

    // Inizializza figli a NULL
    a -> left = NULL;
    a -> right = NULL;

    return a;
}

// Dealloca la memoria relativa ad un nodo capitale
void capitale_distruggi(capitale * a) {
    // Dealloca la memoria dei dati
    free(a -> nome);
    free(a -> nazione);
    
    // Dealloca la memoria del nodo
    free(a);
}

// Legge i campi dal file f per creare il prossimo nodo, restituisce NULL se file finito, il puntatore al nodo altrimenti
capitale * capitale_leggi(FILE * f) {
    // File NULL -> return NULL
    if (f == NULL) return NULL;

    char * buffer;
    size_t len = 0;
    ssize_t charNum;

    // Get next line
    charNum = getline(&buffer, &len, f);

    // File ended -> return NULL
    if (charNum == -1) return NULL;

    // Get rid of final '\n' if there is one
    if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';

    // Get name and nation
    char * nameToken = strtok(buffer, "; ");
    strtok(NULL, "; ");
    char * nationToken = strtok(buffer, "; ");

    // Create node
    capitale * c = capitale_crea(nameToken, nationToken);

    // Deallocate strings memory
    free(nameToken);
    free(nationToken);
    free(buffer);

    return c;
} 


// Stampa su file (guardare capitale_stampa) i valori dell'albero di radice root
void abr_capitale_stampa(const capitale * root, FILE * f) {
  if (root != NULL) {
    abr_capitale_stampa(root -> left, f);
    capitale_stampa(root, f);
    abr_capitale_stampa(root -> right, f);
  }
}

// Stampa su file (guardare capitale_stampa) i valori dell'albero di radice root in preorder
void abr_capitale_stampa_preorder(const capitale * root, FILE * f, int depth) {
  // stampa depth spazi per mostrare la profondità del nodo
  for(int i = 0; i < depth; i++) fprintf(f,". ");

  if(root != NULL) {
    capitale_stampa(root,f);
    abr_capitale_stampa_preorder(root -> left, f, depth + 1);
    abr_capitale_stampa_preorder(root -> right, f, depth + 1);
  }
  else fprintf(f, "X\n");
}

// Dealloca tutti i nodi dell'albero di radice root
void abr_capitale_distruggi(capitale * root) {
  if(root != NULL) {
    abr_capitale_distruggi(root -> left);
    abr_capitale_distruggi(root -> right);
    capitale_distruggi(root);
  }
}

// Inserisce il nodo c nell'albero di radice root se non è già presente, restituisce la radice del nuovo albero
capitale * abr_inserisci(capitale * root, capitale * c) {
    // Inizializza i figli del nodo a NULL
    c -> left = c -> right = NULL;

    // Albero vuoto -> return nodo c
    if(root == NULL) return c;

    // Comparo il nome del nodo corrente (root) con quello del nodo c
    int ris = strcmp(c->nome,root->nome);

    if(ris == 0) { // Nomi uguali -> Non inserisco, stampo errore e dealloco il nodo c
        fprintf(stderr, "Nodo duplicato: ");
        capitale_stampa(c, stderr);
        capitale_distruggi(c);
    }
    else if(ris < 0) // Nome c < Nome root -> Inserisco nel sottoalbero sinistro
        root -> left = abr_inserisci(root -> left, c);
    else // Nome c > Nome root -> Inserisco nel sottoalbero destro
        root -> right = abr_inserisci(root -> right, c);

    // Restituisco l'albero risultante    
    return root;
}
    

// Crea un albero prendendo i nomi dei campi dei nodi dal file f
capitale * crea_abr(FILE * f) {
    // Inizializza albero vuoto
    capitale * root = NULL;

    while (true) {
        // Fetch del prossimo nodo
        capitale * b = capitale_leggi(f);

        // File finito -> return
        if(b == NULL) break;

        // Update dell'albero inserendo il nodo corrente
        root = abr_inserisci(root, b);
    }  

    return root;
}

// Cerca la città (ricerca binaria) nome nell'albero di radice root, restituisce NULL se non trovata, altrimenti il puntatore al nodo
capitale * abr_ricerca(capitale * root, char * nome) {
    // Albero vuoto -> return NULL
    if(root==NULL) return NULL;

    // Ris = risultato della compare
    int ris = strcmp(nome, root -> nome);

    if(ris == 0) { // Radice = risultato
        return root;
    }
    else if(ris < 0) { // Radice > nodo -> ricerca sottoalbero sinistro
        return abr_ricerca(root -> left, nome);
    }
    else { // Radice < nodo -> ricerca sottoalbero destro
        return abr_ricerca(root -> right, nome);
    }
}