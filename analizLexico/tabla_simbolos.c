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
    insertar_ts("for", FOR); // Suponiendo que IMPORT, AS, etc., son constantes definidas.
    insertar_ts("if", IF);
    insertar_ts("while", WHILE); // Suponiendo que IMPORT, AS, etc., son constantes definidas.
    insertar_ts("def", DEF);
    insertar_ts("in", IN);
    insertar_ts("elif", ELIF);
    insertar_ts("else", ELSE);
    insertar_ts("return", RETURN);
    insertar_ts("not", NOT);





}



// Definición de la función auxiliar para recorrido inorden
void _imprimirInorden(TABB nodo) {
    if (nodo == NULL) {
        return; // Caso base: si el nodo es NULL, no hay nada que imprimir.
    }

    TIPOELEMENTOABB elemento;

    // Visita el subárbol izquierdo
    _imprimirInorden(izqAbb(nodo));

    // Procesa el nodo actual
    leerElementoAbb(nodo, &elemento); // Obtiene la información del nodo actual
    printf("Lexema: %s, Tipo Componente: %d\n", elemento.lexema, elemento.tipo_componente);

    // Visita el subárbol derecho
    _imprimirInorden(derAbb(nodo));
}

// Implementación de imprimir_ts usando la función auxiliar
void imprimir_ts() {
    printf("Contenido de la Tabla de Símbolos:\n");
    _imprimirInorden(tablaSimbolos); // Inicia el recorrido desde la raíz del árbol
}



void insertar_ts(char* lexema, int tipo_componente) {

    TIPOELEMENTOABB nuevoElemento;
    nuevoElemento.lexema = strdup(lexema); // Duplica el lexema
    nuevoElemento.tipo_componente = tipo_componente;

    insertarElementoAbb(&tablaSimbolos, nuevoElemento);
}




int buscar_ts (char* lexema) {

    TIPOELEMENTOABB nodoBuscado;
    nodoBuscado.lexema = NULL; // Asegúrate de inicializar
    nodoBuscado.tipo_componente; // Asegúrate de inicializar

    buscarNodoAbb(tablaSimbolos, lexema, &nodoBuscado);

    if(nodoBuscado.lexema!=NULL && lexema!=NULL){
        if (strcmp(nodoBuscado.lexema, lexema) == 0) {
            return nodoBuscado.tipo_componente;
        }
    } else { //Si no se encuentra, se añade
        //printf("No se encuentra el lexema [%s], añadiendo...", lexema);
        insertar_ts(lexema, IDENTIFICADOR); // Suponiendo que IMPORT, AS, etc., son constantes definidas.
        return IDENTIFICADOR; // Indica que el lexema no se encontró.
    }
    return 0;
}





void free_ts() {
    destruirAbb(&tablaSimbolos);
}
