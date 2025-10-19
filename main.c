#include <stddef.h>
#include <stdio.h>
#include "includes/string.h"
#include "includes/vector.h"
#include "includes/divisiones.h"

#define IPC_DIV "./public/serie_ipc_divisiones.csv"
#define IPC_AP "./public/serie_ipc_aperturas.csv"

typedef struct {
	char monto[20];
	char region[30];
	char fechaDesde[10];
	char fechaHasta[10];
} serie_ipc_aperturas;
char *formatearFecha2(char *c);
int compararFechas(const char *f1, const char *f2);
int cmpDivisionesPorFechaRegionYDiv(const void *a, const void *b);
bool filtrarDivisiones(const void *elem, char *grupo);
int regNacional(const void *elem);
void print(const void *elem);
void decodificarFecha(char *fecha);
void formatearDivisiones(char *c, void *elem);
void formatearAperturas(char *c, void *elem);
void formatearFecha(char *c);
void printIPC(const void *elem);
void printDIV(const void *elem);
int main()
{
	FILE *f = fopen(IPC_DIV, "r");
	if (!f) {
		printf("Error al abrir el archivo %s\n", IPC_DIV);
		return -1;
	}
	int code = 0;
	Vector v;
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
	//Descomentar para que se realice, funciona correctamente
	//filtroIPC filtro;
	//menu(&filtro);
	//generarHerramienta(filtro, &v);

	Vector Divisiones;
	vectorCrear(&Divisiones, sizeof(divisiones));
	clasificarDivisiones(&Divisiones, &v, filtrarDivisiones,
			     cmpDivisionesPorFechaRegionYDiv);
	//vectorEliminarPorFiltro(&Servicios, regNacional);
	//vectorEliminarPorFiltro(&Bienes, regNacional);
	//vectorMostrar(&Divisiones, printDIV);
	calcularPromedios(&Divisiones, printIPC);
	vectorDestruir(&Divisiones);

	FILE *f2 = fopen(IPC_AP, "r");
	if (!f2) {
		printf("Error al abrir el archivo %s\n", IPC_DIV);
		return -1;
	}
	vectorDestruir(&v);
	vectorCrear(&v, sizeof(serie_ipc_aperturas));
	if (code != OK) {
		fclose(f2);
		return code;
	}
	//code = vectorInsertarDeArchivoTXT(&v, f2, formatearAperturas, 0);
	if (code != OK) {
		fclose(f2);
		vectorDestruir(&v);
		return code;
	}
	fclose(f2);
	return code;
}

void print(const void *elem)
{
	serie_ipc_divisiones *s = (serie_ipc_divisiones *)elem;
	printf("%s;%s;%s;%s;%s;%s;%s;%s\n", s->code, s->desc, s->clasificador,
	       s->indice_ipc, s->v_m_ipc, s->v_a_ipc, s->region, s->periodo);
}
void printIPC(const void *elem)
{
	indices *i = (indices *)elem;
	printf("%s;%s;%f;%f\n", i->fecha, i->region, i->bienes, i->servicios);
}
void printDIV(const void *elem)
{
	divisiones *d = (divisiones *)elem;
	printf("%s;%s;%s;%s;%s\n", d->fecha, d->region, d->grupo, d->desc,
	       d->indice_ipc);
}
// TODO: Solucionar cpyString de mi propia libreria de string
void formatearDivisiones(char *c, void *elem)
{
	serie_ipc_divisiones *s = (serie_ipc_divisiones *)elem;

	char *act = buscarCharEnStringEnReversa(c, '\n');
	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->periodo, act + 1);
	decodificarFecha(s->periodo);
	formatearFecha(s->periodo);

	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->region, act + 1);

	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->v_a_ipc, act + 1);

	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->v_m_ipc, act + 1);

	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->indice_ipc, act + 1);
	reemplazarCharEnString(s->indice_ipc, ',', '.');

	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->clasificador, act + 1);

	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->desc, act + 1);
	normarlizarPrimerChar(s->desc);

	*act = '\0';
	strcpy(s->code, c);
}

void formatearAperturas(char *c, void *elem)
{
	puts(c);
}
void decodificarFecha(char *fecha)
{
	char *cod =
		"4687195032"; //En el ejercicio no aparece que para el 5 es un 9, pero lo asumo por prueba y error
	fecha[0] = cod[(int)(*(fecha) - '0')];
	fecha[1] = cod[(int)(*(fecha + 1) - '0')];
	fecha[2] = cod[(int)(*(fecha + 2) - '0')];
	fecha[3] = cod[(int)(*(fecha + 3) - '0')];
	fecha[4] = cod[(int)(*(fecha + 4) - '0')];
	fecha[5] = cod[(int)(*(fecha + 5) - '0')];
}
int regNacional(const void *elem)
{
	divisiones *d = (divisiones *)elem;
	return cmpString(d->region, "Nacional") == 0;
}
bool filtrarDivisiones(const void *elem, char *grupo)
{
	const char *codeB[5] = { "01", "02", "03", "12", "05" };
	const char *codeS[7] = { "04", "06", "07", "08", "09", "10", "11" };
	if (includeString(((serie_ipc_divisiones *)elem)->code, codeB, 5)) {
		cpyString(grupo, "Bienes");
		return true;
	} else if (includeString(((serie_ipc_divisiones *)elem)->code, codeS,
				 7)) {
		cpyString(grupo, "Servicios");
		return true;
	}
	return false;
}
int cmpDivisionesPorFechaRegionYDiv(const void *a, const void *b)
{
	divisiones *d1 = (divisiones *)a;
	divisiones *d2 = (divisiones *)b;
	char *f1 = formatearFecha2(d1->fecha);
	char *f2 = formatearFecha2(d2->fecha);
	int res = cmpString(f2, f1);
	free(f1);
	free(f2);
	if (res == 0) {
		res = cmpString(d1->region, d2->region);
		if (res != 0) {
			return res;
		}
		return cmpString(d1->grupo, d2->grupo);
	}
	return res;
}
int compararFechas(const char *f1, const char *f2)
{
	//Formato MMMMMMMM-AAAA
	const char *mes1 = buscarCharEnStringEnReversa(f1, '-');
	const char *mes2 = buscarCharEnStringEnReversa(f2, '-');

	int cmpAnio = cmpString(mes1 + 1, mes2 + 1);
	if (cmpAnio != 0) {
		return cmpAnio;
	}

	// Compare months using length difference
	size_t len1 = mes1 - f1;
	size_t len2 = mes2 - f2;
	if (len1 != len2) {
		return (len1 > len2) ? 1 : -1;
	}

	return cmpNString(f1, f2, len1);
}

char *formatearFecha2(char *c)
{
	char *newFecha = malloc(7 * sizeof(char));
	char meses[12][11] = {
		"Enero",      "Febrero", "Marzo",     "Abril",
		"Mayo",	      "Junio",	 "Julio",     "Agosto",
		"Septiembre", "Octubre", "Noviembre", "Diciembre"
	};
	int i = 0;
	char *anio = buscarCharEnStringEnReversa(c, '-');
	size_t len1 = anio - c;
	while (c && cmpNString(c, meses[i], len1) != 0) {
		i++;
	}
	i++;
	cpyNString(newFecha, anio + 1, 4);
	sprintf(newFecha + 4, "%02d", i);
	newFecha[6] = '\0';
	return newFecha;
}
