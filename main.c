#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "includes/string.h"
#include "includes/vector.h"

#define IPC_DIV "./public/serie_ipc_divisiones.csv"
#define BIENES 5
#define SERVICIOS 7
#define REGIONES 7

typedef struct {
	char fecha[11];
	char region[10];
	char bienes[17];
	char servicios[17];
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

int regNacional(const void *elem);
void promedioMensual(VectorIterador it, char *region, char *fecha,
		     indices *prom);
char *formatearFecha2(char *c);
void calcularPromedioMensual(Vector *D);
void clasificarDivisiones(Vector *D, Vector *v);
void normalizar(char *c);
void seleccionarRegion(char *region);
void calcularIPC(char *sDesde, char *sHasta, int monto);
int aplicarFiltro(const void *elem, const void *filtro);
void menu(filtroIPC *f);
void generarHerramienta(filtroIPC filtro, Vector *v);
void print(const void *elem);
void decodificarFecha(char *fecha);
void formatear(char *c, void *elem);
void formatearFecha(char *c);
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
		return code;
	}
	code = vectorInsertarDeArchivoTXT(&v, f, formatear, 0);
	if (code != OK) {
		return code;
	}
	//filtroIPC filtro;
	//menu(&filtro);
	//generarHerramienta(filtro, &v);
	Vector Divisiones;
	vectorCrear(&Divisiones, sizeof(divisiones));
	clasificarDivisiones(&Divisiones, &v);
	calcularPromedioMensual(&Divisiones);

	vectorDestruir(&v);
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
	printf("%s;%s;%s;%s\n", i->fecha, i->region, i->bienes, i->servicios);
}
void printDIV(const void *elem)
{
	divisiones *d = (divisiones *)elem;
	printf("%s;%s;%s;%s;%s\n", d->fecha, d->region, d->grupo, d->desc,
	       d->indice_ipc);
}
// TODO: Solucionar cpyString de mi propia libreria de string
void formatear(char *c, void *elem)
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
void formatearFecha(char *c)
{
	char meses[12][11] = {
		"Enero",      "Febrero", "Marzo",     "Abril",
		"Mayo",	      "Junio",	 "Julio",     "Agosto",
		"Septiembre", "Octubre", "Noviembre", "Diciembre"
	};
	char mesReg[3];
	cpyString(mesReg, c + 4, 2);
	int mes = atoi(mesReg) - 1;
	//AAAAMM
	size_t len = lenString(meses[mes]);
	char anio[5];
	strncpy(anio, c, 4);
	//MMMMMMMMMM-AAAA
	memcpy(c, meses[mes], len);
	c += len;
	*c = '-';
	strcpy(c + 1, anio);
}
void menu(filtroIPC *f)
{
	printf("Ingrese un monto expresado en pesos: ");
	scanf("%d", &f->monto);
	printf("Seleccione la region: \n");
	printf("1. Nacional\n");
	printf("2. GBA\n");
	printf("3. Pampeana\n");
	printf("4. Cuyo\n");
	printf("5. Noroeste\n");
	printf("6. Noreste\n");
	printf("7. Patagonia\n");
	seleccionarRegion(f->region);
	printf("Ingrese una fecha desde (AAAAMM): ");
	scanf(" %s", f->fechaDesde);
	printf("Ingrese una fecha hasta (AAAAMM): ");
	scanf(" %s", f->fechaHasta);
}
void generarHerramienta(filtroIPC filtro, Vector *v)
{
	serie_ipc_divisiones *s;
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	s = (serie_ipc_divisiones *)vectorIteradorPrimero(&it);
	int encontrados = 0;
	char sDesde[17], sHasta[17];

	formatearFecha((char *)filtro.fechaDesde);
	formatearFecha((char *)filtro.fechaHasta);
	while (!vectorIteradorFin(&it) && encontrados != 2) {
		if (cmpString(s->desc, "Nivel general") == 0 &&
		    cmpString(s->region, filtro.region) == 0) {
			if (cmpString(s->periodo, filtro.fechaDesde) == 0) {
				printf("Encontrado desde: %s\n", s->periodo);
				strcpy(sDesde, s->indice_ipc);
				encontrados++;
			} else if (cmpString(s->periodo, filtro.fechaHasta) ==
				   0) {
				printf("Encontrado hasta: %s\n", s->periodo);
				strcpy(sHasta, s->indice_ipc);
				encontrados++;
			}
		}
		s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
	}
	puts(sDesde);
	puts(sHasta);
	calcularIPC(sDesde, sHasta, filtro.monto);
}
void calcularIPC(char *sDesde, char *sHasta, int monto)
{
	float pDesde = atof(sDesde);
	float pHasta = atof(sHasta);
	float montoAjustado = monto * (pHasta / pDesde);
	float variacion = (pHasta / pDesde - 1) * 100;
	printf("El monto ajustado con una variacion del %f%% es de: %f",
	       variacion, montoAjustado);
}
void seleccionarRegion(char *region)
{
	int opcion = 0;
	do {
		printf("Ingrese una opcion (1-7): ");
		scanf("%d", &opcion);
		switch (opcion) {
		case 1:
			strcpy(region, "Nacional");
			break;
		case 2:
			strcpy(region, "GBA");
			break;
		case 3:
			strcpy(region, "Pampeana");
			break;
		case 4:
			strcpy(region, "Cuyo");
			break;
		case 5:
			strcpy(region, "Noroeste");
			break;
		case 6:
			strcpy(region, "Noreste");
			break;
		case 7:
			strcpy(region, "Patagonia");
			break;
		default:
			printf("Opcion invalida. Intente de nuevo.\n");
			break;
		}
	} while (opcion < 1 || opcion > 7);
}
void clasificarDivisiones(Vector *D, Vector *v)
{
	char **Bienes = malloc(sizeof(char *) * BIENES);
	Bienes = (char *[]){ "Alimentos y bebidas no alcohólicas",
			     "Bebidas alcohólicas y tabaco",
			     "Prendas de vestir y calzado",
			     "Bienes y servicios varios",
			     "Equipamiento y mantenimiento del hogar" };
	char **Servicios = malloc(sizeof(char *) * SERVICIOS);
	Servicios = (char *[55]){
		"Recreación y cultura",
		"Restaurantes y hoteles",
		"Salud",
		"Transporte",
		"Educación",
		"Comunicación",
		"Vivienda, agua, electricidad, gas y otros combustibles"
	};
	serie_ipc_divisiones *s = NULL;
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	divisiones div;
	s = (serie_ipc_divisiones *)vectorIteradorPrimero(&it);
	while (!vectorIteradorFin(&it)) {
		bool bienes =
			includeString(s->desc, (const char **)Bienes, BIENES);
		bool servicios = includeString(
			s->desc, (const char **)Servicios, SERVICIOS);
		if (bienes || servicios) {
			strcpy(div.desc, s->desc);
			strcpy(div.indice_ipc, s->indice_ipc);
			strcpy(div.region, s->region);
			strcpy(div.fecha, s->periodo);
			strcpy(div.grupo, bienes ? "Bienes" : "Servicios");
			vectorInsertar(D, &div);
		}
		s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
	}
}
void calcularPromedioMensual(Vector *D)
{
	vectorEliminarPorFiltro(D, regNacional);
	size_t cantPorAnio = (SERVICIOS + BIENES); //12 meses y 7 regiones
	indices ipc;
	Vector vIpc;
	vectorCrear(&vIpc, sizeof(indices));
	VectorIterador it;
	vectorIteradorCrear(&it, D);
	divisiones *div = (divisiones *)vectorIteradorPrimero(&it);
	strcpy(ipc.fecha, formatearFecha2(div->fecha));
	strcpy(ipc.region, div->region);
	strcpy(ipc.bienes, div->indice_ipc);
	strcpy(ipc.servicios, div->indice_ipc);
	vectorInsertar(&vIpc, &ipc);
	div = (divisiones *)vectorIteradorDesplazamiento(&it, cantPorAnio);
	while (!vectorIteradorFin(&it)) {
		//Me muevo por regiones hasta encontrar la nacional
		strcpy(ipc.fecha, formatearFecha2(div->fecha));
		strcpy(ipc.region, div->region);
		promedioMensual(it, div->region, div->fecha, &ipc);
		vectorInsertar(&vIpc, &ipc);
		div = (divisiones *)vectorIteradorDesplazamiento(&it,
								 cantPorAnio);
		//Estoy en la nacional, calculo el promedio y avanzo 12 posiciones
		//(una por cada mes del año)
	}
	vectorMostrar(&vIpc, printIPC);

	return;
}
char *formatearFecha2(char *c)
{
	char *newFecha = malloc(11 * sizeof(char));
	char meses[12][11] = {
		"Enero",      "Febrero", "Marzo",     "Abril",
		"Mayo",	      "Junio",	 "Julio",     "Agosto",
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
void promedioMensual(VectorIterador it, char *region, char *fecha,
		     indices *prom)
{
	divisiones *div = (divisiones *)vectorIteradorActual(&it);
	float sumaBienes = 0;
	float sumaServicios = 0;
	int contadorBienes = 0;
	int contadorServicios = 0;
	for (int i = 0; i < BIENES + SERVICIOS; i++) {
		if (cmpString(div->grupo, "Bienes") == 0) {
			sumaBienes += atof(div->indice_ipc);
			contadorBienes++;
		} else if (cmpString(div->grupo, "Servicios") == 0) {
			sumaServicios += atof(div->indice_ipc);
			contadorServicios++;
		}
		div = (divisiones *)vectorIteradorSiguiente(&it);
	}
	if (contadorBienes > 0)
		sprintf(prom->bienes, "%f", sumaBienes / contadorBienes);
	else
		strcpy(prom->bienes, "0");
	if (contadorServicios > 0)
		sprintf(prom->servicios, "%f",
			sumaServicios / contadorServicios);
	else
		strcpy(prom->servicios, "0");
	return;
}
int regNacional(const void *elem)
{
	divisiones *d = (divisiones *)elem;
	return cmpString(d->region, "Nacional") == 0;
}
