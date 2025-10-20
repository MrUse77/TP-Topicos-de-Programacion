
#ifndef APERTURAS_H
#define APERTURAS_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"
#include "vector.h"
#include "comun.h"

#define CODE_AP_TAM 21
#define DESC_AP_TAM 61
#define CLASIFICADOR_AP_TAM 35
#define PERIODO_AP_TAM 11
#define INDICE_IPC_AP_TAM 17
#define V_M_IPC_AP_TAM 17
#define V_A_IPC_AP_TAM 17
#define REGION_AP_TAM 10

typedef struct {
	char code[CODE_AP_TAM];
	char desc[DESC_AP_TAM];
	char clasificador[CLASIFICADOR_AP_TAM];
	char periodo[PERIODO_AP_TAM];
	char indice_ipc[INDICE_IPC_AP_TAM];
	char v_m_ipc[V_M_IPC_AP_TAM];
	char v_a_ipc[V_A_IPC_AP_TAM];
	char region[REGION_AP_TAM];
} serie_ipc_aperturas;
typedef struct {
	char periodo[PERIODO_AP_TAM];
	float indice;
	float variacion;
	float montoAjustado;
} aperturas;

void formatearAperturas(char *c, void *elem);
int soloAlquiler(const void *elem);
void calcularPromedioAlquiler(Vector *v, Vector *vA);
void printAperturas(const void *elem);
char *formatearFechaAperturas(char *c);

#endif // !APERTURAS_H
