#ifndef SECUENCIA_PALABRAS_INCLUDED
#define SECUENCIA_PALABRAS_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "../comunes/comunes.h"

#define DEFAULT_PAL_TAM 50

typedef struct{
    char* cadena;
    bool finDeCadena;
}SecuenciaPalabras_t;

typedef struct{
    char cadena[DEFAULT_PAL_TAM];
}Palabra_t;

int secuenciaPalabrasCrear(SecuenciaPalabras_t* sec, char* cadena);
bool secuenciaPalabrasLeer(SecuenciaPalabras_t* sec, Palabra_t* pal);
int secuenciaPalabrasEscribir(SecuenciaPalabras_t* sec, Palabra_t* pal);
int secuenciaPalabrasEscribirCaract(SecuenciaPalabras_t* sec, int ch);
int secuenciaPalabrasCerrar(SecuenciaPalabras_t* sec);
bool secuenciaPalabrasEsFin(SecuenciaPalabras_t* sec);

int palabraModificar(Palabra_t* pal, void (*Modificar)(Palabra_t*));
void palabraMostrar(Palabra_t* pal);

#endif