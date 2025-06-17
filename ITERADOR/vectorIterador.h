#ifndef VECTORITERADOR_H_INCLUDED
#define VECTORITERADOR_H_INCLUDED
#include "TDAvector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    void* pri;
    void* ult;
    void* act;
    bool finIter;
    size_t tamElem;
}
VectorIterador;


void vectorIteradorCrear(VectorIterador* it, Vector* v);
void* vectorIteradorPrimero(VectorIterador* it);
void* vectorIteradorUltimo(VectorIterador* it);
void* vectorIteradorSiguiente(VectorIterador* it);
void* vectorIteradorAnterior(VectorIterador* it);
void* vectorIteradorActual(VectorIterador* it);
bool vectorIteradorFinIter(VectorIterador* it);

#endif // VECTORITERADOR_H_INCLUDED
