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

    // Inicializa el sistema de entrada y carga el primer bloque del archivo fuente
    inicializar_sistema_entrada(argv[1]);

    // Inicializa la tabla de símbolos
    init_ts();
    // Imprime la tabla de símbolos para verificar su estado inicial (opcional)
    imprimir_ts();

    printf("\n########################################\n");

    // Comienza el análisis sintáctico y léxico
    analizar();

    printf("\n########################################\n");

    // Imprime la tabla de símbolos para ver el estado final después del análisis
    imprimir_ts();
    // Libera recursos de la tabla de símbolos
    free_ts();

    // Cierra el archivo y limpia recursos del sistema de entrada
    cerrar_sistema_entrada();

    return 0;
}
