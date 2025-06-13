#include "vector.h"
#include <stdlib.h>
#include <string.h>

Vector* crearVector(Vector* vec, size_t cantInicial, size_t tamElem)
{
    vec->data = malloc(tamElem * cantInicial);

    if(!(vec->data)) return NULL;

    vec->cantElem = cantInicial;
    vec->tamElem = tamElem;
    vec->cap = (cantInicial > vec->cap) ? cantInicial : MINIMUM_CAP;

    return vec;
}

int inicializarVector(Vector* vec, void* elem)
{
    size_t i, tamElem = vec->tamElem;
    char* tmpVec = vec->data;
    char* tmpElem = elem;

    /*if(_chequeoDeTamAux(elem, tamElem)) return ERROR;*/

    for(i = 0; i < vec->cantElem; i++){
        memcpy(tmpVec, tmpElem, tamElem);
        tmpVec += tamElem;
    }

    return EXITO;
}

/*A terminar*/
int inicializarVectorDeArchivo(Vector* vec, FILE* arch, size_t archTam)
{
    return EXITO;
}

/*A terminar*/
int grabarVectorEnArchivo(Vector* vec, FILE* arch, size_t archTam)
{
    return EXITO;
}

void* pop(Vector* vec, void* elem, int pos)
{
    /*Agregar bounds checking*/
    
    if(pos < 0 || pos >= vec->cantElem) return NULL;
    
    char* tmpData = vec->data;
    char* tmpElem = elem;
    char* tmpRet;
    
    tmpData += pos * vec->tamElem;
    memcpy(tmpRet, tmpData, vec->tamElem);
    memcpy(tmpData, tmpElem, vec->tamElem);
    
    return tmpRet;
}

/*A terminar*/
int empujarEnVector(Vector* vec, void* elem)
{
    size_t i;
    char* tmpData = vec->data;
    char* tmpElem = elem;
    char* tmpAux;
    
    _threshold(vec);
    
//    memcpy(vec->data, tmpElem, vec->tamElem);
  
    tmpAux = pop(vec, tmpElem, 0);
    
    for(i = 1; i < vec->cantElem; i++){
        tmpAux = pop(vec, tmpAux, i);
    }
    
    return EXITO;
}

/*A terminarr*/
int apilarEnVector(Vector* vec, void* elem)
{
    if(_chequeoDeTamAux(elem, vec->tamElem)) return ERROR;




    return EXITO;
}

/*A terminar*/
int insertarEnVectorAsc(Vector* vec, void* elem, Cmp cmpFunc)
{
    return EXITO;
}

/*A terminar*/
int insertarEnVectorDsc(Vector* vec, void* elem, Cmp cmpFunc)
{
    return EXITO;
}

/*A terminar*/
int insertarEnVectorPos(Vector* vec, void* elem, int pos)
{
    return EXITO;
}

/*A terminar*/
int eliminarEnVector(Vector* vec, void* elem, Cmp cmpFunc)
{
    return EXITO;
}

int eliminarEnVectorPos(Vector* vec, int pos)
{
    return EXITO;
}

void mostrarVector(Vector* vec, Imprimir imprFunc)
{
    size_t i;
    char* tmp = vec->data;

    for(i = 0; i < vec->cantElem; i++){
        imprFunc((void*) tmp + i * vec->tamElem);
    }
}

int destruirVector(Vector* vec)
{
    free(vec->data);

    vec->tamElem = 0;
    vec->cantElem = 0;
    vec->cap = 0;

    return EXITO;
}

int _chequeoDeTamAux(void* a, size_t tam)
{
    char* tmp = a;

    if(sizeof(tmp) != tam) return ERROR;

    return EXITO;
}

int _threshold(Vector* vec)
{
    if(vec->cantElem == vec->cap){
        _redimensionarVector(vec, INCREMENTO);
    }else if(vec->cantElem / vec->cap <= MIN_THRESHOLD){
        _redimensionarVector(vec, DECREMENTO);
    }

    return EXITO;
}

int _redimensionarVector(Vector* vec, int modif)
{
    void* tmpData;
    size_t tmpCap;

    switch(modif){

    case 1:
        tmpData = realloc(vec->data, vec->cap * FACTOR_INCR);
        tmpCap = vec->cap * FACTOR_INCR;
    case 2:
        tmpData = realloc(vec->data, vec->cap * FACTOR_DECR);
        tmpCap = vec->cap * FACTOR_DECR;
    default:
        return ERROR;
    }

    if(!tmpData) return ERROR;

    vec->data = tmpData;
    vec->cap = tmpCap;

    return EXITO;
}

/*
int _chequeDeCapAux(size_t cantElem, size_t cap)
{
    if(cantElem == cap) return LIMITE_DEL_VECTOR;

    return EXITO;
}*/
