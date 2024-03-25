#include "gestion_errores.h"
#include <stdio.h>
#include "sistema_entrada.h"

// Errores de Archivo
void errorArchivo(int codigo){
    switch(codigo){
        case 1: //Error de apertura
            printf("\033[31;1m [ERROR - No se pudo abrir el archivo] \033[0m");
            break;
        case 2: //Error de lectura
            printf("\033[31;1m [ERROR - No se pudo leer el archivo] \033[0m");
            break;
    }
}

// Errores de Lexema
void errorLexico(int codigo, char *lex) {
    switch (codigo) {
        case 1: //Lexema excede el tamaño del buffer
            printf("\033[31;1m [ERROR en línea %d - El lexema <...%s> excede el tamaño del buffer] \033[0m",getLinea(), lex);
            break;
        case 2: //Lexema mal formado
            /*Misformed lexema*/
            printf("\033[31;1m [ERROR en línea %d - Lexema mal formado] \033[0m", getLinea());
            break;
    }
}