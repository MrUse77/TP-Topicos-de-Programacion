#ifndef CADENAS_H
#define CADENAS_H

#include <stdlib.h>
#include <stddef.h>
#include <memory.h>
#include <ctype.h>

//Manipulacion de cadenas

char* stringCopy(char* dest, char* src);
char* stringNCopy(char* dest, char* src, size_t n);
char* stringDup(char* str);
char* stringNDup(char* str, size_t n);
char* stringCat(char* dest, char* src);
char* stringNCat(char* dest, char* src, size_t n);
char* stringToUpper(char* str);
char* stringToLower(char* str);
char* stringReverse(char* str);

//Examinacion de cadenas

int stringCmp(char* dest, char* src);
int stringNCmp(char* dest, char* src, size_t n);
int stringCmpi(char* dest, char* src);
int stringNCmpi(char* dest, char* src, size_t n);
char* stringChar(char* dest, int ch);
char* stringRChar(char* dest, int ch);
int stringCSpan(char* dest, char* src);
int stringSpan(char* dest, char* src);
char* stringPbreak(char* dest, char* src);
char* stringSubstring(char* dest, char* src);
size_t stringLenght(char* str);

#endif