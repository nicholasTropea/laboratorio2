#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni per stringhe
#include "xerrori.h"

#define LINEFILE __LINE__,__FILE__


typedef struct {
    int* counter;
    int increment;
    pthread_mutex_t mutex; 
} threadData;


void* threadBody(void* args) {
    threadData* data = (threadData*) args;
    
    printf("Thread %ld: started.", getpid());

    for (int i = 0; i < threadIncrements; i++) {
        xpthread_mutex_lock(data.mutex, LINEFILE);

        data.counter++;

        xpthread_mutex_unlock(mutex, LINEFILE);
    }

    printf("Thread %ld: ended.", getpid());

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    int numThread = 4;
    int threadIncrements = 5;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int counter = 0;

    pthread_t threads[numThread];
    threadData td[numThread];

    for (int i = 0; i < numThread; i++) {
        td[i].counter = &counter;
        td[i].mutex = &mutex;
        td[i].increment = threadIncrements;

        xpthread_create(&threads[i], NULL, &threadBody, &td[i], LINEFILE);
    }

    printf("Creati %d threads.\n", numThread);

    for (int i = 0; i < numThread; i++) {
        xpthread_join(threads[i], NULL, LINEFILE);
    }

    xpthread_mutex_destroy(&mutex, LINEFILE);

    printf("Tutti i thread hanno terminato ed hanno incrementato di %d il counter.\nCounter finale: %d.", increment, counter);

    return 0;
}