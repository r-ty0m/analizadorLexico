#ifndef SISTEMA_ENTRADA_H
#define SISTEMA_ENTRADA_H

#define N 31 // Tamaño de cada centinela

// Inicializa el sistema de entrada.
void inicializar_sistema_entrada(const char *nombre_archivo);

// Cierra el sistema de entrada y libera recursos.
void cerrar_sistema_entrada();

// Devuelve el siguiente carácter del sistema de entrada.
char siguiente_caracter();

// Devuelve el carácter actual al sistema de entrada.
void devolver_caracter();

char *devolver_lexema();

// Devuelve el tamaño actual del lexema en proceso.
int tamano_lexema();

void avanzar_inicio();

#endif // SISTEMA_ENTRADA_H
