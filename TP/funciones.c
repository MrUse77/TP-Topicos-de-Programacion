#include "./funciones.h"
#include "../TPString/string.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Prototipos de funciones internas, no accesibles desde main. */
void corregirFecha (char* fecha);
void desencriptarIccGeneral (char* str);
void desencriptarItemsObra (char* str);
void normalizarItemsObra (char* str);

bool esLetra (char c);
char aMayuscula(char c);
int compararFecha (char* fecha1, char* fecha2);
int compRegistrosPeriodoNGeneralAb (const void* regA, const void* regB);
void copiarReg (Vector* v, RegistroExportado* regExp, Registro* reg, const char* tVariable, FILE* archBin);
size_t cantidadIndices (Vector* v);

/* --- Declaraciones --- */

int cargarEnRegistro (void* reg, char* buffer) {
    Registro* r = reg;
    char* act;

    removerCharEnString(buffer, '\"');

    act = buscarEnStringReversa(buffer, '\n');

    if (act == NULL)
        return ERR_BUFFER_CORTO;

    *act = '\0';
    act = buscarEnStringReversa(buffer, ';');

    sscanf(act +1, "%s", r -> indiceICC);

    *act = '\0';
    act = buscarEnStringReversa(buffer, ';');

    sscanf(act +1, "%s", r -> nGeneralAperturas);

    *act = '\0';

    sscanf(buffer, "%s", r -> periodo);

    return TODO_OK;
}

void mostrarRegistro (const void* reg) {
    const Registro* r = reg;

    printf(
        "%s;%s;%s;%s\n",
        r -> periodo, r -> nGeneralAperturas, r -> indiceICC, r -> clasificador
    );
}

void mostrarRegistroExportado (const void* reg) {
    const RegistroExportado* r = reg;

    printf(
        "%s;%s;%s;%s;%s\n",
        r -> periodo, r -> clasificador, r -> nGeneralAperturas, r -> tipoVariable, r -> valor
    );
}

/* Recorre y corrige los registros obtenidos para cada archivo. */
void corregirRegistros (Vector* v, Crr corrector) {

    Registro* reg;
    VectorIterador it;

    /* Elimina la cabecera. */
    vectorEliminarDePos(v, 0);

    vectorIteradorCrear(&it, v);
    reg = (Registro*) vectorIteradorPrimero(&it);

    while (!vectorIteradorFin(&it)) {
        corrector(reg);
        reg = (Registro*) vectorIteradorSiguiente(&it);
    }
}

void corregirIccGeneral (Registro* reg) {

    bool esNGeneral;

    corregirFecha(reg -> periodo);

    reemplazarCharEnString(reg -> indiceICC, ',', '.');

    desencriptarIccGeneral(reg -> nGeneralAperturas);

    reemplazarCharEnString(reg -> nGeneralAperturas, '_', ' ');
    *(reg -> nGeneralAperturas) = aMayuscula(*(reg -> nGeneralAperturas));

    esNGeneral = compararString(reg -> nGeneralAperturas, "Nivel general") == 0;

    copiarString(
        reg -> clasificador,
        esNGeneral ? "Nivel general" : "Capítulos",
        CLASIFICADOR_TAM -1
    );

}

void corregirItemsObra (Registro* reg) {
    corregirFecha(reg -> periodo);

    reemplazarCharEnString(reg -> indiceICC, ',', '.');

    desencriptarItemsObra(reg -> nGeneralAperturas);

    normalizarItemsObra(reg -> nGeneralAperturas);

    copiarString(reg -> clasificador, "Ítems", CLASIFICADOR_TAM -1);
}

void corregirFecha (char* fecha) {
    int dia, mes, anio;

    sscanf(fecha, "%d/%d/%d", &dia, &mes, &anio);
    sprintf(fecha, "%4d-%02d-%02d", anio, mes, dia);
}

