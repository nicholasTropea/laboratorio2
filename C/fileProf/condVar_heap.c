#include "xerrori.h"

#define LINEFILE __LINE__,__FILE__

/*
    Gestione di heap mediante condition variable
    per fare si che le stampe riflettano l'effettivo ordine
    delle operazioni queste sono effettuate quando il 
    mutex è sempre bloccato 
*/

// struct che tiene traccia della memoria disponibile e contiene le variabili cond/mutex per regolarne l'utilizzo
typedef struct {
    pthread_cond_t* cv; // Condition variable
    pthread_mutex_t* mu; // Mutex
    int MB; // Memoria attualmente disponibile
} heap;


// Simula allocazione con spazio limitato
void richiedi(heap* h, int n) {
    // Lock del mutex
    xpthread_mutex_lock(h -> mu, LINEFILE);

    fprintf(stderr, "%2d Richiesti: %3d\n", gettid() % 100, n);

    // Wait sulla condition variable se spazio non disponibile
    while(n > h -> MB) {
        fprintf(stderr, "%2d Negati: %3d\n", gettid() % 100, n);
        xpthread_cond_wait(h -> cv, h -> mu, LINEFILE);
    }

    // Riduco lo spazio rimanente, stampo e rilascio il mutex
    h -> MB -= n;

    fprintf(stderr, "%2d Assegnati: %3d. Rimanenti: %4d\n\n", gettid() % 100,
            n, h -> MB);

    xpthread_mutex_unlock(h -> mu, LINEFILE);
}

// Deallocazione
void libera(heap* h, int n) {
    // Lock del mutex
    xpthread_mutex_lock(h -> mu, LINEFILE);

    // Aumento lo spazio rimanente, sveglio tutti i thread in wait sulla condition variable,
    // stampo e rilascio il mutex
    h -> MB += n;

    xpthread_cond_broadcast(h -> cv, LINEFILE);

    fprintf(stderr, "%2d Restituiti: %3d. Rimanenti: %4d\n\n", gettid() % 100,
            n, h -> MB);

    xpthread_mutex_unlock(h -> mu, LINEFILE); 
}


// Codice dei thread di tipo 1, chiede 10, 20, ..., 50
void* tipo1(void* v) {
    // Converte la struct
    heap* h = (heap*) v;

    // Calcola la memoria, la richiede, attende 1 secondo e la libera
    for(int i = 1; i <= 5; i++) {
        int m = 10 * i;

        richiedi(h, m);
        sleep(1);
        libera(h, m);
    }

    return NULL;
}


// Codice dei thread di tipo 2, chiede 15, 25, ..., 55
void* tipo2(void* v) {
    // Converte la struct
    heap* h = (heap*) v;

    // Calcola la memoria, la richiede, attende 1 secondo e la libera
    for(int i = 1; i <= 5; i++) {
        int m = 10 * i + 5;

        richiedi(h, m);
        sleep(1);
        libera(h, m);
    }
    
    return NULL;
}


int main(int argc, char* argv[]) {
    // Verifica dei parametri
    if(argc != 3) {
        printf("Uso: %s mem numT\n", argv[0]);
        return 1;
    }

    // Conversione dei parametri
    int mem = atoi(argv[1]); // Totale memoria
    int nt = atoi(argv[2]); // Numero dei thread

    assert(nt > 1);

    // Inizializzazione dell'heap 
    pthread_cond_t c = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    heap h;
    h.cv = &c;
    h.mu = &m;
    h.MB = mem;

    // Crea array dei thread
    pthread_t t[nt];

    // Crea un thread di tipo 1
    xpthread_create(&t[0], NULL, &tipo1, &h, LINEFILE);

    // Crea n - 1 thread di tipo 2
    for(int i = 1; i < nt; i++)
        xpthread_create(&t[i], NULL, &tipo2, &h, LINEFILE);

    // Attende che i thread terminino
    for(int i = 0; i < nt; i++)
        xpthread_join(t[i], NULL, LINEFILE);

    // Distrugge mutex e condition variable
    xpthread_cond_destroy(&c, LINEFILE);
    xpthread_mutex_destroy(&m, LINEFILE);

    // Stampa finale e terminazione
    fprintf(stderr, "Terminato con memoria: %d\n", h.MB);

    return 0;
}