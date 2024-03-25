#ifndef ANALIZADOR_LEXICO_H
#define ANALIZADOR_LEXICO_H

#include "definiciones.h"

// Definición de la estructura
typedef struct {
    int tipo_componente;
    char *lexema;
} comp_lexico;

// Función principal. Devolverá el struct rellenado con la información del lexema
void sig_comp_lexico(comp_lexico *lex);

#endif
