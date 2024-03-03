#include "tabla_simbolos.h"
#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definir la estructura de la tabla de símbolos como un árbol binario
TABB tablaSimbolos = NULL;

void init_ts() {
    // Asegura que la tabla de símbolos esté vacía al inicio.
    if (tablaSimbolos != NULL) {
        destruirAbb(&tablaSimbolos);
    }
    crearAbb(&tablaSimbolos);

    insertar_ts("import", IMPORT); // Suponiendo que IMPORT, AS, etc., son constantes definidas.
    insertar_ts("as", AS);
}





void imprimir_ts() {
    // Esta función debería recorrer el árbol y imprimir los elementos.
    // La implementación específica dependerá de cómo quieras visualizar el árbol.
}



int hash_lexema(char* lexema) {
    int hash = 0;
    int length = strlen(lexema);

    // Limita la longitud del lexema para evitar desbordamiento de int.
    int max_length = 6; // Ajustado para evitar desbordamiento.
    if (length > max_length) {
        length = max_length;
    }

    for (int i = 0; i < length; ++i) {
        hash = hash * 1000 + (unsigned char)lexema[i]; // Concatena los valores ASCII.
    }

    return hash;
}

void revertir_hash_lexema(int hash, char* lexema, int length) {
    for (int i = length - 1; i >= 0; --i) {
        lexema[i] = (char)(hash % 1000); // Extrae el último componente de tres cifras.
        hash /= 1000; // Descarta el último componente de tres cifras.
    }
    lexema[length] = '\0'; // Asegura que la cadena esté correctamente terminada.
}



void insertar_ts(char* lexema, int tipo_componente) {
    int lexema_hash = hash_lexema(lexema); // Convierte el lexema a un entero usando la función hash.

    TIPOELEMENTOABB nuevoElemento;
    nuevoElemento.lexema = lexema_hash; // Usa el valor hash como clave.
    nuevoElemento.tipo_componente = tipo_componente;

    insertarElementoAbb(&tablaSimbolos, nuevoElemento);
}




int buscar_ts (char* lexema) {
    int lexema_hash = hash_lexema(lexema); // Convierte el lexema a un entero usando la función hash.

    TIPOELEMENTOABB nodoBuscado;
    buscarNodoAbb(tablaSimbolos, lexema_hash, &nodoBuscado);

    if (nodoBuscado.lexema == lexema_hash) {
        return nodoBuscado.tipo_componente;
    } else {
        return -1; // Indica que el lexema no se encontró.
    }
}





void free_ts() {
    destruirAbb(&tablaSimbolos);
}
