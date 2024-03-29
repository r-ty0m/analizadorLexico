#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "analizador_lexico.h"
#include "tabla_simbolos.h"
#include "sistema_entrada.h"
#include "gestion_errores.h"

//----------------------------DEFINICIONES DE ESTADOS, VARIABLES Y FUNCIONES--------------------------------------------

// ESTADOS PRIMARIOS
#define ESTADO_INICIAL 0
#define ESTADO_LINEA_LOGICA 1
#define ESTADO_COMENTARIO_MONOLINEA 2
#define ESTADO_STRING_O_COMENTARIO_MULTILINEA 9
//#define ESTADO_DECLARACION_CODIFICACION 3
#define ESTADO_IDENTIFICADOR 4
#define ESTADO_NUMERO 5
#define ESTADO_OPERADOR_DELIMITADOR 6
//#define ESTADO_TOKEN_ESPECIAL 7
#define ESTADO_PUNTO 8

//ESTADOS DE SUBAUTOMATAS
//NUMEROS
#define ESTADO_NUMERO_ENTERO 10
#define ESTADO_NUMERO_FLOAT 11
#define ESTADO_NUMERO_ELEVADO 12
#define ESTADO_NUMERO_HEX 13
#define ESTADO_NUMERO_ELEVADO_POSITIVO 14
#define ESTADO_NUMERO_ELEVADO_NEGATIVO 15

//COMENTARIOS Y STRINGS
#define ESTADO_COMENTARIO_MULTILINEA 16
#define ESTADO_STRING 17

// VARIABLES GLOBALES
int estado = ESTADO_INICIAL;
int aceptado = 0;
int saltoLinea = 0; // El último caracter lanzado fue un /n

// SUBAUTÓMATAS INICIALES
void subautomata_linea_logica(comp_lexico *lex, char c);
void subautomata_comentario();
//void subautomata_declaracion_codificacion(comp_lexico *lex, char c);
void subautomata_identificador(comp_lexico *lex, char c);
void subautomata_numeral(comp_lexico *lex, char c);
void subautomata_operador_delimitador(comp_lexico *lex, char c);
//void subautomata_token_especial();
void subautomata_punto(comp_lexico *lex,char c);
void subautomata_string_o_comment(comp_lexico *lex, char c);

// SUBAUTÓMATAS INTERMEDIOS
//NUMEROS
void subautomata_numeral_entero(comp_lexico *lex, char c);
void subautomata_numeral_hex(comp_lexico *lex, char c);
void subautomata_numeral_elevado(comp_lexico *lex, char c);
void subautomata_numeral_float(comp_lexico *lex, char c);
void subautomata_numeral_elevado_positivo(comp_lexico *lex, char c);
void subautomata_numeral_elevado_negativo(comp_lexico *lex, char c);

//COMENTARIOS Y STRINGS
void subautomata_string(comp_lexico *lex, char c);
void subautomata_comentario_multilinea(comp_lexico *lex, char c);



//--------------------------------------------Funciones auxiliares -----------------------------------------------------

void aceptar(){
    aceptado = 1;
    estado = ESTADO_INICIAL;
    if(saltoLinea) saltoLinea=0;
}

void aceptar_asignar_desde_ts(comp_lexico *lex){
    lex->lexema = devolver_lexema();
    lex->tipo_componente = buscar_ts(lex->lexema);
    aceptar();
}
void aceptar_asignar_lexema(comp_lexico *lex, int tipo) {
    lex->lexema = devolver_lexema();
    lex->tipo_componente = tipo;
    aceptar();
}



//-------------------------------------------- Sig Comp Lex ------------------------------------------------------------

