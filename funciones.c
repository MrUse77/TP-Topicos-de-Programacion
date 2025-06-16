#include "TDAvector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funciones.h"
#include "TDAPalabra.h"
#include "SecuenciaPalabra.h"
#include "normalizar.h"

#define ERROR_ARCH 1
#define ERROR_SSCANF 3
#define FACTOR_INCR 1.5
#define ERROR_REALLOC 2
#define OK 0
#define PAR 2
#define IMPAR 4

int archivoAVector(const char* nombreArchivo, Vector *v, leerLinea leer, int tamLinea)
{
    FILE *pf = fopen(nombreArchivo, "rt");
    if(pf == NULL)
    {
        printf("Error al abrir el archivo %s.\n", nombreArchivo);
        return ERROR_ARCH;
    }

    void* posActual = v->vec;
    char linea[tamLinea];
    //setlocale(LC_ALL, "Spanish"); // Para poder leer floats con coma
    fgets(linea, tamLinea, pf);
    while(fgets(linea, tamLinea, pf)) // Mientras haya un renglón que leer...
    {
        leer(linea, posActual); // Cargo campo por campo cada línea en el vector de estructura
        v->ce ++;

        if(v->ce == v->capacidad)  // Nos fijamos si la cantidad de elementos del vector es igual a la capacidad, para aumentarla
        {
            if(!vectorRedimensionar(v, v->capacidad * FACTOR_INCR))
            {
                return ERROR_REALLOC;  // Si no es posible redimensionar el vector por falta de memoria devuelve error
            }
            posActual = v->vec + (v->ce-1) * v->tamElem;
        }

        posActual += v->tamElem;
    }

    // Una vez cargamos todo en el vector, redimensionamos para que el archivo no ocupe memoria de más solo si hay mucha diferencia,
    // para evitar hacer un realloc no muy necesario si no hay tanta diferencia entre la cantidad y la capacidad.
    // En este caso lo convertimos al mismo tamaño de elementos que tiene el vector ya que no vamos a cargar más que esos
    if(v->ce < v->capacidad/1.5)
    {
        vectorRedimensionar(v, v->ce);
    }

    // IMPORTANTE no olvidar liberar la memoria dinámica al final del programa

    fclose(pf);
    return OK;
}

void normalizar_general(Vector *v)
{
    void *ini = v->vec;
    void *ult = v->vec + v->tamElem*(v->ce-1);

    for(void *i = ini; i <= ult; i += v->tamElem)
    {
        GENERAL *pal = (GENERAL*)i;
        normalizar(pal->general_aperturas,pal->general_aperturas);
    }
}

void normalizarItems(Vector *v)
{
    void *ini = v->vec;
    void *ult = v->vec + v->tamElem*(v->ce-1);
    char *p;
    for(void *i = ini; i <= ult; i += v->tamElem)
    {
        ITEMS *pal = (ITEMS*)i;
        p =strchr(pal->nivel_general,'_');
        p++;
        normalizar(pal->nivel_general,p);
    }
}
void leerLineaItems(const char* linea, void* pos) // No está en el .h para que el usuario no pueda llamar a la función
{

    char* punteroLinea = strrchr(linea,';'); // Posicionamos el puntero justo antes del último campo del texto
    ITEMS* actual = (ITEMS*) pos;

    // Indice
    *punteroLinea = '\0'; // Nos aseguramos de que no haya ninguna basura después
    punteroLinea++;
    reemplazar_coma(punteroLinea);
    actual->indice = atof(punteroLinea); // tras testearlo, atoi/atof es un poco más rápido que el sscanf

    // Nivel_general
    punteroLinea = strrchr(linea,';');
    *punteroLinea = '\0';
    punteroLinea++;
    //actual->nivel_general = punteroLinea;
    sscanf(punteroLinea, "\"%[^\"]\"", actual->nivel_general);

    // Periodo
    //actual->periodo = linea;

    sscanf(linea, "\"%[^\"]\"", actual->periodo);


}