void desencriptarIccGeneral (char* str) {

    char* letraActual = str;
    int pos;

    while (*letraActual != '\0') {
        if (esLetra(*letraActual)) {

            pos = (letraActual - str);

            if (pos % 2 == 0)
                *letraActual += 4;
            else
                *letraActual += 2;


            if (*letraActual > 'z')
                *letraActual -= ('z' - 'a' +1);
        }

        letraActual++;
    }
}

void desencriptarItemsObra (char* str) {

    char* letraActual = str;
    const char vEnc[] = "@8310$7|59";
    const char vDes[] = "abeiostlmn";
    int pos;
    char* dir;

    while (*letraActual != '\0') {

        dir = buscarEnString(vEnc, *letraActual);

        if (dir) {
            pos = dir - vEnc;
            *letraActual = *(vDes + pos);
        }



        letraActual++;
    }
}

void normalizarItemsObra (char* str) {

    char* letraActual = str;
    bool primerGuionBajo = false;
    size_t longSeg;

    while (!primerGuionBajo && *letraActual != '\0') {

        if (*letraActual == '_') {
            primerGuionBajo = true;
            *(letraActual +1) = aMayuscula(*(letraActual +1));
            longSeg = longitudString(letraActual +1);
            memmove(str, letraActual +1, longSeg +1);
        }
        else
            letraActual++;
    }

    reemplazarCharEnString(str, '_', ' ');
}

int compRegistrosPeriodo (const void* regA, const void* regB) {

    Registro* rA = (Registro*) regA;
    Registro* rB = (Registro*) regB;

    return compararFecha(rA -> periodo, rB -> periodo);
}

/* Teniendo en cuenta la cantidad de items en nivel_general_aperturas calcula el mes anterior del mismo item  y obtiene la variación. */
void calcularVariacionMensual (Vector* v) {

    VectorIterador it;
    vectorIteradorCrear(&it, v);
    size_t cantidadPorMes = cantidadIndices(v);
    Registro* reg = (Registro*) vectorIteradorPrimero(&it);
    double variacion;
    int i;

    /* Los del primer mes no tienen variación. */
    for(i = 0; i < cantidadPorMes; i++) {
        sprintf(reg -> vMensual, "NA");
        reg = (Registro*) vectorIteradorSiguiente(&it);
    }

    while(!vectorIteradorFin(&it)) {
        Registro* regAnterior = vectorIteradorDesplazamiento(&it, -cantidadPorMes);
        vectorIteradorDesplazamiento(&it, cantidadPorMes);

        variacion = (atof(reg -> indiceICC) / atof(regAnterior -> indiceICC) -1) * 100;
        sprintf(reg-> vMensual, "%.2f", variacion);

        reg = (Registro*) vectorIteradorSiguiente(&it);
    }
}

/* Teniendo en cuenta la cantidad de items en nivel_general_aperturas calcula el año anterior del mismo item y obtiene la variación. */
void calcularVariacionAnual (Vector* v) {

    VectorIterador it;
    vectorIteradorCrear(&it, v);
    size_t cantidadPorAnio = cantidadIndices(v) * 12;
    Registro* reg = (Registro*) vectorIteradorPrimero(&it);
    double variacion;
    int i;

    /* Los del primer año no tienen variación. */
    for (i = 0; i < cantidadPorAnio; i++) {
        sprintf(reg -> vAnual, "NA");
        reg = (Registro*) vectorIteradorSiguiente(&it);
    }

    while(!vectorIteradorFin(&it)) {
        Registro* regAnterior = vectorIteradorDesplazamiento(&it, -cantidadPorAnio);
        vectorIteradorDesplazamiento(&it, cantidadPorAnio);

        variacion = (atof(reg -> indiceICC) / atof(regAnterior -> indiceICC) -1) * 100;
        sprintf(reg-> vAnual, "%.2f", variacion);

        reg = (Registro*) vectorIteradorSiguiente(&it);
    }
}

