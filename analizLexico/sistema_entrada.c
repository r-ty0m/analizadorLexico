#include "sistema_entrada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar memset
#include "gestion_errores.h"

int mallocCounter=0;

// Estructura para manejar los centinelas y los punteros
typedef struct {
    char centinelaA[N];
    char centinelaB[N];
    int inicio;
    int delantero;
    int turno; // 0 para centinelaA, 1 para centinelaB
    FILE *archivo;
    int excedido; //El lexema ha excedido el tamaño máximo del buffer
} SistemaEntrada;

SistemaEntrada se;

void inicializar_sistema_entrada(const char *nombre_archivo) {
    // Inicialización de la estructura SistemaEntrada
    se.inicio = 0;
    se.delantero = 0;
    se.turno = 0; // Comenzamos leyendo desde el centinela A
    se.excedido = 0;
    memset(se.centinelaA, 0, N); // Limpieza inicial de los centinelas
    memset(se.centinelaB, 0, N);


    // Apertura del archivo fuente
    se.archivo = fopen(nombre_archivo, "r");
    if (!se.archivo) {
        //perror("Error al abrir el archivo");
        errorArchivo(1);
        exit(EXIT_FAILURE);
    }

    // Carga del primer bloque de datos en el centinela A
    size_t chars_leidos = fread(se.centinelaA, sizeof(char), N-1, se.archivo);
    se.centinelaA[chars_leidos] = EOF;
    if (chars_leidos < (N-1)) {
        // Si no hemos llenado completamente el centinela, colocamos EOF al final
        se.centinelaA[chars_leidos] = EOF;
    }
}

void cerrar_sistema_entrada() {
    // Verificamos si el archivo está abierto antes de intentar cerrarlo
    if (se.archivo != NULL) {
        fclose(se.archivo);
        se.archivo = NULL; // Aseguramos que el puntero al archivo no quede apuntando a una ubicación ya liberada
    }
}


char siguiente_caracter() {
    //printf("Delantero: [%d] | Inicio: [%d] | Turno: [%d] | centinelaA: [%s] | centinelaB: [%s] \n", se.delantero, se.inicio, se.turno, se.centinelaA, se.centinelaB);
    char caracter = '\0'; // Carácter a devolver

    if(tamano_lexema()>(N-1)){
        se.excedido=1;
        //printf("EXCEDIDO\n");
        //errorLexico(1,"test");
        //avanzar_inicio();
    }

    // Verificamos en qué centinela estamos y obtenemos el carácter actual
    if (se.turno == 0) { // Estamos en el centinela A
        caracter = se.centinelaA[se.delantero];
    } else { // Estamos en el centinela B
        caracter = se.centinelaB[se.delantero - N];
    }

    // Verificamos si hemos alcanzado EOF en el centinela actual
    if (caracter == EOF) {
        if (!feof(se.archivo)) { // Aún hay más archivo por leer
            // Cambiamos de centinela

            se.turno = (se.turno == 0) ? 1 : 0;
            if(se.inicio==se.delantero){ // Así se evita el caso en el que el inicio sea el EOF del bloque
                se.inicio = (se.turno == 0) ? 0 : N;
            }
            se.delantero = (se.turno == 0) ? 0 : N; // Reseteamos el puntero delantero para el nuevo centinela


            // Cargamos el siguiente bloque de datos en el nuevo centinela
            size_t chars_leidos = 0;
            if (se.turno == 0) { // Cargamos en centinela A
                chars_leidos = fread(se.centinelaA, sizeof(char), N-1, se.archivo);
                se.centinelaA[chars_leidos] = EOF; // Colocamos EOF al final
                //printf("Delantero: [%d] | Inicio: [%d] | Turno: [%d] | centinelaA: [%s] | centinelaB: [%s] \n", se.delantero, se.inicio, se.turno, se.centinelaA, se.centinelaB);

            } else { // Cargamos en centinela B
                chars_leidos = fread(se.centinelaB, sizeof(char), N-1, se.archivo);
                se.centinelaB[chars_leidos] = EOF; // Colocamos EOF al final
                //printf("Delantero: [%d] | Inicio: [%d] | Turno: [%d] | centinelaA: [%s] | centinelaB: [%s] \n", se.delantero, se.inicio, se.turno, se.centinelaA, se.centinelaB);

            }

            // Ahora obtenemos el nuevo carácter después de cargar el nuevo bloque
            if (se.turno == 0) {
                caracter = se.centinelaA[se.delantero];
            } else {
                caracter = se.centinelaB[se.delantero - N];
            }
        } else {
            // Si es EOF y hemos terminado de leer el archivo, no hacemos nada más
            // Podríamos manejar de forma diferente si necesitamos cerrar el archivo aquí o más tarde
        }
    }
    // Movemos el puntero delantero una posición adelante
    se.delantero++;

    return caracter; // Devolvemos el carácter actual
}


