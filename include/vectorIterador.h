#ifndef VECTOR_ITERADOR_H_INCLUDED
#define VECTOR_ITERADOR_H_INCLUDED

#include "vector.h"

#define ORIGEN_INI 0
#define ORIGEN_FIN 1
#define ORIGEN_ACT 2

#define FIN_DE_VECTOR 1

typedef struct{
    Vector* vector;
    void* ini;
    void* fin;
    void* cursor;
}VectorIterador;

int vectorIteradorCrear(VectorIterador* iter, Vector* vec);
int vectorIteradorRango(VectorIterador* iter, size_t iniPos, size_t finPos);
int vectorIteradorMover(VectorIterador* iter, int offset, int origen);
int vectorIteradorLeer(VectorIterador* iter, void* elem);
size_t vectorIteradorPosCursor(VectorIterador* iter);
int vectorIteradorCursorAlFinal(VectorIterador* iter);

#endif