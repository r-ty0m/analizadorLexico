#include "gestion_errores.h"
#include <stdio.h>

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

//Lexeme erros
void errorLexico(int codigo, char *lex) {
    switch (codigo) {
        case 1: //Lexema excede el tamaño del buffer
            printf("\033[31;1m [ERROR - El lexema excede el tamaño del buffer] \033[0m");
            break;
        case 2: //Lexema mal formado
            /*Misformed lexema*/
            printf("\033[31;1m [ERROR - Lexema mal formado: [%s] ] \033[0m", lex);
            break;
    }
}