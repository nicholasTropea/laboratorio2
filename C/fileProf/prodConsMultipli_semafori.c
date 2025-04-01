/*
    Esempio paradigma produttori consumatori
    con produttori e consumatori multipli

    I produttori leggono interi dai file e i consumatori
    cacolano il numero e la somma dei primi

    il numero di produttori è uguale al numero di file di input
    il numero di consumatori è stabilito all'inizo del main() 
    
    Usare il numeri.py per generare lunghi elenchi di interi positivi su cui testare il programma
*/

#include "xerrori.h"

#define Buf_size 10
#define LINEFILE __LINE__,__FILE__

/*
    Funzione per stabilire se n e' primo  
*/
bool primo(int n) {
    if(n < 2) return false;

    if(n % 2 == 0) return (n == 2);

    for (int i = 3; i * i <= n; i += 2)
        if(n % i == 0) return false;
        
    return true;
}


// struct contenente i parametri dei thread consumatori
typedef struct {
    int quanti; // Output
    long somma; // Output

    int* buffer; // Buffer prod/cons
    int* pcindex; // Indice dei consumatori 

    pthread_mutex_t* pmutex;
    sem_t* sem_free_slots;
    sem_t* sem_data_items;  
} datic;


// struct contenente i parametri dei thread produttori
typedef struct {
    char* nomefile; // Nome del file da gestire 
    int* buffer; // Buffer prod/cons
    int* ppindex; // Indice dei produttori

    pthread_mutex_t* pmutex;
    sem_t* sem_free_slots;
    sem_t* sem_data_items;  
} datip;


/*
    Funzione eseguita dai thread consumatori
*/
void* cbody(void* arg) {  
    // Converto la struct
    datic* a = (datic*)arg; 

    // Inizializzo i risulati a 0
    a -> quanti = 0;
    a -> somma = 0;
    
    fprintf(stderr, "[C] consumatore partito\n");

    int n; // Numero corrente

    do {
        // Wait sul semaforo, se non ci sono elementi aspetto che vengano aggiunti, dopo ottengo il mutex
        xsem_wait(a -> sem_data_items, LINEFILE);
        xpthread_mutex_lock(a -> pmutex, LINEFILE);

        // Prendo il numero corrente e aggiorno l'indice
        n = a -> buffer[*(a -> pcindex) % Buf_size];
        *(a -> pcindex) += 1;

        // Libero il mutex e aumento il semaforo degli spazi liberi dato che ho consumato un elemento
        xpthread_mutex_unlock(a -> pmutex, LINEFILE);
        xsem_post(a -> sem_free_slots, LINEFILE);

        // Controllo se n è primo, in caso aumento i risultati
        if(n > 0 && primo(n)) {
            a -> quanti++;
            a -> somma += n;
        }
    } while(n != -1); // Se n == -1, i produttori hanno finito di aggiungere elementi e devo terminare.

    pthread_exit(NULL); 
}     


/*
    Funzione eseguita dai thread produttori
*/
void* pbody(void* arg) {  
    // Converto la struct
    datip* a = (datip*)arg;

    fprintf(stderr, "[P] produttore con file %s partito\n", a -> nomefile);

    // Apro il file che ho ricevuto da main
    FILE* f = fopen(a -> nomefile, "rt");
    if(f == NULL) pthread_exit(NULL); 

    // Inizializzo variabile per errore (e) e per il numero corrente (n)
    int e, n;

    while(true) {
        // Leggo il prossimo numero, in caso di errore esco
        e = fscanf(f, "%d", &n);
        if(e != 1) break; // Se il valore è letto correttamente e == 1

        assert(n > 0); // I valori del file devono essere positivi

        // Wait sul semaforo per vedere se ci sono spazi liberi, poi prendo il mutex
        xsem_wait(a -> sem_free_slots, LINEFILE);
        xpthread_mutex_lock(a -> pmutex, LINEFILE);
        
        // Aggiungo l'elemento nella posizione corrente e aggiorno l'indice dei produttori
        a -> buffer[(*a -> ppindex)++ % Buf_size]= n;

        // Rilascio il mutex e aumento il semaforo degli elementi presenti
        xpthread_mutex_unlock(a -> pmutex, LINEFILE);
        xsem_post(a -> sem_data_items, LINEFILE);
    }

    fclose(f); // Chiudo il file assegnato

    pthread_exit(NULL); 
}     


