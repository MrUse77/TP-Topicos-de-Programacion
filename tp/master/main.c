#include <stdio.h>
#include <locale.h>
#include "../comunes/comunes.h"
#include "../vector/vector.h"
#include "../secuenciaPalabras/secuenciaPalabras.h"
#include "../proceso/proceso.h"

void imprimirRegistroDivisiones(void* reg);
int parsearParaDivisiones(FILE* arch, void* reg);
int parsearParaAperturas(FILE* arch, void* reg);

#define CANT_ARGS 3

#define ARG_DIVISIONES_NOM 1
#define ARG_APERTURAS_NOM 2

// main.c serie_ipc_divisiones.csv serie_ipc_aperturas.csv

int main(int argc, char* argv[])
{
    if(argc != CANT_ARGS){
        return ERR_USUARIO;
    }

    setlocale(LC_ALL, "es_AR.UTF-8");
    setlocale(LC_CTYPE, "es_AR.UTF-8");

    Vector_t vecDivisiones, vecAperturas;

    vectorCrear(&vecDivisiones, sizeof(IPCDivisiones));
    vectorLeerDeTexto(&vecDivisiones, argv[ARG_DIVISIONES_NOM], parsearIPCDivisiones);

    vectorEliminarPos(&vecDivisiones, 0);

    corregirCampos(&vecDivisiones, corregirIPCDivisiones);

    vectorCrear(&vecAperturas, sizeof(IPCAperturas));
    vectorLeerDeTexto(&vecAperturas, argv[ARG_APERTURAS_NOM], parsearIPCAperturas);

    vectorEliminarPos(&vecAperturas, 0);

    corregirCampos(&vecAperturas, corregirIPCAperturas);

    vectorEscribirATexto(&vecDivisiones, ARCH_DIVISIONES, parsearParaDivisiones);
    vectorEscribirATexto(&vecAperturas, ARCH_APERTURAS, parsearParaAperturas);

//    herramientaAjustarMontosIPCDivisiones(&vecDivisiones);

    clasificarBySIPCDivisiones(&vecDivisiones);

    vectorDestruir(&vecDivisiones);
    vectorDestruir(&vecAperturas);
    /*

    herramientaCalcularAlquiler(&vecAperturas);

    vectorDestruir(&vecDivisiones);
    vectorDestruir(&vecAperturas);

    return EXITO;

    */
}


void imprimirRegistroDivisiones(void* reg)
{
    IPCDivisiones* tmp = reg;

    printf("%s %s %s %s %s %s %s %s\n", tmp->cod, tmp->desc, tmp->clasif, tmp->indiceIPC, tmp->varMensIPC, tmp->varAnualIPC, tmp->region, tmp->periodo);
}

int parsearParaDivisiones(FILE* arch, void* reg)
{
    IPCDivisiones* tmp = reg;

    fprintf(arch, "%s | %s | %s | %s | %s | %s | %s | %s\n", tmp->cod, tmp->desc, tmp->clasif, tmp->indiceIPC, tmp->varMensIPC, tmp->varAnualIPC, tmp->region, tmp->periodo);
    return EXITO;
}

int parsearParaAperturas(FILE* arch, void* reg)
{
    IPCAperturas* tmp = reg;

    fprintf(arch, "%s | %s | %s | %s | %s | %s | %s | %s\n", tmp->cod, tmp->desc, tmp->clasif, tmp->periodo, tmp->indiceIPC, tmp->varMensIPC, tmp->varAnualIPC, tmp->region);
    return EXITO;
}