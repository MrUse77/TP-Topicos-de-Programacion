#ifndef DIVISIONES_H
#define DIVISIONES_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"
#include "comun.h"
#include "vector.h"

#define BIENES 5
#define SERVICIOS 7
#define REGIONES 7

#define CODE_DIV_TAM 21
#define DESC_DIV_TAM 55
#define CLASIFICADOR_DIV_TAM 35
#define INDICE_IPC_DIV_TAM 17
#define V_M_IPC_DIV_TAM 17
#define V_A_IPC_DIV_TAM 17
#define REGION_DIV_TAM 10
#define PERIODO_DIV_TAM 18
#define GRUPO_DIV_TAM 10

typedef struct {
	char fecha[PERIODO_DIV_TAM];
	char region[REGION_DIV_TAM];
	float bienes;
	float servicios;
	int cantidadB;
	int cantidadS;
} acumuladorRegion;
typedef struct {
	char fecha[PERIODO_DIV_TAM];
	char region[REGION_DIV_TAM];
	float bienes;
	float servicios;
} indices;
typedef struct {
	char code[CODE_DIV_TAM];
	char desc[DESC_DIV_TAM]; //si bien el enunciado dice 31, el archivo tiene descripciones mas largas
	char clasificador[CLASIFICADOR_DIV_TAM];
	char indice_ipc[INDICE_IPC_DIV_TAM];
	char v_m_ipc[V_M_IPC_DIV_TAM];
	char v_a_ipc[V_A_IPC_DIV_TAM];
	char region[REGION_DIV_TAM];
	char periodo[PERIODO_DIV_TAM]; //AAAAMM
} serie_ipc_divisiones;
typedef struct {
	char fecha[PERIODO_DIV_TAM]; //AAAA-MM-DD
	char desc[DESC_DIV_TAM];
	char indice_ipc[INDICE_IPC_DIV_TAM];
	char region[REGION_DIV_TAM];
	char grupo[GRUPO_DIV_TAM];
} divisiones;

typedef bool (*Clasificar)(const void *elem, char *grupo);

void formatearDivisiones(char *c, void *elem);
void formatearFecha(char *c);
void decodificarFecha(char *fecha);

void actualizarMontoDivisiones(Vector *v);
void calcularIPCPromedio(Vector *v);

#endif // !DIVISIONES_H
