#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <memory.h>
#include <stddef.h>

#define FACTOR_INCR 2

#define DEFAULT_CAP 10

#define EXITO 0
#define ERR_SIN_MEM 1
#define ERR_INPUT 2

typedef struct vector
{
    void* data;
    size_t cantElem;
    size_t tamElem;
    size_t capacidad;    
}Vector_t;

int vectorCrear(Vector_t* vector, size_t tamElem);
int vectorDestruir(Vector_t* vector);

int vectorRedimensionar(Vector_t* vector, size_t nuevaCap);

int vectorLeerDeTexto(Vector_t* vector, const char* nomArch, int (*ParsearTxt)(FILE*, void*));
int vectorEscribirATexto(Vector_t* vector, const char* nomArch, int (*ParsearTxt)(FILE*, void*));

int vectorLeerDeBinario(Vector_t* vector, const char* nomArch);
int vectorEscribirABinario(Vector_t* vector, const char* nomArch);

int vectorInsertar(Vector_t* vector, size_t pos, void* elem);
int vectorEmpujar(Vector_t* vector, void* elem);
int vectorInsertarOrd(Vector_t* vector, void* elem, int (*Cmp)(void*, void*));

int vectorEliminarElem(Vector_t* vector, void* elem, int (*Cmp)(void*, void*));
int vectorOrdEliminarElem(Vector_t* vector, void* elem, int (*Cmp)(void*, void*));
int vectorEliminarPos(Vector_t* vector, size_t pos);

int vectorOrdenar(Vector_t* vector, int (*Cmp)(void*, void*));
size_t vectorBuscar(Vector_t* vector, void* elem, int (*Cmp)(void*, void*));

size_t vectorCantElem(Vector_t* vector);
size_t vectorTamElem(Vector_t* vector);
size_t vectorCapacidad(Vector_t* vector);
bool vectorVacio(Vector_t* vector);

void mostrarVector(Vector_t* vector, void (*Mostrar)(void*));

#endif
