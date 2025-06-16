#include "../include/funciones.h"
#include "../include/strings.h"
#include <stdio.h>
#include <stdlib.h>

/* Prototipos internos */

int generarArchivoAuxiliar(FILE *orig, Formatear realizarFormateos,
			   Vector *vec);

char *corregirFormatoDeFecha(Registro *registro);
char *agregarCeroALaIzquierda(Registro *registro);
char *desencriptarNivelGeneral(Registro *registro);
char *normalizarNivelSinGuiones(Registro *registro);
char *corregirFormatoDecimal(char *indice);
char *agregarCampoEnCabecera(char *buffer, char *nombreCampo, Registro *reg,
			     FILE *temp, FILE *orig);
char *desencriptarItemsObra(char *str, const char *v_enc, const char *v_des);
char *normalizarItemsObra(char *str);

void *buscarEnVector(const void *vec, size_t cantElem, size_t tamElem,
		     void *elem, Comparar cmp);
int agregarRegistrosBin(Registro *reg, RegistroBin *regBin, Vector *vBin);
void calcularVariaciones(Vector *vec);

int esLetra(char *c);
char aMayus(char *c);
int cmpChar(const void *a, const void *b);
int cmpRegPerNiv(const void *a, const void *b);
int compararRegistrosBinOpt(const void *a, const void *b);
void eliminar_comillas(char *str);

/* Declaraciones */

/* ------------------------------------------------ Principales ------------------------------------------------ */

/*Funcion general, procesa el archivo de indices generales*/
int corregirArchivo(char *nomArchOrig, Formatear realizarFormateos, Vector *vec)
{
	FILE *orig = fopen(nomArchOrig, "rt");

	if (!orig)
		return ERR_ARCHIVO;

	int codRet = generarArchivoAuxiliar(orig, realizarFormateos, vec);

	fflush(orig);
	fclose(orig);

	calcularVariaciones(vec);
	return codRet;
}

int generarArchivoAuxiliar(FILE *orig, Formatear realizarFormateos, Vector *vec)
{
	Registro reg;
	char buffer[BUFFER_TAM] = { 0 };
	int codRet = TODO_OK;
	fgets(buffer, BUFFER_TAM, orig);
	while (fgets(buffer, BUFFER_TAM, orig)) {
		char *punteroLinea = strrchr(buffer, ';');
		char *endptr;
		*punteroLinea = '\0';
		punteroLinea++;
		eliminar_comillas(punteroLinea);
		corregirFormatoDecimal(punteroLinea);
		reg.indiceICC = strtod(punteroLinea, &endptr);
		punteroLinea = strrchr(buffer, ';');
		*punteroLinea = '\0';
		punteroLinea++;
		eliminar_comillas(punteroLinea);
		copiarString(reg.nivel, punteroLinea, NIVEL_TAM - 1);
		copiarString(reg.periodo, buffer, PERIODO_TAM - 1);
		eliminar_comillas(reg.periodo);
		realizarFormateos(&reg);
		vectorAgregar(vec, &reg);
	}

	if (ferror(orig)) {
		perror("Error al leer el archivo");
		codRet = ERR_ARCHIVO;
	}

	return codRet;
}
/*
* Realiza todas las correcciones para indices_icc_general_capitulos.csv
*/
void formatearNivelGeneral(Registro *reg)
{
	corregirFormatoDeFecha(reg);
	agregarCeroALaIzquierda(reg);
	/*	corregirFormatoDecimal(reg);*/
	desencriptarNivelGeneral(reg);
	normalizarNivelSinGuiones(reg);

	copiarString(reg->clasificador,
		     compararString(reg->nivel, "Nivel general") == 0 ?
			     "Nivel general" :
			     "Capítulos",
		     CLASIFICADOR_TAM - 1);
}

/*
* Realiza todas las correcciones para Indices_items_obra.csv
*/
void formatearItemsObra(Registro *reg)
{
	const char v_enc[] = "@8310$7|59";
	const char v_des[] = "abeiostlmn";

	corregirFormatoDeFecha(reg);
	agregarCeroALaIzquierda(reg);
	/*corregirFormatoDecimal(reg);*/
	desencriptarItemsObra(reg->nivel, v_enc, v_des);
	normalizarItemsObra(reg->nivel);
	copiarString(reg->clasificador, "Ítems", CLASIFICADOR_TAM - 1);
}

/*Funcion general, corrige el formato de la fecha de dd/mm/aaaa a dd-mm-aaaa */
char *corregirFormatoDeFecha(Registro *reg)
{
	char *p = reg->periodo;

	while (*p) {
		if (*p == '/') {
			*p = '-';
		}

		p++;
	}

	return reg->periodo;
}

