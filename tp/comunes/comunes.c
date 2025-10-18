#include "comunes.h"

int logError(int cod, char* file, int line, char* func)
{
    char mensaje[50];
    switch(cod){
        case EXITO:
            return 0;
        case ERR_PUNTERO_NULO:
            stringCopy(mensaje, "Error con operacion con punteros dinamicos");
            break;
        case ERR_SIN_MEM:
            stringCopy(mensaje, "Error al alocar memoria dinamica");
            break;
        case ERR_ARCH:
            stringCopy(mensaje, "Error al abrir archivo");
            break;
        case ERR_BUFFER_CORTO:
            stringCopy(mensaje, "Error en la lectura de archivo. Buffer de lectura demasiado corto");
            break;
        case ERR_REGISTRO:
            stringCopy(mensaje, "Error en la lectura de archivo. Registro corrompido");
            break;
        case ERR_USUARIO:
            stringCopy(mensaje, "Error en el ingreso de informacion");
        default:
            stringCopy(mensaje, "Error en la funcion de error. Muy mal.");
            cod = 90;
    }

    printf("Error codigo: %d; En archivo: %s:%d; en funcion %s\n%s\n", cod, file, line, func, mensaje);
    
    if(cod == 90){
        abort();
    }

    return 0;
}

void* malloc_s(size_t n)
{
    void* tmp = malloc(n);

    if(!tmp){
        return NULL;
    }

    return tmp;
}

void* calloc_s(size_t n, size_t tam)
{  
    void* tmp = calloc(n, tam);

    if(!tmp){
        return NULL;
    }

    return tmp;
}

void* realloc_s(void* ptr, size_t n)
{
    if(!ptr){
        return NULL;
    }

    void* tmp = realloc(ptr, n);

    if(!tmp){
        return NULL;
    }

    return tmp;
}

int free_s(void* ptr)
{
    if(!ptr){
        return ERR_PUNTERO_NULO;
    }

    free(ptr);
    ptr = NULL;

    return EXITO;
}