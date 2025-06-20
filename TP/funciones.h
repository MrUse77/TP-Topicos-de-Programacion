#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "../TPVector/Vector.h"

#define BUFFER_TAM 255
#define PERIODO_TAM 12
#define N_GENERAL_APERTURAS_TAM 41 /* Algunos campos de los archivos miden hasta 40 caracteres */
#define INDICE_ICC_TAM 17
#define CLASIFICADOR_TAM 14
#define V_MENSUAL_TAM 8
#define V_ANUAL_TAM 8
#define T_VARIABLE_TAM 15
#define VALOR_TAM 17

typedef struct {
    char periodo[PERIODO_TAM];
    char nGeneralAperturas[N_GENERAL_APERTURAS_TAM];
    char indiceICC[INDICE_ICC_TAM];
    char clasificador[CLASIFICADOR_TAM];
    char vMensual[V_MENSUAL_TAM];
    char vAnual[V_ANUAL_TAM];
} Registro;

typedef struct {
    char periodo[PERIODO_TAM];
    char clasificador[CLASIFICADOR_TAM];
    char nGeneralAperturas[N_GENERAL_APERTURAS_TAM];
    char tipoVariable[N_GENERAL_APERTURAS_TAM];
    char valor[VALOR_TAM];
} RegistroExportado;


typedef void (*Crr) (Registro* reg);

int cargarEnRegistro (void* reg, char* buffer);
void mostrarRegistro (const void* reg);
void mostrarRegistroExportado (const void* reg);
void corregirRegistros (Vector* v, Crr corrector);

void corregirIccGeneral (Registro* reg);
void corregirItemsObra (Registro* reg);

int compRegistrosPeriodo (const void* regA, const void* regB);
void calcularVariacionMensual (Vector* v);
void calcularVariacionAnual (Vector* v);
int cargarABinario (Vector* v, const char* nomArchBin);

#endif