#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "analizador_lexico.h"
#include "tabla_simbolos.h"
#include "sistema_entrada.h"

// Definiciones de los estados
#define ESTADO_INICIAL 0
#define ESTADO_LINEA_LOGICA 1
#define ESTADO_COMENTARIO 2
#define ESTADO_DECLARACION_CODIFICACION 3
#define ESTADO_IDENTIFICADOR 4
#define ESTADO_NUMERO 5
#define ESTADO_OPERADOR_DELIMITADOR 6
#define ESTADO_TOKEN_ESPECIAL 7
#define ESTADO_PUNTO 8

//ESTADOS DE SUBAUTOMATAS

#define ESTADO_NUMERO_ENTERO 10
#define ESTADO_NUMERO_FLOAT 11
#define ESTADO_NUMERO_ELEVADO 12
#define ESTADO_NUMERO_HEX 13

// Variables globales
int estado = ESTADO_INICIAL;
int aceptado = 0;

// Subautómatas iniciales
void subautomata_linea_logica();
void subautomata_comentario();
void subautomata_declaracion_codificacion(comp_lexico *lex, char c);
void subautomata_identificador(comp_lexico *lex, char c);
void subautomata_numeral(comp_lexico *lex, char c);
void subautomata_operador_delimitador(comp_lexico *lex, char c);
void subautomata_token_especial();
void subautomata_punto(comp_lexico *lex,char c);

//Subautómatas intermedios
void subautomata_numeral_entero(comp_lexico *lex, char c);
void subautomata_numeral_hex(comp_lexico *lex, char c);
void subautomata_numeral_elevado(comp_lexico *lex, char c);
void subautomata_numeral_float(comp_lexico *lex, char c);



//Funciones auxiliares
void aceptar(){
    aceptado = 1;
    estado = ESTADO_INICIAL;
}
void aceptar_asignar_desde_ts(comp_lexico *lex){
    lex->lexema = strdup(devolver_lexema());
    lex->tipo_componente = buscar_ts(lex->lexema);
    aceptar();
}
void aceptar_asignar_lexema(comp_lexico *lex, int tipo) {
    lex->lexema = strdup(devolver_lexema());
    lex->tipo_componente = tipo;
    aceptar();
}

