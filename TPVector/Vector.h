#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>
#include "../TPComun/TPComun.h"

#define CAP_INICIAL 10

#define AUMENTAR 1
#define DISMINUIR 2

#define FACT_INCR 1.5
#define FACT_DECR 0.5
#define FACTOR_OCUP 0.25

#define BURBUJEO 0
#define SELECCION 1
#define INSERCION 2

typedef struct {
    void* vec;
    size_t cantElem;
    size_t tamElem;
    size_t cap;
} Vector;

typedef struct {
    void* act;
    void* ult;
    Vector* vector;
    bool finIter;
} VectorIterador;

typedef void (*Imp) (const void* elem);
typedef int (*Cmp) (const void* a, const void* b);
typedef int (*Fmt) (void* reg, char* buffer);
typedef void (*Accion) (void* elem, void* datos);

int vectorCrear (Vector* v, size_t tamElem);
int vectorCargarDeTxt (Vector* v, const char* nomArchTxt, Fmt formatear, size_t bufferTam);
int vectorCargarDeBin (Vector* v, const char* nomArchBin);
int vectorGuardarEnBin (Vector* v, const char* nomArchBin);
int vectorOrdInsertar (Vector* v, void* elem, Cmp comparar);
int vectorInsertarAlFinal (Vector* v, void* elem);
bool vectorEliminarDePos (Vector* v, int pos);
bool vectorOrdEliminar (Vector* v, void* elem, Cmp comparar);
void vectorMostrar (Vector* v, Imp imprimir);
int vectorDesordBuscar (Vector* v, void* elem, Cmp comparar);
int vectorOrdBuscar(const Vector* v, void* elem, Cmp comparar);
int vectorUnir (Vector* v1, Vector* v2);
int vectorOrdenar (Vector* v, int metodo, Cmp comparar);
void vectorDestruir (Vector* v);


/* Vector iterador */

void vectorIteradorCrear (VectorIterador* it, const Vector* v);
void* vectorIteradorPrimero (VectorIterador* it);
void* vectorIteradorSiguiente (VectorIterador* it);
void* vectorIteradorDesplazamiento (VectorIterador *it, size_t cantidad);
bool vectorIteradorFin (VectorIterador* it);

#endif
