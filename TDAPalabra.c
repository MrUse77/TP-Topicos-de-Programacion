#include <stdio.h>
#include <ctype.h>
#include "TDAPalabra.h"


void palabraATitulo(Palabra* pal)
{
    char* iPal = pal->vPal;
    *iPal = toupper(*iPal);
    iPal++;

    while(*iPal)
    {
        *iPal = tolower(*iPal);
        iPal++;
    }
}

void palabraAMinuscula(Palabra *pal)
{
    char *iPal = pal->vPal;

    while(*iPal)
    {
        *iPal = tolower(*iPal);
        iPal++;
    }
}


void palabraMostrar(const Palabra* pal)
{
    puts(pal->vPal);
}


bool esLetra(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
