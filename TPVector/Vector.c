#include "Vector.h"
#include "../TPString/string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int ordenamientoBurbujeo (Vector* v, Cmp comparar);
int ordenamientoSeleccion (Vector* v, Cmp comparar);
int ordenamientoInsercion (Vector* v, Cmp comparar);
bool redimensionarVector (Vector* v, int operacion);
bool redimensionarVectorA (Vector* v, size_t redimension);
size_t max (size_t a, size_t b);
void intercambiar (void* a, void* b, void* aux, size_t tamElem);
void* buscarMenor (void* ini, void* fin, size_t tamElem, Cmp comparar);

int vectorCrear (Vector* v, size_t tamElem) {

    v -> vec = malloc(tamElem * CAP_INICIAL);

    if (v -> vec == NULL) {
        v -> cantElem = 0;
        v -> cap = 0;
        return SIN_MEM;
    }

    v -> cantElem = 0;
    v -> cap = CAP_INICIAL;
    v -> tamElem = tamElem;

    return TODO_OK;
}

int vectorCargarDeTxt (Vector* v, const char* nomArchTxt, Fmt formatear, size_t bufferTam) {

    FILE* archTxt = fopen(nomArchTxt, "rt");
    void* reg;
    int cod = TODO_OK;
    char* buffer;

    if (!archTxt)
        return ERR_ARCHIVO;

    /* Esto nos evita declararlo como un VLA en el stack. */
    buffer = malloc(sizeof(char) * (bufferTam +1));

    if (!buffer) {
        fclose(archTxt);
        return SIN_MEM;
    }

    reg = malloc(v -> tamElem);

    if (!reg) {
        free(buffer);
        fclose(archTxt);
        return SIN_MEM;
    }

    while (cod == TODO_OK && fgets(buffer, bufferTam, archTxt)) {

        cod = formatear(reg, buffer);

        if (cod == TODO_OK)
            cod = vectorInsertarAlFinal(v, reg);
    }

    fclose(archTxt);
    free(reg);
    free(buffer);

    return cod;
}

int vectorCargarDeBin (Vector* v, const char* nomArchBin) {

    FILE* archBin = fopen(nomArchBin, "rb");
    size_t cantReg;

    if (!archBin)
        return ERR_ARCHIVO;

    fseek(archBin, 0, SEEK_END);

    cantReg = ftell(archBin) / v -> tamElem;
    redimensionarVectorA(v, cantReg);

    fseek(archBin, 0, SEEK_SET);
    fread(v -> vec, v -> tamElem, cantReg, archBin);

    v -> cantElem = cantReg;

    fclose(archBin);

    return TODO_OK;
}

int vectorGuardarEnBin (Vector* v, const char* nomArchBin) {

    FILE* arch = fopen(nomArchBin, "wb");

    if (!arch)
        return ERR_ARCHIVO;

    fwrite(v -> vec, v -> tamElem, v -> cantElem, arch);
    fclose(arch);

    return TODO_OK;
}

int vectorOrdInsertar (Vector* v, void* elem, Cmp comparar) {

    if (v -> cantElem == v -> cap) {
        if (!redimensionarVector(v, AUMENTAR))
            return SIN_MEM;
    }

    char *i, *posIns, *ult;
    ult = v -> vec + (v -> cantElem -1) * v -> tamElem;

    posIns = v -> vec;

    while (posIns <= ult && comparar(posIns, elem) < 0)
        posIns += v -> tamElem;

    if (posIns <= ult && comparar(posIns, elem) == 0)
        return DUPLICADO;

    for (i = ult; i >= posIns; i -= v -> tamElem)
        memcpy(i + v -> tamElem, i, v -> tamElem);

    memcpy(posIns, elem, v -> tamElem);
    v -> cantElem++;

    return TODO_OK;
}

int vectorInsertarAlFinal (Vector* v, void* elem) {

    if (v -> cantElem == v -> cap) {
        if (!redimensionarVector(v, AUMENTAR))
            return SIN_MEM;
    }

    char* posIns = v -> vec + v -> cantElem * v -> tamElem;
    memcpy(posIns, elem, v -> tamElem);
    v -> cantElem++;

    return TODO_OK;
}

