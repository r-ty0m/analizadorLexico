#include "analizador_lexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabla_simbolos.h"

void imprimirComponenteLexico(comp_lexico *lex, int mode);

void analizar() {

    // Inicialización de la estructura
    comp_lexico *lex = (comp_lexico *)malloc(sizeof(comp_lexico));
    if (!lex) {
        fprintf(stderr, "Error al asignar memoria para el componente léxico\n");
        exit(EXIT_FAILURE);
    }
    lex->tipo_componente = -9999;
    lex->lexema = NULL;


    int mode = 2; // Modo de impresión

    // Bucle del analizador
    while (1) {
        sig_comp_lexico(lex);

        if (lex->tipo_componente != -1 && lex->lexema != NULL) {
            imprimirComponenteLexico(lex, mode);
        } else {
            break;
        }

        if (lex->lexema) {
            free(lex->lexema);
            lex->lexema = NULL;
            lex->tipo_componente = -9999;
        }
    }

    free(lex);
    lex=NULL;
}

void imprimirComponenteLexico(comp_lexico *lex, int mode) {
    const char *color;
    switch (mode) {
        case 0:
            if(lex->tipo_componente==10){
                printf(" \033[0;32m");
                printf("<%d,\\n>\n", lex->tipo_componente);
                printf("\033[0;37m");
            }else{
                printf(" <%d,%s> \n", lex->tipo_componente, lex->lexema);
            }
            break;
        case 1:
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
        case 2:
            // Definir colores o formatos según el tipo de componente léxico
            if (lex->tipo_componente == IDENTIFICADOR) {
                color = "\033[0;33m"; // Amarillo
            } else if (lex->tipo_componente >= DELIM_CORCHETE_ABRIR && lex->tipo_componente <= DELIM_PUNTO_Y_COMA) {
                color = "\033[0;34m"; // Azul
            } else if (lex->tipo_componente == INTEGER) {
                color = "\033[0;31m"; // Rojo
            } else if (lex->tipo_componente == FLOAT) {
                color = "\033[0;35m"; // Violeta
            } else if (lex->tipo_componente == STRING) {
                color = "\033[0;36m"; // Cian (utilizado como sustituto de naranja)
            } else if (lex->tipo_componente == LF) {
                printf("\033[0;32m<%d,\\n>\033[0m\n", lex->tipo_componente); // Verde para saltos de línea
                return;
            } else {
                color = "\033[0m"; // Restablecer al color por defecto
            }
            printf("%s<%d,%s>\033[0m ", color, lex->tipo_componente, lex->lexema);
            break;
        default:
            printf("\033[0m<%d,%s> ", lex->tipo_componente, lex->lexema); // Restablecer al color por defecto para otros casos
            break;
    }
}
