#ifndef SISTEMA_ENTRADA_H
#define SISTEMA_ENTRADA_H

#define N 8// Tamaño de cada centinela. Incluye el EOF que ocupa la última posición de cada centinela.
// Para N=8 -> [0|1|2|3|4|5|6|EOF]

// Inicializa el sistema de entrada.
void inicializar_sistema_entrada(const char *nombre_archivo);

// Cierra el sistema de entrada y libera recursos.
void cerrar_sistema_entrada();

// Recupera el siguiente carácter del sistema de entrada.
char siguiente_caracter();

// Devuelve el carácter actual al sistema de entrada.
void devolver_caracter();

// Recupera el lexema del sistema de entrada
char *devolver_lexema();

// Devuelve el tamaño actual del lexema en proceso.
int tamano_lexema();

// Avanza en una unidad el puntero de inicio
void avanzar_inicio();

// Coloca el puntero de inicio a la posición del delantero, descartando así el lexema
void descartar_lexema();

//Recupera el número de línea del archivo fuente
int getLinea();


#endif // SISTEMA_ENTRADA_H
