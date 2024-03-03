#include "analizador_lexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void analizar() {

    comp_lexico *lex = (comp_lexico *)malloc(sizeof(comp_lexico));
    if (lex == NULL) {
        fprintf(stderr, "Error al asignar memoria para el lex\n");
        exit(EXIT_FAILURE);
    }
    lex->tipo_componente = -1;
    lex->lexema = NULL;

    while (1) {

        sig_comp_lexico(lex);

        if (lex->tipo_componente != EOF) {

            printf("\ntamaño lex->lexema: [%d]\n", (strlen(lex->lexema)));
            printf("<%d, %s>\n", lex->tipo_componente, lex->lexema);
        } else break;

        if (lex->lexema != NULL) {
            free(lex->lexema);
        }
    }

    printf("\nFinalizado análisis");

    free(lex);
    lex=NULL;
}
