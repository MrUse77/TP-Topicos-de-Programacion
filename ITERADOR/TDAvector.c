#include "TDAvector.h"
#include <stdbool.h>
#define FACTOR_INCR 1.5
#define OK 0
#define ERROR_REALLOC 2
bool vectorCrear(Vector* v, size_t capacidad, size_t tamElem)
{
    v->vec = malloc(capacidad*tamElem);

    if(!v->vec)
    {
        return false;
    }

    v->capacidad = capacidad;
    v->tamElem = tamElem;
    v->ce = 0;
    return true;
}

bool vectorRedimensionar(Vector* v, size_t capacidad)
{
    void *nV = realloc(v->vec, capacidad * v->tamElem); // Lo guardamos en otras variables para no arriesgarnos a que borre el puntero

    if (!nV)
    {
        return false;
    }

    v->vec = nV;
    v->capacidad = capacidad;
    return true;
}

void intercambiar(void* a, void* b, size_t tamElem)
{
    void* aTemp = malloc(tamElem); // Inicializamos el puntero auxiliar
    memmove(aTemp, a, tamElem); // aTemp = a
    memmove(a, b, tamElem); // a = b
    memmove(b, aTemp, tamElem); // b = a
    free(aTemp);
}

bool vectorInsertarFinal(Vector* v, const void* elem)
{
    if(v->ce == v->capacidad)
    {
        if(!vectorRedimensionar(v, v->capacidad * FACTOR_INCR))
        {
            return false;
        }
    }


    memcpy(v->vec + v->ce*(v->tamElem), elem, v->tamElem);
    (v->ce)++;
    return true;
}

int ordenarInsercion(Vector *v, Cmp cmp)
{
    void *i, *j, *ult = v->vec + v->tamElem * v->ce;
    void *aux = malloc(v->tamElem);
    if (!aux)
    {
        return ERROR_REALLOC;
    }

    for (i = v->vec + v->tamElem; i < ult; i += v->tamElem)
    {
        memmove(aux,i,v->tamElem); // Guarda el elemento aux en posición i
        j = i - v->tamElem;
        while (j >= v->vec && cmp(aux,j)<0) // Vamos recorriendo desde i para la izquierda y viendo cuándo un elemento es <= aux
        {
            memmove(j+v->tamElem, j, v->tamElem); // Mientras un elemento no sea <= aux, los vamos moviendo a la derecha
            j -= v->tamElem;
        }
        // Una vez hayamos encontrado un elemento <= aux, significa que podemos insertarlo y mantener el orden
        memmove(j+v->tamElem, aux, v->tamElem);
    }
    free(aux);
    return OK;
}

void vectorEliminar(Vector* v)
{
    free(v->vec);
    v->vec = NULL;
    v->ce = 0;
    v->capacidad = 0;
}

size_t vectorTamElem(Vector *v)
{
    return v->tamElem;
}

int vectorCantidadElementos(Vector *v)
{
    return v->ce;
}



