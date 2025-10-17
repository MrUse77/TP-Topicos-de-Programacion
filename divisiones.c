#include "includes/string.h"
#include "includes/divisiones.h"
#include "includes/vector.h"

void calcularIPC(char *sDesde, char *sHasta, int monto);
void formatearFecha(char *c);
void seleccionarRegion(char *region);
void promedio(Vector *D, float *grupo, float suma, int count);
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
				cpyString(sDesde, s->indice_ipc);
				encontrados++;
			} else if (cmpString(s->periodo, filtro.fechaHasta) ==
				   0) {
				printf("Encontrado hasta: %s\n", s->periodo);
				cpyString(sHasta, s->indice_ipc);
				encontrados++;
			}
		}
		s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
	}
	puts(sDesde);
	puts(sHasta);
	calcularIPC(sDesde, sHasta, filtro.monto);
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
void calcularPromedioMensual(Vector *S, Vector *B, Fmt print)
{
	Vector vIpc;
	vectorCrear(&vIpc, sizeof(indices));

	VectorIterador itB, itS;
	vectorIteradorCrear(&itB, B);
	vectorIteradorCrear(&itS, S);
	divisiones *b = (divisiones *)vectorIteradorPrimero(&itB);
	divisiones *s = (divisiones *)vectorIteradorPrimero(&itS);
	char fechaActualB[18];
	char fechaActualS[18];
	while (!vectorIteradorFin(&itB) && !vectorIteradorFin(&itS)) {
		cpyString(fechaActualB, b->fecha);
		cpyString(fechaActualS, s->fecha);
		float sumaB = 0, sumaS = 0;
		int countB = 0, countS = 0;
		indices ipc;
		cpyString(ipc.fecha, b->fecha);
		cpyString(ipc.region, b->region);
		while (b && strcmp(b->fecha, fechaActualB) == 0) {
			countB++;
			sumaB += atof(b->indice_ipc);
			b = (divisiones *)vectorIteradorSiguiente(&itB);
		}
		while (s && strcmp(s->fecha, fechaActualS) == 0) {
			countS++;
			sumaS += atof(s->indice_ipc);
			s = (divisiones *)vectorIteradorSiguiente(&itS);
		}
		promedio(B, &ipc.bienes, sumaB, countB);
		promedio(S, &ipc.servicios, sumaS, countS);
		vectorInsertar(&vIpc, &ipc);
		//b = (divisiones *)vectorIteradorSiguiente(&itB);
		//s = (divisiones *)vectorIteradorSiguiente(&itS);
	}

	vectorMostrar(&vIpc, print);

	vectorDestruir(&vIpc);
	return;
}
void clasificarDivisiones(Vector *D, Vector *v, const char **filter,
			  size_t sizeFiltro, const char *grupo)
{
	serie_ipc_divisiones *s = NULL;
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	divisiones div;
	s = (serie_ipc_divisiones *)vectorIteradorPrimero(&it);
	while (!vectorIteradorFin(&it)) {
		if (includeString(s->code, filter, sizeFiltro)) {
			cpyString(div.desc, s->desc);
			cpyString(div.indice_ipc, s->indice_ipc);
			cpyString(div.region, s->region);
			cpyString(div.fecha, s->periodo);
			cpyString(div.grupo, grupo);
			vectorInsertar(D, &div);
		}
		s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
	}
}
void seleccionarRegion(char *region)
{
	int opcion = 0;
	do {
		printf("Ingrese una opcion (1-7): ");
		scanf("%d", &opcion);
		switch (opcion) {
		case 1:
			cpyString(region, "Nacional");
			break;
		case 2:
			cpyString(region, "GBA");
			break;
		case 3:
			cpyString(region, "Pampeana");
			break;
		case 4:
			cpyString(region, "Cuyo");
			break;
		case 5:
			cpyString(region, "Noroeste");
			break;
		case 6:
			cpyString(region, "Noreste");
			break;
		case 7:
			cpyString(region, "Patagonia");
			break;
		default:
			printf("Opcion invalida. Intente de nuevo.\n");
			break;
		}
	} while (opcion < 1 || opcion > 7);
}
void formatearFecha(char *c)
{
	char meses[12][11] = {
		"Enero",      "Febrero", "Marzo",     "Abril",
		"Mayo",	      "Junio",	 "Julio",     "Agosto",
		"Septiembre", "Octubre", "Noviembre", "Diciembre"
	};
	char mesReg[3];
	cpyNString(mesReg, c + 4, 2);
	int mes = atoi(mesReg) - 1;
	//AAAAMM
	size_t len = lenString(meses[mes]);
	char anio[5];
	strncpy(anio, c, 4);
	//MMMMMMMMMM-AAAA
	memcpy(c, meses[mes], len);
	c += len;
	*c = '-';
	cpyString(c + 1, anio);
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
void promedio(Vector *D, float *grupo, float suma, int count)
{
	*grupo = suma / count;
}
