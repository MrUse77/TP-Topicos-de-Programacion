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

	Vector Servicios, Bienes;
	const char *codeB[5] = { "01", "02", "03", "12", "05" };
	const char *codeS[7] = { "04", "06", "07", "08", "09", "10", "11" };
	vectorCrear(&Servicios, sizeof(divisiones));
	vectorCrear(&Bienes, sizeof(divisiones));
	clasificarDivisiones(&Bienes, &v, (const char **)codeB,
			     sizeof(codeB) / sizeof(codeB[0]), "Bienes");
	clasificarDivisiones(&Servicios, &v, (const char **)codeS,
			     sizeof(codeS) / sizeof(codeS[0]), "Servicios");
	vectorEliminarPorFiltro(&Servicios, regNacional);
	vectorEliminarPorFiltro(&Bienes, regNacional);
	/*
	printf("BIENES\n");
	vectorMostrar(&Bienes, printDIV);
	printf("SERVICIOS\n");
	vectorMostrar(&Servicios, printDIV);
	*/
	vectorMostrar(&Bienes, printDIV);
	calcularPromedioMensual(&Servicios, &Bienes, printIPC);

	FILE *f2 = fopen(IPC_AP, "r");
	if (!f2) {
		printf("Error al abrir el archivo %s\n", IPC_DIV);
		return -1;
	}
	vectorDestruir(&v);
	vectorCrear(&v, sizeof(serie_ipc_aperturas));
	if (code != OK) {
		fclose(f);
		return code;
	}
	code = vectorInsertarDeArchivoTXT(&v, f2, formatearAperturas, 0);
	if (code != OK) {
		fclose(f);
		vectorDestruir(&v);
		return code;
	}

	vectorDestruir(&Bienes);
	vectorDestruir(&Servicios);
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
/*
char *formatearFecha2(char *c)
{
	char *newFecha = malloc(11 * sizeof(char));
	char meses[12][11] = {
		"Enero",      "Febrero", "Marzo",     "Abril",
		"Septiembre", "Octubre", "Noviembre", "Diciembre"
	};
	int i = 0;
	char *anio = buscarCharEnStringEnReversa(c, '-');
	*anio = '\0';
	while (cmpString(c, meses[i]) != 0) {
		i++;
	}
	i++;
	cpyString(newFecha, anio + 1, 4);
	*(newFecha + 4) = '-';
	sprintf(newFecha + 5, "%02d", i);
	sprintf(newFecha + 7, "-01");
	newFecha[11] = '\0';
	return newFecha;
}
*/