bool vectorEliminarDePos (Vector* v, int pos) {

    if (pos < 0 || pos > v -> cantElem -1)
        return false;

    char* posElim = v -> vec + pos * v -> tamElem;
    char* final = v -> vec + v -> cantElem * v -> tamElem;

    memmove(posElim, posElim + v -> tamElem, final - posElim);

    v -> cantElem--;

    if ((float) v -> cantElem / v -> cap <= FACTOR_OCUP)
        redimensionarVector(v, DISMINUIR);

    return true;
}

bool vectorOrdEliminar (Vector* v, void* elem, Cmp comparar) {

    int pos = vectorOrdBuscar(v, elem, comparar);

    if (pos == -1)
        return false;

    vectorEliminarDePos(v, pos);     

    return true;
}

void vectorMostrar (Vector* v, Imp imprimir) {

    char* ult = v -> vec + (v -> cantElem -1) * v -> tamElem;
    char* i;

    for (i = v -> vec; i <= ult; i += v -> tamElem)
        imprimir(i);
}

int vectorDesordBuscar (Vector* v, void* elem, Cmp comparar) {

    char* ult = v -> vec + (v -> cantElem -1) * v -> tamElem;
    char* i = v -> vec;
    char* origen =  v -> vec;
    int pos = -1;

    while (pos == -1 && i <= ult) {
        if (comparar(i, elem) == 0)
            pos = (i - origen) / v -> tamElem;
        else
            i += v -> tamElem;
    }

    if (pos != -1)
        memcpy(elem, i, v -> tamElem);

    return pos;
}

int vectorOrdBuscar(const Vector* v, void* elem, Cmp comparar) {

    if (v -> cantElem == 0)
        return -1;

    const char *li, *ls, *m, *origen = v -> vec;

    li = v -> vec;
    ls = v -> vec + (v -> cantElem -1) * v -> tamElem;
    m = li + (((ls - li) / v -> tamElem) / 2) * v -> tamElem;

    while (li <= ls && comparar(m, elem) != 0) {

        if (comparar(elem, m) > 0)
            li = m + v -> tamElem;
        else
            ls = m - v -> tamElem;

        m = li + (((ls - li) / v -> tamElem) / 2) * v -> tamElem;
    }

    if (li > ls)
        return -1;

    memcpy(elem, m, v -> tamElem);

    return (m - origen) / v -> tamElem;
}

int vectorUnir (Vector* v1, Vector* v2) {

    size_t cantElemTotal, tamElem;
    char* iv2;

    if (v1 -> tamElem != v2 -> tamElem)
        return TAMANOS_INCOMPATIBLES;

    cantElemTotal = v1 -> cantElem + v2 -> cantElem;

    if (v1 -> cap < cantElemTotal) {
        if (!redimensionarVectorA(v1, cantElemTotal))
            return SIN_MEM;
    }

    tamElem = v1 -> tamElem;
    iv2 = v1 -> vec + v1 -> cantElem * tamElem;

    memcpy(iv2, v2 -> vec, v2 -> cantElem * tamElem);

    v1 -> cantElem += v2 -> cantElem;

    return TODO_OK;
}

void vectorDestruir (Vector* v) {
    free(v -> vec);

    v -> vec = NULL;
    v -> cap = 0;
    v -> tamElem = 0;
    v -> cantElem = 0;
}

void vectorRecorrer (const Vector* v, Accion accion, void* datos) {

    char* i;
    char* ult = v -> vec + (v -> cantElem -1) * v -> tamElem;

    for (i = v -> vec; i <= ult; i += v -> tamElem)
        accion(i, datos);
}

int vectorOrdenar (Vector* v, int metodo, Cmp comparar) {

    int cod = NO_ELEGIDO;

    switch (metodo) {
        case BURBUJEO:
            cod = ordenamientoBurbujeo(v, comparar);
            break;

        case SELECCION:
            cod = ordenamientoSeleccion(v, comparar);
            break;

        case INSERCION:
            cod = ordenamientoInsercion(v, comparar);
            break;
    }

    return cod;
}

int ordenamientoInsercion (Vector* v, Cmp comparar) {

    char* ult = v -> vec + (v -> cantElem -1) * v -> tamElem;
    char *i, *j, *elem;

    elem = malloc(v -> tamElem);

    if (!elem)
        return SIN_MEM;

    for (i = v -> vec + v -> tamElem; i <= ult; i += v -> tamElem) {
        memcpy(elem, i, v -> tamElem);
        j = i - v -> tamElem;

        while (j >= (char*) v -> vec && comparar(j, elem) > 0)
            j -= v -> tamElem;

        memmove(j + v -> tamElem * 2, j + v -> tamElem, i - (j + v -> tamElem));
        memcpy(j + v -> tamElem, elem, v -> tamElem);
    }

    free(elem);

    return TODO_OK;
}

