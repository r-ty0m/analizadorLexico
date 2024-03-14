#include "analizador_lexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_simbolos.h"


void analizar() {

    comp_lexico *lex = (comp_lexico *)malloc(sizeof(comp_lexico));
    if (lex == NULL) {
        fprintf(stderr, "Error al asignar memoria para el lex\n");
        exit(EXIT_FAILURE);
    }
    lex->tipo_componente = -1;
    lex->lexema = NULL;


    int mode = 1;
    while (1) {

        lex->tipo_componente = random();
        lex->lexema = (char *) random();
        sig_comp_lexico(lex);

        if (lex->tipo_componente != -1 && lex->lexema!=NULL) {

            if(mode==0){
                if(lex->tipo_componente==10){
                    printf(" \033[0;32m");
                    printf("<%d,\\n>\n", lex->tipo_componente);
                    printf("\033[0;37m");
                }else{
                    printf(" <%d,%s> ", lex->tipo_componente, lex->lexema);
                }
            }else if (mode == 1){
                /*
                if(lex->tipo_componente==10){
                    printf(" \033[0;32m");
                    printf("<%d,\\n>\n", lex->tipo_componente);
                    printf("\033[0;37m");
                }else{
                    printf(" %s ", lex->lexema);
                }
                 */
                if (mode == 1) {
                    // Utiliza secuencias de escape ANSI para los colores
                    if (lex->tipo_componente == IDENTIFICADOR) {
                        // Amarillo para identificadores
                        printf("\033[0;33m%s\033[0m ", lex->lexema);
                    } else if (lex->tipo_componente >= DELIM_CORCHETE_ABRIR && lex->tipo_componente <= DELIM_PUNTO_Y_COMA) {
                        // Azul para delimitadores
                        printf("\033[0;34m%s\033[0m ", lex->lexema);
                    } else if (lex->tipo_componente == INTEGER) {
                        // Rojo clarito para integers
                        printf("\033[1;31m%s\033[0m ", lex->lexema);
                    } else if (lex->tipo_componente == FLOAT) {
                        // Violeta clarito para floats
                        printf("\033[1;35m%s\033[0m ", lex->lexema);
                    } else if (lex->tipo_componente == STRING) {
                        // Naranja para strings
                        printf("\033[0;36m%s\033[0m ", lex->lexema); // Nota: ANSI no tiene un "naranja" directo, así que ajusta según prefieras
                    } else if (lex->tipo_componente == LF) {
                        // Verde para saltos de línea
                        printf("\033[0;32m<%d,\\n>\n\033[0m", lex->tipo_componente);
                    } else {
                        // Blanco (default) para otros casos
                        printf("\033[0;34m%s ", lex->lexema);
                    }
                }

            }


        } else break;

        if (lex->lexema != NULL) {
            free(lex->lexema);
            lex->lexema=NULL;
        }

    }

    printf("\nFinalizado análisis");

    free(lex);
    lex=NULL;
}
