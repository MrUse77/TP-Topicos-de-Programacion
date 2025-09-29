#include "secuenciaPalabra.h"

int secuenciaPalabrasCrear(SecuenciaPalabras_t* sec, char* cadena)
{
    sec->cadena = cadena;
    sec->finDeCadena = false;

    return EXITO;
}

bool secuenciaPalabrasLeer(SecuenciaPalabras_t* sec, Palabra_t* pal)
{
    while(*(sec->cadena) != '\0' && !esLetra(*(sec->cadena))){
        (sec->cadena)++;
    }

    if(*(sec->cadena) == '\0'){
        sec->finDeCadena = true;
        return false;
    }

    char* tmp = pal->cadena;

    do{
        *tmp = *(sec->cadena);
        tmp++;
        (sec->cadena)++;
    }while(*(sec->cadena) != '\0' && esLetra(*(sec->cadena)));

    *tmp = '\0';

    return true;
}

int secuenciaPalabrasEscribir(SecuenciaPalabras_t* sec, Palabra_t* pal)
{
    char* tmp = pal->cadena;

    while(*tmp){
        *(sec->cadena) = *tmp;
        (sec->cadena)++;
        tmp++;
    }

    return EXITO;
}

int secuenciaPalabrasEscribirCaract(SecuenciaPalabras_t* sec, int ch)
{
    *(sec->cadena) = ch;
    (sec->cadena)++;
    return EXITO;
}

int secuenciaPalabrasCerrar(SecuenciaPalabras_t* sec)
{
    *(sec->cadena) = '\0';

    return true;
}

int palabraModificar(Palabra_t* pal, void (*Modificar)(char*))
{
    if(Modificar == NULL){
        return false;
    }

    Modificar(pal->cadena);

    return true;
}

void palabraMostrar(Palabra_t* pal)
{
    puts(pal->cadena);
}
