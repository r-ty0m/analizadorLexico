#include <stdio.h>
#include <stdlib.h>
#include "analizador_sintactico.h"
#include "tabla_simbolos.h"
#include "sistema_entrada.h"

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("\nFORMATO INCORRECTO: Usa %s [Código Fuente]\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    inicializar_sistema_entrada(argv[1]); // ---  Inicializa el sistema de entrada y carga el primer bloque del archivo fuente
    init_ts(); // -------------------------------------------  Inicializa la tabla de símbolos
    imprimir_ts(); // ---------------------------------------  Imprime la tabla de símbolos para verificar su estado inicial (opcional)

    printf("\n\n################### ANÁLISIS #####################\n\n");

    analizar(); // ------------------------------------------  Comienza el análisis sintáctico y léxico

    printf("\n\n################### FIN DEL ANÁLISIS #####################\n\n");

    imprimir_ts(); // ---------------------------------------  Imprime la tabla de símbolos para ver el estado final después del análisis
    free_ts(); // -------------------------------------------  Libera recursos de la tabla de símbolos
    cerrar_sistema_entrada(); // ----------------------------  Cierra el archivo y limpia recursos del sistema de entrada


    return 0;
}
