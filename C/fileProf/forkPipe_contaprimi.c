#include "xerrori.h"
#define LINEFILE __LINE__,__FILE__

/*
    Programma per il conteggio di numero dei primi in un
    intervallo usando piu' processi ausiliari
    e una pipe per comunicare i conteggi parziali 
    al processo genitore
*/


/*
    Restituisce true/false a seconda che n sia primo o composto
*/
bool primo(int n) {
    if(n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (int i = 3; i * i <= n; i += 2) { 
        if(n % i == 0) return false;
    }

    return true;
}


/*
    Conta i primi in [a,b)
*/
int contap(int a, int b) {
    int tot = 0;

    for(int i = a; i < b; i++) if(primo(i)) tot++;

    return tot;  
}


/*
    Conta quanti sono i primi in [ argv[1], argv[2] )
*/
int main(int argc, char* argv[]) {
    // Check sugli argomenti e conversione
    if(argc != 4) {
        printf("Uso:\n\t%s n1 n2 p\n", argv[0]);
        exit(1);
    }

    int n0 = atoi(argv[1]);
    int n1 = atoi(argv[2]);
    int p = atoi(argv[3]);
    assert(n0>0 && n1>=n0 && p>0);
    
    // Crea una pipe di comunicazione dai figli al genitore la chiamo up perchè la uso da figli a genitore
    int up[2];
    xpipe(up, LINEFILE);

    // Generazione dei processi figli
    for(int i = 0; i < p; i++) {
        // Genera il figlio i
        pid_t pid = xfork(LINEFILE);

        // Se pid == 0 => è il figlio
        if(pid == 0) {
            // Chiude la pipe in lettura (nota, si usa la close() dato che abbiamo un file descriptor)
            xclose(up[0], LINEFILE);

            // Calcola l'intervallo che deve analizzare 
            int n = (n1 - n0) / p;  // Quanti numeri verifica 
            int start = n0 + n * i; // Inizio del range
            int end = (i == p - 1) ? n1 : n0 + n * (i + 1); // Fine del range  

            // Calcola il risultato e stampa le informazioni
            int tot = contap(start, end);
            printf("Figlio %d: cercato tra %d e %d, trovati %d primi\n", i, start, end, tot);

            // Scrive il risultato nella pipe
            ssize_t e = write(up[1], &tot, sizeof(int));
            
            // Se e != #byte scritti => errore
            if(e != sizeof(int)) termina("Errore scrittura pipe");

            // Chiude la pipe in scrittura
            xclose(up[1], LINEFILE);

            // Termina
            exit(0);
        }  
    }

    // CODICE ESEGUITO SOLO DAL GENITORE 

    int tot = 0; // Risultato

    // Chiude la pipe in scrittura
    xclose(up[1], LINEFILE);

    // Loop fino a quando tutti non hanno chiuso up[1]
    while(true) {
        int x; // Intero preso dalla pipe

        // Legge l'intero dalla pipe e lo salva in x
        ssize_t e = read(up[0], &x, sizeof(int));

        // Se la read restituisce 0 => La pipe è vuota e chiusa in scrittura da tutti
        if(e == 0) break;

        // Print del valore letto e aumento del risultato
        printf("Genitore: letto il valore %d dalla pipe\n", x);
        tot += x;
    }

    // Chiude la pipe in lettura
    xclose(up[0], LINEFILE);

    // Stampa il risultato e termina
    printf("Numero primi p tali che  %d <= p < %d è: %d\n", n0, n1, tot);

    return 0;
}