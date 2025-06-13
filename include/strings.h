#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

char* concatenarString (char* str1, const char* str2, size_t lim);
char* copiarString (char* dest, const char* orig, size_t lim);
int compararString (const char* str1, const char* str2);
size_t longitudString (const char* str);
char* removerCharDeString (char* str, char c);

#endif