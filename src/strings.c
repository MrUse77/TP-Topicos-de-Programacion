#include <string.h>
#include "../include/strings.h"
#include "../include/funciones.h"
#include <stdio.h>

int compararString(const char *str1, const char *str2)
{
	size_t longStr1 = longitudString(str1);
	size_t longStr2 = longitudString(str2);

	if (longStr1 != longStr2)
		return longStr1 - longStr2;

	const char *letraActualStr1 = str1;
	const char *letraActualStr2 = str2;

	while (*letraActualStr1 != '\0' &&
	       *letraActualStr1 - *letraActualStr2 == 0) {
		letraActualStr1++;
		letraActualStr2++;
	}

	return *letraActualStr1 - *letraActualStr2;
}

char *copiarString(char *dest, const char *orig, size_t lim)
{
	const char *letraActualOrig = orig;
	char *letraActualDest = dest;

	while ((letraActualDest - dest) < lim && *letraActualOrig != '\0') {
		*letraActualDest = *letraActualOrig;

		letraActualOrig++;
		letraActualDest++;
	}

	*letraActualDest = '\0';

	return dest;
}

char *concatenarString(char *str1, const char *str2, size_t lim)
{
	size_t longStr1 = longitudString(str1);

	const char *letraActualStr2 = str2;
	char *letraActualStr1 = str1 + longStr1;

	while ((letraActualStr1 - str1) < lim && *letraActualStr2 != '\0') {
		*letraActualStr1 = *letraActualStr2;

		letraActualStr2++;
		letraActualStr1++;
	}

	*letraActualStr1 = '\0';

	return str1;
}

char *removerCharDeString(char *str, char c)
{
	char *letraActualStr = str;

	while (*letraActualStr != '\0') {
		if (*letraActualStr == c) {
			size_t longSeg = longitudString(letraActualStr);
			memmove(letraActualStr, letraActualStr + 1,
				sizeof(char) * (longSeg + 1));
		}

		letraActualStr++;
	}

	return str;
}

size_t longitudString(const char *str)
{
	char *strP = (char *)str;

	while (*strP != '\0')
		strP++;

	return strP - str;
}
void toString(char *str, double num)
{
	if (!str)
		return;
	sprintf(str, "%.17f", num);
}
