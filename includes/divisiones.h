#ifndef DIVISIONES_H
#define DIVISIONES_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"

#include "vector.h"

#define BIENES 5
#define SERVICIOS 7
#define REGIONES 7

typedef struct {
	char fecha[18];
	char region[10];
	float bienes;
	float servicios;
} indices;
typedef struct {
	char code[21];
	char desc[55];
	char clasificador[35];
	char indice_ipc[17];
	char v_m_ipc[17];
	char v_a_ipc[17];
	char region[10];
	char periodo[18]; //AAAAMM
} serie_ipc_divisiones;
typedef struct {
	char fecha[18]; //AAAA-MM-DD
	char desc[55];
	char indice_ipc[17];
	char region[10];
	char grupo[10];
} divisiones;
typedef struct {
	int monto;
	char region[10];
	char fechaDesde[18];
	char fechaHasta[18];
} filtroIPC;

void generarHerramienta(filtroIPC filtro, Vector *v);
void menu(filtroIPC *f);
void calcularPromedioMensual(Vector *S, Vector *B, Fmt print);
void clasificarDivisiones(Vector *D, Vector *v, const char **filtro,
			  size_t sizeFiltro, const char *grupo);

#endif // !DIVISIONES_H
