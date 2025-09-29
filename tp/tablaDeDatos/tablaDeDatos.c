#include "tablaDeDatos.h"

#define DEFAULT_CANT_FILAS 20

#define DEFAULT_SIZE_STRING 76

size_t contarBytesEnRegistro(tipoDeDato* tiposDeDatos, int n);

size_t contarBytesEnRegistro(tipoDeDato* tiposDeDatos, int n)
{
    int i = 0;
    size_t tmp = 0;
    tipoDeDato* tmpPtr = tiposDeDatos;

    for(i < n; i < n; i++, tmpPtr++){
        switch(*tmpPtr){
            case Int:
                tmp += sizeof(int);
                break;
            case Double:
                tmp += sizeof(double);
                break;
            case Float:
                tmp += sizeof(float);
                break;
            case String:
                tmp += DEFAULT_SIZE_STRING;
                break;
            default:
                return -1;
        }
    }
  
    return tmp;
}




TablaDeDatos_t* tablaDeDatosCrear(int cantColumnas, char** nombresColumnas, tipoDeDato* tiposDeDatos)
{
    TablaDeDatos_t dest;
    int i;

    dest.cantColumnas = cantColumnas;
    dest.cantFilas = 0;
    dest.capacidadFilas = DEFAULT_CANT_FILAS;
    dest.nombresCampos = nombresColumnas;
    dest.tiposDeDatos = tiposDeDatos;

    void** tmp = dest.datos;

    tmp = malloc(DEFAULT_CANT_FILAS * sizeof(void*));

    if(!tmp){
        return NULL;
    }

    for(i = 0; i < cantColumnas; i++){
        
    }


    return &dest;
}