#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "strings.h" /* Nuestra bibioteca de strings (termina en s). */
#include "vector.h" /* Nuestra bibioteca de vectores (termina en r). */

/* Nombres de archivos */
#define NOMBRE_ARCH_AUXILIAR "auxiliar.tmp"

/* Estados de retorno */
#define TODO_OK 0
#define ERR_ARCHIVO 1
#define ERR_BUFFER_CORTO 2

/* Tamaños para strings */
#define BUFFER_TAM 1024

#define PERIODO_TAM 11
/* Algunos valores en nivel_general_aperturas de indices_items_obras.csv poseen hasta 40 caracteres */
#define NIVEL_TAM 41
#define VALOR_TAM 32
#define CLASIFICADOR_TAM 14 /* Booleanos */
#define TIPO_VARIABLE_TAM 15
#define bool int
#define false 0
#define true 1

typedef struct {
	char periodo[PERIODO_TAM];
	char nivel[NIVEL_TAM];
	double indiceICC;
	char clasificador[CLASIFICADOR_TAM];
	char var_mensual[VALOR_TAM];
	char var_interanual[VALOR_TAM];
} Registro;
typedef struct {
	char periodo[PERIODO_TAM];
	char nivel_general_aperturas[NIVEL_TAM];
	char clasificador[CLASIFICADOR_TAM];
	char tipo_variable[TIPO_VARIABLE_TAM];
	char valor[VALOR_TAM];
} RegistroBin;

typedef void (*Formatear)(Registro *reg);

int copiarArchivoTxt(char *nomArchDest, char *nomArchOrig);
int corregirArchivo(char *nomArchOrig, Formatear realizarFormateos,
		    Vector *vec);
int unirRegistros(Vector *v1, Vector *v2, Vector *vBin);
void formatearNivelGeneral(Registro *reg);
void formatearItemsObra(Registro *reg);
void ordenarRegistros(Vector *vBin);
void crearYescribirArchivoBinario(Vector *vBin, const char *path);

#endif
