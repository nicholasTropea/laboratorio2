#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


void termina(const char*);
char* converte16(int);



int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("Utilizzo: %s <int1> <int2> ... <intN>\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        char * n = converte16(atoi(argv[i]));
        printf("0x%s\n", n);
        free(n);
    }

    return 0;
}



char* converte16(int n) {
    if (n == 0) {
        char * num = malloc(2 * sizeof(char));
        if (num == NULL) termina("Errore: Memoria Insufficiente.");

        num[0] = '0';
        num[1] = '\0';
        return num;
    }

    int power = 0;
    int temp = n;

    // Calculates number of digits needed
    while (temp > 0) {
        temp /= 16;
        power++;
    }

    char * digits = malloc((power + 1) * sizeof(char));
    if (digits == NULL) termina("Errore: Memoria Insufficiente.");

    for (int i = power - 1; i >= 0; i--) {
        int result = n % 16;
        
        if (result >= 0 && result <= 9) digits[i] = '0' + result;
        else digits[i] = 'A' + (result - 10);

        n /= 16;
    }

    digits[power] = '\0';

    return digits;
}



void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}