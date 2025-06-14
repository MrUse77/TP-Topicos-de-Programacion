#include "vector.h"
#include <stdlib.h>
#include <string.h>

/*hecho*/
Vector* crearVector(size_t tamElem)
{
    Vector* vec = malloc(sizeof(Vector));

    if(!vec) return NULL;

    vec->data = malloc(tamElem * DEFAULT_CAP);

    if(!(vec->data)) return NULL;

    vec->cursor = 0;
    vec->cantElem = 0;
    vec->tamElem = tamElem;
    vec->cap = DEFAULT_CAP;

    return vec;
}

/*A terminar, para despues*/
int inicializarVectorDeArchivoTexto(Vector* vec, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "rt");

    if(!arch) return ERROR;

    char* buffer = malloc(vec->tamElem);

    fgets(buffer, vec->tamElem, arch);
    
    while(!feof(arch)){
        insertarArribaEnVector(vec, buffer);
        fgets(buffer, vec->tamElem, arch);
    }

    fclose(arch);
    free(buffer);

    return EXITO;
}

/*A terminar, para despues*/
int grabarVectorEnArchivoBinario(Vector* vec, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "wb");

    if(!arch) return ERR_FILE;

    fwrite(vec->data, vec->tamElem, vec->cantElem, arch);

    fclose(arch);

    return EXITO;
}


int insertarArribaEnVector(Vector* vec, void* elem)
{
    vec->cantElem++;

    if(_threshold(vec)) return ERROR;

    char* tmpData = vec->data;
    char* tmpElem = elem;

    tmpData += vec->cantElem * vec->tamElem;

    memcpy(tmpData, tmpElem, vec->tamElem);

    return EXITO;
}


int ordenarVector(Vector* vec, Cmp cmpFunc)
{
    size_t tamElem = vec->tamElem;

    char* tmpI = NULL;
    char* tmpJ = NULL;
    char* tmpUlt = vec->data + vec->cantElem * vec->tamElem;

    for(tmpI = vec->data; tmpI < tmpUlt - tamElem; tmpI += tamElem){
        for(tmpJ = vec->data; tmpJ < tmpUlt - tamElem - tmpI; tmpJ += tamElem){
            if(cmpFunc(tmpJ, tmpJ + tamElem) < 0){
                intercambiar(tmpJ, tmpJ + tamElem, tamElem);
            }
        }
    }

    return EXITO;
}


int intercambiar(void* a, void* b, size_t tamElem)
{
    char* tmp = malloc(tamElem);

    memcpy(tmp, a, tamElem);
    memcpy(a, b, tamElem);
    memcpy(b, tmp, tamElem);

    free(tmp);

    return EXITO;
}

/*hecho*/
void mostrarVector(Vector* vec, Imprimir imprFunc)
{
    size_t i;
    char* tmp = vec->data;

    for(i = 0; i < vec->cantElem; i++){
        imprFunc((void*) tmp + i * vec->tamElem);
    }
}

/*hecho*/
int destruirVector(Vector* vec)
{
    free(vec->data);

    free(vec);

    return EXITO;
}

/*mas o menos*/
int _boundsChecking(size_t bound, int pos)
{
    return (pos < 0 || pos >= bound) ? EXITO : ERROR;
}

/*No lo probe*/
int _threshold(Vector* vec)
{
    if(vec->cantElem/ vec->cap >= MAX_THRESHOLD){
        _redimensionarVector(vec, INCREMENTO);
    }else if(vec->cantElem / vec->cap <= MIN_THRESHOLD){
        _redimensionarVector(vec, DECREMENTO);
    }

    return EXITO;
}

/*No lo probe*/
int _redimensionarVector(Vector* vec, int modif)
{
    void* tmpData = NULL;
    size_t tmpCap = 0;

    switch(modif){

    case 1:
        tmpData = realloc(vec->data, vec->cap * FACTOR_INCR);
        tmpCap = vec->cap * FACTOR_INCR;
    case 2:
        tmpData = realloc(vec->data, vec->cap * FACTOR_DECR);
        tmpCap = vec->cap * FACTOR_DECR;
    default:
        return ERROR;
    }

    if(!tmpData) return ERROR;

    vec->data = tmpData;
    vec->cap = tmpCap;

    return EXITO;
}
