#include "iterador.h"

void iteradorCrear(Iterador_t* iterador, Vector_t* vector)
{
    iterador->vec = vector;


    iterador->ini = vector->data;
    iterador->cursor = iterador->ini;
    iterador->fin = vector->data + (vector->cantElem - 1) * vector->tamElem;

    iterador->estadoActual = vector->capacidad;
}

void iteradorDestruir(Iterador_t* iterador)
{
    iterador->vec = NULL;
    iterador->ini = NULL;
    iterador->fin = NULL;
    iterador->estadoActual = 0;
}

int iteradorActualizar(Iterador_t* iterador)
{
    if(iterador->estadoActual != iterador->vec->capacidad || (iterador->fin - iterador->ini) != (){
        iterador->ini = iterador->vec->data;
        iterador->cursor = iterador->ini;
        iterador->fin = iterador->vec->data + (iterador->vec->cantElem - 1) * iterador->vec->tamElem;
        iterador->estadoActual = 
    }
}