void leerLineaGeneral(const char* linea, void* pos) // No está en el .h para que el usuario no pueda llamar a la función
{

    char* punteroLinea = strrchr(linea,';'); // Posicionamos el puntero justo antes del último campo del texto
    GENERAL* actual = (GENERAL*) pos;

    // Indice
    *punteroLinea = '\0'; // Nos aseguramos de que no haya ninguna basura después
    punteroLinea++;
    reemplazar_coma(punteroLinea);
    actual->indice = atof(punteroLinea); // tras testearlo, atoi/atof es un poco más rápido que el sscanf

    // Nivel_general
    punteroLinea = strrchr(linea,';');
    *punteroLinea = '\0';
    punteroLinea++;
    //actual->nivel_general = punteroLinea;
    sscanf(punteroLinea, "\"%[^\"]\"", actual->general_aperturas);

    // Periodo
    //actual->periodo = linea;
    punteroLinea = strchr(linea,'\"');
    sscanf(punteroLinea, "\"%[^\"]\"", actual->periodo);
}

void reemplazar_coma(char *s)
{
    while (*s)
    {
        if (*s == ',')
        {
            *s = '.';
        }
        s++;
    }
}

void DesencriptarItems(Vector *v)
{
    void *ini = v->vec;
    void *ult = v->vec + (v->ce - 1)*v->tamElem;

    for(void *i = ini; i <=ult; i+= v->tamElem)
    {
        ITEMS *encript = (ITEMS*)i;
        cambia_Items(encript->nivel_general);
    }


}

void DesencriptarGeneral(Vector *v)
{
    void *ini = v->vec;
    void *ult = v->vec + (v->ce - 1)*v->tamElem;

    for(void *i = ini; i <=ult; i+= v->tamElem)
    {
        GENERAL *encript = (GENERAL*)i;
        cambia_general(encript->general_aperturas);

    }


}
void cambia_general(char *val)
{
    int contador = 1;
    while(*val)
    {
        if(*val >= 'a' && *val <= 'z')
        {

            if(contador%2 == 0)
            {
                *val += PAR;
            }
            else
            {
                *val +=IMPAR;
            }
            if(*val > 'z')
            {
                *val = *val - ('z' - 'a') - 1;
            }
        }
        val++;
        contador++;
    }

}
void cambia_Items(char *val)
{
    while(*val)
    {
        switch(*val)
        {
        case '@':
            *val = 'a';
            break;
        case '8':
            *val = 'b';
            break;
        case '3':
            *val = 'e';
            break;
        case '1':
            *val = 'i';
            break;
        case '0':
            *val = 'o';
            break;
        case '$':
            *val = 's';
            break;
        case '7':
            *val = 't';
            break;
        case '|':
            *val = 'l';
            break;
        case '5':
            *val = 'm';
            break;
        case '9':
            *val = 'n';
            break;

        }
        val++;
    }
}
void formatoPeriodo(Vector *v, eligeEstructura elegir)
{
    void *ini = v->vec;
    void *ult = v->vec + (v->ce -1)* v->tamElem;

    for(void *i = ini; i <= ult; i += v->tamElem)
    {
        elegir(i);

    }
}
void eligeGeneral(void *elem)
{
    GENERAL *per = (GENERAL*)elem;
    cambiarFecha(per->periodo);
}

void eligeItems(void *elem)
{
    ITEMS *per = (ITEMS*)elem;
    cambiarFecha(per->periodo);
}

int cambiarFecha(char *per)
{
    int anio;
    int mes;
    int dia;
    char nueva_fecha[12];
    if(sscanf(per,"%d/%d/%d",&dia,&mes,&anio)!=3)
    {
        return ERROR_SSCANF;
    }

    snprintf(nueva_fecha, sizeof(nueva_fecha), "%04d-%02d-%02d", anio, mes, dia);
    strcpy(per,nueva_fecha);

    return OK;

}

void clasificadorGeneral(Vector *v)
{
    void *ini = v->vec;
    void *ult = v->vec + (v->ce - 1)*v->tamElem;
    int valor;
    for(void *i = ini; i <= ult; i += v->tamElem)
    {
        GENERAL *clas = (GENERAL*)i;
        valor = clasificar(clas->general_aperturas);
        llenarCampo(clas->clasificador,valor);
    }
}
int clasificar(const char *clas)
{
    return strcmp(clas,"Nivel general");
}

void llenarCampo(char *clas, int valor)
{
    if(valor)
    {
        strcpy(clas, "Capítulos");
    }else
    {
        strcpy(clas, "Nivel general");
    }
}
void clasificadorItems(Vector *v)
{
    void *ini = v->vec;
    void *ult = v->vec + (v->ce - 1)*v->tamElem;

    for(void *i = ini; i <= ult; i += v->tamElem)
    {
        ITEMS *c = (ITEMS*)i;
        strcpy(c->clasificador,"Ítems");
    }
}