/*Funcion general, agrega los "leading zeroes" a los valores de la fecha*/
/*Como contra, la fecha pasa a ser un valor int, no se si estaria bien*/
char *agregarCeroALaIzquierda(Registro *reg)
{
	char *p = reg->periodo;
	int d, m, a;

	sscanf(p, "%d-%d-%d", &d, &m, &a);
	sprintf(p, "%04d-%02d-%02d", a, m, d);

	return reg->periodo;
}

/*Funcion general, desarrolla el proceso de decriptacion del campo nivel general*/
char *desencriptarNivelGeneral(Registro *reg)
{
	size_t contador = 0;
	char *p = reg->nivel;

	while (*p) {
		if (esLetra(p)) {
			if (contador % 2 == 0)
				*p += 4;
			else
				*p += 2;

			if (*p > 'z')
				*p = 'a' + (*p - 'z' - 1);
		}

		contador++;
		p++;
	}

	return reg->nivel;
}

/*Funcion general, normaliza el campo nivel general desencriptado y remueve los guiones*/
char *normalizarNivelSinGuiones(Registro *reg)
{
	char *p = reg->nivel;
	*p = aMayus(p);

	p++;

	while (*p) {
		if (*p == '_') {
			*p = ' ';
		}

		p++;
	}

	return reg->nivel;
}

/*Funcion general, corrige el formato del campo indice de f*,f* a f*.f* */
char *corregirFormatoDecimal(char *indice)
{
	char *p = indice;

	while (*p) {
		if (*p == ',') {
			*p = '.';
		}

		p++;
	}

	return indice;
}

/*
 * Desencripta los valores de nivel_general_aperturas para items obra.
*/
char *desencriptarItemsObra(char *str, const char *v_enc, const char *v_des)
{
	char *letraActual = str;
	char *letraEnc;
	int pos;

	while (*letraActual != '\0') {
		letraEnc = buscarEnVector(v_enc, longitudString(v_enc) + 1,
					  sizeof(char), letraActual, cmpChar);

		if (letraEnc) {
			pos = letraEnc - v_enc;
			*letraActual = *(v_des + pos);
		}

		letraActual++;
	}

	return str;
}

/*
 * Quita todo antes del primer guión bajo, quita los demás guiones bajos y deja en mayúscula la primer letra.
*/
char *normalizarItemsObra(char *str)
{
	char *letraActual = str;
	bool guionEncontrado = false;

	while (*letraActual != '\0') {
		if (*letraActual == '_') {
			if (!guionEncontrado) {
				if (esLetra(letraActual + 1))
					*(letraActual + 1) =
						aMayus(letraActual + 1);

				memmove(str, letraActual + 1,
					sizeof(char) *
						(longitudString(str) + 1));

				letraActual = str;
				guionEncontrado = true;
			} else
				*letraActual = ' ';
		}

		letraActual++;
	}

	return str;
}

int unirRegistros(Vector *v1, Vector *v2, Vector *vBin)
{
	Registro regTmp;
	RegistroBin regBinTmp;
	int cod_ret = TODO_OK;
	if (!v1 || !v2 || !vBin) {
		cod_ret = ERROR;
		return cod_ret;
	}
	vectorReiniciarCursor(v1);
	vectorReiniciarCursor(v2);
	vectorReiniciarCursor(vBin);

	while (!vectorCursorAlFinal(v1) && !vectorCursorAlFinal(v2)) {
		vectorLeer(v1, &regTmp);
		cod_ret = agregarRegistrosBin(&regTmp, &regBinTmp, vBin);
		if (cod_ret != EXITO) {
			fprintf(stderr, "Error al agregar registro binario\n");
			return cod_ret;
		}
		vectorLeer(v2, &regTmp);
		cod_ret = agregarRegistrosBin(&regTmp, &regBinTmp, vBin);
		if (cod_ret != EXITO) {
			fprintf(stderr, "Error al agregar registro binario\n");
			return cod_ret;
		}
	}
	return cod_ret;
}

