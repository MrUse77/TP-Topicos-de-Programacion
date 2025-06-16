#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "TDAvector.h"
#include "funciones.h"
#define CAPACIDAD_DATOS 500
#define TAM_LINEA 350
#define NOMBRE_GENERAL "indices_icc_general_capitulos.csv"
#define MES_MES 1
#define ANIO_MES 12
#define NOMBRE_BIN "arch.bin"
int main()
{
    Vector items;
    Vector general;
    Vector unif;
    vectorCrear(&items,CAPACIDAD_DATOS,sizeof(ITEMS));
    vectorCrear(&general,CAPACIDAD_DATOS,sizeof(GENERAL));
    vectorCrear(&unif,CAPACIDAD_DATOS,sizeof(UNIF));

    archivoAVector("Indices_items_obra.csv",&items,leerLineaItems,TAM_LINEA);
    archivoAVector(NOMBRE_GENERAL,&general,leerLineaGeneral,TAM_LINEA);

    DesencriptarGeneral(&general);
    DesencriptarItems(&items);
    
    normalizar_general(&general);
    
    formatoPeriodo(&items,eligeItems);
    formatoPeriodo(&general,eligeGeneral);
    normalizarItems(&items);
    
    clasificadorGeneral(&general);
    clasificadorItems(&items);
    
    unificar3(&unif,&items,&general);

    ordenarInsercion(&unif,cmpClasificador);
    ordenarInsercion(&unif,cmpPeriodo);
    
    varMensual(&unif);
    varAnual(&unif);
    unificarResult(&unif,NOMBRE_BIN);
    
    mostrarResultsBin(NOMBRE_BIN);
 

    return 0;
}
