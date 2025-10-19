#include "includes/string.h"
#include "includes/divisiones.h"
#include "includes/vector.h"

int buscarIndiceEnVector(Vector *v, char *fecha, char *region);
int cmpDivisionesPorFechaYRegion(const void *a, const void *b);
int promedioMensual(VectorIterador *it, indices *ipc, divisiones *d);
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
void calcularPromedios(Vector *D, Fmt print)
{
	Vector vAc, vIpc;
	vectorCrear(&vAc, sizeof(acumuladorRegion));
	vectorCrear(&vIpc, sizeof(indices));
	VectorIterador it;
	vectorIteradorCrear(&it, D);
	divisiones *d = (divisiones *)vectorIteradorPrimero(&it);
	vectorIteradorPrimero(&it);

	while (!vectorIteradorFin(&it)) {
		int pos = buscarIndiceEnVector(&vAc, d->fecha, d->region);
		acumuladorRegion *ac = (acumuladorRegion *)vectorGet(&vAc, pos);
		float val = atof(d->indice_ipc);
		if (cmpString(d->grupo, "Bienes") == 0) {
			ac->bienes += val;
			ac->cantidadB++;
		} else if (cmpString(d->grupo, "Servicios") == 0) {
			ac->servicios += val;
			ac->cantidadS++;
		}
		d = (divisiones *)vectorIteradorSiguiente(&it);
	}

	vectorIteradorCrear(&it, &vAc);
	acumuladorRegion *ac = (acumuladorRegion *)vectorIteradorPrimero(&it);

	while (!vectorIteradorFin(&it)) {
		indices ipc = { 0 };
		cpyString(ipc.fecha, ac->fecha);
		cpyString(ipc.region, ac->region);
		ipc.bienes = ac->cantidadB > 0 ? ac->bienes / ac->cantidadB : 0;
		ipc.servicios =
			ac->cantidadS > 0 ? ac->servicios / ac->cantidadS : 0;
		vectorInsertar(&vIpc, &ipc);
		ac = (acumuladorRegion *)vectorIteradorSiguiente(&it);
	}

	vectorMostrar(&vIpc, print);

	vectorDestruir(&vAc);
	return;
}
int buscarIndiceEnVector(Vector *v, char *fecha, char *region)
{
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	acumuladorRegion *ac = (acumuladorRegion *)vectorIteradorPrimero(&it);
	int pos = 0;
	while (!vectorIteradorFin(&it)) {
		if (cmpString(ac->fecha, fecha) == 0 &&
		    cmpString(ac->region, region) == 0) {
			return pos;
		}
		ac = (acumuladorRegion *)vectorIteradorSiguiente(&it);
		pos++;
	}
	acumuladorRegion nuevo = { 0 };
	cpyString(nuevo.fecha, fecha);
	cpyString(nuevo.region, region);
	vectorInsertar(v, &nuevo);
	return vectorCE(v) - 1;
}
void clasificarDivisiones(Vector *D, Vector *v, Clasificar c, Cmp cmp)
{
	serie_ipc_divisiones *s = NULL;
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	divisiones div;
	s = (serie_ipc_divisiones *)vectorIteradorPrimero(&it);
	while (!vectorIteradorFin(&it)) {
		if (c(s, div.grupo)) {
			cpyString(div.desc, s->desc);
			cpyString(div.indice_ipc, s->indice_ipc);
			cpyString(div.region, s->region);
			cpyString(div.fecha, s->periodo);
			//		vectorOrdInsertar(D, &div, cmp);
			vectorInsertar(D, &div);
		}
		s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
	}
	//vectorOrdenar(D, INSERCION, cmp);
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
int promedioMensual(VectorIterador *it, indices *ipc, divisiones *d)
{
	int categorias = 0;
	int cReg = 0;
	int suma = 0;
	while (d && strcmp(d->fecha, ipc->fecha) == 0) {
	}
	return categorias > 0 ? suma / categorias : 0;
}
