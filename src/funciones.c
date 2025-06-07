#include "funciones.h"

/*Para la primera parte solo faltaria, agregar el campo extra, implementar el TDA fecha de mierda
y corregir la "construccion" de RegistroIndice*/

/*Funcion auxiliar, para saber si un int es par*/
int esPar(int n)
{
    return (n % 2 == 0 ? 1 : 0);
}

/*Funcion auxiliar, para saber si un char es letra, igual que isalpha form ctype.h*/
int esLetra(char* c)
{
    char aux = aMayus(c);
    return (aux >= 'A' && aux <= 'Z');
}

/*Funcion auxiliar, NO PASA EL CARACTER A MAYUSCULAS, solo retorna el valor del caracter menos 32*/
int aMayus(char* c)
{
    char aux = *c;
    aux -= 32;
    return aux;
}

/*Funcion constructora, en teoria deberia funcionar, pero aun no puedo hacer que no cause errores de memoria*/
RegistroIndice* inicializarRegistroIndice(RegistroIndice* registros)
{
    RegistroIndice* p = registros;
    
    p->periodo = (char*)malloc(PERIODO_TAM);
    
    if(p->periodo == NULL) return NULL;

    p->nivel = (char*)malloc(NIVEL_TAM);

    if(p->nivel == NULL) return NULL;

    p->indiceICC = (char*)malloc(INDICE_TAM);

    if(p->indiceICC == NULL) return NULL;

    return registros;
}

/*Funcion destructora, libera la memoria asignada dinamicamente a RegistroIndice*/
void destruirRegistroIndice(RegistroIndice* registros)
{
    RegistroIndice* p = registros;
    
    free(p->periodo);
    free(p->nivel);
    free(p->indiceICC);
}

/*Funcion general, procesa el archivo de indices generales*/
int corregirFormatoDeIndicesGeneral(FILE* archivo)
{
    RegistroIndice reg;
    
    /*inicializarRegistroIndice(reg);*/

    reg.periodo = (char*)malloc(PERIODO_TAM);

    if(reg.periodo == NULL) return -1;

    reg.nivel = (char*)malloc(NIVEL_TAM);

    if(reg.nivel == NULL) return -1;

    reg.indiceICC = (char*)malloc(INDICE_TAM);

    if(reg.indiceICC == NULL) return -1;

    FILE* temp = fopen(NOMBRE_ARCH_AUXILIAR, "w+t");
    
    if(temp == NULL) return -1;

    char buffer[BUFFER_TAM];
    
    fgets(buffer, BUFFER_TAM, archivo);
    fprintf(temp, "%s", buffer); 

    fgets(buffer, BUFFER_TAM, archivo);

    while(!feof(archivo)){
        sscanf(buffer, FORMATO_GENERAL, reg.periodo, reg.nivel, reg.indiceICC);
        
        corregirFormatoDeFecha(&reg);
        agregarCeroALaIzquierda(&reg);
        desencriptarNivelGeneral(&reg);
        normalizarNivelSinGuiones(&reg);
        corregirFormatoDecimal(&reg);

        printf("Fecha: %s Nivel: %s Indice: %s\n", reg.periodo, reg.nivel, reg.indiceICC);

        fprintf(temp, "%s;%s;%s", reg.periodo, reg.nivel, reg.indiceICC);
        fgets(buffer, BUFFER_TAM, archivo);
    }

    fclose(archivo);
    fclose(temp);

    remove(NOMBRE_ARCH_INDICES_GENERAL);
    rename(NOMBRE_ARCH_AUXILIAR, NOMBRE_ARCH_INDICES_GENERAL);
    
    /*destruirRegistroIndice(reg);*/

    return 0;
}

/*Funcion general, corrige el formato de la fecha de dd/mm/aaaa a dd-mm-aaaa */
char* corregirFormatoDeFecha(RegistroIndice* reg)
{
    char* p = reg->periodo;

    while(*p){
        if(*p == '/'){
            *p = '-';
        }

        p++;
    }

    return reg->periodo;
}

/*Funcion general, agrega los "leading zeroes" a los valores de la fecha*/
/*Como contra, la fecha pasa a ser un valor int, no se si estaria bien*/
char* agregarCeroALaIzquierda(RegistroIndice* reg)
{
    char* p = reg->periodo;
    int d, m, a;
    
    sscanf(p, "\"%d-%d-%d\"\n", &d, &m, &a);
    
    sprintf(p, "%04d-%02d-%02d", a, m, d);

    return reg->periodo;
}

/*Funcion general, desarrolla el proceso de decriptacion del campo nivel general*/
char* desencriptarNivelGeneral(RegistroIndice* reg)
{
    size_t contador = 0;
    char* p = reg->nivel;
    
    printf("A - %s\n", p);
    
    while(*p){
        if(esLetra(p)){
            if(esPar(contador)){
                *p += 2;
            }
            else{
                *p += 4;
            }

            if(*p > 'z'){
                *p = 'a' + (*p - 'z' - 1);
            }
        }
        
        contador++;
        p++;
    }

    return reg->nivel;
}

/*Funcion general, normaliza el campo nivel general desencriptado y remueve los guiones*/
char* normalizarNivelSinGuiones(RegistroIndice* reg)
{
    char* p = reg->nivel;
    
    p++;

    *p = aMayus(p);
    
    while(*p){
        if(*p == '_'){
            *p = ' ';
        }

        p++;
    }
    
    return reg->nivel;
}

/*Funcion general, corrige el formato del campo indice de f*,f* a f*.f* */
char* corregirFormatoDecimal(RegistroIndice* reg)
{
    char* p = reg->indiceICC;

    while(*p){
        if(*p == ','){
            *p = '.';
        }

        p++;
    }

    return reg->indiceICC;
}