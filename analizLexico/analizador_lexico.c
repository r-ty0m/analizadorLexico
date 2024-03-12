#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "analizador_lexico.h"
#include "tabla_simbolos.h"
#include "sistema_entrada.h"

#define ESTADO_BASE 0
#define ESTADO_ALFANUM 1
#define ESTADO_PLUS 2
#define ESTADO_MINUS 3
#define ESTADO_EQUALS 4
#define ESTADO_ASTERISK 5
#define ESTADO_SLASH 6
#define ESTADO_PERCENT 7
#define ESTADO_MENOR 8
#define ESTADO_MAYOR 9
#define ESTADO_EXCLAMATION 10
#define ESTADO_AMPERSAND 11
#define ESTADO_PIPE 12
#define ESTADO_CARET 13
#define ESTADO_AT 14
#define ESTADO_DOUBLE_ASTERISK 15
#define ESTADO_DOUBLE_SLASH 16
#define ESTADO_MENOR_MENOR 17;


void asignar_lexema(comp_lexico *lex, char *strLex, int tipo) {
    lex->lexema = strdup(strLex);
    lex->tipo_componente = tipo;
}

void sig_comp_lexico(comp_lexico *lex) {
    static char buffer[N];
    int buf_len = 0;
    char c;
    int estado = ESTADO_BASE; // Estado inicial del autómata.

    memset(buffer, 0, sizeof(buffer)); // Limpiamos el buffer.

    while (1) {
        c = siguiente_caracter();

        switch (estado) {
            case ESTADO_BASE: // Estado inicial, buscando el inicio de un identificador.

                // CADENAS ALFANUMÉRICAS (IDENTIFICADOR)
                if (isalpha(c) || c == '_') { // Puede ser identificador
                    //buffer[buf_len++] = c;
                    estado = ESTADO_ALFANUM;
                }

                // DELIMITADORES Y OPERADORES
                else if (c == '+') {
                    estado = ESTADO_PLUS;
                } else if (c == '-') {
                    estado = ESTADO_MINUS;
                } else if (c == '=') {
                    estado = ESTADO_EQUALS;
                } else if (c == '*') {
                    estado = ESTADO_ASTERISK;
                } else if (c == '/') {
                    estado = ESTADO_SLASH;
                } else if (c == '%') {
                    estado = ESTADO_PERCENT;
                } else if (c == '<') {
                    estado = ESTADO_MENOR;
                } else if (c == '>') {
                    estado = ESTADO_MAYOR;
                } else if (c == '!') {
                    estado = ESTADO_EXCLAMATION;
                } else if (c == '&') {
                    estado = ESTADO_AMPERSAND;
                } else if (c == '|') {
                    estado = ESTADO_PIPE;
                } else if (c == '^') {
                    estado = ESTADO_CARET;
                } else if (c == '~') {
                    asignar_lexema(lex, "~", OP_COMPLEMENTO_A_UNO);
                    return;
                } else if (c == '(') {
                    asignar_lexema(lex, "(", DELIM_PARENTESIS_ABRIR);
                    return;
                } else if (c == ')') {
                    asignar_lexema(lex, ")", DELIM_PARENTESIS_CERRAR);
                    return;
                } else if (c == '[') {
                    asignar_lexema(lex, "[", DELIM_CORCHETE_ABRIR);
                    return;
                } else if (c == ']') {
                    asignar_lexema(lex, "]", DELIM_CORCHETE_CERRAR);
                    return;
                } else if (c == '{') {
                    asignar_lexema(lex, "{", DELIM_LLAVE_ABRIR);
                    return;
                } else if (c == '}') {
                    asignar_lexema(lex, "}", DELIM_LLAVE_CERRAR);
                    return;
                } else if (c == ',') {
                    asignar_lexema(lex, ",", DELIM_COMA);
                    return;
                } else if (c == ';') {
                    asignar_lexema(lex, ";", DELIM_PUNTO_Y_COMA);
                    return;
                } else if (c == '.') {
                    //asignar_lexema(lex, ".", DELIM_PUNTO);

                    char *lexema = devolver_lexema();
                    lex->lexema = lexema;
                    //lex->lexema = strdup(buffer);
                    lex->tipo_componente = 46; // Asignar tipo de componente
                    return;
                } else if (c == '@') {
                    estado = ESTADO_AT;
                }


                //
                else if (c == EOF || c == '$') { // Si es EOF
                    lex->tipo_componente = EOF;
                    printf("\n $ ENCONTRADO - CERRANDO\n");
                    return;
                }
                else if (c == '#') { // Comentario de una línea
                    estado = 2000; // Cambiamos al estado de comentario de una línea.
                }
                else if (c == '\n') {
                    asignar_lexema(lex,  "\n", LF);
                    avanzar_inicio();
                    return; // Salimos de la función.
                }else{
                    avanzar_inicio();
                }
                break;


            //SUBAUTÓMATA ALFANUM
            case ESTADO_ALFANUM: // Estado de recolección de identificador.
                if (isalnum(c) || c == '_') {
                    //buffer[buf_len++] = c;
                } else {
                    // Final del identificador, devolvemos el último carácter al stream si no es EOF.
                    if (c != EOF) devolver_caracter();

                    char *lexema = devolver_lexema();
                    lex->lexema = lexema;
                    //lex->lexema = strdup(buffer);
                    lex->tipo_componente = buscar_ts(lex->lexema); // Asignar tipo de componente

                    //estado = ESTADO_BASE;
                    return; // Salimos de la función.
                }
                break;

            //SUBAUTÓMATA OPERADORES Y DELIMITADORES
            case ESTADO_PLUS:
                if (c == '=') asignar_lexema(lex, "+=", OP_SUMA_ASIGNACION);
                else { devolver_caracter(); asignar_lexema(lex, "+", OP_SUMA); }
                return;
            case ESTADO_MINUS:
                if (c == '=') asignar_lexema(lex, "-=", OP_RESTA_ASIGNACION);
                else if (c == '>') asignar_lexema(lex, "->", OP_FLECHA);
                else { devolver_caracter(); asignar_lexema(lex, "-", OP_RESTA); }
                return;
            case ESTADO_ASTERISK:
                if (c == '=') asignar_lexema(lex, "*=", OP_MULTIPLICACION_ASIGNACION);
                else if (c == '*'){
                    estado = ESTADO_DOUBLE_ASTERISK;
                    break;
                }
                else { devolver_caracter(); asignar_lexema(lex, "*", OP_MULTIPLICACION); }
                return;
            case ESTADO_DOUBLE_ASTERISK:
                if (c == '=') asignar_lexema(lex, "**=", OP_POTENCIA_ASIGNACION);
                else { devolver_caracter(); asignar_lexema(lex, "**", OP_POTENCIA); }
                return;
            case ESTADO_EQUALS:
                if( c == '=') asignar_lexema(lex, "==", OP_EQUAL_EQUAL);
                else{
                    devolver_caracter();
                    asignar_lexema(lex, "=", OP_EQUAL);
                }
                return;
            case ESTADO_SLASH:
                if(c == '/'){
                    estado = ESTADO_DOUBLE_SLASH;
                    break;
                }else if (c == '=') asignar_lexema(lex, "/=", OP_SLASH_EQUAL);
                else{
                    devolver_caracter();
                    asignar_lexema(lex, "/", OP_SLASH);
                }
                return;
            case ESTADO_DOUBLE_SLASH:
                if(c == '=') asignar_lexema(lex, "//=", OP_SLASH_SLASH_EQUAL);
                else{
                    devolver_caracter();
                    asignar_lexema(lex, "/", OP_SLASH_SLASH);
                }
                return;
            case ESTADO_PERCENT:
                if (c == '=') asignar_lexema(lex, "%=", OP_PERCENT_EQUAL);
                else { devolver_caracter(); asignar_lexema(lex, "%", OP_PERCENT); }
                return;
            case ESTADO_MENOR:
                if(c == '<'){
                    estado = ESTADO_MENOR_MENOR;
                    break;
                }
                else{
                    devolver_caracter();
                    asignar_lexema(lex, "<", OP_MENOR);
                }
                return;
            case ESTADO_MAYOR:
                if(c == '<'){
                    estado = ESTADO_MENOR_MENOR;
                    break;
                }
                else{
                    devolver_caracter();
                    asignar_lexema(lex, "<", OP_MAYOR);
                }
                return;



        }
    }
}