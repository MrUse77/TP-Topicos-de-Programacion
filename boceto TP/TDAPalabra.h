#ifndef TDAPALABRA_H
#define TDAPALABRA_H

#include <stdbool.h>


#define TAM_PAL 51


typedef struct
{
    char vPal[TAM_PAL];
}
Palabra;


void palabraATitulo(Palabra* pal);
void palabraMostrar(const Palabra* pal);
void palabraAMinuscula(Palabra *pal);

bool esLetra(char c);


#endif // TDAPALABRA_H
