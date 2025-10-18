#include "iterador.h"

void iteradorCrear(Iterador_t* iterador, Vector_t* vector)
{
    iterador->vec = vector;

    iterador->ini = vector->data;
    iterador->cursor = iterador->ini;
    iterador->fin = vector->data + (vector->cantElem - 1) * vector->tamElem;
}

void iteradorDestruir(Iterador_t* iterador)
{
    iterador->vec = NULL;
    iterador->ini = NULL;
    iterador->fin = NULL;
    iterador->cursor = NULL;
}

void iteradorActualizar(Iterador_t* iterador)
{
    Vector_t* v = iterador->vec;

    int ce;

    if(v->data != iterador->ini){
        iterador->ini = v->data;
        iterador->cursor = iterador->ini;
        iterador->fin = v->data + (v->cantElem - 1) * v->tamElem;
    }else if((v->cantElem != (ce= ((iterador->fin - iterador->ini)) / v->tamElem)) && v->cantElem){
        iterador->fin = v->data + (v->cantElem - 1) * v->tamElem;
        iterador->cursor = (ce < v->cantElem) ? iterador->fin : iterador->cursor;
    }else if(v->cantElem == 0){
        iterador->cursor = iterador->ini;
        iterador->fin = iterador->ini;
    }
}

void* iteradorSiguiente(Iterador_t* iterador)
{
    if(iterador->cursor > iterador->fin){
        return NULL;
    }

    void* tmp = iterador->cursor;
    iterador->cursor += iterador->vec->tamElem;

    return tmp;
}

int iteradorReiniciar(Iterador_t* iterador)
{
    iterador->cursor = iterador->ini;
    return EXITO;
}

int iteradorMoverCursor(Iterador_t* iterador, size_t cant, int offset)
{
    if(cant > iterador->vec->cantElem){
        return -1;
    }

    if(cant < 0 && offset == ITER_FIN);

    void* origen = NULL;

    switch(offset){
        case ITER_INI:
            origen = iterador->ini;
            break;
        case ITER_ACT:
            origen = iterador->cursor;
            break;
        case ITER_FIN:
            origen = iterador->fin;
            break;
        default:
            return -1;
    }

    iterador->cursor = origen + cant * iterador->vec->tamElem;

    return EXITO;
}

size_t iteradorPosCursor(Iterador_t* iterador)
{
    return ((iterador->cursor - iterador->ini) / iterador->vec->tamElem);
}

bool iteradorEsFin(Iterador_t* iterador)
{
    return (iterador->cursor == iterador->fin) ? true : false;
}
