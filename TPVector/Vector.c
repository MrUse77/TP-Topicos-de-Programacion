#include "Vector.h"
#include "../TPString/string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int ordenamientoInsercion (Vector* v, Cmp comparar);
bool redimensionarVector (Vector* v, int operacion);
bool redimensionarVectorA (Vector* v, size_t redimension);
size_t max (size_t a, size_t b);

int vectorCrear (Vector* v, size_t tamElem) {

    v -> vec = malloc(tamElem * CAP_INICIAL);

    if (v -> vec == NULL) {
        v -> cantElem = 0;
        v -> cap = 0;
        v -> cur = NULL;
        return SIN_MEM;
    }

    v -> cantElem = 0;
    v -> cap = CAP_INICIAL;
    v -> tamElem = tamElem;
    v -> cur = v -> vec;

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
            cod = vectorAgregarAlFinal(v, reg);
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

int vectorAgregarAlFinal (Vector* v, void* elem) {

    if (v -> cantElem == v -> cap) {
        if (!redimensionarVector(v, AUMENTAR))
            return SIN_MEM;
    }

    void* posIns = v -> vec + v -> cantElem * v -> tamElem;
    memcpy(posIns, elem, v -> tamElem);
    v -> cantElem++;

    return TODO_OK;
}

void vectorMostrar (Vector* v, Imp imprimir) {

    void* ult = v -> vec + (v -> cantElem -1) * v -> tamElem;
    void* i;

    for (i = v -> vec; i <= ult; i += v -> tamElem)
        imprimir(i);
}

int vectorDesordBuscar (Vector* v, void* elem, Cmp comparar) {

    void* ult = v -> vec + (v -> cantElem -1) * v -> tamElem;
    void* i = v -> vec;
    int pos = -1;

    while (pos == -1 && i <= ult) {
        if (comparar(i, elem) == 0)
            pos = (i - v -> vec) / v -> tamElem;
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

    const void *li, *ls, *m;

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

    return (m - v -> vec) / v -> tamElem;
}

int vectorUnir (Vector* v1, Vector* v2) {

    size_t cantElemTotal, tamElem;
    void* iv2;

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

void vectorEliminarDePos (Vector* v, int pos) {

    void* posElim = v -> vec + pos * v -> tamElem;
    void* final = v -> vec + v -> cantElem * v -> tamElem;

    memmove(posElim, posElim + v -> tamElem, final - posElim);

    v -> cantElem--;
}

void vectorDestruir (Vector* v) {
    free(v -> vec);

    v -> vec = NULL;
    v -> cap = 0;
    v -> tamElem = 0;
    v -> cantElem = 0;
    v -> cur = NULL;
}

int vectorOrdenar (Vector* v, int metodo, Cmp comparar) {

    int cod;

    switch (metodo) {
        case INSERCION:
            cod = ordenamientoInsercion(v, comparar);
            break;
    }

    return cod;
}

int ordenamientoInsercion (Vector* v, Cmp comparar) {
    void* ult = v -> vec + (v -> cantElem -1) * v -> tamElem;
    void *i, *j, *elem;

    elem = malloc(v -> tamElem);

    if (!elem)
        return SIN_MEM;

    for (i = v -> vec + v -> tamElem; i <= ult; i += v -> tamElem) {
        memcpy(elem, i, v -> tamElem);
        j = i - v -> tamElem;

        while (j >= v -> vec && comparar(j, elem) > 0)
            j -= v -> tamElem;

        memmove(j + v -> tamElem * 2, j + v -> tamElem, i - (j + v -> tamElem));
        memcpy(j + v -> tamElem, elem, v -> tamElem);
    }

    free(elem);

    return TODO_OK;
}

bool redimensionarVector (Vector* v, int operacion) {

    int posCur;

    size_t nuevaCap = operacion == AUMENTAR
    ? v -> cap * FACT_INCR
    : max(CAP_INICIAL, v -> cap * FACT_DECR);

    void* nVec = realloc(v -> vec, nuevaCap * v -> tamElem);

    if (nVec == NULL) {
        v -> cap = 0;
        return false;
    }

    posCur = v -> cur - v -> vec;

    v -> vec = nVec;
    v -> cap = nuevaCap;
    v -> cur = v -> vec + posCur;

    return true;
}

bool redimensionarVectorA (Vector* v, size_t redimension) {

    int posCur;
    size_t nuevaCap = max(CAP_INICIAL, redimension);
    void* nVec = realloc(v -> vec, nuevaCap * v -> tamElem);

    if (nVec == NULL) {
        v -> cap = 0;
        return false;
    }

    posCur = v -> cur - v -> vec;

    v -> vec = nVec;
    v -> cap = nuevaCap;
    v -> cur = v -> vec + posCur;

    return true;
}

size_t max (size_t a, size_t b) {
    return a >= b ? a : b;
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

    void* siguiente = it -> act + v -> tamElem;

    if (siguiente > it -> ult) {
        it -> finIter = true;
        return NULL;

    } else if (siguiente <= v -> vec)
        return NULL;

    it -> act = siguiente;

    return siguiente;
}

void* vectorIteradorDesplazamiento (VectorIterador *it, size_t cantidad) {
    const Vector *v = it -> vector;
    void* cur = it -> act + v-> tamElem * cantidad;

    if (cur < v -> vec || cur > it -> ult)
        return NULL;

    it -> act = cur;

    return it -> act;
}

bool vectorIteradorFin (VectorIterador* it) {
    return it -> finIter;
}