#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
    GLI ARRAY STATICI NON SOPRAVVIVONO ALLA FINE DELLA FUNZIONE

*/

// assert(5 == 7); 
// Assert (asserzione) usata per controlli di funzionamento del programma (logica del programma)

int str_lunghezza(const char*);
char * str_concatena(const char*, const char*);
void termina(char*);


int main(int argc, char ** argv) {
    if (argc < 3) {
        printf("Uso: %s stringa1, stringa2 <altre stringhe>\n", argv[0]);
        return 1;
    }

    int l1 = str_lunghezza(argv[1]);
    printf("Lunghezza <%s> = %d\n", argv[1], l1);

    char * concatenata = str_concatena(argv[1], argv[2]);
    printf("Concatenazione di <%s> e <%s> = %s\n", argv[1], argv[2], concatenata);

    free(concatenata);

    return 0;
}

int str_lunghezza(const char * s) {
    assert(s != NULL);

    int i = 0;

    while (s[i] != '\0') i++;

    return i;
}

char * str_concatena(const char * s1, const char * s2) {
    assert(s1 != NULL);
    assert(s2 != NULL);

    int l1 = strlen(s1), l2 = strlen(s2);

    char * res = malloc((l1 + l2 + 1) * sizeof(char));
    if (res == NULL) termina("Errore: Memoria Insufficiente.");

    for (int i = 0; i < l1; i++) res[i] = s1[i];
    for (int i = 0; i < l2; i++) res[l1 + i] = s2[i];

    res[l1 + l2] = '\0';

    return res;
}

void termina(char * message) {
    puts(message);
    exit(1);
}