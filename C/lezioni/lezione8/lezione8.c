#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

int main(int argc, char ** argv) {
    /*
        Per prendere in input una stringa ci sono 2 modi:
            1. Allocando un tot di memoria prima e prendendo la stringa con scanf(%s, string)
            2. Lasciando scanf allocare la memoria automaticamente per la stringa che arriva in input
                scanf(%ms, &string) bisogna usare il puntatore alla stringa

    */

    printf("Input your name: ");

    char * name;
    int e = scanf("%ms", &name);

    if (e < 1) {
        puts("Input error.");
        return 1;
    }

    printf("Your name is %s.\n", name);

    free(name);

    return 0;
}