/* El funcionamiento de la función sig_comp_lexico() es la siguiente:
 *
 * Se solicita un nuevo caracter al sistema de entrada en el estado inicial.
 *
 * Dependiendo del caracter, se cambiará al estado correspondiente. En este nuevo estado se inicia el subautómata
 * adecuado. Dicho subautómata podrá solicitar nuevos caracteres al sistema de entrada, y podrá hacer cambios de estado.
 *
 * Cada vez que se haga un cambio de estado se volverá a ejecutar el switch(estado) de dentro del while de la función
 * sig_comp_léxico(). El subautómata correspondiente se iniciará en su estado. De esta manera no se harán saltos
 * inrastreables entre subautómatas.
*/

void sig_comp_lexico(comp_lexico *lex) {
    char c;
    aceptado=0;
    estado = ESTADO_INICIAL;

    while(aceptado!=1) {
        switch(estado) {

            case ESTADO_INICIAL:
                c = siguiente_caracter();
                if (c == '\n') {
                    estado = ESTADO_LINEA_LOGICA;
                } else if (isalpha(c) || c == '_') {
                    estado = ESTADO_IDENTIFICADOR;
                } else if (isdigit(c)) {
                    estado = ESTADO_NUMERO;
                } else if (c == '"'){
                    estado = ESTADO_STRING_O_COMENTARIO_MULTILINEA;
                }else if (c == 39){ // si c es '
                    estado = ESTADO_STRING;
                }else if (c == '#') {
                    estado = ESTADO_COMENTARIO_MONOLINEA;
                    avanzar_inicio();
                } else if (strchr("+-*/=<>:{([])},", c) != NULL) { // Si c está en el string "+-*/=<>:{([])},"
                    estado = ESTADO_OPERADOR_DELIMITADOR;
                } else if (c == ' ' || c == '\t') {
                    avanzar_inicio();
                }else if (c == '.') {
                    estado = ESTADO_PUNTO;
                }else if(c == EOF) {
                    lex->tipo_componente = EOF;
                    aceptado=1;
                }else{
                    errorLexico(2,""); // Lexema mal formado. No se inicia ningún autómata.
                    avanzar_inicio();
                }
                break;


            //ESTADOS SUBAUTÓMATAS INICIALES
            case ESTADO_IDENTIFICADOR:
                subautomata_identificador(lex, c);
                break;
            case ESTADO_OPERADOR_DELIMITADOR:
                subautomata_operador_delimitador(lex, c);
                break;
            case ESTADO_PUNTO:
                subautomata_punto(lex,c);
                break;
            case ESTADO_LINEA_LOGICA:
                subautomata_linea_logica(lex, c);
                break;
            case ESTADO_COMENTARIO_MONOLINEA:
                subautomata_comentario();
                break;
            case ESTADO_NUMERO:
                subautomata_numeral(lex, c);
                break;
            case ESTADO_STRING_O_COMENTARIO_MULTILINEA:
                subautomata_string_o_comment(lex, c);
                break;

            //ESTADOS SUBAUTÓMATAS INTERMEDIOS
            //NUMEROS
            case ESTADO_NUMERO_ENTERO:
                subautomata_numeral_entero(lex, c);
                break;
            case ESTADO_NUMERO_HEX:
                subautomata_numeral_hex(lex,c);
                break;
            case ESTADO_NUMERO_FLOAT:
                subautomata_numeral_float(lex, c);
                break;
            case ESTADO_NUMERO_ELEVADO:
                subautomata_numeral_elevado(lex,c);
                break;
            case ESTADO_NUMERO_ELEVADO_POSITIVO:
                subautomata_numeral_elevado_positivo(lex, c);
                break;
            case ESTADO_NUMERO_ELEVADO_NEGATIVO:
                subautomata_numeral_elevado_negativo(lex,c);
                break;

            //COMENTARIOS Y STRINGS
            case ESTADO_COMENTARIO_MULTILINEA:
                subautomata_comentario_multilinea(lex,c);
                break;
            case ESTADO_STRING:
                subautomata_string(lex,c);
                break;
        }
    }
}

//----------------------------------- Implementación Subautómatas Iniciales --------------------------------------------

