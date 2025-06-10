#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* Nombres de archivos */
#define NOMBRE_ARCH_AUXILIAR "auxiliar.tmp"

#define FORMATO_REGISTROS "\"%[^\"]\";\"%[^\"]\";%[^;]"
#define FORMATO_CABECERA "\"%[^\"]\";\"%[^\"]\";\"%[^\"]\""

/* Estados de retorno */
#define TODO_OK 0
#define ERR_ARCHIVO 1
#define ERR_BUFFER_CORTO 2

/* Tamaños para strings */
#define BUFFER_TAM 255

#define PERIODO_TAM 11
/* Algunos valores en nivel_general_aperturas de indices_items_obras.csv poseen hasta 40 caracteres */
#define NIVEL_TAM 41 
#define INDICE_TAM 17
#define CLASIFICADOR_TAM 14

/* Booleanos */
#define bool int
#define false 0
#define true 1

typedef struct{
    char periodo[PERIODO_TAM];
    char nivel[NIVEL_TAM];
    char indiceICC[INDICE_TAM];
    char clasificador[CLASIFICADOR_TAM];
} Registro;

typedef void (*Formatear) (Registro* reg);

int copiarArchivoTxt (char* nomArchDest, char* nomArchOrig);
int corregirArchivo (char* nomArchOrig, Formatear realizarFormateos);
void formatearNivelGeneral (Registro* reg);
void formatearItemsObra (Registro* reg);

char* concatenarString (char* str1, const char* str2, size_t lim);
char* copiarString (char* dest, const char* orig, size_t lim);