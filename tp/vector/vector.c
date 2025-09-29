#include "vector.h"

int meminte(void* dest, void* src, void* tmp, size_t n);

int meminte(void* dest, void* src, void* tmp, size_t n)
{
    memcpy(tmp, dest, n);
    memcpy(dest, src, n);
    memcpy(src, tmp, n);

    return EXITO;
}

int vectorCrear(Vector_t* vector, size_t tamElem)
{
    vector->data = malloc(tamElem * DEFAULT_CAP);

    if(!(vector->data)){
        return ERR_SIN_MEM;
    }

    vector->cantElem = 0;
    vector->tamElem = tamElem;
    vector->capacidad = DEFAULT_CAP;

    return EXITO;
}

int vectorDestruir(Vector_t* vector)
{
    free(vector->data);

    vector->cantElem = 0;
    vector->capacidad = 0;
    vector->tamElem = 0;

    return EXITO;
}

int vectorRedimensionar(Vector_t* vector, size_t nuevaCap)
{
    void* nuevaData = realloc(vector->data, nuevaCap * vector->tamElem);

    if(!(nuevaData)){
        return ERR_SIN_MEM;
    }

    vector->data = nuevaData;
    vector->capacidad = nuevaCap;

    return EXITO;
}

int vectorLeerDeTexto(Vector_t* vector, const char* nomArch, int (ParsearTexto)(FILE*, void*))
{
    FILE* arch = fopen(nomArch, "rt");

    if(!arch){
        return ERR_INPUT;
    }

    void* tmp = malloc(vector->tamElem);

    if(!tmp){
        fclose(arch);
        return ERR_SIN_MEM;
    }

    ParsearTexto(arch, tmp);
    vectorEmpujar(vector, tmp);

    while(!feof(arch)){
        ParsearTexto(arch, tmp);
        vectorEmpujar(vector, tmp);
    }

    fclose(arch);
    free(tmp);

    return EXITO;
}

int vectorEscribirATexto(Vector_t* vector, const char* nomArch, int (ParsearTexto)(FILE*, void*))
{
    FILE* arch = fopen(nomArch, "wt");

    if(!arch){
        return ERR_INPUT;
    }

    void* i = vector->data;
    void* ult = vector->data + (vector->cantElem - 1) * vector->tamElem;

    for(; i <= ult; i += vector->tamElem){
        ParsearTexto(arch, i);
    }

    fclose(arch);

    return EXITO;
}

int vectorLeerDeBinario(Vector_t* vector, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "rb");

    if(!arch){
        return ERR_INPUT;
    }

    fseek(arch, 0, SEEK_END);

    size_t tamArch = ftell(arch) / vector->tamElem;

    fseek(arch, 0, SEEK_SET);

    if(tamArch >= vector->capacidad){
        if(vectorRedimensionar(vector, tamArch * FACTOR_INCR)){
            return ERR_SIN_MEM;
        }
    }

    fread(vector->data, tamArch * vector->tamElem, tamArch, arch);

    fclose(arch);

    return EXITO;
}

int vectorEscribirABinario(Vector_t* vector, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "wb");

    if(!arch){
        return ERR_INPUT;
    }

    fwrite(vector->data, vector->tamElem, vector->cantElem, arch);

    fclose(arch);

    return EXITO;
}

int vectorInsertar(Vector_t* vector, size_t pos, void* elem)
{
    if(pos < 0 || pos > vector->cantElem){
        return ERR_INPUT;
    }

    if(vector->cantElem == vector->capacidad){
        if(vectorRedimensionar(vector, FACTOR_INCR * vector->capacidad)){
            return ERR_SIN_MEM;
        }
    }

    void* i = vector->data + pos * vector->tamElem;

    memcpy(i + vector->tamElem, i, (vector->cantElem - pos) * vector->tamElem);
    memcpy(i, elem, vector->tamElem);

    vector->cantElem++;

    return EXITO;
}

int vectorEmpujar(Vector_t* vector, void* elem)
{
    if(vector->cantElem == vector->capacidad){
        if(vectorRedimensionar(vector, FACTOR_INCR * vector->capacidad)){
            return ERR_SIN_MEM;
        }
    }

    void* i = vector->data + (vector->cantElem) * vector->tamElem;

    memcpy(i, elem, vector->tamElem);

    vector->cantElem++;

    return EXITO;
}

