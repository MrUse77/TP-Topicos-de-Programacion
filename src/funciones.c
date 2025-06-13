#include "../include/funciones.h"
#include "../include/strings.h"

/* Prototipos internos */
typedef int (*Cmp) (void* a, void* b);

int generarArchivoAuxiliar (FILE* temp, FILE* orig, Formatear realizarFormateos);

char* corregirFormatoDeFecha(Registro* registro);
char* agregarCeroALaIzquierda(Registro* registro);
char* desencriptarNivelGeneral(Registro* registro);
char* normalizarNivelSinGuiones(Registro* registro);
char* corregirFormatoDecimal(Registro* registro);
char* agregarCampoEnCabecera (char* buffer, char* nombreCampo, Registro* reg, FILE* temp, FILE* orig);
char* desencriptarItemsObra (char* str, const char* v_enc, const char* v_des);
char* normalizarItemsObra (char* str);

void* buscarEnVector (const void* vec, size_t cantElem, size_t tamElem, void* elem, Cmp cmp);

int esLetra(char* c);
char aMayus(char* c);
int cmpChar (void* a, void* b);


/* Declaraciones */

/* ------------------------------------------------ Principales ------------------------------------------------ */

/*Funcion general, procesa el archivo de indices generales*/
int corregirArchivo (char* nomArchOrig, Formatear realizarFormateos) {

    FILE* orig = fopen(nomArchOrig, "rt");

    if (!orig)
        return ERR_ARCHIVO;

    FILE* temp = fopen(NOMBRE_ARCH_AUXILIAR, "wt");

    if (!temp) {
        fclose(orig);
        return ERR_ARCHIVO;
    }

    int codRet = generarArchivoAuxiliar(temp, orig, realizarFormateos);

    fclose(temp);
    fclose(orig);

    if (codRet == TODO_OK) {
        remove(nomArchOrig);
        rename(NOMBRE_ARCH_AUXILIAR, nomArchOrig);
    }

    return codRet;
}

int generarArchivoAuxiliar (FILE* temp, FILE* orig, Formatear realizarFormateos) {
    Registro reg;
    char buffer[BUFFER_TAM];
    char* dirSaltoLinea;
    char aBuscar = '\n';

    int codRet = TODO_OK;

    /* Obtiene la linea de la cabecera y añade el campo clasificador. */
    if (!agregarCampoEnCabecera(buffer, "Clasificador", &reg, temp, orig))
        return ERR_BUFFER_CORTO;

    /* Obtiene la siguiente linea y verifica que esta posea el formato correcto. */
    fgets(buffer, BUFFER_TAM, orig);

    dirSaltoLinea = buscarEnVector(buffer, BUFFER_TAM, sizeof(char), &aBuscar, cmpChar);

    if (!dirSaltoLinea)
        return ERR_BUFFER_CORTO;

    *dirSaltoLinea = '\0';

    while (codRet != ERR_BUFFER_CORTO && !feof(orig)) {

        /* Obtiene los valores de los campos y realiza los formateos. */
        sscanf(buffer, FORMATO_REGISTROS, reg.periodo, reg.nivel, reg.indiceICC);

        realizarFormateos(&reg);

        /* Escribe el registro formateado en el archivo auxiliar. */
        fprintf(temp, "\"%s\";\"%s\";%s;\"%s\"\n", reg.periodo, reg.nivel, reg.indiceICC, reg.clasificador);

        /* Obtiene la siguiente linea y verifica que esta posea el formato correcto. */
        fgets(buffer, BUFFER_TAM, orig);

        dirSaltoLinea = buscarEnVector(buffer, BUFFER_TAM, sizeof(char), &aBuscar, cmpChar);

        if (!dirSaltoLinea) {
            if (!feof(orig))
                codRet = ERR_BUFFER_CORTO;
        } else
            *dirSaltoLinea = '\0';

    }

    return codRet;
}

/*
* Realiza todas las correcciones para indices_icc_general_capitulos.csv
*/
void formatearNivelGeneral (Registro* reg) {

    corregirFormatoDeFecha(reg);
    agregarCeroALaIzquierda(reg);
    corregirFormatoDecimal(reg);
    desencriptarNivelGeneral(reg);
    normalizarNivelSinGuiones(reg);

    copiarString(
        reg -> clasificador,
        compararString(reg -> nivel, "Nivel general") == 0 ? "Nivel general" : "Capítulos",
        CLASIFICADOR_TAM -1
    );
}

/*
* Realiza todas las correcciones para Indices_items_obra.csv
*/
void formatearItemsObra (Registro* reg) {

    const char v_enc[] = "@8310$7|59";
    const char v_des[] = "abeiostlmn";

    corregirFormatoDeFecha(reg);
    agregarCeroALaIzquierda(reg);
    corregirFormatoDecimal(reg);
    desencriptarItemsObra(reg -> nivel, v_enc, v_des);
    normalizarItemsObra(reg -> nivel);

    copiarString(reg -> clasificador, "Ítems", CLASIFICADOR_TAM -1);
}

/*Funcion general, corrige el formato de la fecha de dd/mm/aaaa a dd-mm-aaaa */
char* corregirFormatoDeFecha(Registro* reg)
{
    char* p = reg->periodo;

    while(*p){
        if(*p == '/'){
            *p = '-';
        }

        p++;
    }

    return reg->periodo;
}