void calcularVariaciones(Vector *vec)
{
	if (!vec || vec->cantElem < 2)
		return;

	vectorReiniciarCursor(vec);
	int i;
	for (i = 0; i < vec->cantElem; i++) {
		Registro *regActual = vectorObtener(vec, i);
		int a, m, d;
		sscanf(regActual->periodo, "%d-%d-%d", &a, &m, &d);

		int mesAnt = m == 1 ? 12 : m - 1;
		int anioMesAnt = m == 1 ? a - 1 : a;

		int anioAnt = a - 1;

		char periodoMesAnt[PERIODO_TAM];
		char periodoAnioAnt[PERIODO_TAM];

		sprintf(periodoMesAnt, "%04d-%02d-%02d", anioMesAnt, mesAnt, d);
		sprintf(periodoAnioAnt, "%04d-%02d-%02d", anioAnt, m, d);

		Registro regBusquedaMesAnt = { 0 };
		Registro regBusquedaAnioAnt = { 0 };

		copiarString(regBusquedaMesAnt.periodo, periodoMesAnt,
			     PERIODO_TAM - 1);
		copiarString(regBusquedaMesAnt.nivel, regActual->nivel,
			     NIVEL_TAM - 1);

		copiarString(regBusquedaAnioAnt.periodo, periodoAnioAnt,
			     PERIODO_TAM - 1);
		copiarString(regBusquedaAnioAnt.nivel, regActual->nivel,
			     NIVEL_TAM - 1);

		int j = vectorBuscar(vec, &regBusquedaMesAnt, cmpRegPerNiv);
		int k = vectorBuscar(vec, &regBusquedaAnioAnt, cmpRegPerNiv);

		if (j != -1) {
			Registro *regMesAnt = vectorObtener(vec, j);
			if (regMesAnt->indiceICC != 0) {
				double varMensual = ((regActual->indiceICC /
						      regMesAnt->indiceICC) -
						     1) *
						    100;

				toString(regActual->var_mensual, varMensual);
			} else {
				copiarString(regActual->var_mensual, "NA",
					     VALOR_TAM - 1);
			}
		} else {
			copiarString(regActual->var_mensual, "NA",
				     VALOR_TAM - 1);
		}

		if (k != -1) {
			Registro *regAnioAnt = vectorObtener(vec, k);
			if (regAnioAnt->indiceICC != 0) {
				double varInteranual =
					((regActual->indiceICC /
					  regAnioAnt->indiceICC) -
					 1) *
					100;
				toString(regActual->var_interanual,
					 varInteranual);
			} else {
				copiarString(regActual->var_interanual, "NA",
					     VALOR_TAM - 1);
			}
		} else {
			copiarString(regActual->var_interanual, "NA",
				     VALOR_TAM - 1);
		}
	}
}

void ordenarRegistros(Vector *vBin)
{
	if (!vBin || vBin->cantElem < 2)
		return;
	vectorOrdenar(vBin, compararRegistrosBinOpt);
}

void crearYescribirArchivoBinario(Vector *vBin, const char *path)
{
	if (!vBin || !vBin->data || vBin->cantElem <= 0) {
		fprintf(stderr, "Vector inválido\n");
		return;
	}

	FILE *arch = fopen(path, "wb");
	if (!arch) {
		fprintf(stderr, "Error al abrir el archivo %s\n", path);
		return;
	}

	size_t cantidadEscrita =
		fwrite(vBin->data, sizeof(RegistroBin), vBin->cantElem, arch);
	if (cantidadEscrita != vBin->cantElem) {
		fprintf(stderr, "Error al escribir en el archivo %s\n", path);
	}

	fclose(arch);
}

/* ------------------------------------------------ Utilitarias ------------------------------------------------ */

/*
 * Copia un archivo txt. Solo para testing, no usar para resolver los items restantes.
*/
int copiarArchivoTxt(char *nomArchDest, char *nomArchOrig)
{
	FILE *orig = fopen(nomArchOrig, "rt");

	if (!orig)
		return ERR_ARCHIVO;

	FILE *dest = fopen(nomArchDest, "wt");

	if (!dest) {
		fclose(orig);
		return ERR_ARCHIVO;
	}

	char buffer[BUFFER_TAM];
	char *dirSaltoLinea;
	char aBuscar = '\n';

	fgets(buffer, BUFFER_TAM, orig);

	dirSaltoLinea = buscarEnVector(buffer, BUFFER_TAM, sizeof(char),
				       &aBuscar, cmpChar);

	if (!dirSaltoLinea)
		return ERR_BUFFER_CORTO;

	while (!feof(orig)) {
		fprintf(dest, "%s", buffer);
		fgets(buffer, BUFFER_TAM, orig);
	}

	fclose(orig);
	fclose(dest);

	return TODO_OK;
}

void *buscarEnVector(const void *vec, size_t cantElem, size_t tamElem,
		     void *elem, Comparar cmp)
{
	void *i = (void *)vec;
	const void *ult = vec + (cantElem - 1) * tamElem;

	while (cmp(i, elem) != 0 && i <= ult)
		i += tamElem;

	if (cmp(i, elem) == 0)
		return i;

	return NULL;
}
int agregarRegistrosBin(Registro *reg, RegistroBin *regBin, Vector *vBin)
{
	int i;
	if (!reg || !regBin || !vBin)
		return ERROR;

	for (i = 0; i < 3; i++) {
		copiarString(regBin->periodo, reg->periodo, PERIODO_TAM - 1);
		copiarString(regBin->nivel_general_aperturas, reg->nivel,
			     NIVEL_TAM - 1);
		copiarString(regBin->clasificador, reg->clasificador,
			     CLASIFICADOR_TAM - 1);
		switch (i) {
		case 0:
			copiarString(regBin->tipo_variable, "indice_icc",
				     TIPO_VARIABLE_TAM - 1);
			toString(regBin->valor, reg->indiceICC);
			break;
		case 1:
			copiarString(regBin->tipo_variable, "var_mensual",
				     TIPO_VARIABLE_TAM - 1);
			copiarString(regBin->valor, reg->var_mensual,
				     VALOR_TAM - 1);
			break;
		case 2:
			copiarString(regBin->tipo_variable, "var_interanual",
				     TIPO_VARIABLE_TAM - 1);
			copiarString(regBin->valor, reg->var_interanual,
				     VALOR_TAM - 1);
			break;
		}
		if (vectorAgregar(vBin, regBin) != EXITO) {
			fprintf(stderr,
				"Error al escribir en el vector binario\n");
			return ERROR;
		}
	}
	return EXITO;
}

