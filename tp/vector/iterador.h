#ifndef ITERADOR_H
#define ITERADOR_H

#include "vector.h"

#define ITER_INI 1
#define ITER_ACT 2
#define ITER_FIN 3

typedef struct{
    Vector_t* vec;
    void* ini;
    void* cursor;
    void* fin;
}Iterador_t;

void iteradorCrear(Iterador_t* iterador, Vector_t* vector);
void iteradorDestruir(Iterador_t* iterador);
void iteradorActualizar(Iterador_t* iterador);
void* iteradorSiguiente(Iterador_t* iterador);
int iteradorReiniciar(Iterador_t* iterador);
int iteradorMoverCursor(Iterador_t* iterador, size_t cant, int offset);
size_t iteradorPosCursor(Iterador_t* iterador);
bool iteradorEsFin(Iterador_t* iterador);

#endif