int ordenamientoBurbujeo (Vector* v, Cmp comparar) {

    void* aux = malloc(v -> tamElem);

    if (!aux)
        return SIN_MEM;

    int i;
    void *j, *ult;
    ult = v -> vec + (v -> cantElem -1) * v -> tamElem;

    for (i = 0; i < v -> cantElem -1; i++) {

        for (j = v -> vec; j < ult - i * v -> tamElem; j += v -> tamElem) {
            if (comparar(j, j + v -> tamElem) > 0) {
                intercambiar(j, j + v -> tamElem, aux, v -> tamElem);
            }
        }
    }

    free(aux);

    return TODO_OK;
}

int ordenamientoSeleccion (Vector* v, Cmp comparar) {

    void* aux = malloc(v -> tamElem);

    if (!aux)
        return SIN_MEM;

    char *i, *m, *ult;
    ult = v -> vec + (v -> cantElem -1) * v -> tamElem;

    for (i = v -> vec; i < ult; i += v -> tamElem) {
        m = buscarMenor(i, ult, v -> tamElem, comparar);
        intercambiar(i, m, aux, v -> tamElem);
    }

    free(aux);

    return TODO_OK;
}


bool redimensionarVector (Vector* v, int operacion) {

    size_t nuevaCap = operacion == AUMENTAR
    ? v -> cap * FACT_INCR
    : max(CAP_INICIAL, v -> cap * FACT_DECR);

    char* nVec = realloc(v -> vec, nuevaCap * v -> tamElem);

    if (nVec == NULL) {
        v -> cap = 0;
        return false;
    }

    v -> vec = nVec;
    v -> cap = nuevaCap;

    return true;
}

bool redimensionarVectorA (Vector* v, size_t redimension) {

    size_t nuevaCap = max(CAP_INICIAL, redimension);
    char* nVec = realloc(v -> vec, nuevaCap * v -> tamElem);

    if (nVec == NULL) {
        v -> cap = 0;
        return false;
    }

    v -> vec = nVec;
    v -> cap = nuevaCap;

    return true;
}

size_t max (size_t a, size_t b) {
    return a >= b ? a : b;
}

void intercambiar (void* a, void* b, void* aux, size_t tamElem) {
    memcpy(aux, a, tamElem);
    memcpy(a, b, tamElem);
    memcpy(b, aux, tamElem);
}

void* buscarMenor (void* ini, void* fin, size_t tamElem, Cmp comparar) {

    char *m, *j;
    m = ini;

    for (j = ini + tamElem; j <= (char*) fin; j += tamElem) {

        if (comparar(j, m) < 0)
            m = j;
    }

    return m;
}

/* Vector iterador */

void vectorIteradorCrear (VectorIterador* it, const Vector* v) {
    it -> act = NULL;
    it -> ult = NULL;
    it -> finIter = true;
    it -> vector = (Vector*) v;
}

void* vectorIteradorPrimero (VectorIterador* it) {
    Vector* v = it -> vector;

    if (v -> cantElem == 0) {
        it -> act = NULL;
        it -> ult = NULL;
        it -> finIter = true;
        return NULL;
    }

    it -> act = v -> vec;
    it -> ult = v -> vec + (v -> cantElem -1) * v -> tamElem;
    it -> finIter = false;

    return it -> act;
}

void* vectorIteradorSiguiente (VectorIterador* it) {

    Vector* v = it -> vector;

    char* siguiente = it -> act + v -> tamElem;

    if (siguiente > (char*) it -> ult) {
        it -> finIter = true;
        return NULL;

    } else if (siguiente <= (char*) v -> vec)
        return NULL;

    it -> act = siguiente;

    return siguiente;
}

void* vectorIteradorDesplazamiento (VectorIterador *it, size_t cantidad) {
    const Vector *v = it -> vector;
    char* cur = it -> act + v-> tamElem * cantidad;

    if (cur < (char*) v -> vec || cur > (char*) it -> ult)
        return NULL;

    it -> act = cur;

    return it -> act;
}

bool vectorIteradorFin (VectorIterador* it) {
    return it -> finIter;
}
