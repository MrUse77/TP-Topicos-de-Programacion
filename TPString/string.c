#include <string.h> /* Solo para usar memmove */
#include "string.h"

char* buscarEnString (const char* str, char c) {

    char* letraActual = (char*) str;

    while (*letraActual != c && *letraActual != '\0')
        letraActual++;

    if (*letraActual == c)
        return letraActual;

    return NULL;
}

char* buscarEnStringReversa (const char* str, char c) {

    size_t longStr = longitudString(str);
    char* letraActual = (char*) str + longStr -1;

    while (letraActual >= str && *letraActual != c)
        letraActual--;

    if (*letraActual == c)
        return letraActual;

    return NULL;
}

char* reemplazarCharEnString (char* str, char orig, char reemp) {

    char* letraActual = str;

    while (*letraActual != '\0') {

        if (*letraActual == orig)
            *letraActual = reemp;

        letraActual++;
    }

    return str;
}

char* removerCharEnString (char* str, char c) {

    char* letraActual = str;
    size_t longSeg;

    while (*letraActual != '\0') {

        if (*letraActual == c) {
            longSeg = longitudString(letraActual +1);
            memmove(letraActual, letraActual +1, longSeg +1); /* con longSeg +1 también toma el byte del /0 */
        }

        letraActual++;
    }

    return str;
}

char* copiarString (char* dest, const char* orig, size_t lim) {

    char* letraActualDest = dest;
    const char* letraActualOrig = orig;

    while ((letraActualDest - dest < lim) && *letraActualOrig != '\0') {

        *letraActualDest = *letraActualOrig;

        letraActualOrig++;
        letraActualDest++;
    }

    *letraActualDest = '\0';

    return dest;
}

int compararString (const void* str1, const void* str2) {

    size_t longStr1 = longitudString(str1);
    size_t longStr2 = longitudString(str2);

    const char* letraActualStr1 = str1;
    const char* letraActualStr2 = str2;

    int dif = *letraActualStr1 - *letraActualStr2;

    if (longStr1 != longStr2)
        return longStr1 - longStr2;

    while (dif == 0 && *letraActualStr1 != '\0') {
        letraActualStr1++;
        letraActualStr2++;

        dif = *letraActualStr1 - *letraActualStr2;
    }

    return dif;
}

char* concatenarString (char* str1, const char* str2, size_t lim) {

    char* letraActualStr1 = str1 + longitudString(str1);
    const char* letraActualStr2 = str2;

    while (letraActualStr1 - str1 < lim && *letraActualStr2 != '\0') {
        *letraActualStr1 = *letraActualStr2;

        letraActualStr1++;
        letraActualStr2++;
    }

    *letraActualStr1 = '\0';

    return str1;
}

size_t longitudString (const char* str) {

    const char* letraActual = str;

    while (*letraActual != '\0')
        letraActual++;

    return letraActual - str;
}
