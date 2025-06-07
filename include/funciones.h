#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define NOMBRE_ARCH_INDICES_GENERAL "indices_icc_general_capitulos.txt"
#define NOMBRE_ARCH_INDICES_OBRA "Indices_items_obra.txt"
#define NOMBRE_ARCH_AUXILIAR "auxiliar.tmp"

#define FORMATO_GENERAL "%[^;];%[^;];%[^;]"

#define BUFFER_TAM 255

#define PERIODO_TAM 15
#define NIVEL_TAM 30
#define INDICE_TAM 15

typedef struct{
    char* buffer;
    int* indices;
}Registro;

typedef struct{
    char* periodo;
    char* nivel;
    char* indiceICC;
}RegistroIndice;

int esPar(int n);
int esLetra(char* c);
int aMayus(char* c);

RegistroIndice* inicializarRegistroIndice(RegistroIndice* registros);
void destruirRegistroIndice(RegistroIndice* registros);
int corregirFormatoDeIndicesGeneral(FILE* archivo);
char* corregirFormatoDeFecha(RegistroIndice* registro);
char* agregarCeroALaIzquierda(RegistroIndice* registro);
char* desencriptarNivelGeneral(RegistroIndice* registro);
char* normalizarNivelSinGuiones(RegistroIndice* registro);
char* corregirFormatoDecimal(RegistroIndice* registro); 

#endif