/*Funcion general, agrega los "leading zeroes" a los valores de la fecha*/
/*Como contra, la fecha pasa a ser un valor int, no se si estaria bien*/
char* agregarCeroALaIzquierda(Registro* reg)
{
    char* p = reg -> periodo;
    int d, m, a;

    sscanf(p, "%d-%d-%d", &d, &m, &a);
    sprintf(p, "%04d-%02d-%02d", a, m, d);

    return reg -> periodo;
}

/*Funcion general, desarrolla el proceso de decriptacion del campo nivel general*/
char* desencriptarNivelGeneral (Registro* reg)
{
    size_t contador = 0;
    char* p = reg->nivel;

    while(*p){
        if(esLetra(p)){
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
char* normalizarNivelSinGuiones(Registro* reg)
{
    char* p = reg->nivel;
    *p = aMayus(p);

    p++;

    while(*p){
        if(*p == '_'){
            *p = ' ';
        }

        p++;
    }

    return reg->nivel;
}

/*Funcion general, corrige el formato del campo indice de f*,f* a f*.f* */
char* corregirFormatoDecimal(Registro* reg)
{
    char* p = reg->indiceICC;

    while(*p){
        if(*p == ','){
            *p = '.';
        }

        p++;
    }

    return reg->indiceICC;
}

/*
 * Copia el valor del campo agregado a la cabecera del archivo.
*/
char* agregarCampoEnCabecera (char* buffer, char* nombreCampo, Registro* reg, FILE* temp, FILE* orig) {

    char* dirSaltoLinea;
    char aBuscar = '\n';
    fgets(buffer, BUFFER_TAM, orig);

    dirSaltoLinea = buscarEnVector(buffer, BUFFER_TAM, sizeof(char), &aBuscar, cmpChar);

    if (!dirSaltoLinea)
        return NULL;

    *dirSaltoLinea = '\0';

    sscanf(buffer, FORMATO_CABECERA, reg -> periodo, reg -> nivel, reg -> indiceICC);
    fprintf(temp, "\"%s\";\"%s\";\"%s\";\"%s\"\n", reg -> periodo, reg -> nivel, reg -> indiceICC, nombreCampo);

    return buffer;
}

/*
 * Desencripta los valores de nivel_general_aperturas para items obra.
*/
char* desencriptarItemsObra (char* str, const char* v_enc, const char* v_des) {

    char* letraActual = str;
    char* letraEnc;
    int pos;

    while (*letraActual != '\0') {

        letraEnc = buscarEnVector(
            v_enc,
            longitudString(v_enc) +1,
            sizeof(char),
            letraActual,
            cmpChar
        );

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
char* normalizarItemsObra (char* str) {

    char* letraActual = str;
    bool guionEncontrado = false;

    while (*letraActual != '\0') {

        if (*letraActual == '_') {

            if (!guionEncontrado) {

                if (esLetra(letraActual +1))
                    *(letraActual +1) = aMayus(letraActual +1);

                memmove(str, letraActual +1, sizeof(char) * (longitudString(str) +1));

                letraActual = str;
                guionEncontrado = true;
            }
            else
                *letraActual = ' ';
        }

        letraActual++;
    }

    return str;
}


/* ------------------------------------------------ Utilitarias ------------------------------------------------ */

/*
 * Copia un archivo txt. Solo para testing, no usar para resolver los items restantes.
*/
int copiarArchivoTxt (char* nomArchDest, char* nomArchOrig) {

    FILE* orig = fopen(nomArchOrig, "rt");

    if (!orig)
        return ERR_ARCHIVO;

    FILE* dest = fopen(nomArchDest, "wt");

    if (!dest) {
        fclose(orig);
        return ERR_ARCHIVO;
    }

    char buffer[BUFFER_TAM];
    char* dirSaltoLinea;
    char aBuscar = '\n';

    fgets(buffer, BUFFER_TAM, orig);

    dirSaltoLinea = buscarEnVector(buffer, BUFFER_TAM, sizeof(char), &aBuscar, cmpChar);

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

void* buscarEnVector (const void* vec, size_t cantElem, size_t tamElem, void* elem, Cmp cmp) {

    void* i = (void*) vec;
    const void* ult = vec + (cantElem -1) * tamElem;

    while (cmp(i, elem) != 0 && i <= ult)
        i += tamElem;

    if (cmp(i, elem) == 0)
        return i;

    return NULL;
}

/* ------------------------------------------------ Auxiliares ------------------------------------------------ */

/*Funcion auxiliar, para saber si un char es letra, igual que isalpha form ctype.h*/
int esLetra (char* c)
{
    char aux = aMayus(c);
    return (aux >= 'A' && aux <= 'Z');
}

/*Funcion auxiliar, pasa el caracter a mayúsculas */
char aMayus (char* c)
{
    char aux = *c;
    aux -= 32;
    return aux;
}

/*
 * Realiza la comparación entre dos char.
*/
int cmpChar (void* a, void* b) {
    char* c1 = a;
    char* c2 = b;

    return *c1 - *c2;
}
