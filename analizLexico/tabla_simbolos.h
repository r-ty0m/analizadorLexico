#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

#include "definiciones.h"

void init_ts();
void imprimir_ts();
int buscar_ts(char* lexema);
void insertar_ts(char* lexema, int tipo_componente);
void free_ts();

#endif
