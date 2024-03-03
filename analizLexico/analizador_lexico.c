#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "analizador_lexico.h"
#include "tabla_simbolos.h"

static char caracter_devuelto = '\0'; // Inicializado a '\0' para indicar que no hay carácter devuelto.

char siguiente_caracter() {
    static const char *entrada = "import scipy.stats as st *";
    static int posicion = 0;

    // Verificar si hay un carácter previamente devuelto.
    if (caracter_devuelto != '\0') {
        char c = caracter_devuelto;
        caracter_devuelto = '\0'; // Resetear para indicar que no hay carácter devuelto.
        return c;
    }

    char c = entrada[posicion];

    // Avanzar la posición si no hemos llegado al final del string.
    if (c != '\0') {
        posicion++;
    }

    return c;
}

void devolver_caracter(char c) {
    // Almacenar el carácter para ser retornado en la próxima llamada a siguiente_caracter.
    caracter_devuelto = c;
}



void sig_comp_lexico(comp_lexico *lex) {
    static char buffer[1024];
    int buf_len = 0;
    char c;
    int estado = 0; // Estado inicial del autómata.

    memset(buffer, 0, sizeof(buffer)); // Limpiamos el buffer.

    while (1) {
        c = siguiente_caracter();
        //printf("\nSiguiente carácter: %c", c);

        switch (estado) {
            case 0: // Estado inicial, buscando el inicio de un identificador.
                if (isalpha(c)) {
                    buffer[buf_len++] = c;
                    estado = 1; // Cambiamos al estado de recolección de identificador.
                } else if (c == 42 ){ // Si es *
                    lex->tipo_componente = EOF;
                    return;
                } else if (c == 46) { // Si es .
                    buffer[buf_len++] = c;
                    buffer[buf_len++] = '\0'; // Aseguramos que el string esté terminado correctamente.
                    lex->lexema = strdup(buffer);
                    lex->tipo_componente = buscar(lex->lexema);
                    estado=0;
                    return; // Salimos de la función.
                }
                break;

            case 1: // Estado de recolección de identificador.
                if (isalnum(c) || c == '_' || c == '-') {
                    buffer[buf_len++] = c;
                    //printf("\nbuffer: %s", buffer);
                } else {
                    // Final del identificador, devolvemos el último carácter al stream si no es EOF.
                    if (c != EOF) devolver_caracter(c);

                    buffer[buf_len] = '\0'; // Aseguramos que el string esté terminado correctamente.
                    lex->lexema = strdup(buffer);
                    // Aquí deberías buscar el lexema en la tabla de símbolos para asignar lex->tipo_componente.



                    lex->tipo_componente = buscar(lex->lexema);

                    // Por ahora, asumimos que es un identificador genérico.
                    //lex->tipo_componente = IDENTIFICADOR;
                    estado=0;
                    return; // Salimos de la función.
                }
                break;
        }
    }
}

