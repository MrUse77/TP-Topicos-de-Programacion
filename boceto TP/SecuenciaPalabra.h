#ifndef SECUENCIAPALABRA_H_INCLUDED
#define SECUENCIAPALABRA_H_INCLUDED


#include <stdbool.h>
#include "TDAPalabra.h"


typedef struct
{
    char* cur;
    bool finSec;
}
SecuenciaPalabras;


void secuenciaPalabrasCrear(SecuenciaPalabras* sec, const char* cad);
bool secuenciaPalabrasLeer(SecuenciaPalabras* sec, Palabra* pal);
bool secuenciaPalabrasFin(const SecuenciaPalabras* sec);
void secuenciaPalabrasEscribir(SecuenciaPalabras* sec, const Palabra* pal);
void secuenciaPalabrasEscribirCaracter(SecuenciaPalabras* sec, char c);
void secuenciaPalabrasCerrar(SecuenciaPalabras* sec);


#endif // SECUENCIAPALABRAS_H



