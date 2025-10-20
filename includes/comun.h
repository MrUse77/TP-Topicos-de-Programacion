
#ifndef COMUN_H
#define COMUN_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"
#include "vector.h"

typedef struct {
	int monto;
	char region[10];
	char fechaDesde[18];
	char fechaHasta[18];
} filtroIPC;

typedef void (*Filtro)(void *filtro);
void menu(void *f, Filtro monto, Filtro region, Filtro periodo);
#endif // !COMUN_H
