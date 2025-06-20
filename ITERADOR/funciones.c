#include "TDAvector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funciones.h"
#include "TDAPalabra.h"
#include "SecuenciaPalabra.h"
#include "normalizar.h"
#include "vectorIterador.h"

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

    char linea[tamLinea];

    fgets(linea, tamLinea, pf);
    void *buffer = malloc(vectorTamElem(v));
    if(!buffer)
    {
        fclose(pf);
        return ERROR_REALLOC;
    }
    while(fgets(linea, tamLinea, pf)) // Mientras haya un renglón que leer...
    {

        leer(linea, buffer); // Cargo campo por campo cada línea en el vector de estructura
        if(!vectorInsertarFinal(v,buffer))
        {
            fclose(pf);
            free(buffer);
            return ERROR_REALLOC;
        }
    }
    free(buffer);
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
    sscanf(punteroLinea, "\"%[^\"]\"", actual->nivel_general);

    // Periodo
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

void normalizar_general(Vector *v)
{
    VectorIterador it;
    vectorIteradorCrear(&it,v);

    GENERAL *pal = (GENERAL*)vectorIteradorPrimero(&it);
    //pasamos general_aperturas como parametro en los 2 lados ya que es ahi donde se va a guardar el resultado de la fuente
    while(!vectorIteradorFinIter(&it))
    {
        normalizar(pal->general_aperturas,pal->general_aperturas);
        pal = (GENERAL*)vectorIteradorSiguiente(&it);
    }

}

void normalizarItems(Vector *v)
{
    VectorIterador it;
    vectorIteradorCrear(&it,v);

    ITEMS *pal = (ITEMS*)vectorIteradorPrimero(&it);
    char *p;
    while(!vectorIteradorFinIter(&it))
    {
        //Avanzamos hasta el primer '_' para no contar todo lo de atras porque piden que lo eliminemos
        p = strchr(pal->nivel_general,'_');
        p++;
        //pasamos el puntero despues del primer '_' y normalizamos
        normalizar(pal->nivel_general,p);
        pal = (ITEMS*)vectorIteradorSiguiente(&it);
    }

}

void DesencriptarItems(Vector *v)
{
    VectorIterador it;
    vectorIteradorCrear(&it,v);
    ITEMS *encript = (ITEMS*)vectorIteradorPrimero(&it);
    //pasamos cada campo del vector cargado con el archivo a cambia_items para que modifique los caracteres segun lo pedido
    while(!vectorIteradorFinIter(&it)){

        cambia_Items(encript->nivel_general);
        encript = (ITEMS*)vectorIteradorSiguiente(&it);
    }

}

