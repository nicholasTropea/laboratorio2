#include "xerrori.h"


bool primo(int n);


// struct usata per passare argomenti ai thread
typedef struct {
  int start;            // Inizio intervallo di ricerca
  int end;              // Fine intervalo di ricerca
  int somma_parziale;   // Parametro di output
} dati;


// Funzione svolta da ogni thread (passata in pthread_create())
void *tbody(void *v) {
  dati *d = (dati *) v;

  int primi = 0;

  fprintf(stderr, "Conto i primi fra %d e %d\n", d -> start, d -> end);

  // Cerco i primi nell'intervallo assegnato
  for (int j = d -> start; j < d -> end; j++) {
    if(primo(j)) primi++; 
  }

  fprintf(stderr, "Il thread che partiva da %d ha terminato\n", d -> start);

  d -> somma_parziale = primi;

  pthread_exit(NULL); // Necessaria alla chiusura di ogni thread
}


int main(int argc, char *argv[]) {
  if(argc != 3) {
    fprintf(stderr,"Uso\n\t%s m num_threads\n", argv[0]);

    exit(1);
  }

  // Conversione input
  int m = atoi(argv[1]);
  if(m < 1) termina("limite primi non valido");

  int p = atoi(argv[2]);
  if(p <= 0) termina("numero di thread non valido");

  // Creazione thread ausiliari
  pthread_t t[p];   // Array di p indentificatori di thread 
  dati d[p];        // Array di p struct che passerò ai p thread

  for (int i = 0; i < p; i++) {
    int n = m / p;  // Quanti numeri verifica ogni thread + o - 

    // Calcolo dell'intervallo di ricerca
    d[i].start = n * i;
    d[i].end = (i == p - 1) ? m : n * (i + 1);

    // Crea il thread i
    xpthread_create(&t[i], NULL, &tbody, &d[i], __LINE__, __FILE__); 
  }

  // Attendo che i thread abbiano finito
  int somma = 0;

  for(int i = 0; i < p; i++) {
    xpthread_join(t[i], NULL, __LINE__, __FILE__);
    somma += d[i].somma_parziale;
  }

  // Restituisce il risultato 
  printf("Numero primi tra 1 e %d (escluso): %d\n", m, somma);

  return 0;
}


// Restituisce true/false a seconda che n sia primo o composto
bool primo(int n) {
  if(n < 2) return false;

  if(n % 2 == 0) {
    if(n == 2)  return true;
    else return false; 
  }

  for (int i = 3; i * i <= n; i += 2) 
    if(n % i == 0) return false;

  return true;
}