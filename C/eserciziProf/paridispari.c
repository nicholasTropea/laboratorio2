#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void fileError(char, const char*);

int main(int argc, char ** argv) {
    if (argc != 4) {
        puts("Utilizzo: paridispari <textFile> <fileName> <fileName>\n");
        return 1;
    }

    FILE * numbers = fopen(argv[1], "rt");
    if (numbers == NULL) fileError('o', argv[1]);

    FILE * pari = fopen(argv[2], "wt");
    if (pari == NULL) fileError('o', argv[2]);

    FILE * dispari = fopen(argv[3], "wt");
    if (dispari == NULL) fileError('o', argv[3]);

    int num;

    while (1) {
        int e = fscanf(numbers, "%d", &num);
        
        if (e == EOF) break;
        else if (e != 1) fileError('r', argv[1]);

        if (num % 2 == 0) {
            e = fprintf(pari, "%d\n", num);
            if (e < 0) fileError('w', argv[2]);
        }
        else {
            e = fprintf(dispari, "%d\n", num);
            if (e < 0) fileError('w', argv[3]);
        }
    }

    if (fclose(numbers) != 0) fileError('c', argv[1]);
    if (fclose(pari) != 0) fileError('c', argv[2]);
    if (fclose(dispari) != 0) fileError('c', argv[3]);
    
    return 0;
}

void fileError(char type, const char * file) {
    printf("Errore: ");
    
    if (type == 'w') printf("Scrittura ");
    else if (type == 'o') printf("Apertura ");
    else if (type == 'c') printf("Chiusura ");
    else printf("Lettura ");

    printf("del file <%s> fallita.\n", file);

    exit(1);
}