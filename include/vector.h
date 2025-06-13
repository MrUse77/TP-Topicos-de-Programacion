#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BUBBLE_SORT 0
/*
#define SELECTION_SORT 1
#define INSERTION_SORT 2
#define QUICK_SORT 3
*/

#define FACTOR_INCR 2
#define FACTOR_DECR 0.75
#define MIN_THRESHOLD 0.3
#define INCREMENTO 1
#define DECREMENTO 2

#define EXITO 0
#define ERROR 1
#define LIMITE_DEL_VECTOR 1
#define NO_ENCONTRADO -1

typedef int (*Cmp)(const void*, const void*);
typedef void (*Accion)(void*);
typedef void (*Imprimir)(const void*);

#define MINIMUM_CAP 10

typedef struct {
    void* data;
    size_t tamElem;
    size_t cantElem;
    size_t cap;
}Vector;

Vector* crearVector(Vector* vec, size_t cantInicial, size_t tamElem);
int inicializarVector(Vector* vec, void* elem);


int inicializarVectorDeArchivo(Vector* vec, const char* nomArch, size_t archTam);
int grabarVectorEnArchivo(Vector* vec, FILE* arch, size_t archTam);

/*
void* pop(Vector* vec, void* elem, int pos);
Empuja un elemento elem al principio de vec
int empujarEnVector(Vector* vec, void* elem);
Apila un elemento elem al final de vec
int apilarEnVector(Vector* vec, void* elem);
*/

/*Inserta un elemento elem de manera ascendente en el vector vec respetando cmpFunc*/
int insertarEnVectorAsc(Vector* vec, void* elem, Cmp cmpFunc);
/*Inserta un elemento elem de manera descendente en el vector vec respetando cmpFunc*/
int insertarEnVectorDsc(Vector* vec, void* elem, Cmp cmpFunc);
/*Inserta un elemento elem en el indice pos del vector vec, sobreescribiendo el valor anterior*/
int insertarEnVectorPos(Vector* vec, void* elem, int pos);

/*
Elimina el elemento elem del vector vec respetando a cmpFunc, de no
ser encontrado regresa NO_ENCONTRADO
*/
int eliminarEnVector(Vector* vec, void* elem, Cmp cmpFunc);

/*
Elimina el elemento ubicado en el indice pos, de no ser valido pos
regresa ERROR
*/
int eliminarEnVectorPos(Vector* vec, int pos);

/*Ordena el vector vec siguiendo el metodo metodo*/
int ordenarVector(Vector* vec, int metodo);
/*
Realiza una busqueda lineal en el vector vec, retorna la posicion de elem
en vec si es encontrado o -1 si no lo es
*/
int busquedaLinealEnVector(Vector* vec, void* elem, Cmp cmpFunc);

/*
Realiza una busqueda binaria en el vector vec, retorna la posicion de elem
en vec si es encontrado 0 -1 si no lo es
*/
int busquedaBinariaEnVector(Vector* vec, void* elem, Cmp cmpFunc);

/*
Muestra en la consola los contenidos del vector vec
respetando a imprFunc
*/
void mostrarVector(Vector* vec, Imprimir imprFunc);
/*Libera la memoria dinamica del vector vec, y resetea toda su informacion*/
int destruirVector(Vector* vec);

int _boundsChecking(size_t bound, int pos);
int _threshold(Vector* vec);
int _redimensionarVector(Vector* vec, int modif);

#endif
