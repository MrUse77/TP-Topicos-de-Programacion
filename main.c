#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "includes/string.h"
#include "includes/vector.h"

#define IPC_DIV "./public/serie_ipc_divisiones.csv"

typedef struct {
	char code[21];
	char desc[31];
	char clasificador[31];
	char indice_ipc[17];
	char v_m_ipc[17];
	char v_a_ipc[17];
	char region[10];
	char periodo[17]; //AAAAMM
} serie_ipc;

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
	VectorIterador it;
	code = vectorCrear(&v, sizeof(serie_ipc));
	vectorIteradorCrear(&it, &v);
	code = vectorInsertarDeArchivoTXT(&v, f, formatear, 2);

	serie_ipc *s = (serie_ipc *)vectorIteradorPrimero(&it);
	while (!vectorIteradorFin(&it)) {
		formatearFecha(s->periodo);
		s = (serie_ipc *)vectorIteradorSiguiente(&it);
	}
	vectorMostrar(&v, print);

	vectorDestruir(&v);
	return code;
}

void print(const void *elem)
{
	serie_ipc *s = (serie_ipc *)elem;
	printf("%s;%s;%s;%s;%s;%s;%s;%s\n", s->code, s->desc, s->clasificador,
	       s->indice_ipc, s->v_m_ipc, s->v_a_ipc, s->region, s->periodo);
}
// TODO: Solucionar cpyString de mi propia libreria de string
void formatear(char *c, void *elem)
{
	serie_ipc *s = (serie_ipc *)elem;

	char *act = buscarCharEnStringEnReversa(c, '\n');
	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	char aux[17];
	strcpy(aux, act + 1);
	formatearFecha(aux);
	strcpy(s->periodo, aux);

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

	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->clasificador, act + 1);

	*act = '\0';
	act = buscarCharEnStringEnReversa(c, ';');
	strcpy(s->desc, act + 1);

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
	decodificarFecha(c);
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