void unificar(Vector *v1, Vector *v2)
{
    void *ini2 = v2->vec;
    void *ult2 = v2->vec + (v2->ce - 1)*v2->tamElem;

    for(void *i = ini2; i <= ult2; i += v2->tamElem)
    {
        vectorInsertarFinal(v1, i);
    }

}

void unificar2(Vector *unif, Vector *v1, Vector *v2)
{
    void *ini1 = v1->vec;
    void *ult1 = v1->vec + (v1->ce - 1)*v1->tamElem;

    for(void *i = ini1; i <= ult1; i += v1->tamElem)
    {
        vectorInsertarFinal(unif, i);
    }

    void *ini2 = v2->vec;
    void *ult2 = v2->vec + (v2->ce - 1)*v2->tamElem;

    for(void *i = ini2; i <= ult2; i += v2->tamElem)
    {
        vectorInsertarFinal(unif, i);
    }
}

int unificar3(Vector *unif, Vector *v1, Vector *v2)
{

    void *ult1 = v1->vec + (v1->ce - 1)*v1->tamElem;

    void *posu = unif->vec;
    void *aux1 = v1->vec;

    //GENERAL *aux2 = (GENERAL*)v2->vec;

    while(aux1 <= ult1)
    {


        if(unif->ce == unif->capacidad)
        {
            if(!vectorRedimensionar(unif,unif->capacidad*FACTOR_INCR))
            {
                return ERROR_REALLOC;
            }
            posu = unif->vec + unif->ce*unif->tamElem;
        }
        UNIF *auxu = (UNIF*)posu;
        ITEMS *actual = (ITEMS*)aux1;

        strncpy(auxu->clasificador, actual->clasificador,sizeof(auxu->clasificador)-1);
        *(auxu->clasificador + (sizeof(auxu->clasificador)-1)) = '\0';

        strncpy(auxu->general_aperturas, actual->nivel_general,sizeof(auxu->general_aperturas)-1);
        *(auxu->general_aperturas + (sizeof(auxu->general_aperturas)-1)) = '\0';

        strncpy(auxu->periodo, actual->periodo,sizeof(auxu->periodo)-1);
        *(auxu->periodo + (sizeof(auxu->periodo)-1)) = '\0';

        auxu->indice = actual->indice;

        posu += unif->tamElem;
        aux1 += v1->tamElem;
        (unif->ce)++;

    }

    void *ini2 = v2->vec;
    void *ult2 = v2->vec + (v2->ce - 1)*v2->tamElem;

    for(void *i = ini2; i <= ult2; i += v2->tamElem)
    {
        vectorInsertarFinal(unif, i);
    }
    return OK;
}

int unificarResult(Vector *unif, const char* nombreArchivo)
{
    FILE *pf = fopen(nombreArchivo,"wb");
    if(!pf)
    {
        return ERROR_ARCH;
    }
    //size_t cantidad = unif->ce*3;

    RESULT result;
    void *iniu = unif->vec;
    void *ultu = unif->vec + (unif->ce-1)*unif->tamElem;
    //encabezado
    strcpy(result.periodo,"periodo");
    strcpy(result.clasificador,"Clasificador");
    strcpy(result.general_aperturas,"nivel_general_aperturas");
    strcpy(result.tipo_varible,"Tipo_aperturas");
    strcpy(result.valor,"Valor");
    fwrite(&result,sizeof(result),1,pf);
    //indice_icc
    for(void *i = iniu; i <= ultu; i += unif->tamElem)
    {
        UNIF *pos = (UNIF*)i;
        strcpy(result.periodo, pos->periodo);
        strcpy(result.clasificador,pos->clasificador);
        strcpy(result.general_aperturas,pos->general_aperturas);
        strcpy(result.tipo_varible,"indice_icc");
        sprintf(result.valor,"%.7f",pos->indice);
        fwrite(&result,sizeof(result),1,pf);

    }
    //var mensual
    for(void *i = iniu; i <= ultu; i += unif->tamElem)
    {
        UNIF *pos = (UNIF*)i;
        strcpy(result.periodo, pos->periodo);
        strcpy(result.clasificador,pos->clasificador);
        strcpy(result.general_aperturas,pos->general_aperturas);
        strcpy(result.tipo_varible,"var_mensual");
        strcpy(result.valor,pos->var_mensual);
        fwrite(&result,sizeof(result),1,pf);

    }

    for(void *i = iniu; i <= ultu; i += unif->tamElem)
    {
        UNIF *pos = (UNIF*)i;
        strcpy(result.periodo, pos->periodo);
        strcpy(result.clasificador,pos->clasificador);
        strcpy(result.general_aperturas,pos->general_aperturas);
        strcpy(result.tipo_varible,"var_interanual");
        strcpy(result.valor,pos->var_interanual);
        fwrite(&result,sizeof(result),1,pf);

    }
    fclose(pf);
    return OK;

}

