#include <stdio.h>
#include <stdlib.h>
#include "./funciones.h"
#include "../TPString/string.h"

#define ARG_NOM_ARCH_ICC_GENERAL 1
#define ARG_NOM_ARCH_ITEMS_OBRA 2
#define ARG_NOM_ARCH_INDICES_UNIF 3

#define RUTA_TAM 254

/* 
 * Nota:
 * Hay cosas duras en la vida, desaprobar un parcial, ver como Boca pierde la septima, 
   o tener que usar comentarios multilinea cuando solo quiero comentar una línea.
 * Atte: Los predicadores de Linux.
*/

/* 
 * Nota 2: 
 * Tenemos al menos 6 chistes más sobre comentarios multilinea pero no queremos ocupar más espacio en el main.
   Aunque considerando que ya estamos ocupando espacio con estas notas, pierde el sentido.
 * Atte: Los predicadores de Linux (de nuevo).
*/

/* 
 * Argumentos a main:
 * TP.exe 
 * ../TP/archivos/indices_icc_general_capitulos.csv 
 * ../TP/archivos/Indices_items_obra.csv 
 * archivos/indices_unificado.dat
*/
int main (int argc, char* argv[]) {

    Vector vecIccGeneral, vecItemsObra;
    int cod;
    
    system("chcp 65001 > nul");

    cod = vectorCrear(&vecIccGeneral, sizeof(Registro));

    if (cod != TODO_OK)
        return cod;

    vectorCrear(&vecItemsObra, sizeof(Registro));

    cod = vectorCargarDeTxt(
        &vecIccGeneral,
        argv[ARG_NOM_ARCH_ICC_GENERAL],
        cargarEnRegistro,
        BUFFER_TAM
    );

    if (cod != TODO_OK)
        return cod;

    cod = vectorCargarDeTxt(
        &vecItemsObra,
        argv[ARG_NOM_ARCH_ITEMS_OBRA],
        cargarEnRegistro,
        BUFFER_TAM
    );

    if (cod != TODO_OK)
        return cod;

    corregirRegistros(&vecIccGeneral, corregirIccGeneral);
    corregirRegistros(&vecItemsObra, corregirItemsObra);

    cod = vectorUnir(&vecIccGeneral, &vecItemsObra);

    if (cod != TODO_OK)
        return cod;

    vectorDestruir(&vecItemsObra);

    /* 
     *  Solo se compara por periodo y no por clasificador ya que, al concatenar los vectores y luego ordenar por periodo, 
        el clasificador también queda ordenado.
    */
    cod = vectorOrdenar(
        &vecIccGeneral, 
        INSERCION, 
        compRegistrosPeriodo
    );

    if (cod != TODO_OK)
        return cod;

    calcularVariacionMensual(&vecIccGeneral);
    calcularVariacionAnual(&vecIccGeneral);

    cod = cargarABinario(&vecIccGeneral, argv[ARG_NOM_ARCH_INDICES_UNIF]);

    if (cod != TODO_OK)
        return cod;

    vectorDestruir(&vecIccGeneral);


    /* Descomentar el codigo de abajo para verificar que se hayan cargado bien los registros en el binario. */

    /*
    char ruta[RUTA_TAM +1] = "../TP/";
    Vector vecPrueba;

    cod = vectorCrear(&vecPrueba, sizeof(RegistroExportado));

    if (cod != TODO_OK)
        return cod;

    concatenarString(ruta, argv[ARG_NOM_ARCH_INDICES_UNIF], RUTA_TAM);
    cod = vectorCargarDeBin(&vecPrueba, ruta);

    if (cod != TODO_OK)
        return cod;

    vectorMostrar(&vecPrueba, mostrarRegistroExportado);

    vectorDestruir(&vecPrueba);
    */

    return TODO_OK;
}