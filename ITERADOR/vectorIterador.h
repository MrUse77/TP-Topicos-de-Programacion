#ifndef VECTORITERADOR_H_INCLUDED
#define VECTORITERADOR_H_INCLUDED
#include "TDAvector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*
typedef struct
{
    void* pri;
    void* ult;
    void* act;
    bool finIter;
    size_t tamElem;
}
VectorIterador;*/

typedef struct
{
    void* act;
    void *ult;
    const Vector *v;
    bool finIter;
}
VectorIterador;

void vectorIteradorCrear(VectorIterador* it, const Vector* v);
void* vectorIteradorPrimero(VectorIterador* it);
void* vectorIteradorUltimo(VectorIterador* it);
void* vectorIteradorSiguiente(VectorIterador* it);
void* vectorIteradorAnterior(VectorIterador* it);
void* vectorIteradorActual(VectorIterador* it);
bool vectorIteradorFinIter(VectorIterador* it);
void* vectorIteradorDesplazamiento(VectorIterador *it, int cantidad);
#endif // VECTORITERADOR_H_INCLUDED
