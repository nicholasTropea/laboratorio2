#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


typedef enum {
    CARDIOVASCOLARE,
    DIABETE
} malattie;


typedef struct {
    int id;
    malattie tipo;
    
    union {
        struct {
            float glicemia, peso;
        } caseD;

        struct {
            int pressione, freq;
        } caseC;
    } vars;
} paziente;


void termina(const char*);


int main(int argc, char ** argv) {
    int n;

    printf("Inserire il numero di pazienti: ");
    int e = scanf("%d", &n);
    if (e != 1) termina("Errore: Numero di pazienti non valido.");

    paziente * pazArr = malloc(n * sizeof(paziente));
    if (pazArr == NULL) termina("Errore: Memoria Insufficiente.");

    for (int i = 0; i < n; i++) {
        printf(
            "Inserire i dati del %d paziente nel seguente modo:\n"
            "<id> <tipoMalattia>\n"
            "<var1> <var2>\n\n"
            "In caso di malattia cardiovascolare, <var1> = pressione, <var2> = frequenza cardiaca\n"
            "In caso di diabete, <var1> = glicemia, <var2> = peso\n"
            , i + 1
        );

        e = scanf("%d %d", &pazArr[i].id, &pazArr[i].tipo);
        if (e != 2) termina("Errore: Input del paziente non valido.");

        switch(pazArr[i].tipo) {
            case CARDIOVASCOLARE:
                e = scanf("%d %d", &pazArr[i].vars.caseC.pressione, &pazArr[i].vars.caseC.freq);
                if (e != 2) termina("Errore: Input del paziente non valido.");
                break;
            default:
                e = scanf("%f %f", &pazArr[i].vars.caseD.glicemia, &pazArr[i].vars.caseD.peso);
                if (e != 2) termina("Errore: Input del paziente non valido.");
        }
    }

    for (int i = 0; i < n; i++) {
        if (pazArr[i].tipo == pazArr[n].tipo) {
            printf("ID: %d ", pazArr[i].id);

            switch (pazArr[i].tipo) {
            case CARDIOVASCOLARE:
                printf(
                    "Pressione: %d "
                    "Frequenza cardiaca: %d"
                    , pazArr[i].vars.caseC.pressione, pazArr[i].vars.caseC.freq
                );

                break;
            
            default:
                printf(
                    "Glicemia: %f "
                    "Peso: %f"
                    , pazArr[i].vars.caseD.glicemia, pazArr[i].vars.caseD.peso
                );

                break;
            }
        }

        printf("\n\n");
    }
    
    return 0;
}


void termina(const char * messaggio) {
    if (errno != 0) perror(messaggio);
    else fprintf(stderr, "%s\n", messaggio);

    exit(1);
}