void subautomata_punto(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (isdigit(c)) { // Es un número, posiblemente un literal flotante
        estado = ESTADO_NUMERO_FLOAT;
    } else { // No es un número, aceptamos el punto como token por sí mismo.
        devolver_caracter(); // Devolvemos el carácter para que sea analizado en el siguiente lexema
        aceptar_asignar_lexema(lex, OP_PUNTO);
    }
}

void subautomata_linea_logica(comp_lexico *lex, char c){ // \n
    if(!saltoLinea){ // Si el último lexema no ha sido un /n, se devolverá el lexema
        aceptar_asignar_lexema(lex, LF);
        saltoLinea=1;
    }else{ // Si ya se ha devuelto un /n, se descarta el lexema.
        descartar_lexema();
        estado = ESTADO_INICIAL;
    }
}

void subautomata_comentario() { //se consumirán caracteres hasta encontrar un /n o un EOF
    char c;
    do {
        c = siguiente_caracter();
        avanzar_inicio(); // Al avanzar el inicio se "elimina" ese caracter del lexema en el sistema de entrada
                          // Esto se puede hacer también con la función descartar_lexema(), pero así tiene más sentido.
    } while(c != '\n' && c != EOF);
    if(c == '\n'){
        estado = ESTADO_LINEA_LOGICA;
    }else{
        devolver_caracter();
        estado = ESTADO_INICIAL;
    }

}

void subautomata_identificador(comp_lexico *lex, char c) {
    c = siguiente_caracter();
    if (!isalnum(c) && c != '_') {
        devolver_caracter();
        aceptar_asignar_desde_ts(lex); // Se devuelve el lexema empleando la tabla de símbolos
    }
}

void subautomata_numeral(comp_lexico *lex, char c) {
    c = siguiente_caracter();
    if (isdigit(c)) { // Redireccionamiento a otros estados
        estado = ESTADO_NUMERO_ENTERO;
    }else if (c == '.'){
        estado = ESTADO_NUMERO_FLOAT;
    }else if (c == 'e'){
        estado = ESTADO_NUMERO_ELEVADO;
    }else if (c == 'x'){
        estado = ESTADO_NUMERO_HEX;
    }else{
        devolver_caracter();
        aceptar_asignar_lexema(lex, INTEGER); // Si el siguiente caracter no lleva a otro estado es entero
    }
}

void subautomata_operador_delimitador(comp_lexico *lex, char c) {
    char siguiente;

    switch (c) {
        case '+':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_SUMA_ASIGNACION);
            } else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_SUMA);
            }
            break;
        case '-':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_RESTA_ASIGNACION);
            } else if (siguiente == '>') {
                aceptar_asignar_lexema(lex, OP_FLECHA);
            } else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_RESTA);
            }
            break;
        case '*':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_MULTI_ASIGNACION);
            } else if (siguiente == '*') {
                aceptar_asignar_lexema(lex, OP_POTENCIA);
            } else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_MULTIPLICACION);
            }
            break;
        case '/':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_DIV_ASIGNACION);
            } else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_DIVISION);
            }
            break;
        case '=':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_EQUAL_EQUAL);
            }else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_EQUAL);
            }
            break;
        case '<':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_MENOR_EQUAL);
            } else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_MENOR);
            }
            break;
        case '>':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_MAYOR_EQUAL);
            } else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_MAYOR);
            }
            break;
        case ':':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_DOS_PUNTOS_EQUAL);
            }else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_DOS_PUNTOS);
            }
            break;
        case '{':
            aceptar_asignar_lexema(lex, DELIM_LLAVE_ABRIR);
            break;
        case '}':
            aceptar_asignar_lexema(lex, DELIM_LLAVE_CERRAR);
            break;
        case '(':
            aceptar_asignar_lexema(lex, DELIM_PARENTESIS_ABRIR);
            break;
        case ')':
            aceptar_asignar_lexema(lex, DELIM_PARENTESIS_CERRAR);
            break;
        case '[':
            aceptar_asignar_lexema(lex, DELIM_CORCHETE_ABRIR);
            break;
        case ']':
            aceptar_asignar_lexema(lex, DELIM_CORCHETE_CERRAR);
            break;
        case ',':
            aceptar_asignar_lexema(lex, DELIM_COMA);
            break;
        case ';':
            aceptar_asignar_lexema(lex, DELIM_PUNTO_Y_COMA);
            break;


    }
}

