#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

// ###############################################################################################
// MERGESORT #####################################################################################
// ###############################################################################################


void mergesort(int*, int);


// int main(int argc, char ** argv) {
//     if (argc < 2) {
//         printf("Uso: %s <int1> <int2> ... <intN>\n", argv[0]);
//         return 1;
//     }


    

//     int * q = &n;
//     printf("Puntatore %p, valore %d\n", q, *q);

//     /*
//         La variabile a è un puntatore a a[0]
//         La variabile q è un puntatore a n
//         Per vedere il valore a cui punta q scrivo *q (dereferencing)
//         Per vedere il valore a cui punta a, posso scrivere *a = a[0]
//         Per leggere n = q* posso anche scrivere q[0]
//         E' equivalente scrivere *a e a[0] per qualsiasi puntatore
//         Inoltre, &a[0] = a per qualsiasi puntatore
//     */

//     return 0;
// }


void mergesort(int * a, int n) { // In place
    assert(a != NULL);
    assert(n > 0);

    if (n == 1) return; // Caso base

    int n1 = n / 2;
    int n2 = n - n1;
    
    // Ordina a[0 ... n1 - 1]
    mergesort(a, n1);

    // Ordina a[n1 ... n - 1]
    mergesort(&a[n1], n2); // &a[n1] = a + n1 = indirizzo della cella di memoria di a[n1]
    // int* + int = int*     10000 + 5 = 10000 + 5(4 = sizeof(int)) = 10020 (indirizzo di memoria del quinto elemento dell' array)

    // Merge
    int * b = malloc(n * sizeof(*b /*= sizeof(int)*/)); // dereferenziare un puntatore nel sizeof restituisce il numero di byte del tipo del puntatore
    if (b == NULL) termina("Errore: Malloc Fallita (Merge)");

    merge(a, n1, &a[n1], n2, b);

    // Copio il risultato da b[] ad a[]
    for(int i = 0; i < n; i++) a[i] = b[i];

    free(b);
}


// void merge(int * a, int na, int * c, int nc, int * b) {
//     assert(a != NULL);
//     assert(b != NULL);
//     assert(c != NULL);

//     // Scorro a[] e c[] e copio il minore in b[]
//     while (i < na && j < nc) {
//         if (a)
//     }

//     while( j < nc) {
//         b[k] = c[j];

//     }

//     // Controlli per sicurezza
//     assert(i == na);
//     assert(j == nc);
//     assert(k == na + nc);
// }