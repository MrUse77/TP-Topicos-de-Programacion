#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "includes/string.h"
#include "includes/vector.h"

#define IPC_DIV "./public/serie_ipc_divisiones.csv"

typedef struct {
	char code[21];
	char desc[35];
	char clasificador[35];
	char indice_ipc[17];
	char v_m_ipc[17];
	char v_a_ipc[17];
	char region[10];
	char periodo[17]; //AAAAMM
} serie_ipc_divisiones;
typedef struct {
	char fecha[11]; //AAAA-MM-DD
	char desc[35];
	char indice_ipc[17];
	char region[10];
	char grupo[10];
} divisiones;
typedef struct {
	int monto;
	char region[10];
	char fechaDesde[17];
	char fechaHasta[17];
} filtroIPC;

void clasificarDivisiones(Vector *B, Vector *S, Vector *v);
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
	Vector Bienes, Servicios;
	vectorCrear(&Bienes, sizeof(divisiones));
	vectorCrear(&Servicios, sizeof(divisiones));
	clasificarDivisiones(&Bienes, &Servicios, &v);

	vectorDestruir(&v);
	return code;
}

void print(const void *elem)
{
	serie_ipc_divisiones *s = (serie_ipc_divisiones *)elem;
	printf("%s;%s;%s;%s;%s;%s;%s;%s\n", s->code, s->desc, s->clasificador,
	       s->indice_ipc, s->v_m_ipc, s->v_a_ipc, s->region, s->periodo);
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
	char *cod = "46871 5032";
	fecha[0] = cod[(int)(*(fecha) - '0')];
	fecha[1] = cod[(int)(*(fecha + 1) - '0')];
	fecha[2] = cod[(int)(*(fecha + 2) - '0')];
	fecha[3] = cod[(int)(*(fecha + 3) - '0')];
	fecha[4] = cod[(int)(*(fecha + 4) - '0')];
	fecha[5] = cod[(int)(*(fecha + 5) - '0')];
}
void formatearFecha(char *c)
{
	char meses[12][10] = {
		"Enero",      "Febrero", "Marzo",     "Abril",
		"Mayo",	      "Junio",	 "Julio",     "Agosto",
		"Septiembre", "Octubre", "Noviembre", "Diciembre"
	};
	char mesReg[3];
	strncpy(mesReg, c + 4, 2);
	int mes = atoi(mesReg) - 1;
	//AAAA MM
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
void clasificarDivisiones(Vector *B, Vector *S, Vector *v)
{
	char **Bienes = malloc(sizeof(char *) * 5);
	Bienes = (char *[]){ "Alimentos y bebidas no alcoholicas",
			     "Bebidas alcoholicas y tabaco",
			     "Prendas de vestir y calzado",
			     "Bienes y servicios varios",
			     "Equipamiento y mantenimiento del hogar" };
	char **Servicios = malloc(sizeof(char *) * 10);
	Servicios = (char *[]){ "Recreacion y cultura",
				"Restaurantes y hoteles",
				"Salud",
				"Transporte",
				"Educacion",
				"Comunicacion",
				"Vivienda",
				"agua",
				"electricidad",
				"gas y otros combustibles" };
	serie_ipc_divisiones *s = NULL;
	VectorIterador it;
	vectorIteradorCrear(&it, v);
	divisiones div;
	s = (serie_ipc_divisiones *)vectorIteradorPrimero(&it);
	while (!vectorIteradorFin(&it)) {
		if (includeString(s->desc, (const char **)Bienes, 5)) {
			/*
			strcpy(div.desc, s->desc);
			strcpy(div.indice_ipc, s->indice_ipc);
			strcpy(div.region, s->region);
			strcpy(div.fecha, s->periodo);
			strcpy(div.grupo, "Bienes");
			vectorInsertar(B, &div);
			*/
			printf("Bienes: %s\n", s->desc);
		} else if (includeString(s->desc, (const char **)Servicios,
					 10)) {
			/*
			strcpy(div.desc, s->desc);
			strcpy(div.indice_ipc, s->indice_ipc);
			strcpy(div.region, s->region);
			strcpy(div.fecha, s->periodo);
			strcpy(div.grupo, "Servicios");
			vectorInsertar(S, &div);
			*/
			printf("Servicios: %s\n", s->desc);
		}
		s = (serie_ipc_divisiones *)vectorIteradorSiguiente(&it);
	}
}
