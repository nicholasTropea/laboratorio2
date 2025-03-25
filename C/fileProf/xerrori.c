#include "xerrori.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// MESSAGGI D'ERRORE //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
  Termina un processo con eventuale messaggio d'errore, si noti che la variabile errno è "thread local",
  quindi ne esiste una diversa per ogni thread.  
*/
void termina(const char *messaggio) {
  if (errno == 0) fprintf(stderr, "== %d == %s\n", getpid(), messaggio);
  else fprintf(stderr, "== %d == %s: %s\n", getpid(), messaggio, strerror(errno));

  exit(1);
}


/*
  Termina un processo con eventuale messaggio d'errore + linea e file, si noti che la variabile errno è "thread local",
  quindi ne esiste una diversa per ogni thread.  
*/
void xtermina(const char *messaggio, int linea, char *file) {
  if (errno == 0) fprintf(stderr, "== %d == %s\n", getpid(), messaggio);
  else fprintf(stderr, "== %d == %s: %s\n", getpid(), messaggio, strerror(errno));

  fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);
  exit(1);
}


/*
  Stampa su stderr il messaggio d'errore associato al codice en in maniera simile a perror, quindi senza terminare.
*/
#define Buflen 100

void xperror(int en, char *msg) {
  char buf[Buflen];
  
  char *errmsg = strerror_r(en, buf, Buflen);

  if(msg != NULL) fprintf(stderr, "%s: %s\n", msg, errmsg);
  else fprintf(stderr, "%s\n", errmsg);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// OPERAZIONI SU FILE /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  Apre un file <path> in modalità <mode> e stampa eventuali errori + linea e file
*/
FILE *xfopen(const char *path, const char *mode, int linea, char *file) {
  FILE *f = fopen(path, mode);

  if(f == NULL) {
    perror("Errore apertura file");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return f;
}


/*
  Chiude un file <fd> e stampa eventuali errori + linea e file
*/
void xclose(int fd, int linea, char *file) {
  int e = close(fd);

  if(e != 0) {
    perror("Errore chiusura file descriptor");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// FUNZIONI PER THREAD ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  NOTA BENE:
    In caso di errore non scrivono il codice d'errore in errno, ma lo restituiscono come return value,
    questo perchè errno non è thread-safe in certe versioni di Linux.

    Return value = 0 => Nessun errore.
*/


/*
  Crea un thread e stampa eventuali errori + linea e file.
*/
int xpthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg, int linea, char *file) {
  int e = pthread_create(thread, attr, start_routine, arg);

  if (e != 0) {
    xperror(e, "Errore pthread_create");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;                       
}


/*
  Aspetta che <thread> termini, se è già terminato fa return immediatamente (<thread> deve essere joinabile).

  Se <retval> != NULL allora l'exit status di <thread> (il valore che <thread> ha passato a pthread_exit())
  viene messo nella locazione puntata da <retval>.
  
  Se <thread> è stato eliminato prima della join, PTHREAD_CANCELED viene messo in <retval>.

  Stampa eventuali errori + linea e file.
*/
int xpthread_join(pthread_t thread, void **retval, int linea, char *file) {
  int e = pthread_join(thread, retval);

  if (e != 0) {
    xperror(e, "Errore pthread_join");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// MUTEX /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  Inizializza DINAMICAMENTE il mutex puntato da <mutex>, secondo gli attributi specificati in <attr>,
  l'implementazione LinuxThreads supporta solo l'attributo MutexKind, che può essere: 'fast', 'recursive' o 'error checking'.

  Se <attr> == NULL, 'fast' è selezionato di default.
  
  NOTA BENE: 
    Per inizializzare un mutex con <attr> == 'fast' STATICAMENTE usare: pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    Inizializzando un mutex STATICAMENTE non è possibile reinizializzarlo una volta chiamata pthread_mutex_destroy(),
    mentre è possibile farlo con mutex inizializzati DINAMICAMENTE.
*/
int xpthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr, int linea, char *file) {
  int e = pthread_mutex_init(mutex, attr);

  if (e != 0) {
    xperror(e, "Errore pthread_mutex_init");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }  

  return e;
}


/*
  Distrugge l'oggetto <mutex>, liberando le sue risorse, se questo è sbloccato.
  (Nell'implementazione LinuxThreads un mutex non ha risorse associate, in realtà pthread_mutex_destroy() non fa niente).

  Return value == EBUSY => Mutex bloccato al tempo di pthread_mutex_destroy().

  Stampa eventuali errori + linea e file.
*/
int xpthread_mutex_destroy(pthread_mutex_t *mutex, int linea, char *file) {
  int e = pthread_mutex_destroy(mutex);

  if (e != 0) {
    xperror(e, "Errore pthread_mutex_destroy");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}


/*
  Se <mutex> è sbloccato lo blocca e lo assegna al thread chiamante, ritornando immediatamente.
  Se <mutex> è bloccato sospende il thread chiamante fino a che non viene sbloccato.

  NOTA BENE:
    Il comportamento a seguito di tentativo di lock di un mutex di cui si è già proprietari varia a seconda del MutexKind,
    con MutexKind == 'fast' (default), il thread va in deadlock.
*/
int xpthread_mutex_lock(pthread_mutex_t *mutex, int linea, char *file) {
  int e = pthread_mutex_lock(mutex);

  if (e != 0) {
    xperror(e, "Errore pthread_mutex_lock");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}


/*
  Sblocca <mutex> e stampa eventuali errori + linea e file.

  NOTA BENE:
    Se MutexKind = 'fast', pthread_mutex_unlock() non effettua check relativi al fatto che il chiamante
    abbia il possesso di <mutex>, quindi può essere sbloccato da qualsiasi thread.

    Se <mutex> è già sbloccato, eseguire pthread_mutex_unlock() risulta in undefined behavior.
*/
int xpthread_mutex_unlock(pthread_mutex_t *mutex, int linea, char *file) {
  int e = pthread_mutex_unlock(mutex);

  if (e != 0) {
    xperror(e, "Errore pthread_mutex_unlock");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// BARRIERE ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  Inizializza e alloca le risorse necessarie per <barrier> con attributi <attr> e <count>
  <count> (Numero di thread che devono chiamare pthread_barrier_wait() per sbloccarla, <count> > 0).

  Stampa eventuali messaggi d'errore + linea e file.

  NOTA BENE:
    Undefined behavior se:
      - Chiamato su una barriera già inizializzata.
      - Chiamato mentre ci sono thread che aspettano lo sblocco di <barrier>
      - Utilizzo di <barrier> non inizializzata.
*/
int xpthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, unsigned int count, int linea, char *file) {
  int e = pthread_barrier_init(barrier, attr, count);

  if (e != 0) {
    xperror(e, "Errore pthread_barrier_init");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }  

  return e;
}


/*
  Distrugge <barrier>, libera le risorse da essa utilizzata e stampa eventuali messaggi d'errore + linea e file.

  NOTA BENE:
    Undefined behavior se:
      - Utilizzo di <barrier> una volta distrutta e non reinizializzata.
      - Chiamata mentre altri thread sono bloccati sulla barriera.
      - Chiamata su <barrier> non inizializzata.
*/
int xpthread_barrier_destroy(pthread_barrier_t *barrier, int linea, char *file) {
  int e = pthread_barrier_destroy(barrier);
  if (e!=0) {
    xperror(e, "Errore pthread_barrier_destroy");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    pthread_exit(NULL);
  }
  return e;
}


/*
  Sincronizza il thread chiamante sulla <barrier>, il chiamante resta in stato di attesa fino a che un numero
  di thread uguale a <count> (Vedi pthread_barrier_init()) si sincronizza sulla barriera, quando ciò accade il thread
  riprende ad eseguire le sue istruzioni.

  pthread_barrier_wait() restituisce il valore 0 a n - 1 thread in attesa, mentre ad un solo thread (random) restituisce
  PTHREAD_BARRIER_SERIAL_THREAD, definito nella libreria pthread.h, dopo ciò, <barrier> torna nello stato iniziale
  risultato da pthread_barrier_init().

  Stampa eventuali errori + linea e file.

  NOTA BENE:
    Undefined behavior se chiamato su <barrier> non inizializzata.
*/
int xpthread_barrier_wait(pthread_barrier_t *barrier, int linea, char *file) {
  int e = pthread_barrier_wait(barrier);

  if (e != 0 && e != PTHREAD_BARRIER_SERIAL_THREAD) {
    xperror(e, "Errore pthread_barrier_wait");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// CONDITION VARIABLES ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  Inizializza DINAMICAMENTE <cond> con attributi <attr> (default se <attr> = NULL) e
  stampa eventuali messaggi d'errore + linea e file.

  NOTA BENE:
    Per inizializzare <cond> STATICAMENTE con default <attr> usare:
    pthread_cond_t myCond = PTHREAD_COND_INITIALIZER;
*/
int xpthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr, int linea, char *file) {
  int e = pthread_cond_init(cond,attr);

  if (e != 0) {
    xperror(e, "Errore pthread_cond_init");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}


/*
  Distrugge <cond>, libera eventuali risorse e stampa eventuali messaggi d'errore + linea e file.
  Esegue un check sul numero di thread in attesa su <cond>, se è diverso da 0 restituisce EBUSY.
*/
int xpthread_cond_destroy(pthread_cond_t *cond, int linea, char *file) {
  int e = pthread_cond_destroy(cond);

  if (e != 0) {
    xperror(e, "Errore pthread_cond_destroy");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}


/*
  Il thread chiamante sblocca <mutex> (secondo pthread_mutex_unlock()) e resta in attesa di un segnale su <cond>,
  quando arriva il segnale automaticamente blocca <mutex> (secondo pthread_mutex_lock()) e riprende l'esecuzione.

  Stampa eventuali messaggi d'errore + linea e file.

  NOTA BENE:
    Il chiamante deve avere la proprietà di <mutex>, altrimenti risulta in undefined behavior.  
*/
int xpthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, int linea, char *file) {
  int e = pthread_cond_wait(cond, mutex);

  if (e != 0) {
    xperror(e, "Errore pthread_cond_wait");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}


/*
  Manda un segnale ad uno dei thread in attesa su <cond> (random), se nessun thread è in attesa non accade nulla.
  Stampa eventuali messaggi d'errore + linea e file.
*/
int xpthread_cond_signal(pthread_cond_t *cond, int linea, char *file) {
  int e = pthread_cond_signal(cond);

  if (e != 0) {
    xperror(e, "Errore pthread_cond_signal");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}


/*
  Manda un segnale ad ogni thread in attesa su <cond>, se nessun thread è in attesa non accade nulla.
  Stampa eventuali messaggi d'errore + linea e file.
*/
int xpthread_cond_broadcast(pthread_cond_t *cond, int linea, char *file) {
  int e = pthread_cond_broadcast(cond);

  if (e != 0) {
    xperror(e, "Errore pthread_cond_broadcast");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    pthread_exit(NULL);
  }

  return e;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// SEMAFORI POSIX ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  NOTA BENE:
    I semafori POSIX sono usati sia da processi che da thread, è utile distinguere tra l'utilizzo di exit() e pthread_exit().

    Threads:
      - Chiusura con exit() -> Causa la terminazione del processo intero.
      - Chiusura con pthread_exit() -> Causa la terminazione del singolo thread.
    Processi:
      - Chiusura con exit() -> Causa la terminazione del processo intero, necessario per stampare errore.
    
    E' possibile distinguere se quello in esecuzione è un thread o un processo utilizzando gettid(),
    se il valore coincide con il PID principale, il codice sta girando su un processo, altrimenti su un thread.
*/


/*
  Crea (o apre in caso esista già) un nuovo semaforo named (FIFO) identificato con <name>.

  Stampa eventuali messaggi d'errore + linea e file.

  Parametri:
    - <value> -> Valore iniziale del semaforo.
    - <oflag> -> Se = O_CREAT, crea il semaforo in caso non esista.
              -> Se = O_CREAT || O_EXCL, restituisce SEM_FAILED e setta errno se il semaforo di nome <name> esiste già.
    - <mode> -> Permessi da assegnare al smeaforo (se creato).
*/
sem_t *xsem_open(const char *name, int oflag, mode_t mode, unsigned int value, int linea, char *file) {
  sem_t *s = sem_open(name, oflag, mode, value);

  if (s == SEM_FAILED) {
    perror("Errore sem_open");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file); 

    exit(1);
  }

  return s;
}


/*
  Chiude il semaforo named (FIFO) <s>, liberando le sue risorse.
  Stampa eventuali messaggi d'errore + linea e file.
*/
int xsem_close(sem_t *s, int linea, char *file) {
  int e = sem_close(s);

  if(e != 0) {
    perror("Errore sem_close"); 
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return e;  
}


/*
  "Rimuove" immediatamente il semaforo named (FIFO) di nome <name>, il semaforo viene distrutto una volta che è
  stato chiuso da tutti i processi/thread che lo avevano aperto.

  Stampa eventuali messaggi d'errore + linea e file.
*/
int xsem_unlink(const char *name, int linea, char *file) {
  int e = sem_unlink(name);

  if(e != 0) {
    perror("Errore sem_unlink"); 
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return e;  
}


/*
  Inizializza il semaforo unnamed <sem> con valore iniziale <value>, stampa eventuali messaggi d'errore + linea e file.

  <pshared> :
    - 0 -> Il semaforo è condiviso tra i vari thread del processo, deve essere allocato come variabile globale o nell'heap.
    - != 0 -> Il semaforo è condiviso tra vari processi, deve essere allocato nella memoria condivisa.
*/
int xsem_init(sem_t *sem, int pshared, unsigned int value, int linea, char *file) {
  int e = sem_init(sem, pshared, value);

  if(e != 0) {
    perror("Errore sem_init"); 
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return e;
}


/*
  Distrugge il semaforo unnamed <sem> (da utilizzare solo su semafori inizializzati con sem_init()) e
  stampa eventuali messaggi d'errore + linea e file.

  Undefined behavior:
    - Chiamata su <sem> mentre altri thread/processi sono in attesa su di esso.
    - Utilizzo di <sem> una volta distrutto, senza che sia reinizializzato con sem_init().
*/
int xsem_destroy(sem_t *sem, int linea, char *file) {
  int e = sem_destroy(sem);

  if(e != 0) {
    perror("Errore sem_destroy"); 
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return e;
}


/*
  Incrementa il semaforo (named o unnamed) <sem> di 1 e stampa eventuali messaggi d'errore + linea e file.
*/
int xsem_post(sem_t *sem, int linea, char *file) {
  int e = sem_post(sem);

  if(e != 0) {
    perror("Errore sem_post"); 
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return e;
}


/*
  Decrementa il semaforo (named o unnamed) <sem> di 1 se possibile,
  altrimenti sospende il thread chiamante fino a quando lo diventa.

  Stampa eventuali messaggi d'errore + linea e file.
*/
int xsem_wait(sem_t *sem, int linea, char *file) {
  int e = sem_wait(sem);

  if(e != 0) {
    perror("Errore sem_wait"); 
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return e;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// OPERAZIONI SU PROCESSI //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  Crea un processo figlio 'duplicando' il processo corrente.
  Stampa eventuali messaggi d'errore + linea e file.

  NOTA BENE: Padre e figlio vengono eseguiti in spazi di memoria separati.

  ELEMENTI DIVERSI TRA PADRE E FIGLIO:
    - Process ID (PID).
    - Parent Process ID.
    - Memory locks NON EREDITATI.
    - I segnali pendenti del figlio sono azzerati.
    - I semaphores adjustments NON SONO EREDITATI.
  
  ELEMENTI EREDITATI:
    - File descriptor dei file aperti dal padre (con file offset ecc...)
    - Lo stato degli oggetti pthread (attenzione sull'uso, si cerchi pthread_atfork())

*/
pid_t xfork(int linea, char *file) {
  pid_t p = fork();

  if(p < 0) {
    perror("Errore fork");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }

  return p;
}


/*
  Sospende l'esecuzione del thread chiamante finché uno dei suoi figli non termina.
  Chiamare wait() permette al sistema di liberare le risorse associate al figlio terminato.

  NOTA BENE:
    - In caso di figlio già terminato, wait() restituisce subito.
    - Un figlio terminato rimane in stato "Zombie" fino a che non viene eseguita wait().
    - (Guardare anche waitpid())
  
  VALORE DI <status>:
    Se <status> != NULL, wait() salva informazioni sullo stato del processo terminato restituendole.
    (Lista dei check su <status> nella pagina linux man 2 wait)
*/
pid_t xwait(int *status, int linea, char *file) {
  pid_t p = wait(status);

  if(p < 0) {
    perror("Errore wait");
    fprintf(stderr, "== %d == Linea: %d, File: %s\n", getpid(), linea, file);

    exit(1);
  }
  
  return p;
}


/*
  
*/
int xpipe(int pipefd[2], int linea, char *file) {
  int e = pipe(pipefd);
  if(e!=0) {
    perror("Errore creazione pipe"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;
}

// ---------------- memoria condivisa POSIX
int xshm_open(const char *name, int oflag, mode_t mode, int linea, char *file)
{
  int e = shm_open(name, oflag, mode);
  if(e== -1) {
    perror("Errore shm_open"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;  
}

int xshm_unlink(const char *name, int linea, char *file)
{
  int e = shm_unlink(name);
  if(e== -1) {
    perror("Errore shm_unlink"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;  
}

int xftruncate(int fd, off_t length, int linea, char *file)
{
  int e = ftruncate(fd,length);
  if(e== -1) {
    perror("Errore ftruncate"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;  
}

void *simple_mmap(size_t length, int fd, int linea, char *file)
{
  void *a =  mmap(NULL, length ,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(a == (void *) -1) {
    perror("Errore mmap"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return a;
}

int xmunmap(void *addr, size_t length, int linea, char *file)
{
  int e = munmap(addr, length);
  if(e== -1) {  
    perror("Errore munmap"); 
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;
}