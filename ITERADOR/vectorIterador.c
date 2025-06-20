#include "vectorIterador.h"
/*
void vectorIteradorCrear(VectorIterador* it, Vector* v)
{
    it->pri = v->vec;
    it->ult = v->vec + (v->ce - 1) * v->tamElem;
    it->act = NULL;
    it->tamElem = v->tamElem;
    it->finIter = true;
}


void* vectorIteradorPrimero(VectorIterador* it)
{
	if(it->pri > it->ult) // Vector vacío
	{
		return NULL;
	}

	it->act = it->pri;
	it->finIter = false;

	return it->act;
}



void* vectorIteradorUltimo(VectorIterador* it)
{
	if(it->pri > it->ult) // Vector vacío
	{
		return NULL;
	}

	it->act = it->ult;
	it->finIter = false;

	return it->act;
}


void* vectorIteradorSiguiente(VectorIterador* it)
{
	if(it->act == it->ult) // Fin iteración.
	{
		it->finIter = true;
		return NULL;
	}

	it->act += it->tamElem;

	return it->act;
}


void* vectorIteradorAnterior(VectorIterador* it)
{
	if(it->act == it->pri) // Fin iteración.
	{
		it->finIter = true;
		return NULL;
	}

	it->act -= it->tamElem;

	return it->act;
}


void* vectorIteradorActual(VectorIterador* it)
{
	return it->act;
}


bool vectorIteradorFinIter(VectorIterador* it)
{
	return it->finIter;
}
*/



void vectorIteradorCrear(VectorIterador* it, const Vector* vector)
{
    it->act = NULL;
    it->ult = NULL;
    it->finIter = true;
    it->v =  vector;
}

void* vectorIteradorPrimero(VectorIterador* it)
{
    const Vector* vector = it->v;

    if (vector->ce == 0)
    {
        it->act = NULL;
        it->ult = NULL;
        it->finIter = true;
        return NULL;
    }

    it->ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    it->act = vector->vec;
    it->finIter = false;

    return it->act;
}

void* vectorIteradorSiguiente(VectorIterador* it)
{
    const Vector *vector = it->v;
    void *siguiente = it->act + vector->tamElem;

    if(siguiente > it->ult)
    {
        it->finIter = true;
        return NULL;
    }
    it->act = siguiente;

    return siguiente;
}

bool vectorIteradorFinIter(VectorIterador* it)
{
    return it->finIter;
}

void *vectorIteradorDesplazamiento(VectorIterador *it, int cantidad)
{
    const Vector *v = it->v;

    if(cantidad < 0)
    {
        if(it->act + v->tamElem*cantidad < v->vec)
        {
            return NULL;
        }
    }else
    {
        if(it->act + v->tamElem*cantidad > it->ult)
        {
            return NULL;
        }
    }

    it->act += v->tamElem*cantidad;

    return it->act;
}