void devolver_caracter() {
    // Retrocedemos el puntero delantero una posición
    se.delantero--;


    // Si al retroceder estamos en la posición -1 del centinela A, necesitamos cambiar al centinela B
    // y ajustar el puntero delantero al final del centinela B (justo antes del EOF)
    if (se.delantero < 0 && se.turno == 0) {
        se.turno = 1; // Cambiamos al centinela B
        se.delantero = (2*N)-1; // Posicionamos el puntero al final del centinela B, considerando que N es el tamaño del buffer
    }
        // Similarmente, si al retroceder estamos en la posición N (el inicio del centinela B en términos del índice global delantero),
        // necesitamos cambiar al centinela A y ajustar el puntero delantero al final del centinela A
    else if (se.delantero == (N-1) && se.turno == 1) {
        se.turno = 0; // Cambiamos al centinela A
        se.delantero = N - 2; // Posicionamos el puntero al final del centinela A
        //TODO: IMPLEMENTAR QUE NO SE CARGUE EL BLOQUE DE NUEVO
    }
}

// Función para devolver el lexema al analizador léxico
char *devolver_lexema() {
    // Calculamos el tamaño del lexema
    if(se.excedido){
        //printf("parar");
    }
    int tamano;

    if(se.excedido == 1){
        tamano = N-1;
        //se.inicio=tamano-N;
        /*
        if(inicio<N){
            se.inicio=tamano-N;
        }else{
            se.inicio=tamano-N;
        }
         */

        if(se.delantero<N){
            // Se ha excedido y el deltantero está en A. El inicio se debe poner N-1 posiciones por detrás.
            // Se coloca en la misma posición pero en el bloque contrario. Así te aseguras de que el tamaño sea N-1
            // Quedará así: [ | | |delantero| | | |EOF]   [ | |inicio| | | | |EOF]  -> diferencia de N-1
            se.inicio= se.delantero + (N);
        }else{
            // Quedará así: [ | |incio| | | | |EOF]   [ | | |delantero| | | |EOF]  -> diferencia de (N-1)
            se.inicio= se.delantero - (N);

        }
    }else{
        tamano = tamano_lexema();
    }


    // Creamos un buffer para almacenar el lexema
    char *lexema = (char *)malloc((tamano+1) * sizeof(char));
    mallocCounter++;

    // Verificamos en qué centinela estamos y copiamos el lexema
    int pos_lexema = 0;
    if (se.inicio < N) { // El inicio del lexema está en el centinela A
        // Calculamos cuánto del lexema podemos copiar desde el centinela A
        int tamano_a_copiar = (tamano > (N-1) - se.inicio) ? (N-1) - se.inicio : tamano;
        strncpy(lexema + pos_lexema, se.centinelaA + se.inicio, tamano_a_copiar);
        pos_lexema += tamano_a_copiar;

        // Si el lexema se extiende al centinela B, copiamos el resto
        if (tamano > tamano_a_copiar) {
            int resto = tamano - tamano_a_copiar;
            strncpy(lexema + pos_lexema, se.centinelaB, resto);
            pos_lexema += resto;
        }
    } else { // El inicio del lexema está en el centinela B
        // Ajustamos el inicio para el centinela B
        int inicioB = se.inicio - N;

        // Calculamos cuánto del lexema podemos copiar desde el centinela B
        int tamano_a_copiar = (tamano > (N-1) - inicioB) ? (N-1) - inicioB : tamano;
        strncpy(lexema + pos_lexema, se.centinelaB + inicioB, tamano_a_copiar);
        pos_lexema += tamano_a_copiar;

        // Si el lexema se extiende al centinela A, copiamos el resto

        if (tamano > tamano_a_copiar) {
            int resto = tamano - tamano_a_copiar;
            strncpy(lexema + pos_lexema, se.centinelaA, resto);
            pos_lexema += resto;
        }
    }

    // Agregamos el caracter nulo al final del lexema
    lexema[tamano] = '\0';

    // Reiniciamos los punteros para el siguiente lexema
    se.inicio = se.delantero;

    if(se.excedido){
        errorLexico(1, lexema);
        se.excedido=0;
    }
    // Devolvemos el lexema
    printf("[%d]",mallocCounter);
    return lexema;
}


int tamano_lexema() {
    // inicio y delantero en el mismo
    if ((se.inicio < (N-1) && se.delantero < N) || (se.inicio >= N && se.delantero >= N)) {
        return se.delantero - se.inicio;
    }
        // inicio en A y delantero en B
    else if (se.inicio < (N-1) && se.delantero >= N) {
        // Calculamos la distancia desde el inicio hasta el final de centinela A
        // y desde el inicio de centinela B hasta la posición del delantero
        return ((N-1) - se.inicio) + (se.delantero - N);
    }
    //Inicio en B y delantero en A
    else if (se.inicio >= N && se.delantero<N) {
        // Calculamos la distancia desde el inicio hasta el final de centinela B (ajustado por N)
        int distanciaB = ((2*N)-1) - se.inicio;
        // Sumamos la distancia desde el inicio de centinela A hasta la posición del delantero
        return distanciaB + se.delantero;
    }

        // Este caso manejaria cualquier situación inesperada, devolviendo 0 por defecto
        // Pero en práctica, no debería ocurrir debido a la lógica de manejo de los punteros
    else {
        return 0;
    }
}

void avanzar_inicio(){
    //if((se.inicio)==N-2) se.inicio=N; //Si al avanzar se pone en el EOF del buffer A, se pasa al comienzo del buffer B
    se.inicio++;
};