void DesencriptarGeneral(Vector *v)
{
    VectorIterador it;
    vectorIteradorCrear(&it,v);
    GENERAL *encript = (GENERAL*)vectorIteradorPrimero(&it);
    //pasamos cada campo del vector cargado con el archivo a cambia_items para que modifique los caracteres segun lo pedido
    while(!vectorIteradorFinIter(&it)){

        cambia_general(encript->general_aperturas);
        encript = (GENERAL*)vectorIteradorSiguiente(&it);
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
    VectorIterador it;
    vectorIteradorCrear(&it,v);
    void *elem = vectorIteradorPrimero(&it);
    //Pasamos la funcion por parametro que elige que tipo de estructura quiere modificar asi reutilizamos la funcion
    //osea directamente le pasa a cambiaFecha la estructura con la que se va a trabajar, sino habria que hacer 2 casi iguales
    while(!vectorIteradorFinIter(&it))
    {
        elegir(elem);
        elem = vectorIteradorSiguiente(&it);
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
    //leemos el periodo tal cual esta de la cadena periodo pasada por parametro
    if(sscanf(per,"%d/%d/%d",&dia,&mes,&anio)!=3)
    {
        return ERROR_SSCANF;
    }
    //Formateamos en un buffer para poder copiarlo como se pide en el periodo
    snprintf(nueva_fecha, sizeof(nueva_fecha), "%04d-%02d-%02d", anio, mes, dia);
    strcpy(per,nueva_fecha);

    return OK;

}

void clasificadorGeneral(Vector *v)
{
   VectorIterador it;
   vectorIteradorCrear(&it,v);
   int valor;
   GENERAL *clas = (GENERAL*)vectorIteradorPrimero(&it);
   //Nos fijamos si es igual a Nivel general, si no es igual quiere decir que hay que asignarle "capitulos" en clasificador
   while(!vectorIteradorFinIter(&it))
   {
       valor = clasificar(clas->general_aperturas);
       llenarCampo(clas->clasificador,valor);
       clas = (GENERAL*)vectorIteradorSiguiente(&it);
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
   VectorIterador it;
   vectorIteradorCrear(&it,v);
    //Asignamos Items a todos los elementos del campo clasificador
   ITEMS *clas = (ITEMS*)vectorIteradorPrimero(&it);
   while(!vectorIteradorFinIter(&it))
   {
       strcpy(clas->clasificador,"Ítems");
       clas = (ITEMS*)vectorIteradorSiguiente(&it);
   }
}

int unificar(Vector *unif, Vector *v1, Vector *v2)
{
    VectorIterador it;
    vectorIteradorCrear(&it,v1);

    GENERAL *gen = (GENERAL*)vectorIteradorPrimero(&it);
    //los campos de GENERAL no coindicen exactamente con el resutante de ambos, asi que copiamos campo a campo en una estructura
    //temporal igual a la unificada

    while(!vectorIteradorFinIter(&it))
    {
        UNIF temp;

        strncpy(temp.clasificador , gen->clasificador,sizeof(temp.clasificador)-1);
        *(temp.clasificador + (sizeof(temp.clasificador)-1)) = '\0';

        strncpy(temp.general_aperturas, gen->general_aperturas,sizeof(temp.general_aperturas)-1);
        *(temp.general_aperturas + (sizeof(temp.general_aperturas)-1)) = '\0';

        strncpy(temp.periodo, gen->periodo,sizeof(temp.periodo)-1);
        *(temp.periodo + (sizeof(temp.periodo)-1)) = '\0';

        temp.indice = gen->indice;

        vectorInsertarFinal(unif,&temp);
        gen = (GENERAL*)vectorIteradorSiguiente(&it);

    }
    //el campo de ITEMS es igual al de UNIF, asi que podemos usar vectorInsertarFinal directamente al no haber problema
    //de espacio con memcpy
    vectorIteradorCrear(&it,v2);
    void *elem = vectorIteradorPrimero(&it);
    while(!vectorIteradorFinIter(&it))
    {
        vectorInsertarFinal(unif,elem);
        elem = vectorIteradorSiguiente(&it);
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
    //encabezado
    strcpy(result.periodo,"periodo");
    strcpy(result.clasificador,"Clasificador");
    strcpy(result.general_aperturas,"nivel_general_aperturas");
    strcpy(result.tipo_varible,"Tipo_aperturas");
    strcpy(result.valor,"Valor");
    fwrite(&result,sizeof(result),1,pf);

    //Iteramos una vez cada vector para poder escribir el archivo con lo pedido, es mejor escribirlo directamente antes
    //de cargar a los 3 juntos en un vector para volver a copiarlo en el archivo, si tuvieramos que trabajar con el resultado
    //de esto... otra seria la historia, ahi si convendria pasarlo primero a un vector
    VectorIterador it;
    vectorIteradorCrear(&it,unif);
    UNIF *pos = vectorIteradorPrimero(&it);
    //indice_icc
    while(!vectorIteradorFinIter(&it))
    {
        strcpy(result.periodo, pos->periodo);
        strcpy(result.clasificador,pos->clasificador);
        strcpy(result.general_aperturas,pos->general_aperturas);
        strcpy(result.tipo_varible,"indice_icc");
        sprintf(result.valor,"%.7f",pos->indice);
        fwrite(&result,sizeof(result),1,pf);
        pos = vectorIteradorSiguiente(&it);
    }
    //var mensual
    pos = vectorIteradorPrimero(&it);
    while(!vectorIteradorFinIter(&it))
    {
        strcpy(result.periodo, pos->periodo);
        strcpy(result.clasificador,pos->clasificador);
        strcpy(result.general_aperturas,pos->general_aperturas);
        strcpy(result.tipo_varible,"var_mensual");
        strcpy(result.valor,pos->var_mensual);;
        fwrite(&result,sizeof(result),1,pf);
        pos = vectorIteradorSiguiente(&it);
    }

    //var_interanual
    pos = vectorIteradorPrimero(&it);
    while(!vectorIteradorFinIter(&it))
    {
        strcpy(result.periodo, pos->periodo);
        strcpy(result.clasificador,pos->clasificador);
        strcpy(result.general_aperturas,pos->general_aperturas);
        strcpy(result.tipo_varible,"var_interanual");
        strcpy(result.valor,pos->var_interanual);;
        fwrite(&result,sizeof(result),1,pf);
        pos = vectorIteradorSiguiente(&it);
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

void varAnual(Vector *v)
{
    VectorIterador it;
    vectorIteradorCrear(&it,v);
    int cantidadPorAnio = 12*cantidad_indices(v);
    UNIF *pos = (UNIF*)vectorIteradorPrimero(&it);

    double var;
    //los del primer año no tienen variacion, multiplicamos a cantidad por 12 ya que cantidad es los distintos indices que hay,
    //representando un mes, entonces al multiplicarlo por 12 tenes el año y queda casi igual que el codigo de var_mensual y limpio
    for(int i = 0; i < cantidadPorAnio; i++)
    {
        sprintf(pos->var_interanual,"NA");
        pos = (UNIF*)vectorIteradorSiguiente(&it);
    }

    while(!vectorIteradorFinIter(&it))
    {
        UNIF * posAnterior = vectorIteradorDesplazamiento(&it,-cantidadPorAnio);
        vectorIteradorDesplazamiento(&it,cantidadPorAnio);
        var = ((pos->indice/posAnterior->indice)-1)*100;
        sprintf(pos->var_interanual,"%.2f",var);

        pos = vectorIteradorSiguiente(&it);
    }
}

void varMensual(Vector *v)
{
    VectorIterador it;
    vectorIteradorCrear(&it,v);
    //calculamos cuantos tipos distintos de indices hay, ya que esto nos sirve para volver x cantidad de items atras
    //simulando un mes atras, mas eficiente y facil que realizar busquedas de fechas y demas, solo hay que realizar
    //una lectura de todo un mes para ver la cantidad que son y listo
    int cantidadPorMes = cantidad_indices(v);
    UNIF *pos = (UNIF*)vectorIteradorPrimero(&it);

    double var;
    //los del primer mes no tienen variacion
    for(int i = 0; i < cantidadPorMes; i++)
    {
        sprintf(pos->var_mensual,"NA");
        pos = (UNIF*)vectorIteradorSiguiente(&it);

    }

    while(!vectorIteradorFinIter(&it))
    {

        UNIF * posAnterior = vectorIteradorDesplazamiento(&it,-cantidadPorMes);
        vectorIteradorDesplazamiento(&it,cantidadPorMes);
        var = ((pos->indice/posAnterior->indice)-1)*100;
        sprintf(pos->var_mensual,"%.2f",var);

        pos = (UNIF*)vectorIteradorSiguiente(&it);

    }

}

int cantidad_indices(Vector *v)
{

    void *pos = v->vec;
    int cantidad = 0;
    char primero[41];
    UNIF *gen = (UNIF*)pos;
    strcpy(primero,gen->general_aperturas);
    //va a haber un indice, sino no tendria usarla, asi que nos parecio buena idea un do while, esto recorre comparando
    //el primer elemento hasta que encuentre otro que sea igual, osea cuando pase un mes
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
    //asignamos prioridad a cada uno para poder ordenarlos segun sea necesario
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


