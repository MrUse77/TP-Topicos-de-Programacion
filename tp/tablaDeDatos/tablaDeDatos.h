#ifndef TABLA_DE_DATOS_H
#define TABLA_DE_DATOS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum{
    Int,
    Double,
    Float,
    String,
}tipoDeDato;

typedef struct{
    char** nombresCampos;
    tipoDeDato* tiposDeDatos;
    int cantColumnas;
    int cantFilas;
    int capacidadFilas;
    void** datos; 
}TablaDeDatos_t;

TablaDeDatos_t* tablaDeDatosCrear(int cantColumnas, const char** nombresCampos, tipoDeDato* tiposDeDatos);
void tablaDeDatosDestruir(TablaDeDatos_t* tabla);

int tablaDeDatosInsertar(TablaDeDatos_t* tabla, void** valores);
int tablaDeDatosEliminar(TablaDeDatos_t* tabla, int indice);

void* tablaDeDatosGet(TablaDeDatos_t* tabla, int fila, int columna);
int tablaDeDatosSet(TablaDeDatos_t* tabla, int fila, int columna, void* valor);

void** tablaDeDatosBuscarFila(TablaDeDatos_t* tabla, int columnas, void* valor);
TablaDeDatos_t* tablaDeDatosFiltrar(TablaDeDatos_t* tabla, bool (*predicado)(void** fila));

int tablaDeDatosOrdenar(TablaDeDatos_t* tabla, int (*Cmp)(void*, void*));

void tablaDeDatosImprimir(TablaDeDatos_t* tabla, void (*Mostrar)(void**));
TablaDeDatos_t* tablaDeDatosCopiar(TablaDeDatos_t* tabla);

#endif