#ifndef SECUENCIA_PALABRAS_INCLUDED
#define SECUENCIA_PALABRAS_INCLUDED

#include <stdio.h>
#include <stdbool.h>

#define EXITO 0

#define DEFAULT_PAL_TAM 50

typedef struct{
    char* cadena;
    bool finDeCadena;
}SecuenciaPalabras_t;

typedef struct{
    char cadena[DEFAULT_PAL_TAM];
}Palabra_t;

int secuenciaPalabrasCrear(SecuenciaPalabras_t* sec, char* cadena);
int secuenciaPalabrasLeer(SecuenciaPalabras_t* sec, Palabra_t* pal);
int secuenciaPalabrasEscribir(SecuenciaPalabras_t* sec, Palabra_t* pal);
int secuenciaPalabrasEscribirCaract(SecuenciaPalabras_t* sec, int ch);
int secuenciaPalabrasCerrar(SecuenciaPalabras_t* sec);
bool secuenciaPalabrasEsFin(SecuenciaPalabras_t* sec);

int palabraModificar(Palabra_t* pal, void (*Modificar)(char*));
int palabraMostrar(Palabra_t* pal);

#endif