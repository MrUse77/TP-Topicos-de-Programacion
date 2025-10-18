#include "algoritmos.h"

int transformarVector(Vector_t* vector, int (*Transformar)(void* dato, void* contexto), void* contexto)
{
    if(!vector || !Transformar){
        return ERR_USUARIO;
    }

    char* i = vector->data;
    char* ult = (char*)vector->data + (vector->cantElem - 1) * vector->tamElem;

    for(; i < ult; i += vector->tamElem){
        Transformar(i, (!contexto ? NULL : contexto));
    }

    return EXITO;
}

Vector_t* copiarVector(Vector_t* vector)
{
    Vector_t* tmp = malloc(sizeof(Vector_t));

    if(!tmp){
        return NULL;
    }

    vectorCrear(tmp, vector->tamElem);

    void* i = vector->data;

    memmove(tmp->data, i, vector->cantElem * vector->tamElem);

    return tmp;
}

Vector_t* mapearVector(Vector_t* vector, void* (*Mapear)(void* dato, void* tpm, void* contexto), size_t n, void* contexto)
{
    Vector_t* tmp = malloc(sizeof(Vector_t));

    if(!tmp){
        return NULL;
    }

    vectorCrear(tmp, n);

    void* elem = malloc(n);

    if(!elem){
        return NULL;
    }

    char* i = vector->data;
    char* ult = (char*)vector->data + (vector->cantElem - 1) * vector->tamElem;

    for(; i < ult; i += vector->tamElem){
        Mapear(i, elem, contexto);
        vectorEmpujar(tmp, elem);
    }

    free(elem);

    return tmp;
}

Vector_t* filtrarVector(Vector_t* vector, int (*Filtrar)(void* dato, void* contexto), void* contexto)
{
    Vector_t* tmp = malloc(sizeof(Vector_t));

    if(!tmp){
        return NULL;
    }

    vectorCrear(tmp, vector->tamElem);

    char* i = vector->data;
    char* ult = (char*)vector->data + (vector->cantElem - 1) * vector->tamElem;

    for(; i < ult; i += vector->tamElem){
        if(Filtrar(i, contexto)){
            vectorEmpujar(tmp, i);
        }
    }

    return tmp;
}

void* reducirVector(Vector_t* vector, void* (*Reducir)(void* dato, void* acumulado))
{
    void* acumulador;
    char* i = vector->data;
    char* ult = (char*)vector->data + (vector->cantElem - 1) * vector->tamElem;

    for(; i < ult; i += vector->tamElem){
        acumulador = Reducir(i, acumulador);
    }

    return acumulador;
}

Vector_t* reducirVectorPorClave(Vector_t* vector, void* (*ObtenerClave)(void* elem), int (*CompararClave)(void* lhs, void* rhs), void* (*Reducir)(void* elem, void* acumulado))
{
    Vector_t* temp = malloc(sizeof(Vector_t));

    if(!temp){
        return NULL;
    }

    vectorCrear(temp, vector->tamElem);

    void* acumulador = malloc(vector->tamElem);

    if(!acumulador){
        return NULL;
    }

    char* i = vector->data;
    char* ult = (char*)vector->data + (vector->cantElem - 1) * vector->tamElem;
    void* claveAnt = NULL;

    while(i < ult){

        memset(acumulador, 0, vector->tamElem);

        claveAnt = ObtenerClave(i);

        while(i < ult && !CompararClave(claveAnt, ObtenerClave(i))){
            
            acumulador = Reducir(i, acumulador);

            i += vector->tamElem;
        }

        vectorEmpujar(temp, acumulador);
    }

    free(acumulador);
    vectorDestruir(vector);

    return temp;
}

Vector_t* unirVectores(Vector_t* lhs, Vector_t* rhs, void* (*Unir)(void* lhs, void* rhs, void* elem), size_t n)
{
    Vector_t* tmp = malloc(sizeof(Vector_t));

    if(!tmp){
        return NULL;
    }

    vectorCrear(tmp, n);

    void* elem = malloc(n);
    char* i = lhs->data;
    char* j = rhs->data;
    char* ult = (char*)lhs->data + (lhs->cantElem - 1) * lhs->tamElem;

    for(; i < ult; i += lhs->tamElem, j += lhs->tamElem){
        elem = Unir(i, j, elem);
        vectorEmpujar(tmp, elem);
    }

    free(elem);

    return tmp;
}