void subautomata_string_o_comment(comp_lexico *lex, char c){

    c = siguiente_caracter();
    if (c == '"') {
        //Ya se han detectado dos "
        c = siguiente_caracter();
        if (c == '"') {
            //Se detectan tres "
            estado = ESTADO_COMENTARIO_MULTILINEA;
            descartar_lexema(); // Se descarta el inicio del comentario ["""] , pues no se devolverá como lexema

        } else { // Es un string vacío
            devolver_caracter();
            aceptar_asignar_lexema(lex, STRING);
        }

    } else {
        // Es un string regular o el inicio de un string.
        estado = ESTADO_STRING;
    }


}

//--------------------------------- Implementación Subautómatas Intermedios --------------------------------------------


//SUBAUTÓMATAS NUMERALES
void subautomata_numeral_entero(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (c == 'e'){
        estado = ESTADO_NUMERO_ELEVADO;
    }else if (!isdigit(c)) {
        devolver_caracter();
        aceptar_asignar_lexema(lex, INTEGER);
    }
}

void subautomata_numeral_hex(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (!isdigit(c) && (c != 'a') && (c != 'b') && (c != 'c') && (c != 'd') && (c != 'e') && (c != 'f')) {
        devolver_caracter();
        aceptar_asignar_lexema(lex, INTEGER);
    }
}

void subautomata_numeral_float(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (!isdigit(c)) {
        devolver_caracter();
        aceptar_asignar_lexema(lex, FLOAT);
    }
}

void subautomata_numeral_elevado(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (isdigit(c)) {
        estado = ESTADO_NUMERO_ELEVADO_POSITIVO;
    }else if (c == '-'){
        estado = ESTADO_NUMERO_ELEVADO_NEGATIVO;
    }else{
        errorLexico(2,"");
    }
}

void subautomata_numeral_elevado_positivo(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (!isdigit(c)) {
        devolver_caracter();
        aceptar_asignar_lexema(lex, FLOAT);
    }
}

void subautomata_numeral_elevado_negativo(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (!isdigit(c)) {
        devolver_caracter();
        aceptar_asignar_lexema(lex, FLOAT);
    }
}

//SUBAUTÓMATAS COMENTARIOS
void subautomata_string(comp_lexico *lex, char c){
    if(c == 39){ // String entre comillas simples ['string']
        c = siguiente_caracter();
        if (c == 39) {
            aceptar_asignar_lexema(lex, STRING);
        }
    }else{ // String entre comillas dobles ["string"]
        c = siguiente_caracter();
        if (c == '"') {
            aceptar_asignar_lexema(lex, STRING);
        }
    }

}

void subautomata_comentario_multilinea(comp_lexico *lex, char c){

    // Se consumen caracteres hasta que se encuentren tres " seguidas o un EOF

    c = siguiente_caracter();
    avanzar_inicio();
    if (c == '"') {
        //Se detecta un "
        c = siguiente_caracter();
        avanzar_inicio();
        if (c == '"') {
            //Se detectan dos "
            c = siguiente_caracter();
            avanzar_inicio();
            if (c == '"') {
                //Se detectan tres "
                estado = ESTADO_INICIAL;
            }else if (c == EOF){
                devolver_caracter();
                estado = ESTADO_INICIAL;
            }
        }else if (c == EOF){
            devolver_caracter();
            estado = ESTADO_INICIAL;
        }
    }else if (c == EOF){
        devolver_caracter();
        estado = ESTADO_INICIAL;
    }
}
