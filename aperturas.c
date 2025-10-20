#include "includes/aperturas.h"
#include "includes/comun.h"
#include "includes/string.h"
#include "includes/vector.h"

static void seleccionarRegionAperturas(void *f)
{
	char *region = ((filtroIPC *)f)->region;
	printf("Seleccione la region: \n");
	printf("1. GBA\n");
	printf("2. Pampeana\n");
	printf("3. Cuyo\n");
	printf("4. Noroeste\n");
	printf("5. Noreste\n");
	printf("6. Patagonia\n");
	int opcion = 0;
	do {
		printf("Ingrese una opcion (1-6): ");
		scanf("%d", &opcion);
		switch (opcion) {
		case 1:
			cpyString(region, "GBA");
			break;
		case 2:
			cpyString(region, "Pampeana");
			break;
		case 3:
			cpyString(region, "Cuyo");
			break;
		case 4:
			cpyString(region, "Noroeste");
			break;
		case 5:
			cpyString(region, "Noreste");
			break;
		case 6:
			cpyString(region, "Patagonia");
			break;
		default:
			printf("Opcion invalida. Intente de nuevo.\n");
			break;
		}
	} while (opcion < 1 || opcion > 7);
}
static void seleccionarPeriodoAperturas(void *f)
{
	char *fechaDesde = ((filtroIPC *)f)->fechaDesde;
	printf("Ingrese una fecha desde (AAAAMM): ");
	scanf(" %s", fechaDesde);
}
static void seleccionarMontoApeturas(void *f)
{
	int *monto = &((filtroIPC *)f)->monto;
	printf("Seleccione el monto a actualizar del alquiler: ");
	scanf("%d", monto);
}

void formatearAperturas(char *c, void *elem)
{
	serie_ipc_aperturas *s = (serie_ipc_aperturas *)elem;
	char *act = buscarCharEnStringEnReversa(c, '\n');

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
	cpyString(s->periodo, act + 1);
	formatearFechaAperturas(s->periodo);

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

void clasificarAperturas(Vector *v, filtroIPC *f, Vector *vA)
{
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	serie_ipc_aperturas *s =
		(serie_ipc_aperturas *)vectorIteradorPrimero(&it);
	aperturas a;
	float indiceDesde;
	char *fechaDesde = formatearFechaAperturas(f->fechaDesde);

	while (!vectorIteradorFin(&it)) {
		if (s) {
			if (cmpString(s->region, f->region) == 0 &&
			    cmpString(s->code, "04.1.1") == 0) {
				cpyString(a.periodo, s->periodo);
				a.indice = atof(s->indice_ipc);
				if (cmpString(s->periodo, fechaDesde) == 0) {
					a.variacion = 0;
					a.montoAjustado = f->monto;
					indiceDesde = atof(s->indice_ipc);
					vectorInsertar(vA, &a);
				} else if (cmpString(s->periodo, fechaDesde) >
					   0) {
					a.montoAjustado = f->monto *
							  (atof(s->indice_ipc) /
							   indiceDesde);
					a.variacion = ((atof(s->indice_ipc) /
							indiceDesde) -
						       1) *
						      100;
					vectorInsertar(vA, &a);
				}
			}
		}
		s = (serie_ipc_aperturas *)vectorIteradorSiguiente(&it);
	}
}
void calcularPromedioAlquiler(Vector *v, Vector *vA)
{
	filtroIPC f;
	menu(&f, seleccionarMontoApeturas, seleccionarRegionAperturas,
	     seleccionarPeriodoAperturas);
	clasificarAperturas(v, &f, vA);
	vectorMostrar(vA, printAperturas);
}
char *formatearFechaAperturas(char *c)
{
	char anio[5];
	cpyNString(anio, c, 4);
	sprintf(c, "%04d-%02d-01", atoi(anio), atoi(c + 4));
	return c;
}
void printAperturas(const void *elem)
{
	aperturas *a = (aperturas *)elem;
	printf("%s, %2f, %2f, %2f\n", a->periodo, a->indice, a->variacion,
	       a->montoAjustado);
}
