#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void revString(const char*);

int main(int argc, char ** argv) {
    if (argc < 2) {
        puts("Utilizzo: nomeFile <string1> <string2> ... <stringN>\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) revString(argv[i]);
    
    return 0;
}

void revString(const char * s) {
    int len = strlen(s);

    for (int i = len - 1; i >= 0; i--) printf("%c", s[i]);
    printf("\n");
}