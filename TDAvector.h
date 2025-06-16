#ifndef TDAVECTOR_H_INCLUDED
#define TDAVECTOR_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct
{
    char periodo[12];
    char nivel_general[41];
    double indice;
    char clasificador[12];
}ITEMS;

typedef struct
{
    char periodo[12];
    char general_aperturas[41];
    double indice;
    char clasificador[12];
}GENERAL;

typedef struct
{
    char periodo[12];
    char general_aperturas[41];
    double indice;
    char clasificador[14];
    char var_mensual[32];
    char var_interanual[32];
}UNIF;


typedef struct
{
    char periodo[12];
    char clasificador[14];
    char general_aperturas[41];
    char tipo_varible[32];
    char valor[32];

}RESULT;


typedef struct
{
    void* vec;
    int ce;
    size_t capacidad, tamElem;
}Vector;
typedef int (*Cmp)(const void *e1, const void *e2);

bool vectorRedimensionar(Vector* v, size_t capacidad);
void mostrarVectorGeneral(Vector *v);
bool vectorCrear(Vector* v, size_t capacidad, size_t tamElem);
void mostrarVectorItes(Vector *v);
void mostrarVectorUnif(Vector *v);
int vectorInsertarEnPos(Vector* v, int elem, int pos);
int vectorInsertarFinal(Vector* v, const void* elem);
int ordenarInsercion(Vector *v, Cmp cmp);

#endif // TDAVECTOR_H_INCLUDED
