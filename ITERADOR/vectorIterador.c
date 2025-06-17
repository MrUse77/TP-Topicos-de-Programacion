#include "vectorIterador.h"

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