int compararString (const char* str1, const char* str2) {

    size_t longStr1 = longitudString(str1);
    size_t longStr2 = longitudString(str2);

    if (longStr1 != longStr2)
        return longStr1 - longStr2;

    const char* letraActualStr1 = str1;
    const char* letraActualStr2 = str2;

    while (*letraActualStr1 != '\0' && *letraActualStr1 - *letraActualStr2 == 0) {
        letraActualStr1++;
        letraActualStr2++;
    }

    return *letraActualStr1 - *letraActualStr2;
}

char* copiarString (char* dest, const char* orig, size_t lim) {

    const char* letraActualOrig = orig;
    char* letraActualDest = dest;

    while ((letraActualDest - dest) < lim && *letraActualOrig != '\0') {

        *letraActualDest = *letraActualOrig;

        letraActualOrig++;
        letraActualDest++;
    }

    *letraActualDest = '\0';

    return dest;
}

char* concatenarString (char* str1, const char* str2, size_t lim) {

    size_t longStr1 = longitudString(str1);

    const char* letraActualStr2 = str2;
    char* letraActualStr1 = str1 + longStr1;

    while ((letraActualStr1 - str1) < lim && *letraActualStr2 != '\0') {
        *letraActualStr1 = *letraActualStr2;

        letraActualStr2++;
        letraActualStr1++;
    }

    *letraActualStr1 = '\0';

    return str1;
}

/* ------------------------------------------------ Auxiliares ------------------------------------------------ */

/*Funcion auxiliar, para saber si un char es letra, igual que isalpha form ctype.h*/
int esLetra(char *c)
{
	char aux = aMayus(c);
	return (aux >= 'A' && aux <= 'Z');
}

/*Funcion auxiliar, pasa el caracter a mayúsculas */
char aMayus(char *c)
{
	char aux = *c;
	aux -= 32;
	return aux;
}

size_t longitudString (const char* str) {
    char* strP = (char*) str;

    while (*strP != '\0')
        strP++;

    return strP - str;
}

/*
 * Realiza la comparación entre dos char.
*/
int cmpChar(const void *a, const void *b)
{
	const char *c1 = a;
	const char *c2 = b;

	return *c1 - *c2;
}

int cmpRegPerNiv(const void *a, const void *b)
{
	const Registro *r1 = a;
	const Registro *r2 = b;
	int cmpPeriodo = compararString(r1->periodo, r2->periodo);
	if (cmpPeriodo != 0)
		return cmpPeriodo;
	int cmpNivel = compararString(r1->nivel, r2->nivel);
	if (cmpNivel != 0)
		return cmpNivel;
	return 0;
}
void eliminar_comillas(char *str)
{
	if (!str)
		return;

	char *dest = str;
	char *src = str;

	while (*src && *src != '\n') {
		if (*src != '"') {
			*dest++ = *src;
		}
		src++;
	}
	*dest = '\0';
}
int compararRegistrosBinOpt(const void *a, const void *b)
{
	const RegistroBin *datoA = (const RegistroBin *)a;
	const RegistroBin *datoB = (const RegistroBin *)b;

	int pesoA = 0, pesoB = 0;

	if (compararString(datoA->tipo_variable, "indice_icc") == 0)
		pesoA = 3;
	else if (compararString(datoA->tipo_variable, "var_mensual") == 0)
		pesoA = 2;
	else
		pesoA = 1;

	if (compararString(datoB->tipo_variable, "indice_icc") == 0)
		pesoB = 3;
	else if (compararString(datoB->tipo_variable, "var_mensual") == 0)
		pesoB = 2;
	else
		pesoB = 1;

	if (pesoA != pesoB) {
		return pesoB - pesoA;
	}

	int cmpPeriodo = compararString(datoA->periodo, datoB->periodo);
	if (cmpPeriodo != 0)
		return cmpPeriodo;
	return compararString(datoA->clasificador, datoB->clasificador);
}
