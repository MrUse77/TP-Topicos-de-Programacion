#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED
void leerLineaItems(const char* linea, void *pos);
void leerLineaGeneral(const char* linea, void *pos);

typedef void (*leerLinea)(const char*, void*);
typedef void (*cambiaValor)(void*);
typedef void (*eligeEstructura)(void *elem);
typedef int (*Cmp)(const void *e1, const void *e2);

int archivoAVector(const char* nombreArchivo, Vector *v, leerLinea leer, int tamLinea);
void reemplazar_coma(char *s);
void cambia_Items(char *nG);
void cambia_general(char *val);
void DesencriptarItems(Vector *v);

void DesencriptarGeneral(Vector *v);
void normalizar_general(Vector *v);
void normalizarItems(Vector *v);
int cambiarFecha(char *per);

void formatoPeriodo(Vector *v, eligeEstructura elegir);

void clasificadorGeneral(Vector *v);
void clasificadorItems(Vector *v);
int clasificar(const char *clas);
void llenarCampo(char *clas, int valor);

void eligeGeneral(void *elem);
void eligeItems(void *elem);

int unificar(Vector *unif, Vector *v1, Vector *v2);
int prioridad(const void *e1);

int cantidad_indices(Vector *v);
void varMensual(Vector *v);
void varAnual(Vector *v);

int unificarResult(Vector *unif, const char* nombreArchivo);
void mostrarResultsBin(const char *nombreArchivo);

int cmpPeriodo(const void *e1, const void *e2);
int cmpClasificador(const void *e1, const void *e2);

#endif // FUNCIONES_H_INCLUDED
