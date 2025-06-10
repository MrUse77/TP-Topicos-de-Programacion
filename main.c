#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/funciones.h"

/* Esto podría ir como argumentos a main */
#define NOMBRE_ARCH_INDICES_GENERAL "indices_icc_general_capitulos.csv"
#define NOMBRE_ARCH_INDICES_OBRA "Indices_items_obra.csv"

/*
 * 1. Corrección de indices nivel general.
 * 2. Corrección de indices items obra.
 * 3. Unificación y ordenamiento de estructuras.
 * 4. Calculo de variaciones mensuales e interanuales.
 * 5. Exportado a binario.
*/
int main () {

    int cod;

    /*
     * Esto regenera los archivos a su estado original para evitar tener que copiar los archivos originales
       cada vez que se quiere probar algo nuevo. El tamaño de los strings que alojan el path completo esta seteado en 255
       para asegurar que no se queden cortos. Esto podría reemplazarse por una librería de strings dinámicos.
    */
    char dirBackup[255] = "backup/";
    char dirArchivos[255] = "archivos/";
    concatenarString(dirArchivos, NOMBRE_ARCH_INDICES_GENERAL, 254);
    concatenarString(dirBackup, NOMBRE_ARCH_INDICES_GENERAL, 254);

    cod = copiarArchivoTxt(dirArchivos, dirBackup);

    if (cod != TODO_OK)
        return cod;

    copiarString(dirBackup, "backup/", 254);
    copiarString(dirArchivos, "archivos/", 254);
    concatenarString(dirArchivos, NOMBRE_ARCH_INDICES_OBRA, 254);
    concatenarString(dirBackup, NOMBRE_ARCH_INDICES_OBRA, 254);

    cod = copiarArchivoTxt(dirArchivos, dirBackup);

    if (cod != TODO_OK)
        return cod;

    /* Acá ya arranca la corrección propiamente dicha */

    copiarString(dirArchivos, "archivos/", 254);
    concatenarString(dirArchivos, NOMBRE_ARCH_INDICES_GENERAL, 254);

    cod = corregirArchivo(dirArchivos, formatearNivelGeneral);

    if (cod != TODO_OK)
        return cod;

    copiarString(dirArchivos, "archivos/", 254);
    concatenarString(dirArchivos, NOMBRE_ARCH_INDICES_OBRA, 254),

    cod = corregirArchivo(dirArchivos, formatearItemsObra);

    return TODO_OK;
}