int main(int argc, char* argv[]) {
    // Verifica parametri
    if (argc < 2) {
        printf("Uso\n\t%s file1 [file2 file3 ... filek]\n", argv[0]);
        exit(1);
    }

    int cons = 3;  // Numero thread consumatori    
    int prod = argc - 1;  // Numero thread produttori = Numero di file passati

    // Risultati
    int tot_primi = 0;
    long tot_somma = 0;

    int buffer[Buf_size]; // Buffer prod/cons
    int cindex = 0, pindex = 0; // Indici consumatori e produttori

    pthread_mutex_t cmu = PTHREAD_MUTEX_INITIALIZER; // Mutex per consumatori
    pthread_mutex_t pmu = PTHREAD_MUTEX_INITIALIZER; // Mutex per produttori

    // Semafori per produttori e consumatori 
    sem_t sem_free_slots, sem_data_items;
    xsem_init(&sem_free_slots, 0, Buf_size, LINEFILE); // Spazi liberi
    xsem_init(&sem_data_items, 0, 0, LINEFILE); // Elementi presenti

    // Array di thread e struct consumatori
    pthread_t tc[cons];
    datic ac[cons];

    // Creo i thread consumatori
    for(int i = 0; i < cons; i++) {
        // Inizializzo la struct
        ac[i].buffer = buffer;
        ac[i].pcindex = &cindex;

        ac[i].pmutex = &cmu;
        ac[i].sem_data_items = &sem_data_items;
        ac[i].sem_free_slots = &sem_free_slots;

        // Faccio partire il thread
        xpthread_create(&tc[i], NULL, &cbody, &ac[i], LINEFILE);
    }

    fprintf(stderr, "Thread consumatori creati");
    
    // Array di thread e struct produttori
    pthread_t tp[prod];
    datip ap[prod];

    // Creo i thread produttori
    for(int i = 0; i < prod; i++) {
        // Inizializzo la struct
        ap[i].buffer = buffer;
        ap[i].ppindex = &pindex;

        ap[i].pmutex = &pmu;
        ap[i].sem_data_items = &sem_data_items;
        ap[i].sem_free_slots = &sem_free_slots;

        ap[i].nomefile = argv[i + 1];

        // Faccio partire il thread
        xpthread_create(&tp[i], NULL, &pbody, &ap[i], LINEFILE);
    }

    fprintf(stderr, "Thread produttori creati");

    // Join dei thread produttori
    for(int i = 0; i < prod; i++) {
        xpthread_join(tp[i], NULL, LINEFILE);
    }

    // Terminazione thread consumatori (aggiungo #consumatori -1 nel buffer)
    for(int i = 0; i < cons; i++) {
        xsem_wait(&sem_free_slots, LINEFILE);
        buffer[pindex++ % Buf_size]= -1;
        xsem_post(&sem_data_items, LINEFILE);
    }

    puts("Valori di terminazione scritti");

    // Join dei thread consumatori e calcolo risulato
    for(int i = 0; i < cons; i++) {
        xpthread_join(tc[i], NULL, LINEFILE);

        // Raccolgo i dati del consumatore i
        tot_primi += ac[i].quanti;
        tot_somma += ac[i].somma;
    }

    // Distruggo mutex e semafori
    xpthread_mutex_destroy(&pmu, LINEFILE);
    xpthread_mutex_destroy(&cmu, LINEFILE);
    xsem_destroy(&sem_data_items, LINEFILE);
    xsem_destroy(&sem_free_slots, LINEFILE);
    
    // Stampo il risultato e termino
    fprintf(stderr, "Trovati %d primi con somma %ld\n", tot_primi, tot_somma);

    return 0;
}