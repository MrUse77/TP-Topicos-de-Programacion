#include "SecuenciaPalabra.h"


void secuenciaPalabrasCrear(SecuenciaPalabras* sec, const char* cad)
{
    sec->cur = (char*)cad;
    sec->finSec = false;
}


bool secuenciaPalabrasLeer(SecuenciaPalabras* sec, Palabra* pal)
{
    while(*sec->cur && !esLetra(*sec->cur)) // *sec->cur != '\0'
    {
        sec->cur++;
    }

    if(!*sec->cur) //*sec->cur == '\0'
    {
        sec->finSec = true;
        return false;
    }

    char* iPal = pal->vPal;
    do
    {
        *iPal = *sec->cur;
        iPal++;
        sec->cur++;
    }
    while(*sec->cur && esLetra(*sec->cur));

    *iPal = '\0';

    return true;
}


bool secuenciaPalabrasFin(const SecuenciaPalabras* sec)
{
    return sec->finSec;
}


void secuenciaPalabrasEscribir(SecuenciaPalabras* sec, const Palabra* pal)
{
    const char* iPal = pal->vPal;
    while(*iPal)
    {
        *sec->cur = *iPal;
        iPal++;
        sec->cur++;
    }
}


void secuenciaPalabrasEscribirCaracter(SecuenciaPalabras* sec, char c)
{
    *sec->cur = c;
    sec->cur++;
}


void secuenciaPalabrasCerrar(SecuenciaPalabras* sec)
{
    *sec->cur = '\0';
}
