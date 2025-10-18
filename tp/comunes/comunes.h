#ifndef COMUNES_INCLUDED
#define COMUNES_INCLUDED

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../cadenas/cadenas.h"

#define EXITO 100
#define ERR_PUNTERO_NULO 0
#define ERR_SIN_MEM 1
#define ERR_ARCH 2
#define ERR_BUFFER_CORTO 3
#define ERR_REGISTRO 4
#define ERR_USUARIO 5

#define TRY(stmt) do{           \
                if((stmt) != EXITO){ \
                    logError((stmt), __FILE__, __LINE__, (char*)__func__);               \
                               \
}}while(0);      \
                                \

int logError(int cod, char* file, int line, char* func);

void* malloc_s(size_t n); 
void* calloc_s(size_t n, size_t tam);
void* realloc_s(void* ptr, size_t n);
int free_s(void* ptr);

#endif