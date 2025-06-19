#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Redimesionamiento*/
#define DEFAULT_CAP 10
#define MAXIMUM_CAP 1000000000 /*1 GB*/
#define INCREMENTO 1
#define FACTOR_INCR 2
#define FACTOR_INCR_FILE 1.25

/*Codigos de retorno*/
#define EXITO 0
#define ERROR 1

/*Funciones genericas*/
typedef void (*Accion)(void*);
typedef int (*Comparar)(const void*, const void*);
typedef void (*Imprimir)(const void*);
typedef void (*FileRead)(FILE*, void*);
typedef void (*FileWrite)(FILE*, void*);


/*Struct vector*/
typedef struct{
    void* data;
    size_t cursor;
    size_t tamElem;
    size_t cantElem;
    size_t cap;
}Vector;

/*Creacion y destruccion*/
Vector* crearVector(Vector* vec, size_t tamElem);
int destruirVector(Vector* vec);

/*Manejo de vectores y archivos*/
int vectorGuardarTexto(Vector* vec, const char* path, FileRead fReadFunc);
int vectorCargarTexto(Vector* vec, const char* path, FileWrite fWriteFunc);
int vectorGuardarBinario(Vector* vec, const char* path);
int vectorCargarBinario(Vector* vec, const char* path);

/*Manejo del cursor*/
int vectorReiniciarCursor(Vector* vec);
int vectorLeer(Vector* vec, void* dest);
int vectorEscribir(Vector* vec, void* src);
int vectorMover(Vector* vec, size_t pos);
size_t vectorPosCursor(Vector* vec);
int vectorCursorAlFinal(Vector* vec);

/*Agregar, eliminar, modificar*/
int vectorAgregar(Vector* vec, void* elem);
int vectorEliminarUltimo(Vector* vec);
void* vectorObtener(Vector* vec, size_t pos);
int vectorModificar(Vector* vec, void* elem, size_t pos);
int vectorVaciar(Vector* vec);

/*Redimensionamiento del vector*/
int vectorRedimensionar(Vector* vec, size_t nuevaCap);

/*Buscar y ordenar*/
int vectorBuscar(Vector* vec, void* clave, Comparar cmpFunc);
int vectorOrdenar(Vector* vec, Comparar cmpFunc);
int intercambiar(void* a, void* b, size_t tamElem);

/*Iterar por el vector*/
int vectorRecorrer(Vector* vec, Accion actFunc);
int vectorMostrar(Vector* vec, Imprimir imprFunc);

/*Extras*/
size_t vectorTam(Vector* vec);
size_t vectorCapacidad(Vector* vec);
int vectorVacio(Vector* vec);

#endif
