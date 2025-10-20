#include "includes/comun.h"
#include "includes/string.h"
#include "includes/divisiones.h"
#include "includes/vector.h"

static void seleccionarPeriodoDivisiones(void *f)
{
	char *fechaDesde = ((filtroIPC *)f)->fechaDesde;
	char *fechaHasta = ((filtroIPC *)f)->fechaHasta;
	printf("Ingrese una fecha desde (AAAAMM): ");
	scanf(" %s", fechaDesde);
	printf("Ingrese una fecha hasta (AAAAMM): ");
	scanf(" %s", fechaHasta);
}
static void seleccionarRegionDivisiones(void *f)
{
	char *region = ((filtroIPC *)f)->region;
	printf("Seleccione la region: \n");
	printf("1. Nacional\n");
	printf("2. GBA\n");
	printf("3. Pampeana\n");
	printf("4. Cuyo\n");
	printf("5. Noroeste\n");
	printf("6. Noreste\n");
	printf("7. Patagonia\n");
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
static void seleccionarMontoDivisiones(void *f)
{
	int *monto = &((filtroIPC *)f)->monto;
	printf("Selecciona el monto: ");
	scanf("%d", monto);
}
static bool filtrarDivisiones(const void *elem, char *grupo)
{
	const char *codeB[BIENES] = { "01", "02", "03", "12", "05" };
	const char *codeS[SERVICIOS] = { "04", "06", "07", "08",
					 "09", "10", "11" };
	if (includeString(((serie_ipc_divisiones *)elem)->code, codeB,
			  BIENES)) {
		cpyString(grupo, "Bienes");
		return true;
	} else if (includeString(((serie_ipc_divisiones *)elem)->code, codeS,
				 SERVICIOS)) {
		cpyString(grupo, "Servicios");
		return true;
	}
	return false;
}
static int regNacional(const void *elem)
{
	indices *i = (indices *)elem;
	return cmpString(i->region, "Nacional") == 0;
}
static void printIPC(const void *elem)
{
	indices *i = (indices *)elem;
	printf("%s;%s;%f;%f\n", i->fecha, i->region, i->bienes, i->servicios);
}
static void printNacional(const void *elem)
{
	if (cmpString(((indices *)elem)->region, "Nacional") == 0) {
		printIPC(elem);
	}
}
void formatearDivisiones(char *c, void *elem)
{
	removerCharEnString(c, '"');
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

/***** actualizarMontoDivisiones ******/
void calcularIPC(char *sDesde, char *sHasta, int monto)
{
	float pDesde = atof(sDesde);
	float pHasta = atof(sHasta);
	float montoAjustado = monto * (pHasta / pDesde);
	float variacion = (pHasta / pDesde - 1) * 100;
	printf("El monto ajustado con una variacion del %f%% es de: %f\n",
	       variacion, montoAjustado);
}
void generarHerramienta(filtroIPC filtro, Vector *v)
{
	serie_ipc_divisiones *s;
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	s = (serie_ipc_divisiones *)vectorIteradorPrimero(&it);
	int encontrados = 0;
	char sDesde[PERIODO_DIV_TAM], sHasta[PERIODO_DIV_TAM];

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
	calcularIPC(sDesde, sHasta, filtro.monto);
}
void actualizarMontoDivisiones(Vector *v)
{
	filtroIPC filtro;
	menu(&filtro, seleccionarMontoDivisiones, seleccionarRegionDivisiones,
	     seleccionarPeriodoDivisiones);
	generarHerramienta(filtro, v);
}

/***** calcularIPCPromedio ******/
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

	//	vectorEliminarPorFiltro(&vIpc, regNacional);
	vectorMostrar(&vIpc, printNacional);

	vectorDestruir(&vAc);
	return;
}
void clasificarDivisiones(Vector *D, Vector *v, Clasificar c)
{
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	divisiones div;
	serie_ipc_divisiones *s =
		(serie_ipc_divisiones *)vectorIteradorPrimero(&it);
	while (!vectorIteradorFin(&it)) {
		if (c(s, div.grupo)) {
			cpyString(div.desc, s->desc);
			cpyString(div.indice_ipc, s->indice_ipc);
			cpyString(div.region, s->region);
			cpyString(div.fecha, s->periodo);
			vectorInsertar(D, &div);
		}
		s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
	}
}
void calcularIPCPromedio(Vector *v)
{
	Vector Divisiones;
	vectorCrear(&Divisiones, sizeof(divisiones));
	clasificarDivisiones(&Divisiones, v, filtrarDivisiones);
	calcularPromedios(&Divisiones, printIPC);
	vectorEliminarPorFiltro(&Divisiones, regNacional);
	vectorDestruir(&Divisiones);
}
