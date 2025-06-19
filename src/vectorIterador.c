#include "../include/vectorIterador.h"

/*Probado*/
int vectorIteradorCrear(VectorIterador* iter, Vector* vec)
{
    if(vec->cantElem == 0) return ERROR;

    iter->vector = vec;
    iter->cursor = vec->data;
    iter->ini = vec->data;
    iter->fin = vec->data + (vec->cantElem * vec->tamElem);

    return EXITO;
}

/*Probado*/
int vectorIteradorRango(VectorIterador* iter, size_t iniPos, size_t finPos)
{   
    if(iniPos == finPos || iniPos < 0 || finPos < iniPos || finPos > iter->vector->cantElem) return ERROR;

    char* tmpData = iter->vector->data;

    iter->ini = tmpData + iniPos * iter->vector->tamElem;
    iter->fin = tmpData + finPos * iter->vector->tamElem;
    iter->cursor = iter->ini;

    return EXITO;
}

/*Probado, pero no me queda claro si funciona bien ORIGEN_ACT*/
int vectorIteradorMover(VectorIterador* iter, int offset, int origen)
{
    if(offset < 0 || offset > iter->vector->cantElem) return ERROR;

    char* tmpData = iter->vector->data;

    switch(origen){
        case ORIGEN_INI:            
            iter->cursor = tmpData + offset * iter->vector->tamElem;
            break;
        case ORIGEN_ACT:
            iter->cursor += offset * iter->vector->tamElem;
            break;
        case ORIGEN_FIN:
            iter->cursor = tmpData + iter->vector->cantElem * iter->vector->tamElem - offset * iter->vector->tamElem;
            break;
        default:
            return ERROR;
    }

    return EXITO;
}

/*Probado*/
int vectorIteradorLeer(VectorIterador* iter, void* elem)
{
    if(iter->cursor == iter->fin) return FIN_DE_VECTOR;

    memcpy(elem, iter->cursor, iter->vector->tamElem);

    iter->cursor += iter->vector->tamElem;

    return EXITO;
}

/*Probado*/
size_t vectorIteradorPosCursor(VectorIterador* iter)
{
    char* tmpCursor = iter->cursor;
    char* tmpIni = iter->ini;
    size_t ret = (tmpCursor - tmpIni) / iter->vector->tamElem;

    return ret;
}

/*Probado*/
int vectorIteradorCursorAlFinal(VectorIterador* iter)
{
    if(iter->cursor == iter->fin) return FIN_DE_VECTOR;

    return !FIN_DE_VECTOR;
}