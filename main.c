#include <stddef.h>
#include <stdio.h>
#include "includes/vector.h"
#include "includes/divisiones.h"
#include "includes/aperturas.h"

#define IPC_DIV "./public/serie_ipc_divisiones.csv"
#define IPC_AP "./public/serie_ipc_aperturas.csv"
#define ALQUILERES "./public/alquileres.dat"

void printAP(const void *elem);
void printIPC(const void *elem);
void printDIV(const void *elem);
int main()
{
	int code = 0;
	//Primera Parte
	Vector v;
	FILE *f = fopen(IPC_DIV, "r");
	if (!f) {
		printf("Error al abrir el archivo %s\n", IPC_DIV);
		return -1;
	}
	code = vectorCrear(&v, sizeof(serie_ipc_divisiones));
	if (code != OK) {
		fclose(f);
		return code;
	}
	code = vectorInsertarDeArchivoTXT(&v, f, formatearDivisiones, 0);
	if (code != OK) {
		fclose(f);
		vectorDestruir(&v);
		return code;
	}
	fclose(f);
	actualizarMontoDivisiones(&v);
	calcularIPCPromedio(&v);
	vectorDestruir(&v);

	//Segunda Parte
	Vector v2, vA;
	FILE *f2 = fopen(IPC_AP, "r");
	if (!f2) {
		printf("Error al abrir el archivo %s\n", IPC_AP);
		return -1;
	}
	FILE *alquileres = fopen(ALQUILERES, "wb");
	if (!alquileres) {
		fclose(f2);
		printf("Error al abrir el archivo %s\n", ALQUILERES);
		return -1;
	}
	code = vectorCrear(&v2, sizeof(serie_ipc_aperturas));
	if (code != OK) {
		fclose(alquileres);
		fclose(f2);
		return code;
	}
	code = vectorCrear(&vA, sizeof(aperturas));
	if (code != OK) {
		vectorDestruir(&v2);
		fclose(alquileres);
		fclose(f2);
		return code;
	}
	code = vectorInsertarDeArchivoTXT(&v2, f2, formatearAperturas, 0);
	if (code != OK) {
		fclose(f2);
		vectorDestruir(&v2);
		vectorDestruir(&vA);
		return code;
	}
	calcularPromedioAlquiler(&v2, &vA);
	code = vectorGuardarAArchivoBIN(&vA, alquileres);
	if (code != OK) {
		fclose(f2);
		vectorDestruir(&v2);
		vectorDestruir(&vA);
		return code;
	}
	code = vectorInsertarDeArchivoBIN(&vA, alquileres);
	if (code != OK) {
		fclose(f2);
		vectorDestruir(&v2);
		vectorDestruir(&vA);
		return code;
	}
	puts("");
	puts("Variacion de aluileres desde archivo");
	vectorMostrar(&vA, printAperturas);
	vectorDestruir(&v2);
	vectorDestruir(&vA);
	fclose(f2);
	fclose(alquileres);

	return code;
}

void print(const void *elem)
{
	serie_ipc_divisiones *s = (serie_ipc_divisiones *)elem;
	printf("%s;%s;%s;%s;%s;%s;%s;%s\n", s->code, s->desc, s->clasificador,
	       s->indice_ipc, s->v_m_ipc, s->v_a_ipc, s->region, s->periodo);
}
void printDIV(const void *elem)
{
	divisiones *d = (divisiones *)elem;
	printf("%s;%s;%s;%s;%s\n", d->fecha, d->region, d->grupo, d->desc,
	       d->indice_ipc);
}
void printAP(const void *elem)
{
	serie_ipc_aperturas *s = (serie_ipc_aperturas *)elem;
	printf("%s, %s, %s, %s, %s, %s, %s, %s\n", s->code, s->desc,
	       s->clasificador, s->periodo, s->indice_ipc, s->v_m_ipc,
	       s->v_a_ipc, s->region);
}
