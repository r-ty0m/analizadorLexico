cmake_minimum_required(VERSION 3.27)
project(analizLexico C)

set(CMAKE_C_STANDARD 11)

add_executable(analizLexico main.c
        analizador_lexico.c
        analizador_sintactico.c
        gestion_errores.c
        sistema_entrada.c
        tabla_simbolos.c
        abb.c
        sistema_entrada_save_c
        sistema_entrada_save_h
        analiz_lexico_save_c
        analiz_lexico_save_h
        analizador_sintactico_save)
