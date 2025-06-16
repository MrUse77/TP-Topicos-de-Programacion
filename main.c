#include "include/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/funciones.h"

/* Esto podría ir como argumentos a main */
#define NOMBRE_ARCH_INDICES_GENERAL "indices_icc_general_capitulos.csv"
#define NOMBRE_ARCH_INDICES_OBRA "Indices_items_obra.csv"
#define NOMBRE_ARCH_INDICES_GENERAL_BIN "indices_icc_general_capitulos.bin"

/*
 * 1. Corrección de indices nivel general.
 * 2. Corrección de indices items obra.
 * 3. Unificación y ordenamiento de estructuras.
 * 4. Calculo de variaciones mensuales e interanuales.
 * 5. Exportado a binario.
*/
void imprimirReg(const void *);
void imprimirRegBin(const void *);
int main()
{
	int cod;

	char dirArchivos[255] = "archivos/";
	/*
     * Esto regenera los archivos a su estado original para evitar tener que copiar los archivos originales
       cada vez que se quiere probar algo nuevo. El tamaño de los strings que alojan el path completo esta seteado en 255
       para asegurar que no se queden cortos. Esto podría reemplazarse por una librería de strings dinámicos.


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
*/
	/* Acá ya arranca la corrección propiamente dicha */

	concatenarString(dirArchivos, NOMBRE_ARCH_INDICES_GENERAL, 254);

	Vector v, v2, vBin;
	v = *crearVector(&v, sizeof(Registro));
	if (!v.data)
		return ERR_ARCHIVO;

	cod = corregirArchivo(dirArchivos, formatearNivelGeneral, &v);
	if (cod != TODO_OK) {
		destruirVector(&v);
		return cod;
	}
	/*mostrarVector(&v, imprimirReg);*/

	copiarString(dirArchivos, "archivos/", 254);
	concatenarString(dirArchivos, NOMBRE_ARCH_INDICES_OBRA, 254);

	v2 = *crearVector(&v2, sizeof(Registro));
	cod = corregirArchivo(dirArchivos, formatearItemsObra, &v2);
	if (cod != TODO_OK) {
		destruirVector(&v);
		destruirVector(&v2);
		return cod;
	}

	vBin = *crearVector(&vBin, sizeof(RegistroBin));
	cod = unirRegistros(&v, &v2, &vBin);
	ordenarRegistros(&vBin);
	if (cod != TODO_OK) {
		destruirVector(&v);
		destruirVector(&v2);
		return cod;
	}
	copiarString(dirArchivos, "archivos/", 254);
	concatenarString(dirArchivos, NOMBRE_ARCH_INDICES_GENERAL_BIN, 254);
	crearYescribirArchivoBinario(&vBin, dirArchivos);

	destruirVector(&v);
	destruirVector(&v2);
	return TODO_OK;
}
void imprimirReg(const void *reg)
{
	const Registro *r = reg;
	printf("%20s;%20s;%.17f;%20s;%20s;%20s\n", r->periodo, r->nivel,
	       r->indiceICC, r->clasificador, r->var_mensual,
	       r->var_interanual);
}
void imprimirRegBin(const void *reg)
{
	const RegistroBin *r = reg;
	printf("%11s;%17s;%41s;%15s;%32s\n", r->periodo, r->clasificador,
	       r->nivel_general_aperturas, r->tipo_variable, r->valor);
}