int vectorInsertarOrd(Vector_t* vector, void* elem, int (*Cmp)(void*, void*))
{
    if(vector->cantElem == vector->capacidad){
        if(vectorRedimensionar(vector, FACTOR_INCR * vector->capacidad)){
            return ERR_SIN_MEM;
        }
    }

    void* i = vector->data;
    void* j = vector->data + (vector->cantElem - 1) * vector->tamElem; 

    while(Cmp(elem, i) > 0 && i <= j){
        i +=  vector->tamElem;
    }

    if(Cmp(elem, i) == 0 && i > j){
        return ERR_INPUT;
    }

    memmove(i + vector->tamElem, i, j - i + vector->tamElem);

    memmove(i, elem, vector->tamElem);
    vector->cantElem++;

    return EXITO;
}

int vectorEliminarElem(Vector_t* vector, void* elem, int (*Cmp)(void*, void*))
{
    if(Cmp == NULL){
        return ERR_INPUT;
    }

    size_t pos = vectorBuscar(vector, elem, Cmp);

    if(!pos){
        return ERR_INPUT;
    }

    vectorEliminarPos(vector, pos);

    return EXITO;
}

int vectorOrdEliminarElem(Vector_t* vector, void* elem, int (*Cmp)(void*, void*))
{
    if(Cmp == NULL){
        return ERR_INPUT;
    }

    size_t pos = vectorBuscar(vector, elem, Cmp);

    if(pos == -1){
        return ERR_INPUT;
    }

    vectorEliminarPos(vector, --pos);

    return EXITO;
}

int vectorEliminarPos(Vector_t* vector, size_t pos)
{
    if(pos < 0 || pos > vector->cantElem){
        return ERR_INPUT;
    }

    void* i = vector->data + pos * vector->tamElem;

    memcpy(i, i + vector->tamElem, (vector->cantElem - pos) * vector->tamElem);

    vector->cantElem--;

    return EXITO;
}

int vectorOrdenar(Vector_t* vector, int (*Cmp)(void*, void*))
{
    void* i = vector->data + vector->tamElem;
    void* j = vector->data;
    void* ult = vector->data + (vector->cantElem - 1) * vector->tamElem;
    void* tmp = malloc(vector->tamElem);

    if(!tmp){
        return ERR_SIN_MEM;
    }
    
    for(; i <= ult; i += vector->tamElem){
        j = i - vector->tamElem;
        memcpy(tmp, i, vector->tamElem);
    
        while(Cmp(j, tmp) > 0 && j >= vector->data){
            memmove(j + vector->tamElem, j, vector->tamElem);
            j -= vector->tamElem;
        }

        memmove(j + vector->tamElem, tmp, vector->tamElem);
    }

    return EXITO;
}

size_t vectorBuscar(Vector_t* vector, void* elem, int (*Cmp)(void*, void*))
{
    void* ori = vector->data;
    void* li = vector->data;
    void* ls = vector->data + (vector->cantElem - 1) * vector->tamElem;
    void* med = li + ((ls - li) / (2 * vector->tamElem)) * vector->tamElem;

    int cmp = 0;

    while(li <= ls && (cmp = Cmp(med, elem)) != 0){
        if(cmp > 0){
            ls = med - vector->tamElem;
        }else{
            li = med + vector->tamElem;
        }

        med = li + ((ls - li) / (2 * vector->tamElem)) * vector->tamElem;
    }

    if(li > ls){
        return -1;
    }

    printf("%zu\n", ((med - ori) / vector->tamElem));

    return ((med - ori) / vector->tamElem);    
}

size_t vectorCantElem(Vector_t* vector)
{
    return vector->cantElem;
}

size_t vectorTamElem(Vector_t* vector)
{
    return vector->tamElem;
}

size_t vectorCapacidad(Vector_t* vector)
{
    return vector->capacidad;
}

bool vectorVacio(Vector_t* vector)
{
    return vector->cantElem == 0 ? true : false;
}

void mostrarVector(Vector_t* vector, void (*Mostrar)(void*))
{
    void* i = vector->data;
    void* ult = vector->data + (vector->cantElem - 1) * vector->tamElem;

    for(; i <= ult; i += vector->tamElem){
        Mostrar(i);
    }
}