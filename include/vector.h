#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*Algoritmos de ordenamiento*/
#define BUBBLE_SORT 0

/*Redimensionamiento del vector*/
#define FACTOR_INCR 2
#define FACTOR_DECR 0.75
#define MAX_THRESHOLD 0.8
#define MIN_THRESHOLD 0.3
#define INCREMENTO 1
#define DECREMENTO 2
#define DEFAULT_CAP 10


/*Codigos de retorno*/
#define EXITO 0
#define ERROR 1
#define LIMITE_DEL_VECTOR 1
#define NO_ENCONTRADO -1
#define ERR_FILE 2

/*Metodos*/
typedef int (*Cmp)(const void*, const void*);

typedef void (*Imprimir)(const void*);

/*Struct vector*/

typedef struct {
    void* data;
    size_t cursor;
    size_t tamElem;
    size_t cantElem;
    size_t cap;
}Vector;


Vector* crearVector(size_t tamElem);

int inicializarVectorDeArchivoTexto(Vector* vec, const char* nomArch);

int grabarVectorEnArchivoBinario(Vector* vec, const char* nomArch);

int insertarArribaEnVector(Vector* vec, void* elem);

int ordenarVector(Vector* vec, Cmp cmpFunc);

int intercambiar(void* a, void* b, size_t tamElem);

void mostrarVector(Vector* vec, Imprimir imprFunc);

int destruirVector(Vector* vec);

int _boundsChecking(size_t bound, int pos);

int _threshold(Vector* vec);

int _redimensionarVector(Vector* vec, int modif);

#endif
