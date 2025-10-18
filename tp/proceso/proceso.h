#ifndef PROCESO_INCLUDED
#define PROCESO_INCLUDED

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../comunes/comunes.h"
#include "../vector/vector.h"
#include "../vector/iterador.h"
#include "../vector/algoritmos.h"
#include "../secuenciaPalabras/secuenciaPalabras.h"
#include "../cadenas/cadenas.h"
#include "../formulario/formulario.h"

#define ARCH_DIVISIONES "pruebaDivisiones.csv"
#define ARCH_APERTURAS "pruebaAperturas.csv"
#define ARCH_BIENES "pruebaBienes.csv"
#define ARCH_SERVICIOS "pruebaServicios.csv"
#define ARCH_NACIONAL "pruebaNacional.csv"

#define DIVISIONES_COD_LEN 15
#define DIVISIONES_DESC_LEN 61
#define DIVISIONES_CLASIF_LEN 40
#define DIVISIONES_INDICES_LEN 18
#define DIVISIONES_REGION_LEN 15
#define DIVISIONES_PERIODO_LEN 30

#define APERTURAS_COD_LEN 15
#define APERTURAS_DESC_LEN 61
#define APERTURAS_CLASIF_LEN 40
#define APERTURAS_INDICES_LEN 18
#define APERTURAS_PERIODO_LEN 30
#define APERTURAS_REGION_LEN 15

#define CLASIFICADO_DESC_LEN 30
#define CLASIFICADO_CLASIF_LEN 30
#define CLASIFICADO_PERIODO_LEN 9
#define CLASIFICADO_IND_IPC_LEN 15
#define CLASIFICADO_REGION_LEN 15
#define CLASIFICADO_GRUPO_LEN 15


typedef struct{
    char cod[DIVISIONES_COD_LEN];
    char desc[DIVISIONES_DESC_LEN];
    char clasif[DIVISIONES_CLASIF_LEN];
    char indiceIPC[DIVISIONES_INDICES_LEN];
    char varMensIPC[DIVISIONES_INDICES_LEN];
    char varAnualIPC[DIVISIONES_INDICES_LEN];
    char region[DIVISIONES_REGION_LEN];
    char periodo[DIVISIONES_PERIODO_LEN];
}IPCDivisiones;

typedef struct{
    double monto;
    char region[DIVISIONES_REGION_LEN];
    char periodoIni[DIVISIONES_PERIODO_LEN];
    char periodoFin[DIVISIONES_PERIODO_LEN];
}Respuesta;

typedef struct{
    char fecha[CLASIFICADO_PERIODO_LEN];
    char desc[CLASIFICADO_DESC_LEN];
    char indiceIPC[CLASIFICADO_IND_IPC_LEN];
    char region[CLASIFICADO_REGION_LEN];
    char grupo[CLASIFICADO_GRUPO_LEN];
}IPCClasificado;

typedef struct{
    char fecha[DIVISIONES_PERIODO_LEN];
    char region[DIVISIONES_REGION_LEN];
    char indiceBienes[DIVISIONES_INDICES_LEN];
    char indiceServicios[DIVISIONES_INDICES_LEN];
}IPCPromedio;

typedef struct{
    char cod[APERTURAS_COD_LEN];
    char desc[APERTURAS_DESC_LEN];
    char clasif[APERTURAS_CLASIF_LEN];
    char periodo[APERTURAS_PERIODO_LEN];
    char indiceIPC[APERTURAS_INDICES_LEN];
    char varMensIPC[APERTURAS_INDICES_LEN];
    char varAnualIPC[APERTURAS_INDICES_LEN];
    char region[APERTURAS_REGION_LEN];
}IPCAperturas;

typedef struct{
    char desc[APERTURAS_DESC_LEN];
    char periodo[APERTURAS_PERIODO_LEN];
    char indiceIPC[APERTURAS_INDICES_LEN];
    char acumuladoIPC[APERTURAS_INDICES_LEN];
    char montoAjustado[APERTURAS_INDICES_LEN];
}IPCAlquileres;

int parsearIPCDivisiones(FILE* arch, void* reg);
int parsearIPCAperturas(FILE* arch, void* reg);

int corregirCampos(Vector_t* vec, int (*Corrector)(void*));

int corregirIPCDivisiones(void* reg);
int decodificarFechaDivisiones(IPCDivisiones* reg, int* decod);
int convertirFechaDivisiones(IPCDivisiones* reg);
int normalizarDescripcionDivisiones(IPCDivisiones* reg);
void palabraATitulo(Palabra_t* pal);
void palabraAMinuscula(Palabra_t* pal);

int corregirIPCAperturas(void* reg);
int corregirFormatoFechaAperturas(IPCAperturas* reg);

int herramientaAjustarMontosIPCDivisiones(Vector_t* vec);
Respuesta preguntarAjustarMonto(void);
int filtrarIPCDivisiones(void* dato, void* contexto);
int clasificarBySIPCDivisiones(Vector_t* vec);
int filtrarBienes(void* dato, void* contexto);
int filtrarServicios(void* dato, void* contexto);
int filtrarBienesDebug(void* dato, void* contexto);
int filtrarServiciosDebug(void* dato, void* contexto);
void* obtenerPeriodo(void* elem);
void* obtenerCod(void* elem);
int compararPeriodo(void* lhs, void* rhs);
int compararCodBienes(void* lhs, void* rhs);
int compararCodServicios(void* lhs, void* rhs);
void* reducirBySProm(void* dato, void* acumulado);
void* unirBienesYServicios(void* lhs, void* rhs, void* elem);
int parsearIPCPromedio(FILE* arch, void* reg);
                                            
int herramientaCalcularAlquilerIPCAperturas(Vector_t* vec);


#endif // PROCESO_INCLUDED