/* Desglosa cada registro en 3 y los va cargando secuencialmente. */
int cargarABinario (Vector* v, const char* nomArchBin) {

    FILE* archBin = fopen(nomArchBin, "wb");
    Registro reg;
    RegistroExportado regExp;

    if (!archBin)
        return ERR_ARCHIVO;

    copiarReg(v, &regExp, &reg, "indice_icc", archBin);
    copiarReg(v, &regExp, &reg, "var_mensual", archBin);
    copiarReg(v, &regExp, &reg, "var_interanual", archBin);

    fclose(archBin);

    return TODO_OK;
}


/* --- Auxiliares --- */

bool esLetra (char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

char aMayuscula(char c) {
    if (c >= 'a' && c <= 'z')
        return c -= ('a' - 'A');

    return c;
}

int compararFecha (char* fecha1, char* fecha2) {

    int dia, mes, anio, f1, f2;

    sscanf(fecha1, "%d-%d-%d", &anio, &mes, &dia);
    f1 = anio * 10000 + mes * 100 + dia;

    sscanf(fecha2, "%d-%d-%d", &anio, &mes, &dia);
    f2 = anio * 10000 + mes * 100 + dia;

    return f1 - f2;
}

/* Compara primero por periodo y luego desempata por el nombre del indice en nivel_general_aperturas. */
int compRegistrosPeriodoNGeneralAb (const void* regA, const void* regB) {

    Registro* rA = (Registro*) regA;
    Registro* rB = (Registro*) regB;

    int compFecha = compararFecha(rA -> periodo, rB -> periodo);

    if (compFecha != 0)
        return compFecha;

    return compararString(rA -> nGeneralAperturas, rB -> nGeneralAperturas);
}

void copiarReg (Vector* v, RegistroExportado* regExp, Registro* reg, const char* tVariable, FILE* archBin) {

    char valor[VALOR_TAM];
    VectorIterador it;

    vectorIteradorCrear(&it, v);

    reg = (Registro*) vectorIteradorPrimero(&it);

    while (!vectorIteradorFin(&it)) {

        if (compararString(tVariable, "indice_icc") == 0)
            copiarString(valor, reg -> indiceICC, VALOR_TAM);
        else {
            if (compararString(tVariable, "var_mensual") == 0)
                copiarString(valor, reg -> vMensual, VALOR_TAM);
            else
                copiarString(valor, reg -> vAnual, VALOR_TAM);
        }

        copiarString(regExp -> periodo, reg -> periodo, PERIODO_TAM);
        copiarString(regExp -> clasificador, reg -> clasificador, CLASIFICADOR_TAM);
        copiarString(regExp -> nGeneralAperturas, reg -> nGeneralAperturas, N_GENERAL_APERTURAS_TAM);
        copiarString(regExp -> tipoVariable, tVariable, T_VARIABLE_TAM);
        copiarString(regExp -> valor, valor, VALOR_TAM);

        fwrite(regExp, sizeof(RegistroExportado), 1, archBin);

        reg = (Registro*) vectorIteradorSiguiente(&it);
    }
}

/* Calcula cuantos indices se manejan por mes en nivel_general_aperturas */
size_t cantidadIndices (Vector* v) {

    size_t cantidad = 0;
    VectorIterador it;
    Registro* reg;

    vectorIteradorCrear(&it, v);

    char primerIndice[N_GENERAL_APERTURAS_TAM];
    reg = (Registro*) vectorIteradorPrimero(&it);

    /* Si no encuentra indices para cotejar, devuelve 0 */
    if (vectorIteradorFin(&it))
        return cantidad;

    copiarString(primerIndice, reg -> nGeneralAperturas, N_GENERAL_APERTURAS_TAM -1);
    cantidad++;

    reg = (Registro*) vectorIteradorSiguiente(&it);

    /* Se verifica el fin del iterador por las dudas, aunque con el juego de datos dado, nunca llega al fin */
    while (!vectorIteradorFin(&it) && compararString(reg -> nGeneralAperturas, primerIndice) != 0) {
        cantidad++;
        reg = (Registro*) vectorIteradorSiguiente(&it);
    }

    return cantidad;
}