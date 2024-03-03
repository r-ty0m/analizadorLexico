#include <stdio.h>
#include <stdlib.h>
#include "analizador_sintactico.h"
#include "tabla_simbolos.h"
#include "sistema_entrada.h" // Incluye el sistema de entrada

// Incluye otros archivos necesarios

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("\n FORMATO INCORRECTO: Usa %s [Código Fuente]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Suponiendo la existencia de abrir_archivo() en sistema_entrada
    //abrir_archivo(argv[1]); // Prepara el sistema de entrada para usar el archivo

    init_ts(); // Inicializa la tabla de símbolos
    imprimir_ts(); // Imprime la tabla de símbolos

    printf("\n ######################################## \n");

    analizar(); // Comienza el análisis sintáctico sin parámetros

    //imprimir_ts();
    //free_ts(); // Libera recursos de la tabla de símbolos

    // Suponiendo la existencia de cerrar_archivo() en sistema_entrada
    //cerrar_archivo(); // Cierra el archivo y limpia recursos del sistema de entrada

    return 0;
}
