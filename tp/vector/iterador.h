#ifndef ITERADOR_H
#define ITERADOR_H

#include "vector.h"

typedef struct{
    Vector_t* vec;
    size_t estadoActual;  //Como joraca hago esto?
    void* ini;
    void* cursor;
    void* fin;
}Iterador_t;

void iteradorCrear(Iterador_t* iterador, Vector_t* vector);
void iteradorDestruir(Iterador_t* iterador);
int iteradorActualizar(Iterador_t* iterador);
void* iteradorSiguiente(Iterador_t* iterador);
int iteradorReiniciar(Iterador_t* iterador);
int iteradorMoverCursor(Iterador_t* iterador, size_t pos);
size_t iteradorPosCursor(Iterador_t* iterador);
bool iteradorEsFin(Iterador_t* iterador);

#endif