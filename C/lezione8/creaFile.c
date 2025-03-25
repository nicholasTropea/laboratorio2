#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

int main(int argc, char ** argv) {
    printf("Input a number and a string: ");

    int n;
    char * word;

    int e = scanf("%d %ms", &n, &word);

    if (e != 2) {
        puts("Input error.");
        return 1;
    }

    int wordLen = strlen(word);

    for (int i = 1; i <= n; i++) {
        char * fileName;

        // Fa una malloc automatica della lunghezza del print e assegna a fileName
        e = asprintf(&fileName, "%s.%d.txt", word, i);

        if (e == -1) {
            puts("Allocation Failed.");
            exti(1);
        }

        FILE * f = fopen(fileName, "wt");
        fprintf(f, "%d\n", i);
        fclose(f);
        free(fileName);
    }


    
    return 0;
}