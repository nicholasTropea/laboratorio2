#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char ** argv) {
    if (argc < 2) {
        puts("Utilizzo: concatena <stringa1> <stringa2> ... <stringaN>\n");
        return 1;
    }

    int len = 0;
    for (int i = 1; i < argc; i++) len += strlen(argv[i]);

    char * res = malloc((len + 1) * sizeof(char));

    if (res == NULL) {
        puts("Errore: Memoria Insufficiente.");
        exit(1);
    }

    int i = 0;

    for (int j = 1; j < argc; j++) {
        int k = 0;
        while (argv[j][k] != '\0') res[i++] = argv[j][k++];
    }

    res[len] = '\0';

    printf("Stringa concatenata: %s\n", res);

    free(res);
    
    return 0;
}