void mostrarResultsBin(const char *nombreArchivo) {
    FILE *pf = fopen(nombreArchivo, "rb");
    if (!pf) {
        perror("Error abriendo archivo");
        return;
    }

    RESULT reg;
    size_t leidos;


    // Leer un RESULT cada iteración
    while ((leidos = fread(&reg, sizeof(RESULT), 1, pf)) == 1) {
        printf("%-15s | %-15s | %-40s | %-20s | %-30s\n",
               reg.periodo,
               reg.clasificador,
               reg.general_aperturas,
               reg.tipo_varible,
               reg.valor);
    }

    if (ferror(pf)) {
        perror("Error durante la lectura");
    }

    fclose(pf);
}

void varMensual(Vector *v)
{
    int cantidad = cantidad_indices(v);
    UNIF *pos = (UNIF*)v->vec;

    double var;
    //los del primer mes no tienen variacion
    for(int i = 0; i < cantidad; i++)
    {
        sprintf(pos->var_mensual,"NA");
        pos ++;
    }
    void *ini = v->vec + v->tamElem*cantidad;
    void *ult = v->vec + (v->ce -1)*v->tamElem;
    for(void *i = ini; i <= ult; i += v->tamElem)
    {
        pos = (UNIF*)i;
        UNIF *posAnterior = i - v->tamElem*cantidad;
        var = ((pos->indice/posAnterior->indice)-1)*100;
        sprintf(pos->var_mensual, "%.2f",var);
    }
}

void varAnual(Vector *v)
{
    int cantidad = cantidad_indices(v);
    UNIF *pos = (UNIF*)v->vec;

    double var;
    //los del primer mes no tienen variacion
    for(int i = 0; i < cantidad*12; i++)
    {
        sprintf(pos->var_interanual,"NA");
        pos ++;
    }
    void *ini = v->vec + v->tamElem*cantidad*12;
    void *ult = v->vec + (v->ce -1)*v->tamElem;
    for(void *i = ini; i <= ult; i += v->tamElem)
    {
        pos = (UNIF*)i;
        UNIF *posAnterior = i - v->tamElem*cantidad*12;
        var = ((pos->indice/posAnterior->indice)-1)*100;
        sprintf(pos->var_interanual, "%.2f",var);
    }
}

int cantidad_indices(Vector *v)
{
    void *pos = v->vec;
    int cantidad = 0;
    char primero[41];
    UNIF *gen = (UNIF*)pos;
    strcpy(primero,gen->general_aperturas);

    do
    {
        cantidad++;
        pos += v->tamElem;
        gen = (UNIF*)pos;


    }while(strcmp(primero, gen->general_aperturas) != 0);

    return cantidad;
}


int cmpPeriodo(const void *e1, const void *e2)
{
     const ITEMS *p1 = (const ITEMS*)e1;
     const ITEMS *p2 = (const ITEMS*)e2;

    return strcmp(p1->periodo, p2->periodo);
}
int prioridad(const void *e1)
{
    ITEMS *p1 = (ITEMS*)e1;
    if(strcmp(p1->clasificador,"Nivel general") == 0)
    {
        return 0;
    }else if(strcmp(p1->clasificador,"Capítulos") == 0)
    {
        return 1;
    }else
    {
        return 2;
    }
}
int cmpClasificador(const void *e1, const void *e2)
{
    const ITEMS *p1 = (const ITEMS*)e1;
    const ITEMS *p2 = (const ITEMS*)e2;
    int pr1, pr2;
    pr1 = prioridad(p1);
    pr2 = prioridad(p2);

    return pr1 - pr2;
}