void sig_comp_lexico(comp_lexico *lex) {
    char c;
    aceptado=0;
    estado = ESTADO_INICIAL;

    while(aceptado!=1) {
        //c = siguiente_caracter();
        switch(estado) {
            case ESTADO_INICIAL:
                c = siguiente_caracter();
                if (c == '\n' || c == '\\') {
                    estado = ESTADO_LINEA_LOGICA;
                    avanzar_inicio();
                } else if (c == '#') {
                    estado = ESTADO_COMENTARIO;
                    avanzar_inicio();
                } else if (isalpha(c) || c == '_') {
                    estado = ESTADO_IDENTIFICADOR;
                } else if (isdigit(c)) {
                    estado = ESTADO_NUMERO;
                    //avanzar_inicio();
                } else if (strchr("+-*/=<>:{([])}", c) != NULL) { //TODO: Cambiar esto por algo más eficiente
                    estado = ESTADO_OPERADOR_DELIMITADOR;
                    //avanzar_inicio();
                } else if (c == ' ' || c == '\t') {
                    //estado = ESTADO_TOKEN_ESPECIAL;
                    avanzar_inicio();
                }else if (c == '.') {
                    estado = ESTADO_PUNTO;
                    //avanzar_inicio();
                }else{
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
                subautomata_linea_logica();
                break;
            case ESTADO_COMENTARIO:
                subautomata_comentario();
                break;
            case ESTADO_NUMERO:
                subautomata_numeral(lex, c);
                break;
            case ESTADO_TOKEN_ESPECIAL:
                subautomata_token_especial();
                break;

            //ESTADOS SUBAUTÓMATAS INTERMEDIOS
            case ESTADO_NUMERO_ENTERO:
                subautomata_numeral_entero(lex, c);
                break;
            case ESTADO_NUMERO_HEX:
                subautomata_numeral_hex(lex,c);
                break;
        }
    }
}

// #####################################################
//        IMPLEMENTACIÓN SUBAUTÓMATAS INICIALES
// #####################################################

void subautomata_punto(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (isdigit(c)) { // Es un número, posiblemente un literal flotante
        // Aquí asumimos que devolvemos el '.' al flujo para que el subautomata_float
        // pueda procesar el número completo correctamente, incluyendo el punto inicial.
        //devolver_caracter(); // Devolvemos el punto
        //devolver_caracter(); // Devolvemos el dígito para procesarlo en el contexto del número completo
        //subautomata_float(lex); // Suponiendo que este subautómata está implementado para manejar flotantes.
    } else { // No es un número, aceptamos el punto como token por sí mismo.
        devolver_caracter(); // Devolvemos el carácter no numérico al flujo
        aceptar_asignar_lexema(lex, OP_PUNTO);
    }
}

void subautomata_linea_logica() {
    // Implementar el subautómata correspondiente
    estado = ESTADO_INICIAL; // O manejar según sea necesario
}

void subautomata_comentario() {
    char c;
    do {
        c = siguiente_caracter();
    } while(c != '\n' && c != EOF);
    estado = ESTADO_INICIAL;
}

void subautomata_identificador(comp_lexico *lex, char c) {
    c = siguiente_caracter();
    if (!isalnum(c) && c != '_') {
        if (c != EOF) devolver_caracter();
        aceptar_asignar_desde_ts(lex);
    }
}

void subautomata_numeral(comp_lexico *lex, char c) {
    c = siguiente_caracter();
    if (isdigit(c)) {
        estado = ESTADO_NUMERO_ENTERO;
    }else if (c == '.'){
        estado = ESTADO_NUMERO_FLOAT;
    }else if (c == 'e'){
        estado = ESTADO_NUMERO_ELEVADO;
    }else if (c == 'x'){
        estado = ESTADO_NUMERO_HEX;
    }else{
        devolver_caracter();
        aceptar_asignar_lexema(lex, INTEGER);
    }
}

void subautomata_operador_delimitador(comp_lexico *lex, char c) {
    //char c = siguiente_caracter(); // Suponemos que ya tenemos el primer carácter del operador/delimitador.
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
                //TODO: Implementar cambio de estado a ** (en wilcoxon no ocurre, implementar solo si es necesario)
            } else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_MULTIPLICACION);
            }
            break;
        case '/':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_DIV_ASIGNACION);
            }else if (siguiente == '/') {
                //aceptar_asignar_lexema(lex, "**", OP_POTENCIA);
                //TODO: Implementar cambio de estado a // en wilcoxon no ocurre, implementar solo si es necesario)
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
            }else if (siguiente == '<') {
                //aceptar_asignar_lexema(lex, "**", OP_POTENCIA);
                //TODO: Implementar cambio de estado a << en wilcoxon no ocurre, implementar solo si es necesario)
            } else {
                devolver_caracter();
                aceptar_asignar_lexema(lex, OP_MENOR);
            }
            break;
        case '>':
            siguiente = siguiente_caracter();
            if (siguiente == '=') {
                aceptar_asignar_lexema(lex, OP_MAYOR_EQUAL);
            }else if (siguiente == '/') {
                //aceptar_asignar_lexema(lex, "**", OP_POTENCIA);
                //TODO: Implementar cambio de estado a // en wilcoxon no ocurre, implementar solo si es necesario)
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

void subautomata_token_especial() {
    char c;
    // Implementar la lógica para procesar tokens especiales
    estado = ESTADO_INICIAL;
}

// #####################################################
//        IMPLEMENTACIÓN SUBAUTÓMATAS INTERMEDIOS
// #####################################################

//subautómatas numerales
void subautomata_numeral_entero(comp_lexico *lex, char c){
    c = siguiente_caracter();
    if (!isdigit(c)) {
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
