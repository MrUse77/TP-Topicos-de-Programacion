#include "TDAPalabra.h"
#include "SecuenciaPalabra.h"
#include "normalizar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* normalizar(char* cadDest, const char* cadOrig)
{
    SecuenciaPalabras secL;
    secuenciaPalabrasCrear(&secL, cadOrig);

    SecuenciaPalabras secE;
    secuenciaPalabrasCrear(&secE, cadDest);

    Palabra pal;
    if(secuenciaPalabrasLeer(&secL, &pal))
    {
        palabraATitulo(&pal);
        secuenciaPalabrasEscribir(&secE,&pal);
    }

    while(secuenciaPalabrasLeer(&secL, &pal))
    {
        secuenciaPalabrasEscribirCaracter(&secE, ' ');

        palabraAMinuscula(&pal);

        secuenciaPalabrasEscribir(&secE, &pal);

    }

    secuenciaPalabrasCerrar(&secE);

    return cadDest;
}

char* normalizarI(char* cadDest, const char* cadOrig)
{
    const char *aux = strchr(cadOrig,'_');
    aux++;
    SecuenciaPalabras secL;
    secuenciaPalabrasCrear(&secL, aux);

    SecuenciaPalabras secE;
    secuenciaPalabrasCrear(&secE, cadDest);

    Palabra pal;
    if(secuenciaPalabrasLeer(&secL, &pal))
    {
        palabraATitulo(&pal);
        secuenciaPalabrasEscribir(&secE,&pal);
    }

    while(secuenciaPalabrasLeer(&secL, &pal))
    {
        secuenciaPalabrasEscribirCaracter(&secE, ' ');

        palabraAMinuscula(&pal);

        secuenciaPalabrasEscribir(&secE, &pal);

    }

    secuenciaPalabrasCerrar(&secE);

    return cadDest;
}
